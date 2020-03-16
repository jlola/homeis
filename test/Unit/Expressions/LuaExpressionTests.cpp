/*
 * LuaExpressionTests.cpp
 *
 *  Created on: 8. 5. 2018
 *      Author: pc
 */
#ifdef HOMEISTEST

#include "gtest/gtest.h"
#include "fakeit.hpp"
#include "HisDevices.h"
#include "LuaExpression.h"
#include "HisDevValueEmail.h"
#include "IModbusProvider.h"
#include "HisdevVirtual.h"
#include "IFile.h"
#include "LuaExpressionTests.h"
#include "StringBuilder.h"

using namespace fakeit;

LuaExpressionTests::LuaExpressionTests() {
}

TEST_F(LuaExpressionTests,WriteToEmailTagFromLuaExpression)
{
	string fromAddr = "fromAddr";
	string strreceivers = "receiver1;receiver2";
	string subject = "subject";
	string body = "test body";

	Mock<IWriteToDevice> deviceWriterMock;
	ValueChangedEventArgs args(NULL);
	When(Method(deviceWriterMock,WriteToDevice).Using(Any<ValueChangedEventArgs>())).AlwaysReturn();

	Mock<IEmailSender> emailSenderMock;
	When(Method(emailSenderMock,Send)).AlwaysReturn();
	IEmailSender* emailSender = &emailSenderMock.get();

	Mock<IDirectory> directoryMock;
	When(Method(directoryMock,Exists)).AlwaysReturn(true);

	Mock<IFile> fileMock;
	When(Method(fileMock,Delete)).AlwaysReturn();
	When(Method(fileMock,getexepath)).AlwaysReturn("exepath");

	string luascript = StringBuilder::Format("email = '%s'",body.c_str());
	When(Method(fileMock,ReadWholeFile)).AlwaysReturn(luascript);
	When(Method(fileMock,Exists).Using("exepath/Expressions/expressionName.lua")).Return(false,true);

	Mock<IExpressionRuntime> expressionRuntimeMock;
	When(Method(expressionRuntimeMock,Add)).AlwaysReturn();

	Mock<IHisDevFactory> factoryMock;
	When(Method(factoryMock,GetFile)).AlwaysReturn(&fileMock.get());
	When(Method(factoryMock,GetDirectory)).AlwaysReturn(&directoryMock.get());
	When(Method(factoryMock,GetEmailSender)).AlwaysReturn(emailSender);
	IHisDevFactory* factory = &factoryMock.get();

	Mock<IModbusProvider> modbusProviderMock;
	IModbusProvider* modbusProvider = &modbusProviderMock.get();
	HisDevVirtual* virtualDev = new HisDevVirtual(factory);
	HisDevices devices("fileName",modbusProvider);
	devices.Add(virtualDev);
	HisDevValueEmail* emailValue = new HisDevValueEmail(&deviceWriterMock.get(), factory);
	emailValue->SetFromAddr(fromAddr);
	emailValue->SetReceivers(strreceivers);
	emailValue->SetAddressName("email");
	emailValue->SetName(subject);
	virtualDev->Add(emailValue);
	HisDevFolder folder("folderName",factory);
	HisDevValueId* valueId = new HisDevValueId(emailValue,factory);
	folder.Add(valueId);
	LuaExpression expression(&folder,&devices,"expressionName",&expressionRuntimeMock.get(),factory);
	expression.SetExpression("");
	expression.SetRunning(true);
	expression.Evaluate();
	Verify(Method(emailSenderMock,Send).Using(fromAddr,strreceivers,subject,body));
}

LuaExpressionTests::~LuaExpressionTests() {
}

#endif
