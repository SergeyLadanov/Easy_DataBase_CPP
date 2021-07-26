#ifndef __EASY_DB_CELL_HPP_
#define __EASY_DB_CELL_HPP_

#include <cstdint>
#include "easy_db_string.hpp"
#include "easy_db_datetime.hpp"

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

    }Value;


};


#endif