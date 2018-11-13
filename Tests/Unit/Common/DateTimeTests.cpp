/*
 * DateTimeTests.cpp
 *
 *  Created on: 1. 7. 2017
 *      Author: pc
 */
#ifdef HOMEISTEST

#include "gtest/gtest.h"
#include <stdint.h>
#include <string>
#include "fakeit.hpp"
#include "Common/DateTime.h"
#include "DateTimeTests.h"

namespace AF {

DateTimeTests::DateTimeTests() {
	// TODO Auto-generated constructor stub

}

TEST_F(DateTimeTests,ToStringTest)
{
	tm stm;
	stm.tm_sec = 10;
	stm.tm_min = 47;
	stm.tm_hour = 9;
	stm.tm_year = 2017-1900;
	stm.tm_mon = 7-1;
	stm.tm_mday = 2;
	DateTime time(stm);
	//2012-04-23T18:25:43.511Z
	std::string strdate = time.ToString();
	const std::string expected = "2017-07-02T09:47:10.00Z";
	ASSERT_EQ(strdate,expected);
}

TEST_F(DateTimeTests,FromStringTest)
{
	const std::string expected = "2017-07-02T09:47:10.00Z";
	DateTime parsed = DateTime::Parse(expected);
	tm stm;
	stm.tm_sec = 10;
	stm.tm_min = 47;
	stm.tm_hour = 9;
	stm.tm_year = 2017-1900;
	stm.tm_mon = 7-1;
	stm.tm_mday = 2;
	DateTime _time(stm);
	ASSERT_EQ(parsed==_time,true);
}

TEST_F(DateTimeTests,OperatorGraterThanTest)
{
	tm stm;
	stm.tm_sec = 10;
	stm.tm_min = 47;
	stm.tm_hour = 9;
	stm.tm_year = 2017-1900;
	stm.tm_mon = 7-1;
	stm.tm_mday = 2;
	DateTime dt1(stm);
	stm.tm_min = 48;
	DateTime dt2(stm);
	ASSERT_EQ(dt1 < dt2,true);
	ASSERT_EQ(dt1 > dt2,false);
}

TEST_F(DateTimeTests,OperatorDateTimeEqualResultTrueTest)
{
	tm stm;
	stm.tm_sec = 10;
	stm.tm_min = 47;
	stm.tm_hour = 9;
	stm.tm_year = 2017-1900;
	stm.tm_mon = 7-1;
	stm.tm_mday = 2;
	DateTime dt1(stm);
	DateTime dt2(stm);
	ASSERT_EQ(dt1 == dt2,true);
}

TEST_F(DateTimeTests,OperatorDateTimeEqualResultFalseTest)
{
	tm stm;
	stm.tm_sec = 10;
	stm.tm_min = 47;
	stm.tm_hour = 9;
	stm.tm_year = 2017-1900;
	stm.tm_mon = 7-1;
	stm.tm_mday = 2;
	DateTime dt1(stm);
	stm.tm_min = 48;
	DateTime dt2(stm);
	ASSERT_EQ(dt1 == dt2,false);
}

TEST_F(DateTimeTests,OperatorDateTimeNotEqualResultTrueTest)
{
	tm stm;
	stm.tm_sec = 10;
	stm.tm_min = 47;
	stm.tm_hour = 9;
	stm.tm_year = 2017-1900;
	stm.tm_mon = 7-1;
	stm.tm_mday = 2;
	DateTime dt1(stm);
	stm.tm_min = 48;
	DateTime dt2(stm);
	ASSERT_EQ(dt1 != dt2,true);
}

TEST_F(DateTimeTests,OperatorDateTimeNotEqualResultFalseTest)
{
	tm stm;
	stm.tm_sec = 10;
	stm.tm_min = 47;
	stm.tm_hour = 9;
	stm.tm_year = 2017-1900;
	stm.tm_mon = 7-1;
	stm.tm_mday = 2;
	DateTime dt1(stm);
	DateTime dt2(stm);
	ASSERT_EQ(dt1 != dt2,false);
}

TEST_F(DateTimeTests,DateSubtract)
{
	int diffSecondsExpected = 11;
	tm stm;
	stm.tm_sec = 10;
	stm.tm_min = 47;
	stm.tm_hour = 9;
	stm.tm_year = 2017-1900;
	stm.tm_mon = 7-1;
	stm.tm_mday = 2;
	DateTime dt1(stm);
	stm.tm_sec += diffSecondsExpected;
	DateTime dt2(stm);
	auto diffSeconds = dt2 - dt1;
	ASSERT_EQ(diffSecondsExpected,diffSeconds);
}

DateTimeTests::~DateTimeTests() {
}

}

#endif
