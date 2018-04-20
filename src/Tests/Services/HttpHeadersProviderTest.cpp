/*
 * HttpHeadersProviderTest.cpp
 *
 *  Created on: 7. 4. 2018
 *      Author: pc
 */

#include "HttpHeadersProviderTest.h"
#include "HttpHeadersProvider.h"

#ifdef HOMEISTEST

namespace AF {


HttpHeadersProviderTest::HttpHeadersProviderTest() {
}

TEST_F(HttpHeadersProviderTest,GetErrorMessageJson_TestCreateMessage)
{
	HttpHeadersProvider headersProvider("*");
	string json = headersProvider.GetErrorMessageJson("Error message");
	string expected = "{\n    \"error\": \"Error message\"\n}";
	printf(json.c_str());
	ASSERT_EQ(json,expected);
}

HttpHeadersProviderTest::~HttpHeadersProviderTest() {
}
}

#endif
