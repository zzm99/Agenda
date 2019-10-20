//#include "../include/Date.hpp"
#include "Date.hpp"
#include <gtest/gtest.h>
 
  // #include <iostream>
  // using namespace std;

Date::Date(){
  this->m_year = 0;
  this->m_month = 0;
  this->m_day = 0;
  this->m_hour = 0;
  this->m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute){
    this->m_year = t_year;
    this->m_month = t_month;
    this->m_day = t_day;
    this->m_hour = t_hour;
    this->m_minute = t_minute;
}

Date::Date(const std::string &dateString){
    Date rem = stringToDate(dateString);
    if(isValid(rem)){
      this->m_year = rem.m_year;
      this->m_month = rem.m_month;
      this->m_day = rem.m_day;
      this->m_hour = rem.m_hour;
      this->m_minute = rem.m_minute;
    }
    else
    {
      this->m_year = 0;
      this->m_month = 0;
      this->m_day = 0;
      this->m_hour = 0;
      this->m_minute = 0;
    }
}


int Date::getYear(void) const{
    return this->m_year;
}


void Date::setYear(const int t_year){
    this->m_year = t_year;
}

int Date::getMonth(void) const{
    return this->m_month;
}

void Date::setMonth(const int t_month){
    this->m_month = t_month;
}

int Date::getDay(void) const{
    return this->m_day;
}

void Date::setDay(const int t_day){
    this->m_day = t_day;
}


int Date::getHour(void) const{
    return this->m_hour;
}

void Date::setHour(const int t_hour){
    this->m_hour = t_hour;
}


int Date::getMinute(void) const{
    return  this->m_minute;
}

void Date::setMinute(const int t_minute){
    this->m_minute = t_minute;
}

bool Date::isValid(const Date &t_date){
    int month_day[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(t_date.getYear() < 1000 || t_date.getYear() > 9999) return false;
    if(t_date.getMonth() > 12 || t_date.getMonth() <= 0 || t_date.getHour() >= 24 || t_date.getHour() < 0 || t_date.getMinute() >= 60 || t_date.getMinute() < 0)
      return false;
    if(t_date.getYear() % 4 == 0 && t_date.getYear() % 100 != 0 || t_date.getYear() % 400 == 0){
      if(t_date.getMonth() == 2)
        if(t_date.getDay() > 29 || t_date.getDay() <= 0)
          return false;
        else
          return true;
    }
    if(t_date.getDay() > month_day[t_date.getMonth()-1] || t_date.getDay() <= 0)
      return false;
    return true;
}
  
Date Date::stringToDate(const std::string &t_dateString){
    for(int i=0; i<=3; i++)
      if(t_dateString[i] < '0' || t_dateString[i] > '9')
      {
        Date ret;
        return ret;
      }
    if(t_dateString[4] != '-' || t_dateString[7] != '-' || t_dateString[10] != '/' ||t_dateString[13] != ':')
    {
      Date ret;
      return ret;
    }
    for(int i=5; i<=6; i++)
      if(t_dateString[i] < '0' || t_dateString[i] > '9')
      {
        Date ret;
        return ret;
      }
    for(int i=8; i<=9; i++)
      if(t_dateString[i] < '0' || t_dateString[i] > '9')
      {
        Date ret;
        return ret;
      }
    for(int i=11; i<=12; i++)
      if(t_dateString[i] < '0' || t_dateString[i] > '9')
      {
        Date ret;
        return ret;
      }
    for(int i=14; i<=15; i++)
      if(t_dateString[i] < '0' || t_dateString[i] > '9')
      {
        Date ret;
        return ret;
      }
    Date ret;
    ret.m_year = (t_dateString[0]-'0')*1000 + (t_dateString[1]-'0')*100 + (t_dateString[2]-'0')*10 + (t_dateString[3]-'0');
    ret.m_month = (t_dateString[5]-'0')*10 + (t_dateString[6]-'0');
    ret.m_day = (t_dateString[8]-'0')*10 + (t_dateString[9]-'0');
    ret.m_hour = (t_dateString[11]-'0')*10 + (t_dateString[12]-'0');
    ret.m_minute = (t_dateString[14]-'0')*10 + (t_dateString[15]-'0');
    return ret;
}

  
std::string Date::dateToString(const Date &t_date){
    if(!isValid(t_date))
    {
      return "0000-00-00/00:00";
    }
    std::string ret = "";
    int tmpyear = t_date.getYear();
    int tmpmonth = t_date.getMonth();
    int tmpday = t_date.getDay();
    int tmphour = t_date.getHour();
    int tmpmin = t_date.getMinute();
    ret += tmpyear / 1000 + '0';
    tmpyear %= 1000;
    ret += tmpyear / 100 + '0';
    tmpyear %= 100;
    ret += tmpyear / 10 + '0';
    tmpyear %= 10;
    ret += tmpyear / 1 + '0';
    ret += '-';
    if(tmpmonth < 10){
      ret += '0';
      ret += tmpmonth+'0';
    }
    else
    {
      ret += tmpmonth / 10 + '0';
      tmpmonth %= 10;
      ret += tmpmonth / 1 + '0';
    }
    ret += '-';
    if(tmpday < 10){
      ret += '0';
      ret += tmpday+'0';
    }
    else
    {
      ret += tmpday / 10 + '0';
      tmpday %= 10;
      ret += tmpday / 1 + '0';
    }
    ret += '/';
    if(tmphour < 10){
      ret += '0';
      ret += tmphour+'0';
    }
    else
    {
      ret += tmphour / 10 + '0';
      tmphour %= 10;
      ret += tmphour / 1 + '0';
    }
    ret += ':';
    if(tmpmin < 10){
      ret += '0';
      ret += tmpmin+'0';
    }
    else
    {
      ret += tmpmin / 10 + '0';
      tmpmin %= 10;
      ret += tmpmin / 1 + '0';
    }
    return ret;
}

  
Date &Date::operator=(const Date &t_date){
    this->m_year = t_date.getYear();
    this->m_month = t_date.getMonth();
    this->m_day = t_date.getDay();
    this->m_hour = t_date.getHour();
    this->m_minute = t_date.getMinute();
    return *this;
}
  
bool Date::operator==(const Date &t_date) const{
    if(this->m_year != t_date.getYear()) return false;
    if(this->m_month != t_date.getMonth())return false;
    if(this->m_day != t_date.getDay())return false;
    if(this->m_hour != t_date.getHour())return false;
    if(this->m_minute != t_date.getMinute())return false;
    return true;
}

bool Date::operator>(const Date &t_date) const{
    if(this->m_year < t_date.getYear()) return false;
    if(this->m_year > t_date.getYear()) return true;
    if(this->m_month < t_date.getMonth())return false;
    if(this->m_month > t_date.getMonth())return true;
    if(this->m_day < t_date.getDay())return false;
    if(this->m_day > t_date.getDay())return true;
    if(this->m_hour < t_date.getHour())return false;
    if(this->m_hour > t_date.getHour())return true;
    if(this->m_minute < t_date.getMinute())return false;
    if(this->m_minute > t_date.getMinute())return true;
    return false;
}

bool Date::operator<(const Date &t_date) const{
    if(this->m_year < t_date.getYear()) return true;
    if(this->m_year > t_date.getYear()) return false;
    if(this->m_month < t_date.getMonth())return true;
    if(this->m_month > t_date.getMonth())return false;
    if(this->m_day < t_date.getDay())return true;
    if(this->m_day > t_date.getDay())return false;
    if(this->m_hour < t_date.getHour())return true;
    if(this->m_hour > t_date.getHour())return false;
    if(this->m_minute < t_date.getMinute())return true;
    if(this->m_minute > t_date.getMinute())return false;
    return false;
}

  
bool Date::operator>=(const Date &t_date) const{
    if(this->m_year < t_date.getYear()) return false;
    if(this->m_year > t_date.getYear()) return true;
    if(this->m_month < t_date.getMonth())return false;
    if(this->m_month > t_date.getMonth())return true;
    if(this->m_day < t_date.getDay())return false;
    if(this->m_day > t_date.getDay())return true;
    if(this->m_hour < t_date.getHour())return false;
    if(this->m_hour > t_date.getHour())return true;
    if(this->m_minute < t_date.getMinute())return false;
    if(this->m_minute > t_date.getMinute())return true;
    return true;
}

  
bool Date::operator<=(const Date &t_date) const{
    if(this->m_year < t_date.getYear()) return true;
    if(this->m_year > t_date.getYear()) return false;
    if(this->m_month < t_date.getMonth())return true;
    if(this->m_month > t_date.getMonth())return false;
    if(this->m_day < t_date.getDay())return true;
    if(this->m_day > t_date.getDay())return false;
    if(this->m_hour < t_date.getHour())return true;
    if(this->m_hour > t_date.getHour())return false;
    if(this->m_minute < t_date.getMinute())return true;
    if(this->m_minute > t_date.getMinute())return false;
    return true;
}

// #include <iostream>
// using namespace std;

// int main(){
//   Date rem("2000-02-30/08:00");
//   cout << rem.getDay() << endl;
//   return 0;
// }