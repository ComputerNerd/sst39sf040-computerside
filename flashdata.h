//IC manufacturer information
//All data in this file is available from:
//http://www.idhw.com/textual/chip/jedec_spd_man.html

#ifndef FLASHDATA_H
#define FLASHDATA_H

//Return manufacturer name by ID
char* getManufacturer(int ID){
    char* manufacturer;
    switch(ID){
        case 0b00000001: manufacturer = "AMD"; break;
        case 0b00000010: manufacturer = "AMI"; break;
        case 0b00000100: manufacturer = "Fujitsu"; break;
        case 0b00000111: manufacturer = "Hitachi"; break;
        case 0b00001000: manufacturer = "Inmos"; break;
        case 0b00001011: manufacturer = "Intersil"; break;
        case 0b00001101: manufacturer = "Mostek"; break;
        case 0b00001110: manufacturer = "Motorola"; break;
        case 0b00010000: manufacturer = "NEC"; break;
        case 0b00010011: manufacturer = "Conexant (Rockwell)"; break;
        case 0b00010101: manufacturer = "Philips Semi. (Signetics)"; break;
        case 0b00010110: manufacturer = "Synertek"; break;
        case 0b00011001: manufacturer = "Xicor"; break;
        case 0b00011010: manufacturer = "Zilog"; break;
        case 0b00011100: manufacturer = "Mitsubishi"; break;
        case 0b00011111: manufacturer = "Atmel"; break;
        case 0b00100000: manufacturer = "SGS/Thomson"; break;
        case 0b00100011: manufacturer = "Wafer Scale Integration"; break;
        case 0b00100101: manufacturer = "Tristar"; break;
        case 0b00100110: manufacturer = "Visic"; break;
        case 0b00101001: manufacturer = "MicrochipTechnology"; break;
        case 0b00101010: manufacturer = "Ricoh Ltd."; break;
        case 0b00101100: manufacturer = "Micron Technology"; break;
        case 0b00101111: manufacturer = "ACTEL"; break;
        case 0b00110001: manufacturer = "Catalyst"; break;
        case 0b00110010: manufacturer = "Panasonic"; break;
        case 0b00110100: manufacturer = "Cypress"; break;
        case 0b00110111: manufacturer = "Zarlink (formerly Plessey)"; break;
        case 0b00111000: manufacturer = "UTMC"; break;
        case 0b00111011: manufacturer = "Integrated CMOS (Vertex)"; break;
        case 0b00111101: manufacturer = "Tektronix"; break;
        case 0b00111110: manufacturer = "Sun Microsystems"; break;
        case 0b01000000: manufacturer = "MOSEL"; break;
        case 0b01000011: manufacturer = "Xerox"; break;
        case 0b01000101: manufacturer = "SunDisk"; break;
        case 0b01000110: manufacturer = "Elan Circuit Tech."; break;
        case 0b01001001: manufacturer = "Xilinx"; break;
        case 0b01001010: manufacturer = "Compaq"; break;
        case 0b01001100: manufacturer = "SCI"; break;
        case 0b01001111: manufacturer = "I3 Design System"; break;
        case 0b01010001: manufacturer = "Crosspoint Solutions"; break;
        case 0b01010010: manufacturer = "Alliance Semiconductor"; break;
        case 0b01010100: manufacturer = "Hewlett-Packard"; break;
        case 0b01010111: manufacturer = "New Media"; break;
        case 0b01011000: manufacturer = "MHS Electronic"; break;
        case 0b01011011: manufacturer = "Kawasaki Steel"; break;
        case 0b01011101: manufacturer = "TECMAR"; break;
        case 0b01011110: manufacturer = "Exar"; break;
        case 0b01100001: manufacturer = "Northern Telecom"; break;
        case 0b01100010: manufacturer = "Sanyo"; break;
        case 0b01100100: manufacturer = "Crystal Semiconductor"; break;
        case 0b01100111: manufacturer = "Asparix"; break;
        case 0b01101000: manufacturer = "Convex Computer"; break;
        case 0b01101011: manufacturer = "Transwitch"; break;
        case 0b01101101: manufacturer = "Cannon"; break;
        case 0b01101110: manufacturer = "Altera"; break;
        case 0b01110000: manufacturer = "QUALCOMM"; break;
        case 0b01110011: manufacturer = "AMS (Austria Micro)"; break;
        case 0b01110101: manufacturer = "Aster Electronics"; break;
        case 0b01110110: manufacturer = "Bay Networks (Synoptic)"; break;
        case 0b01111001: manufacturer = "Thesys"; break;
        case 0b01111010: manufacturer = "Solbourne Computer"; break;
        case 0b01111100: manufacturer = "Dialog"; break;
        case 0b10000011: manufacturer = "Fairchild"; break;
        case 0b10000101: manufacturer = "GTE"; break;
        case 0b10000110: manufacturer = "Harris"; break;
        case 0b10001001: manufacturer = "Intel"; break;
        case 0b10001010: manufacturer = "I.T.T."; break;
        case 0b10001100: manufacturer = "Monolithic Memories"; break;
        case 0b10001111: manufacturer = "National"; break;
        case 0b10010001: manufacturer = "RCA"; break;
        case 0b10010010: manufacturer = "Raytheon"; break;
        case 0b10010100: manufacturer = "Seeq"; break;
        case 0b10010111: manufacturer = "Texas Instruments"; break;
        case 0b10011000: manufacturer = "Toshiba"; break;
        case 0b10011011: manufacturer = "Eurotechnique"; break;
        case 0b10011101: manufacturer = "Lucent (AT&T)"; break;
        case 0b10011110: manufacturer = "Exel"; break;
        case 0b10100001: manufacturer = "Lattice Semi."; break;
        case 0b10100010: manufacturer = "NCR"; break;
        case 0b10100100: manufacturer = "IBM"; break;
        case 0b10100111: manufacturer = "Intl. CMOS Technology"; break;
        case 0b10101000: manufacturer = "SSSI"; break;
        case 0b10101011: manufacturer = "VLSI"; break;
        case 0b10101101: manufacturer = "Hyundai Electronics"; break;
        case 0b10101110: manufacturer = "OKI Semiconductor"; break;
        case 0b10110000: manufacturer = "Sharp"; break;
        case 0b10110011: manufacturer = "IDT"; break;
        case 0b10110101: manufacturer = "DEC"; break;
        case 0b10110110: manufacturer = "LSI Logic"; break;
        case 0b10111001: manufacturer = "Thinking Machine"; break;
        case 0b10111010: manufacturer = "Thomson CSF"; break;
        case 0b10111100: manufacturer = "Honeywell"; break;
        case 0b10111111: manufacturer = "SST"; break;
        case 0b11000001: manufacturer = "Infineon (formerly Siemens)"; break;
        case 0b11000010: manufacturer = "Macronix"; break;
        case 0b11000100: manufacturer = "Plus Logic"; break;
        case 0b11000111: manufacturer = "European Silicon Str."; break;
        case 0b11001000: manufacturer = "Apple Computer"; break;
        case 0b11001011: manufacturer = "Protocol Engines"; break;
        case 0b11001101: manufacturer = "Seiko Instruments"; break;
        case 0b11001110: manufacturer = "Samsung"; break;
        case 0b11010000: manufacturer = "Klic"; break;
        case 0b11010011: manufacturer = "Tandem"; break;
        case 0b11010101: manufacturer = "Intg. Silicon Solutions"; break;
        case 0b11010110: manufacturer = "Brooktree"; break;
        case 0b11011001: manufacturer = "Performance Semi."; break;
        case 0b11011010: manufacturer = "Winbond Electronic"; break;
        case 0b11011100: manufacturer = "Bright Micro"; break;
        case 0b11011111: manufacturer = "PCMCIA"; break;
        case 0b11100000: manufacturer = "LG Semiconductor"; break;
        case 0b11100011: manufacturer = "Array Microsystems"; break;
        case 0b11100101: manufacturer = "Analog Devices"; break;
        case 0b11100110: manufacturer = "PMC-Sierra"; break;
        case 0b11101001: manufacturer = "Quality Semiconductor"; break;
        case 0b11101010: manufacturer = "Nimbus Technology"; break;
        case 0b11101100: manufacturer = "Micronas (ITT Intermetall)"; break;
        case 0b11101111: manufacturer = "NEXCOM"; break;
        case 0b11110001: manufacturer = "Sony"; break;
        case 0b11110010: manufacturer = "Cray Research"; break;
        case 0b11110100: manufacturer = "Vitesse"; break;
        case 0b11110111: manufacturer = "Zentrum or ZMD"; break;
        case 0b11111000: manufacturer = "TRW"; break;
        case 0b11111011: manufacturer = "Allied-Signal"; break;
        case 0b11111101: manufacturer = "Media Vision"; break;
        case 0b11111110: manufacturer = "Level One Communication"; break;
        default: manufacturer = "Unknown";
    }
    return manufacturer;
}

#endif
