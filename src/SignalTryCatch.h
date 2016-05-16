/*
 * SignalTryCatch.h
 *
 *  Created on: 30. 11. 2015
 *      Author: pc
 */

#ifndef SRC_SIGNALTRYCATCH_H_
#define SRC_SIGNALTRYCATCH_H_

#include <csignal>
#include <csetjmp>

class SignalTryCatch {
	jmp_buf gBuffer;        // A buffer to hold info on where to jump to
public:
	SignalTryCatch();
	virtual ~SignalTryCatch();
};

extern SignalTryCatch signaltrycatch;

#endif /* SRC_SIGNALTRYCATCH_H_ */
