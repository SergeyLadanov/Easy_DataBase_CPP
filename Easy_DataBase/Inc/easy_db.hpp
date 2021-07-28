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
    uint32_t Capacity = 0;
    uint32_t WriteIndex = 0;

    uint32_t SelectedMinId = 0;
    uint32_t SelectedMaxId = 0;
    uint32_t SelectedRowCount = 0;

public:

    // Конструктор класса
    EasyDataBase(const char *db_name, Easy_DB_Cell *rowCells, uint8_t column_number, uint32_t capacity)
    {
        sprintf(Name, "%s.edb", db_name);
        Row.ColumnNumber = column_number;
        Row.Cells = rowCells;
        Row.RecordId = 0;
        Capacity = capacity;
    }

    // Получение числа колонок
    uint8_t GetColumNumber(void)
    {
        return Row.ColumnNumber;
    }

    Easy_DB_Cell *GetRowCell(uint32_t index);
    Easy_DB_Cell *RowCells(void);
    int8_t Init(void);
    int8_t WriteRow(void);
    int8_t Select(void);
    Easy_DB_Cell *ReadSelectedRow(void);
    int8_t EndSelect(void);
};



#endif