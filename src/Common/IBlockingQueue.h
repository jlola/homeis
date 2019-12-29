/*
 * IBlockingQueue.h
 *
 *  Created on: 9. 12. 2019
 *      Author: pc
 */

#ifndef SRC_COMMON_IBLOCKINGQUEUE_H_
#define SRC_COMMON_IBLOCKINGQUEUE_H_

#include <stddef.h>

template <typename T>
class IBlockingQueue
{
public:
	virtual void Push(const T& item) = 0;

	virtual bool Pop(T &item) = 0;

	virtual void RequestShutdown() = 0;

	virtual size_t Size()=0;

	virtual ~IBlockingQueue()
	{

	}
};


#endif /* SRC_COMMON_IBLOCKINGQUEUE_H_ */
