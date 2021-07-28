
#include "main.hpp"
#include "easy_db.hpp"
#include <ctime> 
#include <unistd.h>


static Easy_DB_Cell test_row[5] = {{EDB_DATE_TIME}, {EDB_FLOAT}, {EDB_UINT32}, {EDB_UINT32}, {EDB_STRING}};

EasyDataBase test_db("Test", test_row, 5, 1000);


// Основная программа
int main(void)
{
    Easy_DB_DateTime dt, start, end;
    struct tm *u;
	time_t timer;
    uint32_t testNumber = 0;
    
    static char strBuf[64];
    test_row[4].Value.Str.Init(strBuf, sizeof(strBuf));
    test_db.Init();
    
    // Пример установки значений
    test_db.GetRowCell(0)->SetValue(dt);
    test_db.GetRowCell(1)->SetValue(0.123f);
    test_db.GetRowCell(2)->SetValue((uint32_t) 0);
    test_db.GetRowCell(4)->SetValue((char *) "Hello world!!!");

    printf("Reading by range...\r\n");
    start.SDate.Date = 29;
    start.SDate.Month = 07;
    start.SDate.Year = 21;
    start.STime.Hours = 00;
    start.STime.Minutes = 22;
    start.STime.Seconds = 55;

    end.SDate.Date = 29;
    end.SDate.Month = 07;
    end.SDate.Year = 21;
    end.STime.Hours = 00;
    end.STime.Minutes = 23;
    end.STime.Seconds = 10;    

    test_db.Select(&start, &end);

    printf("Selected row count: %d\r\n", test_db.GetSelectedRowCount());


    for (uint32_t i = 0; i < test_db.GetSelectedRowCount(); i++)
    {
        if (test_db.ReadSelectedRow(i) == -1)
        {
            printf("Failed to read row\r\n");
        }
        else
        {
            printf("Row values: %02d/%02d/%02d %02d:%02d:%02d   %2.4f, %d, %d, %s\r\n", test_db.RowCells()[0].Value.Dt.SDate.Date, test_db.RowCells()[0].Value.Dt.SDate.Month, test_db.RowCells()[0].Value.Dt.SDate.Year,
                    test_db.RowCells()[0].Value.Dt.STime.Hours, test_db.RowCells()[0].Value.Dt.STime.Minutes, test_db.RowCells()[0].Value.Dt.STime.Seconds,
                    test_db.RowCells()[1].Value.F32, test_db.RowCells()[2].Value.UI32, test_db.RowCells()[3].Value.UI32, test_db.RowCells()[4].Value.Str.Buf);
        }
    }



    printf("Reading total...\r\n\r\n");

    test_db.Select();

    printf("Selected row count: %d\r\n", test_db.GetSelectedRowCount());

    for (uint32_t i = 0; i < test_db.GetSelectedRowCount(); i++)
    {
        if (test_db.ReadSelectedRow(i) == -1)
        {
            printf("Failed to read row\r\n");
        }
        else
        {
            printf("Row values: %02d/%02d/%02d %02d:%02d:%02d   %2.4f, %d, %d, %s\r\n", test_db.RowCells()[0].Value.Dt.SDate.Date, test_db.RowCells()[0].Value.Dt.SDate.Month, test_db.RowCells()[0].Value.Dt.SDate.Year,
                    test_db.RowCells()[0].Value.Dt.STime.Hours, test_db.RowCells()[0].Value.Dt.STime.Minutes, test_db.RowCells()[0].Value.Dt.STime.Seconds,
                    test_db.RowCells()[1].Value.F32, test_db.RowCells()[2].Value.UI32, test_db.RowCells()[3].Value.UI32, test_db.RowCells()[4].Value.Str.Buf);
        }
    }


    while(1)
    {

        sleep(5);
        // Пример записи значений
        timer = time(NULL);
		u = localtime(&timer);

        dt.STime.Hours = u->tm_hour;
        dt.STime.Minutes = u->tm_min;
        dt.STime.Seconds = u->tm_sec;

        dt.SDate.Date = u->tm_mday;
        dt.SDate.Month = u->tm_mon + 1;
        dt.SDate.Year = u->tm_year - 100;

        printf("Time and Date: %02d/%02d/%02d %02d:%02d:%02d\r\n", dt.SDate.Date, dt.SDate.Month, dt.SDate.Year,
                 dt.STime.Hours, dt.STime.Minutes, dt.STime.Seconds);

        test_db.GetRowCell(0)->SetValue(dt);
        test_db.GetRowCell(2)->SetValue(testNumber++);
        test_db.GetRowCell(3)->SetValue(testNumber + 10);

        printf("Writing row...\r\n");

        if (test_db.WriteRow() == -1)
        {
            printf("Failed to write row\r\n");
        }

    }

    return 0;  
}