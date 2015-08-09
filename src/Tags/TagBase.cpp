/*
 * TagBase.cpp
 *
 *  Created on: Mar 9, 2013
 *      Author: Josef Lola
 */
#include <string>
#include "srutil/event/event.hpp"
#include "srutil/delegate/delegate.hpp"

#include "EHisDevDirection.h"
#include "ValueEventArgs.h"
#include "HisDevBase.h"
#include "EDataType.h"

#include "TagBase.h"

TagBase::TagBase(HisDevBase  *pDev,EDataType dataType)
{
	type = dataType;
	dev = pDev;
}

void TagBase::OnValueChanged(ValueChangedEventArgs args)
{
//	switch((int)args.Type)
//	{
//		case EDataType::Bool:
//			break;
//		case EDataType::Double:
//			break;
//	}
}

void TagBase::Enable(bool enable)
{
	if (enable)
	{
		//OnValueChangedDelegate delegate = OnValueChangedDelegate::from_method<TagBase, &TagBase::OnValueChanged>(this);
		//dev->Register(delegate,this);
	}
	else
	{
		//dev->UnRegister(this);
	}
}

void TagBase::Refresh()
{

}

void TagBase::Save()
{

}

void TagBase::Load()
{

}
