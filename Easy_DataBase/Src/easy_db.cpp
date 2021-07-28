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

    return &Row.Cells[index];
}

// Указатель на набор ячеек
Easy_DB_Cell *EasyDataBase::RowCells(void)
{
    return Row.Cells;
}

// Выделение строк
int8_t EasyDataBase::Select(void)
{
    uint8_t *readBuffer = new uint8_t[Row.Size()];
    uint32_t read_bytes = 0;
    int8_t status = 0;

    SelectedMinId = 0xFFFFFFFF;
    SelectedMaxId = 0;
    SelectedRowCount = 0;

    #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
    File = fopen(Name, "rb");

    if (File == nullptr)
    {
        printf("Failed to open file for selecting\r\n");
        status = -1;
    }
    #endif

    #if defined(__arm__)
        
    #endif

    else
    {

        for (uint32_t i = 0; i < Capacity; i++)
        {
            #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
            read_bytes = fread(readBuffer, sizeof(uint8_t), Row.Size(), File);
            #endif

            #if defined(__arm__)
        
            #endif

            if (read_bytes != Row.Size())
            {
                break;
            }


            if (Row.DeSerialize(readBuffer) != Row.Size())
            {
                printf("Record checksum error\r\n");
                continue;
            }

            SelectedRowCount++;

            if (Row.RecordId > SelectedMaxId)
            {
                SelectedMaxId = Row.RecordId;
            }

            if (Row.RecordId < SelectedMinId)
            {
                SelectedMinId = Row.RecordId;
            }
        }

        #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
        fclose(File);
        #endif


    }
  
    delete[] readBuffer;

    return status;
}

// Выделение записей по дате и времени, с указанием колонки, содержащей дату и время
int8_t EasyDataBase::Select(Easy_DB_DateTime *start, Easy_DB_DateTime *end, uint8_t dtColIndex = 0)
{
    
}


// Чтение выделенной строки
int8_t EasyDataBase::ReadSelectedRow(uint32_t index)
{
    uint8_t *readBuffer = new uint8_t[Row.Size()];
    uint32_t memIndex = ((SelectedMinId % Capacity) + index) % Capacity;
    uint32_t read_bytes = 0;
    int8_t status = 0;

    for (uint32_t i = 0; i < Capacity; i++)
    {
        #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
        File = fopen(Name, "rb");

        if (File == nullptr)
        {
            printf("Failed to open file for writing row\r\n"); 
            status = -1;
            break;
        }

        fseek(File, Row.Size() * memIndex, SEEK_SET);

        read_bytes = fread(readBuffer, sizeof(uint8_t), Row.Size(), File);

        fclose(File);

        #endif

        #if defined(__arm__)
        
        #endif

        if (read_bytes != Row.Size())
        {
            printf("Read bytes no match with rowsize!\r\n");
            printf("Read bytes: %d\r\n", read_bytes);
            printf("Row size: %d\r\n", Row.Size());
            status = -1;
            break;
        }

        

        if (Row.DeSerialize(readBuffer) != Row.Size())
        {
            printf("Record checksum error\r\n");
            memIndex = (memIndex + 1) % Capacity;
            continue;
        }
        else
        {
            printf("Row id: %d\r\n", Row.RecordId);
            break;
        }

    }

    delete[] readBuffer;
    
    return status;
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
    fwrite(writeBuffer, sizeof(uint8_t), Row.Size(), File);
    fclose(File);

    #endif


    #if defined(__arm__)
    
    #endif

    WriteIndex = (WriteIndex + 1) % Capacity;
    Row.RecordId = (Row.RecordId + 1) % (Capacity * 2);

    delete[] writeBuffer;

    return 0;
}