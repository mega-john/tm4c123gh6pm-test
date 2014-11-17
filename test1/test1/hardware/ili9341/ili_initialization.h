//first - value, second - data or command
static const uint8_t init_data[][2] =
{
{ 0xCB, 0 },//Power control A (CBh)
{ 0x39, 1 },
{ 0x2C, 1 },
{ 0x00, 1 },
{ 0x34, 1 },
{ 0x02, 1 },

{ 0xCF, 0 },//Power control B (CFh)
{ 0x00, 1 },
{ 0X81, 1 },
{ 0X30, 1 },

{ 0xE8, 0 },//Driver timing control A (E8h)
{ 0x85, 1 },
{ 0x00, 1 },
{ 0x78, 1 },

{ 0xEA, 0 },//Driver timing control B (EAh)
{ 0x00, 1 },
{ 0x00, 1 },

{ 0xED, 0 },// Power on sequence control (EDh)
{ 0x64, 1 },
{ 0x03, 1 },
{ 0X12, 1 },
{ 0X81, 1 },

{ 0xF7, 0 },//Pump ratio control (F7h)
{ 0x20, 1 },

{ 0xC0, 0 },//Power Control 1 (C0h)
{ 0x23, 1 },

{ 0xC1, 0 },//Power Control 2 (C1h)
{ 0x10, 1 },

{ 0xC5, 0 },//VCOM Control 1(C5h)
{ 0x3e, 1 },
{ 0x28, 1 },

{ 0xC7, 0 },//VCOM Control 2(C7h)
{ 0x86, 1 },

{ 0x36, 0 },//Memory Access Control (36h)
{ 0x48, 1 },

{ 0x3A, 0 },//COLMOD: Pixel Format Set (3Ah)
{ 0x55, 1 },

{ 0xB1, 0 }, //Frame Rate Control (In Normal Mode/Full Colors) (B1h)
{ 0x00, 1 }, //division ratio for internal clocks:fosc
//{ 0x18, 1 }, //24 clocks
{ 0x10, 1 }, //16 clocks
//{ 0x1f, 1 }, //31 clocks

{ 0xB2, 0 }, //Frame Rate Control (In Idle Mode/8 colors) (B2h)
{ 0x00, 1 }, //division ratio for internal clocks:fosc
//{ 0x18, 1 }, //24 clocks
{ 0x10, 1 }, //16 clocks
//{ 0x1f, 1 }, //31 clocks

{ 0xB6, 0 },//Display Function Control (B6h)
{ 0x08, 1 },
{ 0x82, 1 },
{ 0x27, 1 },

{ 0xF2, 0 },//Enable 3 gamma control (F2h)
{ 0x02, 1 },

{ 0x26, 0 },//Gamma Set (26h)
{ 0x00, 1 },

{ 0xE0, 0 },//Positive Gamma Correction (E0h)
{ 0x0F, 1 },
{ 0x31, 1 },
{ 0x2B, 1 },
{ 0x0C, 1 },
{ 0x0E, 1 },
{ 0x08, 1 },
{ 0x4E, 1 },
{ 0xF1, 1 },
{ 0x37, 1 },
{ 0x07, 1 },
{ 0x10, 1 },
{ 0x03, 1 },
{ 0x0E, 1 },
{ 0x09, 1 },
{ 0x00, 1 },

{ 0XE1, 0 },//Negative Gamma Correction (E1h)
{ 0x00, 1 },
{ 0x0E, 1 },
{ 0x14, 1 },
{ 0x03, 1 },
{ 0x11, 1 },
{ 0x07, 1 },
{ 0x31, 1 },
{ 0xC1, 1 },
{ 0x48, 1 },
{ 0x08, 1 },
{ 0x0F, 1 },
{ 0x0C, 1 },
{ 0x31, 1 },
{ 0x36, 1 },
{ 0x0F, 1 },

{ 0x11, 0 },//Sleep Out (11h)
};
