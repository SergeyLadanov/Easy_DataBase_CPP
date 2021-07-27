#ifndef __EASY_DB_ROW_HPP_
#define __EASY_DB_ROW_HPP_

#include "easy_db_cell.hpp"

class EasyDB_Row
{
public:
    Easy_DB_Cell *Data;
    uint32_t RecordId;
    uint8_t ColumnNumber;
private:

    uint16_t Crc16(uint8_t *pcBlock, uint32_t len)
    {
        uint16_t crc = 0x1D0F;
        uint8_t i;

        while (len--)
        {
            crc ^= *pcBlock++ << 8;

            for (i = 0; i < 8; i++)
                crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
        }
        return crc;
    }
    
    uint16_t CRC16;

    // Преобразование строки БД в массив байт
    uint32_t Serialize(uint8_t *out)
    {
        memcpy(out, &RecordId, sizeof(RecordId));
        out += sizeof(RecordId);

        for (uint32_t i = 0; i < ColumnNumber; i++)
        {
            out += Data[i].Serialize(out);
        }
    }

    // массива байт в строку БД
    uint32_t DeSerialize(uint8_t *in)
    {
        memcpy(&RecordId, in, sizeof(RecordId));
        in += sizeof(RecordId);

        for (uint32_t i = 0; i < ColumnNumber; i++)
        {
            in += Data[i].DeSerialize(in);
        }
    }

};



#endif