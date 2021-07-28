
#include "main.hpp"
#include "easy_db.hpp"

 static Easy_DB_Cell test_row[3] = {{EDB_DATE_TIME}, {EDB_FLOAT}, {EDB_UINT32}};


EasyDataBase test_db("Test", test_row, 4, 20);

// uint8_t TestBuffer[1024];





// Основная программа
int main(void)
{
    Easy_DB_DateTime dt;
    //uint32_t test_bytes;

    test_db.Init();

    dt.Skip = 0;
    dt.SDate.Date = 28;
    dt.SDate.Month = 07;
    dt.SDate.Year = 21;
    dt.STime.Hours = 9;
    dt.STime.Minutes = 25;
    dt.STime.Seconds = 0;

    test_db.GetRowCell(0)->SetValue(dt);
    test_db.GetRowCell(1)->SetValue(0.123f);
    test_db.GetRowCell(2)->SetValue((uint32_t) 0);

    for (uint32_t i = 0; i < 1; i++)
    {
        if (test_db.WriteRow() == -1)
        {
            printf("Failed to write row\r\n");
        }

        test_db.GetRowCell(2)->SetValue((uint32_t) i);
    }

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



    
    // Проба сериализации/десериализации
    // printf("Test: %2.1f\r\n", test_db.GetRowCell(1)->Value.F32);

    // test_bytes = test_db.Row.Serialize(TestBuffer);

    // printf("%d bytes serialized\r\n", test_bytes);

    // if ((test_bytes = test_db.Row.DeSerialize(TestBuffer)) == 0)
    // {
    //     printf("Checksum error\r\n");
    // }
    // else
    // {
    //     printf("%d bytes deserialized\r\n", test_bytes);
    // }
    //-----------------------------------------------------------

    return 0;  
}