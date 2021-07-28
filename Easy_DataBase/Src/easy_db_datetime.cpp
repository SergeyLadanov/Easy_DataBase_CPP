#include "easy_db_datetime.hpp"


// Метод сравнения даты и времени
int Easy_DB_DateTime::Compare(Easy_DB_DateTime *value1, Easy_DB_DateTime *value2)
{
    if ((value1->Skip & SKIPS_YEAR) == 0 && value1->SDate.Year != value2->SDate.Year)
    {
        if (value1->SDate.Year < value2->SDate.Year)
        {
            return -1;
        }
        return 1;
    }
    if ((value1->Skip & SKIPS_MONTH) == 0 && value1->SDate.Month != value2->SDate.Month)
    {
        if (value1->SDate.Month < value2->SDate.Month)
        {
            return -1;
        }
        return 1;
    }
    if ((value1->Skip & SKIPS_DAY) == 0 && value1->SDate.Date != value2->SDate.Date)
    {
        if (value1->SDate.Date < value2->SDate.Date)
        {
            return -1;
        }
        return 1;
    }
    if ((value1->Skip & SKIPS_HOUR) == 0 && value1->STime.Hours != value2->STime.Hours)
    {
        if (value1->STime.Hours < value2->STime.Hours)
        {
            return -1;
        }
        return 1;
    }
    if ((value1->Skip & SKIPS_MINUTE) == 0 && value1->STime.Minutes != value2->STime.Minutes)
    {
        if (value1->STime.Minutes < value2->STime.Minutes)
        {
            return -1;
        }
        return 1;
    }
    if ((value1->Skip & SKIPS_SECOND) == 0 && value1->STime.Seconds != value2->STime.Seconds)
    {
        if (value1->STime.Seconds < value2->STime.Seconds)
        {
            return -1;
        }
        return 1;
    }
    return 0;
}