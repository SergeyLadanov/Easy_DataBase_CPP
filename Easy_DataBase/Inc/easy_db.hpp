#ifndef __EASY_DB_HPP_
#define __EASY_DB_HPP_

#include "easy_db_row.hpp"
#include <cstdio>

#if defined(__arm__)
#include "fatfs.h"
#endif


#define EDB_NAMEBUFFER_SIZE 64

class EasyDataBase
{
private:
    #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
    FILE *File;
    #endif

    #if defined(__arm__)
    FIL File;
    #endif

    char Name[EDB_NAMEBUFFER_SIZE];
    

    EasyDB_Row Row;

    uint32_t MinId;
    uint32_t MaxId;
    uint32_t RowCount;

public:
    // Конструктор класса
    EasyDataBase(char *db_name, Easy_DB_Cell *row, uint8_t column_number)
    {
        sprintf(Name, db_name);
        Row.ColumnNumber = column_number;
        Row.Data = row;
    }

    
};



#endif