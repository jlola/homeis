/*
 * HisLock.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: Josef Lola
 */

#include "HisLock.h"
#include "logger.h"

BOOL EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
    pthread_mutex_t *lpMutex = (pthread_mutex_t*)lpCriticalSection;

    if(lpMutex == NULL)
        return FALSE;

    if(pthread_mutex_lock(lpMutex) != 0)
        return FALSE;

    return TRUE;
}

BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
    pthread_mutex_t *lpMutex = (pthread_mutex_t*)lpCriticalSection;

    if(lpMutex == NULL)
        return FALSE;

    return pthread_mutex_trylock(lpMutex);
}

BOOL LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
    pthread_mutex_t *lpMutex = (pthread_mutex_t*)lpCriticalSection;

    if(lpMutex == NULL)
        return FALSE;

    if(pthread_mutex_unlock(lpMutex) != 0)
        return FALSE;

    return TRUE;
}

BOOL InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
    pthread_mutex_t *lpMutex = (pthread_mutex_t*)lpCriticalSection;
    pthread_mutexattr_t mutexattr;

    if(lpMutex == NULL)
        return FALSE;

    pthread_mutexattr_init(&mutexattr);
    //pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_NORMAL);
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK_NP);

    if(pthread_mutex_init(lpMutex, &mutexattr) != 0)
        return FALSE;

    pthread_mutexattr_destroy(&mutexattr);

    return TRUE;
}

BOOL DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
    pthread_mutex_t *lpMutex = (pthread_mutex_t*)lpCriticalSection;
    if(lpMutex == NULL)
        return FALSE;

    if(pthread_mutex_trylock(lpMutex))
        pthread_mutex_unlock(lpMutex);
    else
        pthread_mutex_unlock(lpMutex);

    pthread_mutex_destroy(lpMutex);

    return TRUE;
}

HisLock::HisLock(LPCRITICAL_SECTION & csLock) :
	selfCreated(false),csLock(csLock)
{
	::EnterCriticalSection(this->csLock);
}

HisLock::HisLock() :
	selfCreated(true)
{
	csLock = CreateMutex();
}

LPCRITICAL_SECTION HisLock::CreateMutex()
{
	LPCRITICAL_SECTION _csLock = new CRITICAL_SECTION();
	::InitializeCriticalSection(_csLock);
	return _csLock;
}

void HisLock::DeleteMutex(LPCRITICAL_SECTION & _csLock)
{
	::DeleteCriticalSection(_csLock);
	delete(_csLock);
	_csLock = NULL;
}

void HisLock::Lock()
{
	::EnterCriticalSection(csLock);
}

void HisLock::Unlock()
{
	::LeaveCriticalSection(csLock);
}

HisLock::~HisLock()
{
	Unlock();
	if (selfCreated)
	{
		DeleteMutex(csLock);
	}
}
