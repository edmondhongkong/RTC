/* 
 * DS3231.cpp - Library to set & get time from RTC DS3231
 * Created by Manjunath CV. July 08, 2017, 02:18 AM
 * Released into the public domain.
 */

#include <Time.h>
#include <Wire.h>
#include "DS3231.h"


bool DS3231::begin()
{
    Wire.begin(); // join i2c bus
    Wire.beginTransmission (0x68);
    return (Wire.endTransmission() == 0 ?  true : false);  
}

/*----------------------------------------------------------------------------------------------
getSecond

+---------+--------+-------+-------+-------+-------+-------+-------+--------+----------+-------+
| ADDRESS | BIT 7  | BIT 6 | BIT 5 | BIT 4 | BIT 3 | BIT 2 | BIT 1 | BIT 0  | FUNCTION | RANGE |
+---------+--------+-------+-------+-------+-------+-------+-------+--------+----------+-------+
|   0x00  |    0   |       10 Seconds      |             Seconds            |  Seconds | 00-59 |
+---------+--------+-----------------------+--------------------------------+----------+-------+

----------------------------------------------------------------------------------------------*/
uint8_t DS3231::getSecond()
{
    uint8_t second;
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x00);  // Second Register
    Wire.endTransmission();
    Wire.requestFrom(DS3231_ADDR, 1);
    second = Wire.read();
    return (bcd2bin(second));
}

void DS3231::setSecond(uint8_t second)
{
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x00);  // Second Register
    Wire.write(bin2bcd(second));
    Wire.endTransmission();
}


/*-----------------------------------------------------------
getMinute
-----------------------------------------------------------*/
uint8_t DS3231::getMinute()
{
    uint8_t minute;
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x01);  // Minute Register
    Wire.endTransmission();
    Wire.requestFrom(DS3231_ADDR, 1);
    minute = Wire.read();
    return (bcd2bin(minute));
}

void DS3231::setMinute(uint8_t minute)
{
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x01);  // Minute Register
    Wire.write(bin2bcd(minute));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getHour
-----------------------------------------------------------*/
uint8_t DS3231::getHour()
{
    uint8_t hour;
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x02);  // Hour Register
    Wire.endTransmission();
    Wire.requestFrom(DS3231_ADDR, 1);
    hour = Wire.read();
    return (bcd2bin(hour));
}

void  DS3231::setHour(uint8_t hour)
{
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x02);  // Hour Register
    Wire.write(bin2bcd(hour));
    Wire.endTransmission();
}


/*-----------------------------------------------------------
getWeek
-----------------------------------------------------------*/
uint8_t DS3231::getWeek()
{
    uint8_t week;
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x03);  // Week Register
    Wire.endTransmission();
    Wire.requestFrom(DS3231_ADDR, 1);
    week = Wire.read();
    return week;
}

void DS3231::setWeek(uint8_t week)
{
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x03);  // Minute Register
    Wire.write(week);
    Wire.endTransmission();
}


/*-----------------------------------------------------------
getDay
-----------------------------------------------------------*/
      
uint8_t DS3231::getDay()
{
    uint8_t day;
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x04);  // Day Register
    Wire.endTransmission();
    Wire.requestFrom(DS3231_ADDR, 1);
    day = Wire.read();
    return (bcd2bin(day));
}

void DS3231::setDay(uint8_t day)
{
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x04);  // Day Register   
    Wire.write(bin2bcd(day));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getMonth (Perfect)
-----------------------------------------------------------*/
uint8_t DS3231::getMonth()
{
    uint8_t month;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x05);  // Month Register
    Wire.endTransmission();

    Wire.requestFrom(DS3231_ADDR, 1);
    month = Wire.read();
    
    bitWrite(month,7,0); 

    month = bcd2bin(month);
    return (month);
}
/*-----------------------------------------------------------
setMonth (Perfect)
-----------------------------------------------------------*/

void DS3231::setMonth(uint8_t month)
{
    uint8_t century;
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x05);  // Month Register
    Wire.endTransmission();

    Wire.requestFrom(DS3231_ADDR, 1);
    century = Wire.read();
    
    century = century & 0x80;

    month = bin2bcd(month);
    month = century | month;
    
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x05);  // Month Register
    Wire.write(month);
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getYear (Completed)
-----------------------------------------------------------*/
uint16_t DS3231::getYear()
{
    uint8_t c_data ,century, data;
    uint16_t year;

     /* Setting Century Bit*/
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x05);  // Month Register to read Century
    Wire.endTransmission();
    Wire.requestFrom(DS3231_ADDR, 1);
    c_data = Wire.read();

    century = bitRead(c_data,7);

    if(century==1)
        year = 2000;
    else
        year = 1900;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x06);  // Year Register
    Wire.endTransmission();

    Wire.requestFrom(DS3231_ADDR, 1);
    data = Wire.read();
    year = bcd2bin(data)+year;
    return (year);
}

void DS3231::setYear(uint16_t year)
{

    uint8_t c_data, century;
    
    if(year >= 2000)
        century = 1;
    else
        century = 0;

    year = year % 100; //Converting to 2 Digit

    /* Setting Century Bit*/
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x05);  // Month Register to read Century
    Wire.endTransmission();
    Wire.requestFrom(DS3231_ADDR, 1);
    c_data = Wire.read();
    bitWrite(c_data,7,century);
    
    /*Writing Century Bit in Month Register (0x05)*/
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x05);  // Month Register to write Century
    Wire.write(c_data);
    Wire.endTransmission();

    /*Writing 2 Digit year to Year Register(0x06)*/
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x06);  // Year Register to write year   
    Wire.write(bin2bcd(year));
    Wire.endTransmission();
}


/*-----------------------------------------------------------
setTime
-----------------------------------------------------------*/

void DS3231::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x00);  // Year Register
    Wire.write(bin2bcd(second));
    Wire.write(bin2bcd(minute));
    Wire.write(bin2bcd(hour));
    Wire.endTransmission();
}


/*-----------------------------------------------------------
setDate (Should be Optimised)
-----------------------------------------------------------*/
void DS3231::setDate(uint8_t day, uint8_t month, uint16_t year)
{
    uint8_t c_data, century;

//Setting Day
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x04);  // Year Register
    Wire.write(bin2bcd(day));
    Wire.endTransmission();

//Setting Month
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x05);  // Month Register
    Wire.endTransmission();

    Wire.requestFrom(DS3231_ADDR, 1);
    century = Wire.read();
    
    century = century & 0x80;

    month = bin2bcd(month);
    month = century | month;
    
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x05);  // Month Register
    Wire.write(month);
    Wire.endTransmission();

//Setting Year
    if(year >= 2000)
        century = 1;
    else
        century = 0;

    year = year % 100; //Converting to 2 Digit

    /* Setting Century Bit*/
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x05);  // Month Register to read Century
    Wire.endTransmission();
    Wire.requestFrom(DS3231_ADDR, 1);
    c_data = Wire.read();
    bitWrite(c_data,7,century);
    
    /*Writing Century Bit in Month Register (0x05)*/
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x05);  // Month Register to write Century
    Wire.write(c_data);
    Wire.endTransmission();

    /*Writing 2 Digit year to Year Register(0x06)*/
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x06);  // Year Register to write year   
    Wire.write(bin2bcd(year));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
setEpoch()
-----------------------------------------------------------*/

void DS3231::setEpoch(time_t epoch, time_t e_year, int16_t offset)
{
	time_t rawtime;
	struct tm epoch_tm , *ptr_epoch_tm;
    uint16_t year;

    epoch  = epoch - e_year;

	rawtime = epoch;
	ptr_epoch_tm = gmtime(&rawtime);	
	epoch_tm = *ptr_epoch_tm; 
	
    setSecond(epoch_tm.tm_sec); //0x00 - Seconds
    setMinute(epoch_tm.tm_min);
    setHour(epoch_tm.tm_hour);
    setWeek(epoch_tm.tm_wday+1);
    setDay(epoch_tm.tm_mday);
    setMonth(epoch_tm.tm_mon+1);
    setYear(epoch_tm.tm_year+1900);

	  /*  //For Debug 
        Serial.println();
        Serial.print("tm_sec : ");
        Serial.println(epoch_tm.tm_sec);
        Serial.print("tm_min : ");
        Serial.println(epoch_tm.tm_min);
        Serial.print("tm_hour : ");
        Serial.println(epoch_tm.tm_hour);
        Serial.print("tm_mday : ");
        Serial.println(epoch_tm.tm_mday);
        Serial.print("tm_mon : ");
        Serial.println(epoch_tm.tm_mon);
        Serial.print("tm_year : ");
        Serial.println(epoch_tm.tm_year);
        Serial.print("tm_wday : ");
        Serial.println(epoch_tm.tm_wday);
        Serial.print("tm_yday : ");
        Serial.println(epoch_tm.tm_yday);
        Serial.print("tm_isdst : ");
        Serial.println(epoch_tm.tm_isdst);
        Serial.println();
    */
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getEpoch()
-----------------------------------------------------------*/
time_t DS3231::getEpoch()
{
    time_t epoch;
	struct tm epoch_tm;
	
    epoch_tm.tm_sec = getSecond();
    epoch_tm.tm_min = getMinute();
    epoch_tm.tm_hour = getHour();
    epoch_tm.tm_wday = getWeek() - 1;
    epoch_tm.tm_mday = getDay();
    epoch_tm.tm_mon = getMonth() - 1;
    epoch_tm.tm_year = getYear() - 1900;

    epoch = mktime (&epoch_tm);	

    return (epoch);
}

/*-----------------------------------------------------------
enableAlaram() *Incomplete*
-----------------------------------------------------------*/
uint8_t DS3231::enableAlaram()
{
    uint8_t reg;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x0E);  // Hour Register
    Wire.endTransmission();

    Wire.requestFrom(DS3231_ADDR, 1);
    reg = Wire.read();

    bitWrite(reg,2,1); // Write bit INTCN to 1 to enable INT/SQW pin 
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x0E);  // Month Register
    Wire.write(bin2bcd(reg));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
DS3231 Exclusive Functions
-----------------------------------------------------------*/
bool DS3231::lostPower(void)
{
    uint8_t data;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x0F); 
    Wire.endTransmission();

    Wire.requestFrom(DS3231_ADDR, 1);
    data = Wire.read();

    return (data >> 7);
}

void DS3231::StartClock(void)
{
    uint8_t data;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x0F);
    Wire.endTransmission();

    Wire.requestFrom(DS3231_ADDR, 1);
    data = Wire.read();

    bitWrite(data,7,0); // Write  EOSC Register to 0 to start the clock. 

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x0F);  // Month Register
    Wire.write(bin2bcd(data));
    Wire.endTransmission();
}


float DS3231::getTemp()
{
    float f_temp;
    uint8_t temp_msb, temp_lsb;

    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(0x11); 
    Wire.endTransmission();

    Wire.requestFrom(DS3231_ADDR, 2);
    temp_msb = Wire.read();
    temp_lsb = Wire.read() >> 6;

    f_temp = (0.25 * temp_lsb) + temp_msb;

    return(f_temp);
}

static uint8_t DS3231::bcd2bin (uint8_t val) 
{ 
    return val - 6 * (val >> 4); 
}
static uint8_t DS3231::bin2bcd (uint8_t val) 
{ 
    return val + 6 * (val / 10); 
}

// Debug Functions
uint8_t DS3231::getRegister(uint8_t reg)
{
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(DS3231_ADDR, 1);
    reg = Wire.read();
    return (reg);
}

void DS3231::setRegister(uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(DS3231_ADDR);                       
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}
