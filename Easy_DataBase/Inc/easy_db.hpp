#ifndef __EASY_DB_HPP_
#define __EASY_DB_HPP_

#include "easy_db_row.hpp"
#include <cstdio>

#if defined(__arm__)
#include "fatfs.h"
#endif

#define EDB_CELL_ARRAY_LEN(X) (sizeof(X)/sizeof(Easy_DB_Cell))
#define EDB_NAMEBUFFER_SIZE 64

class EasyDataBase
{
private:
    char Name[EDB_NAMEBUFFER_SIZE];

    bool Enabled = false;

    EasyDB_Row Row;
    uint32_t Capacity = 0;
    uint32_t WriteIndex = 0;

    uint32_t SelectedMinId = 0;
    uint32_t SelectedMaxId = 0;

    uint32_t SelectedMinIndex = 0;
    uint32_t SelectedMaxIndex = 0;
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

    // Получить число выделенных строк
    uint32_t GetSelectedRowCount(void)
    {
        return SelectedRowCount;
    }

    // Получение маскимального числа строк БД
    uint32_t GetCapacity(void)
    {
        return Capacity;
    }

    // Получение количества строк в БД
    uint32_t GetRowCount(void)
    {
    	if (Select() == -1)
    	{
    		return 0;
    	}

    	return GetSelectedRowCount();
    }

    // Чтение первой строки БД
    int8_t ReadFirstRow(void)
    {
    	if (Select() == -1)
    	{
    		return -1;
    	}

    	return ReadSelectedRow(0);
    }

    // Чтение последней строки БД
    int8_t ReadLastRow(void)
    {
    	if (Select() == -1)
    	{
    		return -1;
    	}

    	return ReadSelectedRow(GetSelectedRowCount() - 1);
    }



    Easy_DB_Cell *GetRowCell(uint32_t index);
    Easy_DB_Cell *RowCells(void);
    int8_t Init(void);
    int8_t WriteRow(void);
    int8_t Select(void);
    int8_t Select(Easy_DB_DateTime *start, Easy_DB_DateTime *end, uint8_t dtColIndex = 0);
    int8_t ReadSelectedRow(uint32_t index);
    int8_t Clear(void);
};



#endif
