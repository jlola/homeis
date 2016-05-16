/*
 * SignalTryCatch.cpp
 *
 *  Created on: 30. 11. 2015
 *      Author: pc
 */
#include <iostream>
#include <SignalTryCatch.h>

using namespace std;

class NullPointerException
{
	int data;
};

SignalTryCatch signaltrycatch;

void SigactionHookHandler( int iSignal, siginfo_t * psSiginfo, void * psContext)
{
   cout << "Signal Handler Exception Caught: std::exception -- signal : " << iSignal << " from SigactionHookHandler()" << endl;

   sigset_t x;
   sigemptyset (&x);
   sigaddset(&x, SIGSEGV);
   sigprocmask(SIG_UNBLOCK, &x, NULL);

   throw NullPointerException();
}


SignalTryCatch::SignalTryCatch() {
	//signal(SIGINT, SigactionHookHandler);       // Reactivate this handler.
	//struct sigaction oNewSigAction;

	 //memset(&oNewSigAction, 0, sizeof oNewSigAction);

	 //oNewSigAction.sa_sigaction = SigactionHookHandler;
	 //oNewSigAction.sa_flags     = SA_SIGINFO;

	 //int iResult = sigaction( SIGSEGV, &oNewSigAction, NULL );
}

SignalTryCatch::~SignalTryCatch() {
	// TODO Auto-generated destructor stub
}

