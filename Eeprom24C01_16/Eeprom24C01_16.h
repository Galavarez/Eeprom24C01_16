
#ifndef Eeprom24C01_16_h
#define Eeprom24C01_16_h

/******************************************************************************
 * Header file inclusion.
 ******************************************************************************/

#include <Arduino.h>

class Eeprom24C01_16
{
    public:
    
        Eeprom24C01_16(byte deviceAddress);
      
        void initialize();
        
        void writeByte_24C01_02(byte address, byte data);
        void writeByte_24C04_16(word address, byte data);
        
        void writeBytes_24C01_02(byte address, byte length, byte* p_data);
        void writeBytes_24C04_16(word address, word length, byte* p_data);

        byte readByte_24C01_02(byte address);
        byte readByte_24C04_16(word address);

        void readBytes_24C01_02(byte address, byte length, byte* p_data);
        void readBytes_24C04_16(word address, word length, byte* p_data);

    private:

        byte m_deviceAddress;

        void writePage_24C01_02(byte address, byte length, byte* p_data);
        void writePage_24C04_16(word address, byte length, byte* p_data);

        void writeBuffer_24C01_02(byte address, byte length, byte* p_data);
        void writeBuffer_24C04_16(word address, byte length, byte* p_data);

        void readBuffer_24C01_02(byte address, byte length, byte* p_data);
        void readBuffer_24C04_16(word address, byte length, byte* p_data);

};

#endif 