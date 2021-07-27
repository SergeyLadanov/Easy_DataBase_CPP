#ifndef __EASY_DB_CELL_HPP_
#define __EASY_DB_CELL_HPP_

#include <cstdint>
#include "easy_db_string.hpp"
#include "easy_db_datetime.hpp"

// Типы значений
typedef enum {
	EDB_FLOAT = 1,
	EDB_UINT64,
	EDB_UINT32,
	EDB_UINT16,
	EDB_UINT8,
	EDB_BOOL,
	EDB_STRING,
	EDB_DOUBLE,
	EDB_DATE_TIME
}EDB_DataType;


// Класс ячейки с данными
class Easy_DB_Cell
{    
public:
    uint8_t Type;
    union __Value
    {
        float F32;
        double Double;
        Easy_DB_String Str;
        Easy_DB_DateTime Dt;
        uint64_t UI64;
        uint32_t UI32;
        uint16_t UI16;
        uint8_t UI8;
        int64_t I64;
        int32_t I32;
        int16_t I16;
        int8_t I8;
        bool BoolVal;
    }Value;

    // Размер значения в байтах
    uint32_t Size(void)
    {
        switch(Type)
        {
            case EDB_FLOAT :
                return sizeof(Value.F32);
            break;

            case EDB_UINT64 :
                return sizeof(Value.UI64);
            break;

            case EDB_UINT32 :
                return sizeof(Value.UI32);
            break;

            case EDB_UINT16 :
                return sizeof(Value.UI16);
            break;

            case EDB_UINT8 :
                return sizeof(Value.UI8);
            break;

            case EDB_BOOL :
                return sizeof(Value.BoolVal);
            break;

            case EDB_STRING :
                return (uint32_t) Value.Str.BufferSize();
            break;

            case EDB_DOUBLE :
                return sizeof(Value.Double);
            break;

            case EDB_DATE_TIME :
                return (sizeof(Value.Dt.SDate) + sizeof(Value.Dt.STime));
            break;
        }

        return 0;
    }

    // Преобразование значения в массив байт
    uint32_t Serialize(uint8_t *out)
    {
        memcpy(out, &Value, Size());
        return Size();
    }

    // Преобразование массива байт в значение
    uint32_t DeSerialize(uint8_t *in)
    {
        memcpy(&Value, in, Size());
        return Size();
    }
};


#endif