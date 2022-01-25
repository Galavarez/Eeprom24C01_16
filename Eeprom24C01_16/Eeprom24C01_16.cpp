#include <Arduino.h>
#include <Wire.h>

#include <Eeprom24C01_16.h>

#define EEPROM__RD_BUFFER_SIZE    BUFFER_LENGTH

#define EEPROM__WR_BUFFER_SIZE    (BUFFER_LENGTH - 1)



/******************************************************************************
Size Memory:
AT24C01 - 128 byte
AT24C02 - 256 byte
AT24C04 - 512 byte
AT24C08 - 1024 byte
AT24C16 - 2048 byte
******************************************************************************/

Eeprom24C01_16::Eeprom24C01_16(byte deviceAddress)
{    
    m_deviceAddress = deviceAddress;
}

/***************************************** initialize *****************************************/

void Eeprom24C01_16::initialize()
{    
    Wire.begin();
}

/***************************************** writeByte *****************************************/

void Eeprom24C01_16::writeByte_24C01_02(byte address, byte data)
{
    Wire.beginTransmission(m_deviceAddress);
    Wire.write(address);
    Wire.write(data);
    Wire.endTransmission();
}

void Eeprom24C01_16::writeByte_24C04_16(word address, byte data)
{
    Wire.beginTransmission((byte)(m_deviceAddress | ((address >> 8) & 0x07)));
    Wire.write(address & 0xFF);
    Wire.write(data);
    Wire.endTransmission();
}

/***************************************** writeBytes *****************************************/


void Eeprom24C01_16::writeBytes_24C01_02(byte address, byte length, byte* p_data)
{
    byte EEPROM__PAGE_SIZE = 8;
    
    // Write first page if not aligned.
    byte notAlignedLength = 0;
    byte pageOffset = address % 8;
    if (pageOffset > 0)
    {
        notAlignedLength = EEPROM__PAGE_SIZE - pageOffset;
        if (length < notAlignedLength)
        {
            notAlignedLength = length;
        }
        writePage_24C01_02(address, notAlignedLength, p_data);
        length -= notAlignedLength;
    }

    if (length > 0)
    {
        address += notAlignedLength;
        p_data += notAlignedLength;

        // Write complete and aligned pages.
        byte pageCount = length / EEPROM__PAGE_SIZE;
        for (byte i = 0; i < pageCount; i++)
        {
            writePage_24C01_02(address, EEPROM__PAGE_SIZE, p_data);
            address += EEPROM__PAGE_SIZE;
            p_data += EEPROM__PAGE_SIZE;
            length -= EEPROM__PAGE_SIZE;
        }

        if (length > 0)
        {
            // Write remaining uncomplete page.
            writePage_24C01_02(address, length, p_data);
        }
    }
}

void Eeprom24C01_16::writeBytes_24C04_16(word address, word length, byte* p_data)
{
    byte EEPROM__PAGE_SIZE = 16;

    // Write first page if not aligned.
    byte notAlignedLength = 0;
    byte pageOffset = address % EEPROM__PAGE_SIZE;
    if (pageOffset > 0)
    {
        notAlignedLength = EEPROM__PAGE_SIZE - pageOffset;
        if (length < notAlignedLength)
        {
            notAlignedLength = length;
        }
        writePage_24C04_16(address, notAlignedLength, p_data);
        length -= notAlignedLength;
    }

    if (length > 0)
    {
        address += notAlignedLength;
        p_data += notAlignedLength;

        // Write complete and aligned pages.
        byte pageCount = length / EEPROM__PAGE_SIZE;
        for (byte i = 0; i < pageCount; i++)
        {
            writePage_24C04_16(address, EEPROM__PAGE_SIZE, p_data);
            address += EEPROM__PAGE_SIZE;
            p_data += EEPROM__PAGE_SIZE;
            length -= EEPROM__PAGE_SIZE;
        }

        if (length > 0)
        {
            // Write remaining uncomplete page.
            writePage_24C04_16(address, length, p_data);
        }
    }
}

/***************************************** readByte *****************************************/

byte Eeprom24C01_16::readByte_24C01_02(byte address)
{
    Wire.beginTransmission(m_deviceAddress);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(m_deviceAddress, (byte)1);
    byte data = 0;
    if (Wire.available())
    {
        data = Wire.read();
    }
    return data;
}


byte Eeprom24C01_16::readByte_24C04_16(word address)
{
    Wire.beginTransmission((byte)(m_deviceAddress | ((address >> 8) & 0x07)));
    Wire.write(address & 0xFF);
    Wire.endTransmission();
    Wire.requestFrom((byte)(m_deviceAddress | ((address >> 8) & 0x07)), (byte)1);
    byte data = 0;
    if (Wire.available())
    {
        data = Wire.read();
    }
    return data;
}

/***************************************** readBytes *****************************************/

void Eeprom24C01_16::readBytes_24C01_02(byte address, byte length, byte* p_data)
{
 
    byte bufferCount = length / EEPROM__RD_BUFFER_SIZE;
    for (byte i = 0; i < bufferCount; i++)
    {
        byte offset = i * EEPROM__RD_BUFFER_SIZE;
        readBuffer_24C01_02(address + offset, EEPROM__RD_BUFFER_SIZE, p_data + offset);
    }

    byte remainingBytes = length % EEPROM__RD_BUFFER_SIZE;
    byte offset = length - remainingBytes;
    readBuffer_24C01_02(address + offset, remainingBytes, p_data + offset);
}


void Eeprom24C01_16::readBytes_24C04_16(word address, word length, byte* p_data)
{
    byte bufferCount = length / EEPROM__RD_BUFFER_SIZE;
    for (byte i = 0; i < bufferCount; i++)
    {
        word offset = i * EEPROM__RD_BUFFER_SIZE;
        readBuffer_24C04_16(address + offset, EEPROM__RD_BUFFER_SIZE, p_data + offset);
    }

    byte remainingBytes = length % EEPROM__RD_BUFFER_SIZE;
    word offset = length - remainingBytes;
    readBuffer_24C04_16(address + offset, remainingBytes, p_data + offset);
}

/******************************************************************************
 * Private method definitions.
 ******************************************************************************/


/***************************************** writePage *****************************************/

void Eeprom24C01_16::writePage_24C01_02(byte address, byte length, byte* p_data)
{
    // Write complete buffers.
    byte bufferCount = length / EEPROM__WR_BUFFER_SIZE;
    for (byte i = 0; i < bufferCount; i++)
    {
        byte offset = i * EEPROM__WR_BUFFER_SIZE;
        writeBuffer_24C01_02(address + offset, EEPROM__WR_BUFFER_SIZE, p_data + offset);
    }

    // Write remaining bytes.
    byte remainingBytes = length % EEPROM__WR_BUFFER_SIZE;
    byte offset = length - remainingBytes;
    writeBuffer_24C01_02(address + offset, remainingBytes, p_data + offset);
}

void Eeprom24C01_16::writePage_24C04_16(word address, byte length, byte* p_data)
{
    // Write complete buffers.
    byte bufferCount = length / EEPROM__WR_BUFFER_SIZE;
    for (byte i = 0; i < bufferCount; i++)
    {
        byte offset = i * EEPROM__WR_BUFFER_SIZE;
        writeBuffer_24C04_16(address + offset, EEPROM__WR_BUFFER_SIZE, p_data + offset);
    }

    // Write remaining bytes.
    byte remainingBytes = length % EEPROM__WR_BUFFER_SIZE;
    byte offset = length - remainingBytes;
    writeBuffer_24C04_16(address + offset, remainingBytes, p_data + offset);
}


/***************************************** writeBuffer *****************************************/

void Eeprom24C01_16::writeBuffer_24C01_02(byte address, byte length, byte* p_data)
{
    Wire.beginTransmission(m_deviceAddress);
    Wire.write(address);
    for (byte i = 0; i < length; i++)
    {
        Wire.write(p_data[i]);
    }
    Wire.endTransmission();
    
    // Write cycle time (tWR). See EEPROM memory datasheet for more details.
    delay(10);
}

void Eeprom24C01_16::writeBuffer_24C04_16(word address, byte length, byte* p_data)
{
    Wire.beginTransmission((byte)(m_deviceAddress | ((address >> 8) & 0x07)));
    Wire.write(address & 0xFF);
    for (byte i = 0; i < length; i++)
    {
        Wire.write(p_data[i]);
    }
    Wire.endTransmission();
    
    // Write cycle time (tWR). See EEPROM memory datasheet for more details.
    delay(10);
}

/***************************************** readBuffer *****************************************/

void Eeprom24C01_16::readBuffer_24C01_02(byte address, byte length, byte* p_data)
{
    Wire.beginTransmission(m_deviceAddress);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(m_deviceAddress, length);
    for (byte i = 0; i < length; i++)
    {
        if (Wire.available())
        {
            p_data[i] = Wire.read();
        }
    }
}

void Eeprom24C01_16::readBuffer_24C04_16(word address, byte length, byte* p_data)
{
    Wire.beginTransmission((byte)(m_deviceAddress | ((address >> 8) & 0x07)));
    Wire.write(address & 0xFF);
    Wire.endTransmission();
    Wire.requestFrom((byte)(m_deviceAddress | ((address >> 8) & 0x07)), length);
    for (byte i = 0; i < length; i++)
    {
        if (Wire.available())
        {
            p_data[i] = Wire.read();
        }
    }
}