/******************************************************************//**
* @file RV8803C7.cpp
*
* @author Casey Quinn
*
* @version 1.0
*
* Started: 19JUN20
*
* Updated: 
*
* @brief Source file for RV8803C7 class
*
**********************************************************************/


#include "RV8803C7.h"  


// Convert normal decimal numbers to binary coded decimal
uint8_t decToBcd(int val)
{
  return (uint8_t)( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
int bcdToDec(uint8_t val)
{
  return (int)( (val/16*10) + (val%16) );
}

HAL_StatusTypeDef RV8803C7_set_time (uint8_t sec, uint8_t min, uint8_t hour)
{
	uint8_t data[3];
	data[0] = decToBcd(sec);
	data[1] = decToBcd(min);
	data[2] = decToBcd(hour);

    //Make sure data is within range.
    if((sec > 59) || (min > 59) || (hour > 23))
    {
        return(HAL_ERROR);
    }
    else
    {
    	return(HAL_I2C_Mem_Write(&hi2c1, RV8803C7_I2C_ADRS << 1, 0x00, 1, data, 3, 1000));
    }
}

HAL_StatusTypeDef RV8803C7_set_calendar (uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
{
	uint8_t data[4];
	data[0] = (1 << dow);
	data[1] = decToBcd(dom);
	data[2] = decToBcd(month);
	data[3] = decToBcd(year);

    //Make sure data is within range.
    if(((dow > 6)) ||
       ((dom < 1) || (dom > 31)) ||
       ((month < 1) || (month > 12)) ||
       (year > 99))
    {
        return(HAL_ERROR);
    }
    else
    {
    	return(HAL_I2C_Mem_Write(&hi2c1, RV8803C7_I2C_ADRS << 1, 0x03, 1, data, 4, 1000));
    }
}

HAL_StatusTypeDef RV8803C7_get_time (RV8803C7_time_t* time)
{
	uint8_t data[3];
	HAL_StatusTypeDef rtn_val = HAL_ERROR;
	rtn_val = HAL_I2C_Mem_Read(&hi2c1, RV8803C7_I2C_ADRS << 1, 0x00, 1, data, 3, 1000);
	time->seconds = bcdToDec(data[0]);
	time->minutes = bcdToDec(data[1]);
	time->hours = bcdToDec(data[2]);
	return(rtn_val);
}

HAL_StatusTypeDef RV8803C7_get_calendar (RV8803C7_calendar_t* calendar)
{
	uint8_t data[4];
	HAL_StatusTypeDef rtn_val = HAL_ERROR;
	rtn_val = HAL_I2C_Mem_Read(&hi2c1, RV8803C7_I2C_ADRS << 1, 0x03, 1, data, 4, 1000);
	calendar->day = (uint8_t)(log2((float)data[0]));
	calendar->date = bcdToDec(data[1]);
	calendar->month = bcdToDec(data[2]);
	calendar->year = bcdToDec(data[3]);
	return(rtn_val);
}



/**********************************************************//**
* Sets the time on RV8803C7
* Struct data is in integrer format, not BCD.  Fx will convert
* to BCD for you.
*
* On Entry:
*     @param[in] time - struct cotaining time data; 
*
* On Exit:
*     @return return value = 0 on success, non-0 on failure
*
* Example:
* @code
* 
* //instantiate rtc object
* RV8803C7 rtc(D14, D15); 
* 
* //time = 14:00:00
* RV8803C7_time_t time = {14, 0, 0}
* uint16_t rtn_val;
*
* rtn_val = rtc.set_time(time);
*
* @endcode
**************************************************************/
/*uint16_t RV8803C7::set_time(RV8803C7_time_t time)
{
    uint8_t data[] = {0,0,0,0};
    uint8_t data_length = 0;
    uint8_t max_hour = 24;
    
    data[data_length++] = SECONDS;
    data[data_length++] = uchar_2_bcd(time.seconds);
    data[data_length++] = uchar_2_bcd(time.minutes);
    
    //format Hours register
    data[data_length] = uchar_2_bcd(time.hours);
    
    max_hour = max_hour - 1;
    
    data_length++;
    
    //Make sure data is within range.  
    if((time.seconds > 59) || (time.minutes > 59) || (time.hours > max_hour))
    {
        return(1);
    }
    else
    {   
        //printf("w_adrs: %d, data_length: %d\r\n", w_adrs, data_length);
        return(write(w_adrs,(const char*) data, data_length));
    }
}
*/

/**********************************************************//**
* Sets the calendar on RV8803C7
*
* On Entry:
*     @param[in] calendar - struct cotaining calendar data 
*
* On Exit:
*     @return return value = 0 on success, non-0 on failure
*
* Example:
* @code
* 
* //instantiate rtc object
* RV8803C7 rtc(D14, D15); 
* 
* //see datasheet for calendar format
* RV8803C7_calendar_t calendar = {1, 1, 1, 0}; 
* uint16_t rtn_val;
*
* rtn_val = rtc.set_calendar(calendar);
*
* @endcode
**************************************************************/
/*uint16_t RV8803C7::set_calendar(RV8803C7_calendar_t calendar)
{
    uint8_t data[] = {0,0,0,0,0};
    uint8_t data_length = 0;

    data[data_length++] = DAY;
    data[data_length++] = (1 << calendar.day);
    data[data_length++] = uchar_2_bcd(calendar.date);
    data[data_length++] = uchar_2_bcd(calendar.month);
    data[data_length++] = uchar_2_bcd(calendar.year);

    //Make sure data is within range.  
    if(((calendar.day < 1) || (calendar.day > 7)) || 
       ((calendar.date < 1) || (calendar.date > 31)) || 
       ((calendar.month < 1) || (calendar.month > 12)) || 
       (calendar.year > 99))
    {
        return(1);
    }
    else
    {
        return(write(w_adrs,(const char*) data, data_length));
    }
}
*/


/**********************************************************//**
* Gets the time on RV8803C7
*
* On Entry:
*     @param[in] time - pointer to struct for storing time data
*
* On Exit:
*     @param[out] time - contains current integrer rtc time 
*                        data
*     @return return value = 0 on success, non-0 on failure
*
* Example:
* @code
* 
* //instantiate rtc object
* RV8803C7 rtc(D14, D15); 
* 
* //time = 14:00:00
* RV8803C7_time_t time = {14, 0, 0} 
* uint16_t rtn_val;
*
* rtn_val = rtc.get_time(&time);
*
* @endcode
**************************************************************/
/*uint16_t RV8803C7::get_time(RV8803C7_time_t* time)
{
    uint16_t rtn_val = 1;
    uint8_t data[3];
    
    data[0] = SECONDS;
    rtn_val = write(w_adrs, (const char*) data, 1);
    
    if(!rtn_val)
    {
        rtn_val = read(r_adrs,(char *) data, 3);
        
        time->seconds = bcd_2_uchar(data[0]);
        time->minutes = bcd_2_uchar(data[1]);
        time->hours = bcd_2_uchar((data[2]&0x3F));
    } 
  
    return(rtn_val);
}
*/

/**********************************************************//**
* Gets the calendar on RV8803C7
*
* On Entry:
*     @param[in] calendar - pointer to struct for storing 
*                           calendar data
*
* On Exit:
*     @param[out] calendar - contains current integer rtc 
*                            calendar data
*     @return return value = 0 on success, non-0 on failure
*
* Example:
* @code
* 
* //instantiate rtc object
* RV8803C7 rtc(D14, D15); 
* 
* //see datasheet for calendar format
* RV8803C7_calendar_t calendar = {1, 1, 1, 0}; 
* uint16_t rtn_val;
*
* rtn_val = rtc.get_calendar(&calendar);
*
* @endcode
**************************************************************/
/*uint16_t RV8803C7::get_calendar(RV8803C7_calendar_t* calendar)
{
    uint16_t rtn_val = 1;
    uint8_t data[4];
    
    data[0] = DAY;
    rtn_val = write(w_adrs, (const char*) data, 1);
    
    if(!rtn_val)
    {
        rtn_val = read(r_adrs,(char *) data, 4);
        
        calendar->day = (uint8_t)(log2((float)data[0]));
        calendar->date = bcd_2_uchar(data[1]);
        calendar->month = bcd_2_uchar((data[2]&0x1F));
        calendar->year = bcd_2_uchar(data[3]);
    } 
  
    return(rtn_val);
}
*/


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
time_t RV8803C7_get_epoch(void)
{
    //system vars
    struct tm sys_time;
    
    //RTC vars
    RV8803C7_time_t rtc_time = {0,0,0};
    RV8803C7_calendar_t rtc_calendar = {0,0,0,0};
    
    RV8803C7_get_calendar(&rtc_calendar);
    RV8803C7_get_time(&rtc_time);
    
    sys_time.tm_wday = rtc_calendar.day;
    sys_time.tm_mday = rtc_calendar.date;
    sys_time.tm_mon = rtc_calendar.month - 1;
    sys_time.tm_year = rtc_calendar.year + 100;
    
    sys_time.tm_hour = rtc_time.hours;
    sys_time.tm_min = rtc_time.minutes;
    sys_time.tm_sec = rtc_time.seconds;
    
    if(rtc_time.seconds == 59){
        get_time(&rtc_time);
        
        sys_time.tm_hour = rtc_time.hours;
        sys_time.tm_min = rtc_time.minutes;
        sys_time.tm_sec = rtc_time.seconds;
            
    }
    
    //make epoch time
    return(mktime(&sys_time));
}

uint8_t RV8803C7_get_hundredth_secs(void)
{
	HAL_StatusTypeDef rtn_val = HAL_ERROR;
    uint8_t hundredth_sec = 0;
    uint8_t data[1];
    
    rtn_val = HAL_I2C_Mem_Read(&hi2c1, RV8803C7_I2C_ADRS << 1, 0x10, 1, data, 1, 1000);
    
    if(rtn_val == HAL_OK)
    {
        hundredth_sec = bcdToDec(data[0]);
    } 
  
    return(hundredth_sec);
}


