# Eeprom24C01_16

Eeprom24C01_16 библиотека для Arduino которая может работать с Eeprom микросхем AT24C01 по ATAT24C16 и их аналогом 5ME3.
Библиотека Eeprom24C01_16 сделанна из 2х других библиотек (я просто их объеденил в одну):
1. https://github.com/jlesech/Eeprom24C01_02
2. https://github.com/jlesech/Eeprom24C04_08_16

Спасибо за их создание jlesech.


# Как пользоваться

#include <Eeprom24C01_16.h>

// Если размер чипа (size_dump) до 256 байт то изпользуем writeBytes_24C01_02 иначе writeBytes_24C04_16

if (size_dump <= 256) 
{
  eeprom.writeBytes_24C01_02(address, count, array_bytes); 
}
else
{
  eeprom.writeBytes_24C04_16(address, count, array_bytes); 
}

# Переменные и функции в библиотеке
void writeByte_24C01_02(byte address, byte data);
void writeByte_24C04_16(word address, byte data);

void writeBytes_24C01_02(byte address, byte length, byte* p_data);
void writeBytes_24C04_16(word address, word length, byte* p_data);

byte readByte_24C01_02(byte address);
byte readByte_24C04_16(word address);

void readBytes_24C01_02(byte address, byte length, byte* p_data);
void readBytes_24C04_16(word address, word length, byte* p_data);

void writePage_24C01_02(byte address, byte length, byte* p_data);
void writePage_24C04_16(word address, byte length, byte* p_data);

void writeBuffer_24C01_02(byte address, byte length, byte* p_data);
void writeBuffer_24C04_16(word address, byte length, byte* p_data);

void readBuffer_24C01_02(byte address, byte length, byte* p_data);
void readBuffer_24C04_16(word address, byte length, byte* p_data);
