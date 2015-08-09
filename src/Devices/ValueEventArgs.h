/*
 * ValueEventArgs.h
 *
 *  Created on: Mar 15, 2013
 *      Author: root
 */

#ifndef VALUEEVENTARGS_H_
#define VALUEEVENTARGS_H_

#include <string>
#include <sys/time.h>
#include "EDataType.h"
#include "HisDateTime.h"

using namespace std;

class HisDevValueBase;

class ValueChangedEventArgs
{
	HisDevValueBase *value;
	public: ValueChangedEventArgs(HisDevValueBase* val);

	public: ValueChangedEventArgs(ValueChangedEventArgs & src);
	//ValueChangedEventArgs();
	HisDevValueBase* GetValue();
};

#endif /* VALUEEVENTARGS_H_ */
