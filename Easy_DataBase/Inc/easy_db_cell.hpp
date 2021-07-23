#ifndef __EASY_DB_CELL_HPP_
#define __EASY_DB_CELL_HPP_

#include <cstdint>
#include <cstring>



typedef struct Easy_DB_String
{
    char *Str;
private:
    uint16_t BuffSize;
public:

    // Получение длины строки
    uint16_t Length(void)
    {
        return strlen(Str);
    }

    // Получение размера буфера строки
    uint16_t BufferSize(void)
    {
        return BuffSize;
    }
};



class Easy_DB_Cell
{
private:
    uint8_t Type;

    union Value
    {
        float F32;
        double Double;

    };
    
    
public:


}


#endif