#include <gtest/gtest.h>
#include <string>
#include "../include/Date.hpp"
using namespace std;

TEST(DateTest, DateToStr){
	Date date1(2001, 1, 1, 17, 20);
	EXPECT_STREQ("2001-01-01/17:20", Date::dateToString(date1).c_str());
	Date date2(2001, 2, 1, 1, 20);
	EXPECT_STREQ("2001-02-01/01:20", Date::dateToString(date2).c_str());
	Date date3(1800, 2, 29, 17, 20);
	EXPECT_STREQ("0000-00-00/00:00", Date::dateToString(date3).c_str());
}

TEST(DateTest, IntConstructor){
	Date date(2000, 9, 9, 9, 9);
	EXPECT_EQ(2000, date.getYear());
	EXPECT_EQ(9, date.getMonth());
	EXPECT_EQ(9, date.getDay());
	EXPECT_EQ(9, date.getHour());
	EXPECT_EQ(9, date.getMinute());
}