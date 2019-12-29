/*
 * Event.h
 *
 *  Created on: 12. 10. 2019
 *      Author: pc
 */

#ifndef SRC_COMMON_EVENT_H_
#define SRC_COMMON_EVENT_H_

#include <mutex>
#include <condition_variable>

class Event
{
private:
    bool m_bFlag;
    bool isWaiting;
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_condition;

public:
    inline Event() : m_bFlag(false),isWaiting(false) { }

    inline void Wait() const
    {
        std::unique_lock< std::mutex > lock(m_mutex);
        m_condition.wait(lock,[&]()->bool{ return m_bFlag; });
    }

    //template< typename R,typename P >
	bool Wait(const int crRelTime)
	{
		isWaiting = true;
		std::chrono::milliseconds ms(crRelTime);
		std::unique_lock<std::mutex> lock(m_mutex);
		bool result = m_condition.wait_for(lock,ms,[&]()->bool{ return m_bFlag; });
		isWaiting = false;
		if (!result)
			return false;
		return true;
	}

    inline bool Signal()
    {
        bool bWasSignalled;
        m_mutex.lock();
        bWasSignalled = m_bFlag;
        m_bFlag = true;
        m_mutex.unlock();
        m_condition.notify_all();
        return bWasSignalled == false;
    }

    inline bool Reset()
    {
    	if (isWaiting) return false;
        bool bWasSignalled;
        m_mutex.lock();
        bWasSignalled = m_bFlag;
        m_bFlag = false;
        m_mutex.unlock();
        return bWasSignalled;
    }

    inline bool IsSet() const { return m_bFlag; }

    inline bool IsWaiting() const { return isWaiting; }
};

#endif /* SRC_COMMON_EVENT_H_ */
