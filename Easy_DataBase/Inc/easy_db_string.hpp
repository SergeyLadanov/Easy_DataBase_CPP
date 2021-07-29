#ifndef __EASY_DB_STRING_HPP_
#define __EASY_DB_STRING_HPP_

#include <cstring>
#include <cstdint>


class Easy_DB_String
{
public:
    char *Buf = nullptr;
private:
    uint16_t BuffSize = 0;
public:

    // Получение длины строки
    uint16_t Length(void)
    {
        return strlen(Buf);
    }

    // Получение размера буфера строки
    uint16_t BufferSize(void)
    {
        return BuffSize;
    }

    // Инициализация строки
    void Init(char *buf, uint16_t len)
    {
        Buf = buf;
        BuffSize = len;
    }

};



#endif
