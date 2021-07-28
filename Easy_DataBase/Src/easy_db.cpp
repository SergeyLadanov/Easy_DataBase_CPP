#include "easy_db.hpp"    

// Загрузка инфомации о БД    
void EasyDataBase::Init(void)
{
    
}


// Получение ячейки строки
Easy_DB_Cell *EasyDataBase::GetRowCell(uint32_t index)
{
    if (index >= Row.ColumnNumber)
    {
        return nullptr;
    }

    return &Row.Data[index];

}


// Запись строки в файл
int8_t EasyDataBase::WriteRow(void)
{
    
    return 0;
}