#ifndef __EASY_DB_DATETIME_HPP_
#define __EASY_DB_DATETIME_HPP_

#include <cstdint>

class Easy_DB_DateTime
{
private:
    /**
     * @brief  RTC Time structure definition
     */
    typedef struct
    {
        uint8_t Hours;
        uint8_t Minutes;          
        uint8_t Seconds;
        uint32_t SubSeconds;
        uint32_t SecondFraction;
        uint8_t TimeFormat;       
        uint32_t DayLightSaving;  
        uint32_t StoreOperation;  
    }RTC_TimeTypeDef;

    /**
     * @brief  RTC Date structure definition
     */
    typedef struct
    {
        uint8_t WeekDay;
        uint8_t Month;
        uint8_t Date;
        uint8_t Year; 
    }RTC_DateTypeDef;
    
public:
    uint8_t Skip;
	RTC_DateTypeDef SDate;
	RTC_TimeTypeDef STime;
};





#endif