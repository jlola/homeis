/*
 * LuaExpressionTests.cpp
 *
 *  Created on: 8. 5. 2018
 *      Author: pc
 */
#include "gtest/gtest.h"
#include "fakeit.hpp"
#include "HisDevices.h"
#include "LuaExpression.h"
#include "HisDevValueEmail.h"
#include "IModbusProvider.h"
#include "HisdevVirtual.h"
#include "IFile.h"
#include "LuaExpressionTests.h"

using namespace fakeit;

LuaExpressionTests::LuaExpressionTests() {
}

TEST_F(LuaExpressionTests,WriteToEmailTagFromLuaExpression)
{
	Mock<IHisDevFactory> factoryMock;
	Mock<IFile> fileMock;
	Mock<IDirectory> directoryMock;
	When(Method(directoryMock,Exists)).AlwaysReturn(true);
	//.Using("exepath/Expressions/expressionName.lua")
	When(Method(fileMock,Delete)).AlwaysReturn();
	When(Method(fileMock,getexepath)).AlwaysReturn("exepath");
	When(Method(fileMock,ReadWholeFile)).AlwaysReturn("i = 1");
	When(Method(fileMock,Exists).Using("exepath/Expressions/expressionName.lua")).Return(false,true);
	Mock<IExpressionRuntime> expressionRuntimeMock;
	When(Method(factoryMock,GetFile)).AlwaysReturn(&fileMock.get());
	When(Method(factoryMock,GetDirectory)).AlwaysReturn(&directoryMock.get());
	IHisDevFactory* factory = &factoryMock.get();
	Mock<IModbusProvider> modbusProviderMock;
	IModbusProvider* modbusProvider = &modbusProviderMock.get();
	HisDevVirtual* virtualDev = new HisDevVirtual(factory);
	HisDevices devices("",modbusProvider);
	devices.Add(virtualDev);
	HisDevFolder folder("folderName",factory);
	LuaExpression expression(&folder,&devices,"expressionName",&expressionRuntimeMock.get(),factory);
	expression.SetExpression("");
	expression.Evaluate();
}

LuaExpressionTests::~LuaExpressionTests() {
}


