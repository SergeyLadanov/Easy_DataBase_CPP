#ifndef __EASY_DB_ROW_HPP_
#define __EASY_DB_ROW_HPP_

#include "easy_db_cell.hpp"

#define DEBUG_ROW 0

class EasyDB_Row
{
public:
    Easy_DB_Cell *Cells;
    uint32_t RecordId;
    uint8_t ColumnNumber;
private:

    uint16_t Crc16(uint8_t *in, uint32_t len)
    {
        uint16_t crc = 0x1D0F;
        uint8_t i;

        while (len--)
        {
            crc ^= *in++ << 8;

            for (i = 0; i < 8; i++)
                crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
        }
        return crc;
    }
    
    uint16_t CRC16;

public:

    // Преобразование строки БД в массив байт
    uint32_t Serialize(uint8_t *out)
    {
        uint8_t *ptr = out;
        memcpy(ptr, &RecordId, sizeof(RecordId));
        ptr += sizeof(RecordId);

        for (uint32_t i = 0; i < ColumnNumber; i++)
        {
            ptr += Cells[i].Serialize(ptr);
        }

        // Расчет контрольной суммы записи
        CRC16 = Crc16(out, Size() - sizeof(CRC16));
        memcpy(ptr, &CRC16, sizeof(CRC16));

        #if DEBUG_ROW != 0
        printf("Row size in bytes (serialized): %d\r\n", Size());
        #endif

        return Size();
    }

    // Преобразование массива байт в строку БД
    uint32_t DeSerialize(uint8_t *in)
    {
        uint8_t *ptr = in;
        uint16_t check_crc;
        memcpy(&RecordId, ptr, sizeof(RecordId));
        ptr += sizeof(RecordId);

        for (uint32_t i = 0; i < ColumnNumber; i++)
        {
            ptr += Cells[i].DeSerialize(ptr);
        }

        memcpy(&CRC16, ptr, sizeof(CRC16));

        check_crc = Crc16(in, Size() - sizeof(CRC16));

        #if DEBUG_ROW != 0
        printf("Row size in bytes (deserialized): %d\r\n", Size());
        #endif

        if (check_crc == CRC16)
        {
            return Size();
        }

        return 0;
    }

    // Размер строки в байтах
    uint32_t Size(void)
    {
        uint32_t result = 0;
        result += sizeof(RecordId);

        for (uint32_t i = 0; i < ColumnNumber; i++)
        {
            result += Cells[i].Size();;
        }

        result += sizeof(CRC16);

        return result;
    }


};



#endif