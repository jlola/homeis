/*
 * HomeisTest.cpp
 *
 *  Created on: 28. 1. 2017
 *      Author: pc
 */


#include <stdio.h>

#include "gtest/gtest.h"

#ifdef HOMEISTEST

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  //::testing::GTEST_FLAG(filter) = "DevicesTests.TestLoadDeviceWithoutScanAndThenScan";
  //::testing::GTEST_FLAG(filter) = "ExpressionsTest.AddTagToExpressionThenRemove";
  return RUN_ALL_TESTS();
}

#endif


