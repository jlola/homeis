/*
 * BlockingQueue.h
 *
 *  Created on: 9. 12. 2019
 *      Author: pc
 */

#ifndef SRC_COMMON_BLOCKINGQUEUE_H_
#define SRC_COMMON_BLOCKINGQUEUE_H_

#include <condition_variable>
#include <queue>
#include "IBlockingQueue.h"

template <typename T> class BlockingQueue : public IBlockingQueue<T>
{
  std::condition_variable _cvCanPop;
  std::mutex _sync;
  std::queue<T> _qu;
  bool _bShutdown = false;

public:
  void Push(const T& item)
  {
    {
      std::unique_lock<std::mutex> lock(_sync);
      _qu.push(item);
    }
    _cvCanPop.notify_one();
  }

  void RequestShutdown() {
    {
      std::unique_lock<std::mutex> lock(_sync);
      _bShutdown = true;
    }
    _cvCanPop.notify_all();
  }

  bool Pop(T &item) {
    std::unique_lock<std::mutex> lock(_sync);
    for (;;) {
      if (_qu.empty()) {
        if (_bShutdown) {
          return false;
        }
      }
      else {
        break;
      }
      _cvCanPop.wait(lock);
    }
    item = std::move(_qu.front());
    _qu.pop();
    return true;
  }

  size_t Size() {
	  std::unique_lock<std::mutex> lock(_sync);
	  return _qu.size();
  }
};


#endif /* SRC_COMMON_BLOCKINGQUEUE_H_ */
