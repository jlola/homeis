/*
 * ValueEventArgs.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: Josef Lola
 */

#include "HisDevValue.h"
#include "ValueEventArgs.h"

ValueChangedEventArgs::ValueChangedEventArgs(HisDevValueBase* val)
{
	value = val;
}

ValueChangedEventArgs::ValueChangedEventArgs(ValueChangedEventArgs & src)
{
	value = src.value;
}

HisDevValueBase* ValueChangedEventArgs::GetValue()
{
	return value;
}
