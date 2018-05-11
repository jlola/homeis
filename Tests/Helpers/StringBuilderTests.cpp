/*
 * StringBuilderTests.cpp
 *
 *  Created on: 6. 5. 2018
 *      Author: pc
 */

#include "StringBuilder.h"
#include "StringBuilderTests.h"

StringBuilderTests::StringBuilderTests() {
}

TEST_F(StringBuilderTests,StringBuilderTests_split_merge_success)
{
	std::string expectedstr = "one;two;three;four";
	vector<string> v = StringBuilder::split(expectedstr,';');
	std::string resultstr = StringBuilder::join(v,";");
	ASSERT_EQ(expectedstr,resultstr);
}

StringBuilderTests::~StringBuilderTests() {
}

