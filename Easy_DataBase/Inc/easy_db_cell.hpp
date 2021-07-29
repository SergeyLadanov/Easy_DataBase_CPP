#ifndef __EASY_DB_CELL_HPP_
#define __EASY_DB_CELL_HPP_

#include <cstdint>
#include <cstdio>
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
    uint8_t Type = 0;

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
                return sizeof(Value.Dt);
            break;
        }

        return 0;
    }

    // Преобразование значения в массив байт
    uint32_t Serialize(uint8_t *out)
    {

        switch (Type)
        {
            case EDB_STRING :
                memcpy(out, Value.Str.Buf, Size());
                break;

            default:
                memcpy(out, &Value, Size());
            break;
        }

        return Size();
    }

    // Преобразование массива байт в значение
    uint32_t DeSerialize(uint8_t *in)
    {
        switch (Type)
        {
            case EDB_STRING :
                memcpy(Value.Str.Buf, in, Size());
                break;

            default:
                memcpy(&Value, in, Size());
            break;
        }

        return Size();
    }

    // Установка значения
    inline void SetValue(float val)
    {
        Value.F32 = val;
    }

    inline void SetValue(double val)
    {
        Value.Double = val;
    }

    inline void SetValue(int8_t val)
    {
        Value.I8 = val;
    }


    inline void SetValue(int16_t val)
    {
        Value.I16 = val;
    }

    inline void SetValue(int32_t val)
    {
        Value.I32 = val;
    }

    inline void SetValue(int64_t val)
    {
        Value.I64 = val;
    }

    inline void SetValue(uint8_t val)
    {
        Value.UI8 = val;
    }

    inline void SetValue(uint16_t val)
    {
        Value.UI16 = val;
    }


    inline void SetValue(uint32_t val)
    {
        Value.UI32 = val;
    }

    inline void SetValue(uint64_t val)
    {
        Value.UI64 = val;
    }


    inline void SetValue(Easy_DB_DateTime val)
    {
        Value.Dt = val;
    }


    inline void SetValue(char *str)
    {
        sprintf(Value.Str.Buf, str);
    }


    inline void SetValue(bool val)
    {
        Value.BoolVal = val;
    }




    // Получение значения
    inline float GetFloat(void)
    {
        return Value.F32;
    }


    inline double GetDouble(void)
	{
		return Value.Double;
	}

	inline int8_t GetInt8(void)
	{
		return Value.I8;
	}


	inline int16_t GetInt16(void)
	{
		return Value.I16;
	}

	inline int32_t GetInt32(void)
	{
		return Value.I32;
	}

	inline int64_t GetInt64(void)
	{
		return Value.I64;
	}

	inline uint8_t GetUint8(void)
	{
		return Value.UI8;
	}

	inline uint16_t GetUint16(void)
	{
		return Value.UI16;
	}

    inline uint32_t GetUint32(void)
    {
        return Value.UI32;
    }

    inline uint64_t GetUint64(void)
    {
        return Value.UI64;
    }


    inline Easy_DB_DateTime GetDateTime(void)
    {
        return Value.Dt;
    }


    inline char *GetString(void)
    {
    	return Value.Str.Buf;
    }

    inline bool GetBool(void)
    {
        return Value.BoolVal;
    }

};


#endif
