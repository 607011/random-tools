// $Id$
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifdef _DEBUG
#include <cstdio>
#endif

#include <pthread.h>

#include "WorkerThread.h"
#include "Mutex.h"
#include "MutexLocker.h"

WorkerThread::WorkerThread(ThreadPool* pool) : pool(pool), doQuit(false), idling(false)
{
    // ....
}


WorkerThread::~WorkerThread()
{
	terminate();
	wait();
}


void WorkerThread::terminate(void)
{
    MutexLocker locker(&triggerMutex);
	doQuit = true;
    triggerCond.wake();
}


void WorkerThread::run(void)
{
#ifdef _DEBUG
    pthread_t id = pthread_self();
    printf("WorkerThread %08x running ...\n", id.p);
#endif
	while (!doQuit)
	{
		bool wasBusy = false;
		Job* job;
		while ((job = pool->nextJob()) != NULL)
		{
			job->run();
			wasBusy = true;
			pool->jobDone(this);
		}
		if (wasBusy)
		{
			pool->jobsDone();
		}
#ifdef _DEBUG
        printf("WorkerThread %08x waiting for trigger ...\n", id.p);
#endif
        triggerMutex.lock();
		idling = true;
        triggerCond.wait(&triggerMutex);
		idling = false;
#ifdef _DEBUG
        printf("WorkerThread %08x triggered.\n", id.p);
#endif
        triggerMutex.unlock();
	}
	if (doQuit)
	{
		pool->jobDone(this);
	}
#ifdef _DEBUG
    printf("WorkerThread %08x quitting ...\n", id.p);
#endif
    pthread_exit(NULL);
}


void WorkerThread::trigger(void)
{
    MutexLocker locker(&triggerMutex);
	if (!isRunning())
	{
		start();
	}
	else
	{
		triggerCond.wake();
	}
}
