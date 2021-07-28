
#include "main.hpp"
#include "easy_db.hpp"
#include <ctime> 
#include <unistd.h>

 static Easy_DB_Cell test_row[4] = {{EDB_DATE_TIME}, {EDB_FLOAT}, {EDB_UINT32}, {EDB_UINT32}};


EasyDataBase test_db("Test", test_row, 4, 25);


uint32_t testNumber = 0;





// Основная программа
int main(void)
{
    Easy_DB_DateTime dt, start, end;
    struct tm *u;
	time_t timer;
    dt.Skip = 0;

    test_db.Init();

    
    start.SDate.Date = 28;
    start.SDate.Month = 07;
    start.SDate.Year = 21;
    start.STime.Hours = 9;
    start.STime.Minutes = 25;
    start.STime.Seconds = 0;

    end.SDate.Date = 28;
    end.SDate.Month = 07;
    end.SDate.Year = 21;
    end.STime.Hours = 9;
    end.STime.Minutes = 25;
    end.STime.Seconds = 0;

    test_db.GetRowCell(0)->SetValue(dt);
    test_db.GetRowCell(1)->SetValue(0.123f);
    test_db.GetRowCell(2)->SetValue((uint32_t) 0);

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
            printf("Row values: %2.4f, %d\r\n", test_db.RowCells()[1].Value.F32, test_db.RowCells()[2].Value.UI32);
        }
    }


    while(1)
    {
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

        printf("Writing row...\r\n");

        if (test_db.WriteRow() == -1)
        {
            printf("Failed to write row\r\n");
        }

        test_db.GetRowCell(0)->SetValue(dt);
        test_db.GetRowCell(2)->SetValue(testNumber++);
        test_db.GetRowCell(3)->SetValue(testNumber + 10);


        sleep(5);
    }

    return 0;  
}