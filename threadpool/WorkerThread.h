// $Id$
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __THREADPOOL_WORKERTHREAD_H_
#define __THREADPOOL_WORKERTHREAD_H_

#include <pthread.h>
#include <semaphore.h>

#include "ThreadPool.h"
#include "Thread.h"
#include "Mutex.h"
#include "WaitCondition.h"

class WorkerThread : public Thread
{
public:
	WorkerThread(ThreadPool* pool);
	~WorkerThread();

	virtual void run(void);
	void trigger(void);
	void terminate(void);
	inline bool isQuitting(void) const { return doQuit; }
	inline bool isWaiting(void) const { return idling; }

private:
	bool doQuit;
	bool idling;

	ThreadPool* pool;
	sem_t ready;

	Mutex triggerMutex;
	WaitCondition triggerCond;
};

#endif // __THREADPOOL_WORKERTHREAD_H_
