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
    
public:
    EasyDB_Row Row;
    uint32_t Capacity;

    uint32_t SelectedMinId;
    uint32_t SelectedMaxId;
    uint32_t SelectedRowCount;


    // Конструктор класса
    EasyDataBase(const char *db_name, Easy_DB_Cell *row, uint8_t column_number, uint32_t capacity)
    {
        sprintf(Name, db_name);
        Row.ColumnNumber = column_number;
        Row.Data = row;
        Capacity = capacity;
    }


    void Init(void);
    Easy_DB_Cell *GetRowCell(uint32_t index);
    int8_t WriteRow(void);
    

    
};



#endif