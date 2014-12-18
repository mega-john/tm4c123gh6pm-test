/*
 * 1wire_uart.c
 *
 *  Created on: 10 дек. 2014 г.
 *      Author: john
 */
#include "1wire_uart.h"

// Буфер для приема/передачи по 1-wire
uint8_t ow_buf[8];

#define OW_0    0x00
#define OW_1    0xff
#define OW_R_1  0xff

//*****************************************************************************
//
// The number of SysTick ticks per second used for the SysTick interrupt.
//
//*****************************************************************************
#define SYSTICKS_PER_SECOND     100

//*****************************************************************************
//
// The size of the memory transfer source and destination buffers (in words).
//
//*****************************************************************************
#define MEM_BUFFER_SIZE         1024

//*****************************************************************************
//
// The size of the UART transmit and receive buffers.  They do not need to be
// the same size.
//
//*****************************************************************************
#define UART_TXBUF_SIZE         256
#define UART_RXBUF_SIZE         256

//*****************************************************************************
//
// The source and destination buffers used for memory transfers.
//
//*****************************************************************************
static uint32_t g_ui32SrcBuf[MEM_BUFFER_SIZE];
static uint32_t g_ui32DstBuf[MEM_BUFFER_SIZE];

//*****************************************************************************
//
// The transmit and receive buffers used for the UART transfers.  There is one
// transmit buffer and a pair of recieve ping-pong buffers.
//
//*****************************************************************************
static uint8_t g_ui8TxBuf[UART_TXBUF_SIZE];
static uint8_t g_ui8RxBufA[UART_RXBUF_SIZE];
static uint8_t g_ui8RxBufB[UART_RXBUF_SIZE];

//*****************************************************************************
//
// The count of uDMA errors.  This value is incremented by the uDMA error
// handler.
//
//*****************************************************************************
static uint32_t g_ui32uDMAErrCount = 0;

//*****************************************************************************
//
// The count of times the uDMA interrupt occurred but the uDMA transfer was not
// complete.  This should remain 0.
//
//*****************************************************************************
static uint32_t g_ui32BadISR = 0;

//*****************************************************************************
//
// The count of UART buffers filled, one for each ping-pong buffer.
//
//*****************************************************************************
static uint32_t g_ui32RxBufACount = 0;
static uint32_t g_ui32RxBufBCount = 0;

//*****************************************************************************
//
// The count of memory uDMA transfer blocks.  This value is incremented by the
// uDMA interrupt handler whenever a memory block transfer is completed.
//
//*****************************************************************************
static uint32_t g_ui32MemXferCount = 0;

//*****************************************************************************
//
// The CPU usage in percent, in 16.16 fixed point format.
//
//*****************************************************************************
static uint32_t g_ui32CPUUsage;

//*****************************************************************************
//
// The number of seconds elapsed since the start of the program.  This value is
// maintained by the SysTick interrupt handler.
//
//*****************************************************************************
static uint32_t g_ui32Seconds = 0;

//*****************************************************************************
//
// The control table used by the uDMA controller.  This table must be aligned
// to a 1024 byte boundary.
//
//*****************************************************************************
#pragma DATA_ALIGN(ui8ControlTable, 1024)
uint8_t ui8ControlTable[1024];

//*****************************************************************************
//
// The interrupt handler for uDMA errors.  This interrupt will occur if the
// uDMA encounters a bus error while trying to perform a transfer.  This
// handler just increments a counter if an error occurs.
//
//*****************************************************************************
void uDMAErrorHandler(void)
{
    uint32_t ui32Status;

    //
    // Check for uDMA error bit
    //
    ui32Status = uDMAErrorStatusGet();

    //
    // If there is a uDMA error, then clear the error and increment
    // the error counter.
    //
    if (ui32Status)
    {
        uDMAErrorStatusClear();
        g_ui32uDMAErrCount++;
    }
}

//*****************************************************************************
//
// The interrupt handler for uDMA interrupts from the memory channel.  This
// interrupt will increment a counter, and then restart another memory
// transfer.
//
//*****************************************************************************
void uDMAIntHandler(void)
{
    uint32_t ui32Mode;

    //
    // Check for the primary control structure to indicate complete.
    //
    ui32Mode = uDMAChannelModeGet(UDMA_CHANNEL_SW);
    if (ui32Mode == UDMA_MODE_STOP)
    {
        //
        // Increment the count of completed transfers.
        //
        g_ui32MemXferCount++;

        //
        // Configure it for another transfer.
        //
        uDMAChannelTransferSet(UDMA_CHANNEL_SW, UDMA_MODE_AUTO, g_ui32SrcBuf, g_ui32DstBuf, MEM_BUFFER_SIZE);

        //
        // Initiate another transfer.
        //
        uDMAChannelEnable(UDMA_CHANNEL_SW);
        uDMAChannelRequest(UDMA_CHANNEL_SW);
    }

    //
    // If the channel is not stopped, then something is wrong.
    //
    else
    {
        g_ui32BadISR++;
    }
}

#define UART_NUMBER 2
#define u(a,b)    a##b
#define UARTx_BASE  u(UART,2)_BASE



//*****************************************************************************
//
// The interrupt handler for UART1.  This interrupt will occur when a DMA
// transfer is complete using the UART1 uDMA channel.  It will also be
// triggered if the peripheral signals an error.  This interrupt handler will
// switch between receive ping-pong buffers A and B.  It will also restart a TX
// uDMA transfer if the prior transfer is complete.  This will keep the UART
// running continuously (looping TX data back to RX).
//
//*****************************************************************************
void UART2IntHandler(void)
{
    uint32_t ui32Status;
    uint32_t ui32Mode;

    //
    // Read the interrupt status of the UART.
    //
    ui32Status = UARTIntStatus(UARTx_BASE, 1);

    //
    // Clear any pending status, even though there should be none since no UART
    // interrupts were enabled.  If UART error interrupts were enabled, then
    // those interrupts could occur here and should be handled.  Since uDMA is
    // used for both the RX and TX, then neither of those interrupts should be
    // enabled.
    //
    UARTIntClear(UART2_BASE, ui32Status);

    //
    // Check the DMA control table to see if the ping-pong "A" transfer is
    // complete.  The "A" transfer uses receive buffer "A", and the primary
    // control structure.
    //
    ui32Mode = uDMAChannelModeGet(UDMA_CHANNEL_UART2RX | UDMA_PRI_SELECT);

    //
    // If the primary control structure indicates stop, that means the "A"
    // receive buffer is done.  The uDMA controller should still be receiving
    // data into the "B" buffer.
    //
    if (ui32Mode == UDMA_MODE_STOP)
    {
        //
        // Increment a counter to indicate data was received into buffer A.  In
        // a real application this would be used to signal the main thread that
        // data was received so the main thread can process the data.
        //
        g_ui32RxBufACount++;

        //
        // Set up the next transfer for the "A" buffer, using the primary
        // control structure.  When the ongoing receive into the "B" buffer is
        // done, the uDMA controller will switch back to this one.  This
        // example re-uses buffer A, but a more sophisticated application could
        // use a rotating set of buffers to increase the amount of time that
        // the main thread has to process the data in the buffer before it is
        // reused.
        //
        uDMAChannelTransferSet(UDMA_CHANNEL_UART2RX | UDMA_PRI_SELECT, UDMA_MODE_PINGPONG, (void *) (UART2_BASE + UART_O_DR), g_ui8RxBufA,
                sizeof(g_ui8RxBufA));
    }

    //
    // Check the DMA control table to see if the ping-pong "B" transfer is
    // complete.  The "B" transfer uses receive buffer "B", and the alternate
    // control structure.
    //
    ui32Mode = uDMAChannelModeGet(UDMA_CHANNEL_UART2RX | UDMA_ALT_SELECT);

    //
    // If the alternate control structure indicates stop, that means the "B"
    // receive buffer is done.  The uDMA controller should still be receiving
    // data into the "A" buffer.
    //
    if (ui32Mode == UDMA_MODE_STOP)
    {
        //
        // Increment a counter to indicate data was received into buffer A.  In
        // a real application this would be used to signal the main thread that
        // data was received so the main thread can process the data.
        //
        g_ui32RxBufBCount++;

        //
        // Set up the next transfer for the "B" buffer, using the alternate
        // control structure.  When the ongoing receive into the "A" buffer is
        // done, the uDMA controller will switch back to this one.  This
        // example re-uses buffer B, but a more sophisticated application could
        // use a rotating set of buffers to increase the amount of time that
        // the main thread has to process the data in the buffer before it is
        // reused.
        //
        uDMAChannelTransferSet(UDMA_CHANNEL_UART2RX | UDMA_ALT_SELECT, UDMA_MODE_PINGPONG, (void *) (UART2_BASE + UART_O_DR), g_ui8RxBufB,
                sizeof(g_ui8RxBufB));
    }

    //
    // If the UART1 DMA TX channel is disabled, that means the TX DMA transfer
    // is done.
    //
    if (!uDMAChannelIsEnabled(UDMA_CHANNEL_UART2TX))
    {
        //
        // Start another DMA transfer to UART1 TX.
        //
        uDMAChannelTransferSet(UDMA_CHANNEL_UART2TX | UDMA_PRI_SELECT, UDMA_MODE_BASIC, g_ui8TxBuf, (void *) (UART2_BASE + UART_O_DR),
                sizeof(g_ui8TxBuf));

        //
        // The uDMA TX channel must be re-enabled.
        //
        uDMAChannelEnable(UDMA_CHANNEL_UART2TX);
    }
}

//*****************************************************************************
//
// Initializes the UART2 peripheral and sets up the TX and RX uDMA channels.
// The UART is configured for loopback mode so that any data sent on TX will be
// received on RX.  The uDMA channels are configured so that the TX channel
// will copy data from a buffer to the UART TX output.  And the uDMA RX channel
// will receive any incoming data into a pair of buffers in ping-pong mode.
//
//*****************************************************************************
void
InitUART2Transfer(void)
{
    unsigned int uIdx;

    //
    // Fill the TX buffer with a simple data pattern.
    //
    for(uIdx = 0; uIdx < UART_TXBUF_SIZE; uIdx++)
    {
        g_ui8TxBuf[uIdx] = uIdx;
    }

    //
    // Enable the UART peripheral, and configure it to operate even if the CPU
    // is in sleep.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART2);

    //
    // Configure the UART communication parameters.
    //
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,
                            UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                            UART_CONFIG_PAR_NONE);

    //
    // Set both the TX and RX trigger thresholds to 4.  This will be used by
    // the uDMA controller to signal when more data should be transferred.  The
    // uDMA TX and RX channels will be configured so that it can transfer 4
    // bytes in a burst when the UART is ready to transfer more data.
    //
    UARTFIFOLevelSet(UART2_BASE, UART_FIFO_TX4_8, UART_FIFO_RX4_8);

    //
    // Enable the UART for operation, and enable the uDMA interface for both TX
    // and RX channels.
    //
    UARTEnable(UART2_BASE);
    UARTDMAEnable(UART2_BASE, UART_DMA_RX | UART_DMA_TX);

    //
    // This register write will set the UART to operate in loopback mode.  Any
    // data sent on the TX output will be received on the RX input.
    //
    HWREG(UART2_BASE + UART_O_CTL) |= UART_CTL_LBE;

    //
    // Enable the UART peripheral interrupts.  Note that no UART interrupts
    // were enabled, but the uDMA controller will cause an interrupt on the
    // UART interrupt signal when a uDMA transfer is complete.
    //
    IntEnable(INT_UART2);

    //
    // Put the attributes in a known state for the uDMA UART1RX channel.  These
    // should already be disabled by default.
    //
    uDMAChannelAttributeDisable(UDMA_CHANNEL_UART2RX,
                                    UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST |
                                    UDMA_ATTR_HIGH_PRIORITY |
                                    UDMA_ATTR_REQMASK);

    //
    // Configure the control parameters for the primary control structure for
    // the UART RX channel.  The primary contol structure is used for the "A"
    // part of the ping-pong receive.  The transfer data size is 8 bits, the
    // source address does not increment since it will be reading from a
    // register.  The destination address increment is byte 8-bit bytes.  The
    // arbitration size is set to 4 to match the RX FIFO trigger threshold.
    // The uDMA controller will use a 4 byte burst transfer if possible.  This
    // will be somewhat more effecient that single byte transfers.
    //
    uDMAChannelControlSet(UDMA_CHANNEL_UART2RX | UDMA_PRI_SELECT,
                              UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 |
                              UDMA_ARB_4);

    //
    // Configure the control parameters for the alternate control structure for
    // the UART RX channel.  The alternate contol structure is used for the "B"
    // part of the ping-pong receive.  The configuration is identical to the
    // primary/A control structure.
    //
    uDMAChannelControlSet(UDMA_CHANNEL_UART2RX | UDMA_ALT_SELECT,
                              UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 |
                              UDMA_ARB_4);

    //
    // Set up the transfer parameters for the UART RX primary control
    // structure.  The mode is set to ping-pong, the transfer source is the
    // UART data register, and the destination is the receive "A" buffer.  The
    // transfer size is set to match the size of the buffer.
    //
    uDMAChannelTransferSet(UDMA_CHANNEL_UART2RX | UDMA_PRI_SELECT,
                               UDMA_MODE_PINGPONG,
                               (void *)(UART2_BASE + UART_O_DR),
                               g_ui8RxBufA, sizeof(g_ui8RxBufA));

    //
    // Set up the transfer parameters for the UART RX alternate control
    // structure.  The mode is set to ping-pong, the transfer source is the
    // UART data register, and the destination is the receive "B" buffer.  The
    // transfer size is set to match the size of the buffer.
    //
    uDMAChannelTransferSet(UDMA_CHANNEL_UART2RX | UDMA_ALT_SELECT,
                               UDMA_MODE_PINGPONG,
                               (void *)(UART2_BASE + UART_O_DR),
                               g_ui8RxBufB, sizeof(g_ui8RxBufB));

    //
    // Put the attributes in a known state for the uDMA UART2TX channel.  These
    // should already be disabled by default.
    //
    uDMAChannelAttributeDisable(UDMA_CHANNEL_UART2TX,
                                    UDMA_ATTR_ALTSELECT |
                                    UDMA_ATTR_HIGH_PRIORITY |
                                    UDMA_ATTR_REQMASK);

    //
    // Set the USEBURST attribute for the uDMA UART TX channel.  This will
    // force the controller to always use a burst when transferring data from
    // the TX buffer to the UART.  This is somewhat more effecient bus usage
    // than the default which allows single or burst transfers.
    //
    uDMAChannelAttributeEnable(UDMA_CHANNEL_UART2TX, UDMA_ATTR_USEBURST);

    //
    // Configure the control parameters for the UART TX.  The uDMA UART TX
    // channel is used to transfer a block of data from a buffer to the UART.
    // The data size is 8 bits.  The source address increment is 8-bit bytes
    // since the data is coming from a buffer.  The destination increment is
    // none since the data is to be written to the UART data register.  The
    // arbitration size is set to 4, which matches the UART TX FIFO trigger
    // threshold.
    //
    uDMAChannelControlSet(UDMA_CHANNEL_UART2TX | UDMA_PRI_SELECT,
                              UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE |
                              UDMA_ARB_4);

    //
    // Set up the transfer parameters for the uDMA UART TX channel.  This will
    // configure the transfer source and destination and the transfer size.
    // Basic mode is used because the peripheral is making the uDMA transfer
    // request.  The source is the TX buffer and the destination is the UART
    // data register.
    //
    uDMAChannelTransferSet(UDMA_CHANNEL_UART2TX | UDMA_PRI_SELECT,
                               UDMA_MODE_BASIC, g_ui8TxBuf,
                               (void *)(UART2_BASE + UART_O_DR),
                               sizeof(g_ui8TxBuf));

    //
    // Now both the uDMA UART TX and RX channels are primed to start a
    // transfer.  As soon as the channels are enabled, the peripheral will
    // issue a transfer request and the data transfers will begin.
    //
    uDMAChannelEnable(UDMA_CHANNEL_UART2RX);
    uDMAChannelEnable(UDMA_CHANNEL_UART2TX);
}

//-----------------------------------------------------------------------------
// функция преобразует один байт в восемь, для передачи через USART
// ow_byte - байт, который надо преобразовать
// ow_bits - ссылка на буфер, размером не менее 8 байт
//-----------------------------------------------------------------------------
void OW_toBits(uint8_t ow_byte, uint8_t *ow_bits)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if (ow_byte & 0x01)
        {
            *ow_bits = OW_1;
        }
        else
        {
            *ow_bits = OW_0;
        }
        ow_bits++;
        ow_byte = ow_byte >> 1;
    }
}

//-----------------------------------------------------------------------------
// обратное преобразование - из того, что получено через USART опять собирается байт
// ow_bits - ссылка на буфер, размером не менее 8 байт
//-----------------------------------------------------------------------------
uint8_t OW_toByte(uint8_t *ow_bits)
{
    uint8_t ow_byte, i;
    ow_byte = 0;
    for (i = 0; i < 8; i++)
    {
        ow_byte = ow_byte >> 1;
        if (*ow_bits == OW_R_1)
        {
            ow_byte |= 0x80;
        }
        ow_bits++;
    }

    return ow_byte;
}

//-----------------------------------------------------------------------------
// инициализирует USART и DMA
//-----------------------------------------------------------------------------
uint8_t OW_InitUART()
{
//    GPIO_InitTypeDef GPIO_InitStruct;
//    USART_InitTypeDef USART_InitStructure;
//
//    if (OW_USART == USART1)
//    {
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
//
//        // USART TX
//        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
//        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//
//        GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//        // USART RX
//        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
//        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//
//        GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
//
//        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//    }
//
//    if (OW_USART == USART2)
//    {
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
//
//        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
//        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//
//        GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
//        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//
//        GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//
//        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//    }
//
//    USART_InitStructure.USART_BaudRate = 115200;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;
//    USART_InitStructure.USART_Parity = USART_Parity_No;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
//
//    USART_Init(OW_USART, &USART_InitStructure);
//    USART_Cmd(OW_USART, ENABLE);
    return OW_OK;
}

//-----------------------------------------------------------------------------
// осуществляет сброс и проверку на наличие устройств на шине
//-----------------------------------------------------------------------------
uint8_t OW_ResetUART()
{
//    uint8_t ow_presence;
//    USART_InitTypeDef USART_InitStructure;
//
//    USART_InitStructure.USART_BaudRate = 9600;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;
//    USART_InitStructure.USART_Parity = USART_Parity_No;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
//    USART_Init(OW_USART, &USART_InitStructure);
//
//    // отправляем 0xf0 на скорости 9600
//    USART_ClearFlag(OW_USART, USART_FLAG_TC);
//    USART_SendData(OW_USART, 0xf0);
//    while (USART_GetFlagStatus(OW_USART, USART_FLAG_TC) == RESET)
//    {
//#ifdef OW_GIVE_TICK_RTOS
//        taskYIELD();
//#endif
//    }
//
//    ow_presence = USART_ReceiveData(OW_USART);
//
//    USART_InitStructure.USART_BaudRate = 115200;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;
//    USART_InitStructure.USART_Parity = USART_Parity_No;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
//    USART_Init(OW_USART, &USART_InitStructure);
//
//    if (ow_presence != 0xf0)
//    {
//        return OW_OK;
//    }

    return OW_NO_DEVICE;
}

//-----------------------------------------------------------------------------
// процедура общения с шиной 1-wire
// sendReset - посылать RESET в начале общения.
//      OW_SEND_RESET или OW_NO_RESET
// command - массив байт, отсылаемых в шину. Если нужно чтение - отправляем OW_READ_SLOTH
// cLen - длина буфера команд, столько байт отошлется в шину
// data - если требуется чтение, то ссылка на буфер для чтения
// dLen - длина буфера для чтения. Прочитается не более этой длины
// readStart - с какого символа передачи начинать чтение (нумеруются с 0)
//      можно указать OW_NO_READ, тогда можно не задавать data и dLen
//-----------------------------------------------------------------------------
uint8_t OW_Send(uint8_t sendReset, uint8_t *command, uint8_t cLen, uint8_t *data, uint8_t dLen, uint8_t readStart)
{

//    // если требуется сброс - сбрасываем и проверяем на наличие устройств
//    if (sendReset == OW_SEND_RESET)
//    {
//        if (OW_Reset() == OW_NO_DEVICE)
//        {
//            return OW_NO_DEVICE;
//        }
//    }
//
//    while (cLen > 0)
//    {
//
//        OW_toBits(*command, ow_buf);
//        command++;
//        cLen--;
//
//        DMA_InitTypeDef DMA_InitStructure;
//
//        // DMA на чтение
//        DMA_DeInit(OW_DMA_CH_RX);
//        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(USART2->DR);
//        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) ow_buf;
//        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//        DMA_InitStructure.DMA_BufferSize = 8;
//        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//        DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//        DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
//        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//        DMA_Init(OW_DMA_CH_RX, &DMA_InitStructure);
//
//        // DMA на запись
//        DMA_DeInit(OW_DMA_CH_TX);
//        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(USART2->DR);
//        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) ow_buf;
//        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//        DMA_InitStructure.DMA_BufferSize = 8;
//        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//        DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//        DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
//        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//        DMA_Init(OW_DMA_CH_TX, &DMA_InitStructure);
//
//        // старт цикла отправки
//        USART_ClearFlag(OW_USART, USART_FLAG_RXNE | USART_FLAG_TC | USART_FLAG_TXE);
//        USART_DMACmd(OW_USART, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
//        DMA_Cmd(OW_DMA_CH_RX, ENABLE);
//        DMA_Cmd(OW_DMA_CH_TX, ENABLE);
//
//        // Ждем, пока не примем 8 байт
//        while (DMA_GetFlagStatus(OW_DMA_FLAG) == RESET)
//        {
//#ifdef OW_GIVE_TICK_RTOS
//            taskYIELD();
//#endif
//        }
//
//        // отключаем DMA
//        DMA_Cmd(OW_DMA_CH_TX, DISABLE);
//        DMA_Cmd(OW_DMA_CH_RX, DISABLE);
//        USART_DMACmd(OW_USART, USART_DMAReq_Tx | USART_DMAReq_Rx, DISABLE);
//
//        // если прочитанные данные кому-то нужны - выкинем их в буфер
//        if (readStart == 0 && dLen > 0)
//        {
//            *data = OW_toByte(ow_buf);
//            data++;
//            dLen--;
//        }
//        else
//        {
//            if (readStart != OW_NO_READ)
//            {
//                readStart--;
//            }
//        }
//    }

    return OW_OK;
}
