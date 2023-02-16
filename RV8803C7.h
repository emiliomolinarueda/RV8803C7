/******************************************************************//**
* @file RV8803C7.h
*
* @author Casey Quinn
*
* @version 1.0
*
* Started: 19JUN20
*
* Updated: 
*
* @brief Header file for RV8803C7 class
*
**********************************************************************/


#ifndef RV8803C7_H
#define RV8803C7_H

#include "main.h"
#include "stm32wbxx_hal.h"
#include <time.h>

#define _M_LN2  0.693147180559945309417 // Natural log of 2
#define log2(x) (log(x)/_M_LN2)

#define RV8803C7_I2C_ADRS 0x32

extern I2C_HandleTypeDef hi2c1;

/*
#define AM_PM     (1 << 5) 
#define MODE      (1 << 6)
#define DY_DT     (1 << 6)
#define ALRM_MASK (1 << 7)

//control register bit masks
#define A1IE  (1 << 0)
#define A2IE  (1 << 1)
#define INTCN (1 << 2)
#define RS1   (1 << 3)
#define RS2   (1 << 4)
#define CONV  (1 << 5)
#define BBSQW (1 << 6)
#define EOSC  (1 << 7)

//status register bit masks
#define A1F     (1 << 0)
#define A2F     (1 << 1)
#define BSY     (1 << 2)
#define EN32KHZ (1 << 3)
#define OSF     (1 << 7)
*/  

/**
* RV8803C7_time_t - Struct for containing time data.
* 
* Members:
*
* - uint32_t seconds - Use decimal value. Member fx's convert to BCD
*
* - uint32_t minutes - Use decimal value. Member fx's convert to BCD
*
* - uint32_t hours   - Use decimal value. Member fx's convert to BCD
*
*/
typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;

}RV8803C7_time_t;


/**
* RV8803C7_calendar_t - Struct for containing calendar data.
* 
* Members:
*
* - uint32_t day   - Use decimal value. Member fx's convert to BCD
*
* - uint32_t date  - Use decimal value. Member fx's convert to BCD
*
* - uint32_t month - Use decimal value. Member fx's convert to BCD
*
* - uint32_t year  - Use decimal value. Member fx's convert to BCD
*/
typedef struct
{
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
}RV8803C7_calendar_t;


/**
* RV8803C7_cntl_stat_t - Struct for containing control and status 
* register data.
* 
* Members:
*
* - uint8_t control - Binary data for read/write of control register 
*
* - uint8_t status  - Binary data  for read/write of status register 
*/
typedef struct
{
    uint8_t control; 
    uint8_t status; 
}RV8803C7_cntl_stat_t;
        
                
// Convert normal decimal numbers to binary coded decimal
uint8_t decToBcd(int val);

// Convert binary coded decimal to normal decimal numbers
int bcdToDec(uint8_t val);

HAL_StatusTypeDef RV8803C7_set_time (uint8_t sec, uint8_t min, uint8_t hour);

HAL_StatusTypeDef RV8803C7_set_calendar (uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);

HAL_StatusTypeDef RV8803C7_get_time (RV8803C7_time_t* time);

HAL_StatusTypeDef RV8803C7_get_calendar (RV8803C7_calendar_t* calendar);


    
/**********************************************************//**
* Get epoch time based on current RTC time and date.
* RV8803C7 must be configured and running before this fx is
* called
*
* On Entry:
*
* On Exit:
*     @return return value = epoch time
*
* Example:
* @code
*
* //instantiate rtc object
* RV8803C7 rtc(D14, D15);
*
* time_t epoch_time;
*
* epoch_time = rtc.get_epoch();
*
* @endcode
**************************************************************/
time_t RV8803C7_get_epoch(void);
        
uint32_t RV8803C7_get_hundredth_secs(void);
        
#endif /* RV8803C7_H*/
