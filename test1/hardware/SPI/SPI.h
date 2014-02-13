#include <stdint.h>

#define SPI_CLOCK_DIV2 2
#define SPI_CLOCK_DIV4 4
#define SPI_CLOCK_DIV8 8
#define SPI_CLOCK_DIV16 16
#define SPI_CLOCK_DIV32 32
#define SPI_CLOCK_DIV64 64
#define SPI_CLOCK_DIV128 128

#define SPI_MODE0 0x00
#define SPI_MODE1 0x80
#define SPI_MODE2 0x40
#define SPI_MODE3 0xC0

#define BOOST_PACK_SPI 2

void SPI_begin(uint8_t module);
void SPI_end();
void SPI_setBitOrder(uint8_t);
void SPI_setDataMode(uint8_t);
void SPI_setClockDivider(uint8_t);
void SPI_setModule(uint8_t);
void SPI_transfer8(uint8_t);
void SPI_transfer16(uint16_t data);
void SPI_transfer24(uint32_t data);
void SPI_transfer32(uint32_t data);
uint8_t SPI_read8(void);
