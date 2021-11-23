#include "easy_db.hpp"


#if USE_DEBUG_EDB != 0
#include "debug_log.h"
#define EDB_LOG(...) DEBUG_LOG(__VA_ARGS__)
#else
#define EDB_LOG(...)
#endif


// Загрузка инфомации о БД
int8_t EasyDataBase::Init(void)
{
    int8_t status = 0;
    // Создание
    #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
    FILE *File;
    File = fopen(Name, "rb");
    if (File == nullptr)
    {
        File = fopen(Name, "wb");

        if (File == nullptr)
        {
            #if DEBUG_EDB != 0
            printf("Failed to create database!\r\n");
            #endif
            return -1;
        }
    }

    if (fclose(File))
    {
        #if DEBUG_EDB != 0
        printf("Failed to close file!\r\n");
        #endif
    }

    #endif

    #if defined(__arm__)
    FILINFO fno;


    if (f_stat(Name, &fno) != FR_OK)
    {
    	FIL *File = new FIL;

    	if (f_open(File, Name, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
    	{

    		EDB_LOG("Failed to create easy database file\r\n");

			status = -1;

    	}
    	else
    	{
            int tries = 5;
            FRESULT fres = FR_OK;
            do
            {
                fres = f_close(File);
                if (tries-- <= 0)
                {
                	EDB_LOG("Failed to close file!\r\n");
                	status = -1;
                	break;
                }
            }
            while(fres != FR_OK);
    	}



    	delete File;

    	if (status == -1)
    	{
    		return status;
    	}
    }

    #endif

    Enabled = true;


    status = Select();

    if (SelectedRowCount > 0)
    {
        RecordId = (SelectedMaxId + 1) % (2 * Capacity);
        WriteIndex = (SelectedMaxIndex + 1) % Capacity;
    }


    return status;
}

// Очистка базы данных
int8_t EasyDataBase::Clear(void)
{
	int8_t status = 0;

	if (!Enabled)
	{
		return -1;
	}

	#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
	FILE *File;
	File = fopen(Name, "wb");
	if (File == nullptr)
	{
		EDB_LOG("Failed to open file for clear\r\n");
		status = -1;
	}
	else
	{
		if (fclose(File))
        {
			EDB_LOG("Failed to close file for clear\r\n");
        }
	}

	#endif


	#if defined(__arm__)
	FIL *File = new FIL;

	if (f_open(File, Name, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
	{
		EDB_LOG("Failed to open file for clear\r\n");
		status = -1;
	}
	else
	{
        int tries = 5;
        FRESULT fres = FR_OK;
        do
        {
            fres = f_close(File);
            if (tries-- <= 0)
            {
            	EDB_LOG("Failed to close file for clear\r\n");
            	status = -1;
            	break;
            }
        }
        while(fres != FR_OK);
	}

	delete File;

	#endif

	if (status == 0)
	{
		RecordId = 0;
		WriteIndex = 0;
	}

	return status;
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

    if (!Enabled)
    {
        return -1;
    }

    uint8_t *readBuffer = new uint8_t[Row.Size()];
    uint32_t read_bytes = 0;
    int8_t status = 0;

    SelectedMinId = 0xFFFFFFFF;
    SelectedMaxId = 0;
    SelectedRowCount = 0;

    uint32_t id_bkp = 0;
    bool first_iteration = true;
    bool founded = false;





    #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
    FILE *File;
    File = fopen(Name, "rb");

    if (File == nullptr)
    {
    	EDB_LOG("Failed to open file for selecting\r\n");
        status = -1;
    }
    #endif

    #if defined(__arm__)
    FIL *File = new FIL;

    if (f_open(File, Name, FA_READ) != FR_OK)
    {
    	EDB_LOG("Failed to open file for selecting\r\n");

		status = -1;
    }
    #endif

    else
    {

        for (uint32_t i = 0; i < Capacity; i++)
        {
            #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
            read_bytes = fread(readBuffer, sizeof(uint8_t), Row.Size(), File);
            #endif

            #if defined(__arm__)
            if (f_read(File, readBuffer, Row.Size(), (UINT *) &read_bytes) != FR_OK)
            {
            	status = -1;
            	break;
            }
            #endif

            if (read_bytes != Row.Size())
            {
                break;
            }


            if (Row.DeSerialize(readBuffer) != Row.Size())
            {
            	EDB_LOG("Record checksum error\r\n");
                continue;
            }

            SelectedRowCount++;
            EDB_LOG("Read selectid in select:%d\r\n", (int) Row.RecordId);

            if (first_iteration)
            {
            	id_bkp = Row.RecordId;
                SelectedMinId = id_bkp;
            	first_iteration = false;
                SelectedMinIndex = i;
            }
            else
            {
                if (!founded)
                {
                    if (((id_bkp + 1) % (2 * Capacity)) != Row.RecordId)
                    {

                    	EDB_LOG("Founed!\r\n");
                    	EDB_LOG("Previous Id: %d\r\n", (int) id_bkp);
                    	EDB_LOG("New id: %d\r\n", (int) Row.RecordId);


                        SelectedMaxIndex = i - 1;
                        SelectedMinIndex = i;
                        SelectedMaxId = id_bkp;
                        SelectedMinId = Row.RecordId;
                        founded = true;

                        EDB_LOG("Found MaxIndex: %d\r\n", (int) SelectedMaxIndex);
                        EDB_LOG("Found MinIndex: %d\r\n", (int) SelectedMinIndex);
                        EDB_LOG("Found MaxId: %d\r\n", (int) SelectedMaxId);
                        EDB_LOG("Found MinId: %d\r\n", (int) SelectedMinId);
                        
                    }
                    else
                    {
                        SelectedMaxIndex = i;
                        SelectedMaxId = Row.RecordId;
                        id_bkp = Row.RecordId;
                    }
                }
            }

        }

        #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
        if (fclose(File))
        {
        	EDB_LOG("Failed to close file in selecting!\r\n");
        }
        #endif

		#if defined(__arm__)

        int tries = 5;
        FRESULT fres = FR_OK;
        do
        {
            fres = f_close(File);
            if (tries-- <= 0)
            {
            	EDB_LOG("Failed to close file in selecting!\r\n");
            	status = -1;
            	break;
            }
        }
        while(fres != FR_OK);

        delete File;

		#endif


    }
    
    EDB_LOG("\r\n------------Reuslt-----------\r\n\r\n");

    EDB_LOG("Found MaxIndex: %d\r\n", (int) SelectedMaxIndex);
    EDB_LOG("Found MinIndex: %d\r\n", (int) SelectedMinIndex);
    EDB_LOG("Found MaxId: %d\r\n", (int) SelectedMaxId);
    EDB_LOG("Found MinId: %d\r\n", (int) SelectedMinId);

    EDB_LOG("\r\n------------------------------\r\n");


    delete[] readBuffer;

    return status;
}

// Выделение записей по дате и времени, с указанием колонки, содержащей дату и время
int8_t EasyDataBase::Select(Easy_DB_DateTime *start, Easy_DB_DateTime *end, uint8_t dtColIndex)
{

    if (!Enabled)
    {
        return -1;
    }

    uint8_t *readBuffer = new uint8_t[Row.Size()];
    uint32_t read_bytes = 0;
    int8_t status = 0;


    SelectedMinId = 0xFFFFFFFF;
    SelectedMaxId = 0;
    SelectedRowCount = 0;

    uint32_t id_bkp = 0;
    bool first_iteration = true;
    bool founded = false;

    #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
    FILE *File;
    File = fopen(Name, "rb");

    if (File == nullptr)
    {
    	EDB_LOG("Failed to open file for selecting\r\n");
        status = -1;
    }
    #endif

    #if defined(__arm__)
    FIL *File = new FIL;

    if (f_open(File, Name, FA_READ) != FR_OK)
    {
    	EDB_LOG("Failed to open file for selecting\r\n");

		status = -1;
    }
    #endif

    else
    {

        for (uint32_t i = 0; i < Capacity; i++)
        {
            #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
            read_bytes = fread(readBuffer, sizeof(uint8_t), Row.Size(), File);
            #endif

			#if defined(__arm__)
			if (f_read(File, readBuffer, Row.Size(), (UINT *) &read_bytes) != FR_OK)
			{
				status = -1;
				break;
			}
			#endif

            if (read_bytes != Row.Size())
            {
                break;
            }


            if (Row.DeSerialize(readBuffer) != Row.Size())
            {
            	EDB_LOG("Record checksum error\r\n");
                continue;
            }

            if ((Easy_DB_DateTime::Compare(&Row.Cells[dtColIndex].Value.Dt, start) >= 0) &&
                (Easy_DB_DateTime::Compare(&Row.Cells[dtColIndex].Value.Dt, end) <= 0))
            {
                SelectedRowCount++;

                if (first_iteration)
                {
                    id_bkp = Row.RecordId;
                    SelectedMinId = id_bkp;
                    first_iteration = false;
                    SelectedMinIndex = i;
                }
                else
                {
                    if (!founded)
                    {
                        if (((id_bkp + 1) % (2 * Capacity)) != Row.RecordId)
                        {
                        	EDB_LOG("Founed!\r\n");
                        	EDB_LOG("Previous Id: %d\r\n", (int) id_bkp);
                        	EDB_LOG("New id: %d\r\n", (int) Row.RecordId);

                            SelectedMaxIndex = i - 1;
                            SelectedMinIndex = i;
                            SelectedMaxId = id_bkp;
                            SelectedMinId = Row.RecordId;
                            founded = true;


                            EDB_LOG("Found MaxIndex: %d\r\n", (int) SelectedMaxIndex);
                            EDB_LOG("Found MinIndex: %d\r\n", (int) SelectedMinIndex);
                            EDB_LOG("Found MaxId: %d\r\n", (int) SelectedMaxId);
                            EDB_LOG("Found MinId: %d\r\n", (int) SelectedMinId);

                            
                        }
                        else
                        {
                            SelectedMaxIndex = i;
                            SelectedMaxId = Row.RecordId;
                            id_bkp = Row.RecordId;
                        }
                    }
                }

                    
            }
        }

        #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
        if (fclose(File))
        {
        	EDB_LOG("Failed to close file in selecting!\r\n");
        }
        #endif

		#if defined(__arm__)

        int tries = 5;
        FRESULT fres = FR_OK;
        do
        {
            fres = f_close(File);
            if (tries-- <= 0)
            {
            	EDB_LOG("Failed to close file in selecting!\r\n");
            	status = -1;
            	break;
            }
        }
        while(fres != FR_OK);

		delete File;

		#endif


    }

    delete[] readBuffer;

    return status;
}


// Чтение выделенной строки
int8_t EasyDataBase::ReadSelectedRow(uint32_t index)
{
    if (!Enabled)
    {
        return -1;
    }

    uint8_t *readBuffer = new uint8_t[Row.Size()];
    uint32_t memIndex = (SelectedMinIndex + index) % Capacity;
    uint32_t read_bytes = 0;
    int8_t status = 0;

    for (uint32_t i = 0; i < Capacity; i++)
    {
        #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
        FILE *File;
        File = fopen(Name, "rb");

        if (File == nullptr)
        {
        	EDB_LOG("Failed to open file for reading row\r\n");
            status = -1;
        }
        else
        {

            if (fseek(File, Row.Size() * memIndex, SEEK_SET))
            {
            	EDB_LOG("Failed to seek file\r\n");
                status = -1;
            }
            else
            {
                read_bytes = fread(readBuffer, sizeof(uint8_t), Row.Size(), File);
            }

            

            if (fclose(File))
            {
            	EDB_LOG("Failed to close file in reading\r\n");
            }
        }

        #endif

        #if defined(__arm__)

        FIL *File = new FIL;
        if (f_open(File, Name, FA_READ) != FR_OK)
        {
        	EDB_LOG("Failed to open file for reading row\r\n");
			status = -1;
        }
		else
		{

			if (f_lseek(File, Row.Size() * memIndex) != FR_OK)
			{
				EDB_LOG("Failed to seek file\r\n");
				status = -1;
			}
            else
            {
                if (f_read(File, readBuffer, Row.Size(), (UINT *) &read_bytes) != FR_OK)
                {
                	EDB_LOG("Failed to read bytes from file\r\n");
                    status = -1;
                }
            }


	        int tries = 5;
	        FRESULT fres = FR_OK;
	        do
	        {
	            fres = f_close(File);
	            if (tries-- <= 0)
	            {
	            	EDB_LOG("Failed to close file in reading\r\n");
	            	status = -1;
	            	break;
	            }
	        }
	        while(fres != FR_OK);

        }

        delete File;

        #endif

        if (status == -1)
        {
        	break;
        }

        if (read_bytes != Row.Size())
        {
        	EDB_LOG("Read bytes no match with rowsize!\r\n");
        	EDB_LOG("Read bytes: %d\r\n", (int) read_bytes);
        	EDB_LOG("Row size: %d\r\n", (int) Row.Size());
            status = -1;
            break;
        }



        if (Row.DeSerialize(readBuffer) != Row.Size())
        {
        	EDB_LOG("Record checksum error\r\n");
            memIndex = (memIndex + 1) % Capacity;
            continue;
        }
        else
        {
        	EDB_LOG("Row id: %d\r\n", (int) Row.RecordId);
            break;
        }

    }

    delete[] readBuffer;

    return status;
}

// Запись строки в файл
int8_t EasyDataBase::WriteRow(void)
{
    if (!Enabled)
    {
        return -1;
    }

    int8_t status = 0;

    uint8_t *writeBuffer = new uint8_t[Row.Size()];
    uint32_t write_bytes = 0;

    Row.RecordId = RecordId;
    Row.Serialize(writeBuffer);

    #if defined(_WIN32) || defined(_WIN64) || defined(__linux__)
    FILE *File;
    File = fopen(Name, "r+b");


    if (File == nullptr)
    {
    	EDB_LOG("Failed to open file for writing row\r\n");
        status = -1;
    }
    else
    {
		if (fseek(File, Row.Size() * WriteIndex, SEEK_SET))
        {
			EDB_LOG("Failed to seek file for writing\r\n");
            status = -1;
        }
        else
        {
            write_bytes = fwrite(writeBuffer, sizeof(uint8_t), Row.Size(), File);
        }

		if (fclose(File))
        {
			EDB_LOG("Failed to close file while writing\r\n");
        }
    }

    #endif


    #if defined(__arm__)

    FIL *File = new FIL;

    if (f_open(File, Name, FA_WRITE | FA_OPEN_EXISTING) != FR_OK)
    {
    	EDB_LOG("Failed to open file for writing row\r\n");
		status = -1;
    }
    else
    {
        if (f_lseek(File, Row.Size() * WriteIndex) != FR_OK)
        {
        	EDB_LOG("Failed to seek file\r\n");
    		status = -1;
        }
        else
        {
        	if (f_write(File, writeBuffer, Row.Size(), (UINT *) &write_bytes) != FR_OK)
        	{
        		EDB_LOG("Error of writing data\r\n");
				status = -1;
        	}
        }


        int tries = 5;
        FRESULT fres = FR_OK;
        do
        {
            fres = f_close(File);
            if (tries-- <= 0)
            {
            	EDB_LOG("Error of closing file in writing data\r\n");
            	status = -1;
            	break;
            }
        }
        while(fres != FR_OK);

    }

    delete File;
    #endif

    if (write_bytes != Row.Size())
    {
    	EDB_LOG("Error of writing data\r\n");
        status = -1;
    }

    if (status == 0)
    {

    	EDB_LOG("Writed Id: %d\r\n", (int) RecordId);
    	EDB_LOG("Writed Index: %d\r\n", (int) WriteIndex);

		WriteIndex = (WriteIndex + 1) % Capacity;
		RecordId = (RecordId + 1) % (Capacity * 2);
    }

    delete[] writeBuffer;

    return status;
}
