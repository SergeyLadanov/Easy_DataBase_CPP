#ifndef __EASY_DB_DATETIME_HPP_
#define __EASY_DB_DATETIME_HPP_

#include <cstdint>

class Easy_DB_DateTime
{

public:

// DataType enumerates skipped fields from date time.
typedef enum
{
	// Nothing is skipped from date time.
	SKIPS_NONE = 0x0,
	// Year part of date time is skipped.
	SKIPS_YEAR = 0x1,
	// Month part of date time is skipped.
	SKIPS_MONTH = 0x2,
	// Day part is skipped.
	SKIPS_DAY = 0x4,
	// Day of week part of date time is skipped.
	SKIPS_DAYOFWEEK = 0x8,
	// Hours part of date time is skipped.
	SKIPS_HOUR = 0x10,
	// Minute part of date time is skipped.
	SKIPS_MINUTE = 0x20,
	// Seconds part of date time is skipped.
	SKIPS_SECOND = 0x40,
	// Hundreds of seconds part of date time is skipped.
	SKIPS_MS = 0x80,
	//Devitation is skipped.
	SKIPS_DEVITATION = 0x100
} DATE_TIME_SKIPS;

private:
    /**
     * @brief  RTC Time structure definition
     */
    typedef struct
    {
        uint8_t Hours = 0;
        uint8_t Minutes = 0;          
        uint8_t Seconds = 0;
        uint32_t SubSeconds = 0;
        uint32_t SecondFraction = 0;
        uint8_t TimeFormat = 0;       
        uint32_t DayLightSaving = 0;  
        uint32_t StoreOperation = 0;  
    }RTC_TimeTypeDef;

    /**
     * @brief  RTC Date structure definition
     */
    typedef struct
    {
        uint8_t WeekDay = 0;
        uint8_t Month = 0;
        uint8_t Date = 0;
        uint8_t Year = 0; 
    }RTC_DateTypeDef;
    
public:
    uint8_t Skip = 0;
	RTC_DateTypeDef SDate;
	RTC_TimeTypeDef STime;

    // Сравнение даты и времени
    static int Compare(Easy_DB_DateTime *start, Easy_DB_DateTime *end);
};





#endif