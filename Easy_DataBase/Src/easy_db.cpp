#include "easy_db.hpp"    

// Загрузка инфомации о БД    
int8_t EasyDataBase::Init(void)
{

    // Создание 
    #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
        File = fopen(Name, "rb");
        if (File == nullptr)
        {
            File = fopen(Name, "wb");

            if (File == nullptr)
            {
                printf("Failed to create database!\r\n");
                return -1;
            }
        }

        fclose(File);

    #endif

    #if defined(__arm__)
    
    #endif


    return 0;
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
    uint8_t *writeBuffer = new uint8_t[Row.Size()];
    Row.Serialize(writeBuffer);

    #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
    File = fopen(Name, "r+b");
    

    if (File == nullptr)
    {
        printf("Failed to open file for writing row\r\n");
        return -1;
    }

    fseek(File, Row.Size() * WriteIndex, SEEK_SET);
    fwrite(writeBuffer, Row.Size(), sizeof(uint8_t), File);
    fclose(File);

    #endif


    #if defined(__arm__)
    
    #endif

    WriteIndex = (WriteIndex + 1) % Capacity;
    Row.RecordId = (Row.RecordId + 1) % (Capacity * 2);

    delete[] writeBuffer;

    return 0;
}