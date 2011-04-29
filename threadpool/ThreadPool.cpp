// $Id: ThreadPool.cpp,v 23b46739a27e 2009/02/27 15:25:22 oliver $
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifdef _DEBUG
#include <cstdio>
#endif

#include "ThreadPool.h"
#include "WorkerThread.h"
#include "MutexLocker.h"


ThreadPool::ThreadPool(int maxThreads)
{
	canceled = false;
	finishedSignalSent = false;
	numThreadsCompleted = 0;
	numJobs0 = 0;
	requestedPoolSize = -1;
    if (maxThreads <= 0)
        maxThreads = 1; // TODO: maxThreads aus Anzahl verfügbarer Prozessorkerne ermitteln
    setMaxThreadCount(maxThreads);
}


ThreadPool::~ThreadPool()
{
	waitForTerminated();
	pool.clear();
    jobs.clear();
}


// Append a job to the job queue.
// This function is thread-safe.
void ThreadPool::enqueue(Job* job)
{
    jobMutex.lock();
    jobs.push_back(job);
    jobMutex.unlock();
}


void ThreadPool::run(void)
{
	for (WorkerThreadList::iterator i = pool.begin(); i != pool.end(); ++i)
        (*i)->start();
    signalAll();
}


void ThreadPool::quitAll(void)
{
	for (WorkerThreadList::iterator i = pool.begin(); i != pool.end(); ++i)
        (*i)->terminate();
}


void ThreadPool::cancel(void)
{
    MutexLocker locker(&jobMutex);
	canceled = (jobs.size() == 0);
	jobs.clear();
}


void ThreadPool::signalAll(void)
{
    jobMutex.lock();
	int numJobs = numJobs0 = jobs.size();
	finishedSignalSent = false;
	numThreadsCompleted = 0;
    jobMutex.unlock();
	for (WorkerThreadList::iterator i = pool.begin(); i != pool.end(); ++i)
        (*i)->trigger();
}


void ThreadPool::waitForFinished(void)
{
    ready.acquire(pool.size());
}


void ThreadPool::jobsDone(void)
{
    ready.release();
    jobMutex.lock();
	if (jobs.size() == 0)
	{
		++numThreadsCompleted;
	}
	bool allThreadsCompleted = (numThreadsCompleted == pool.size());
    jobMutex.unlock();
	if (allThreadsCompleted && !finishedSignalSent)
	{
		finishedSignalSent = true;
		numJobs0 = 0;
	}
}


void ThreadPool::eraseThread(WorkerThreadList::iterator i)
{
	(*i)->terminate();
	(*i)->wait();
	pool.erase(i);
}


void ThreadPool::jobDone(WorkerThread* src)
{
    jobMutex.lock();
	int numJobsReady = numJobs0 - jobs.size();
    jobMutex.unlock();
	if (requestedPoolSize > pool.size())
	{
        MutexLocker locker(&poolMutex);
		for (WorkerThreadList::iterator i = pool.begin(); i != pool.end(); ++i)
		{
			if (src == (*i))
			{
				eraseThread(i);
				break;
			}
		}
	}
}


void ThreadPool::setPriority(int p)
{
	//priority = p;
	//for (WorkerThreadList::iterator i = pool.begin(); i != pool.end(); ++i)
	//	(*i)->setPriority(priority);
}


void ThreadPool::waitForTerminated(void)
{
	for (WorkerThreadList::iterator i = pool.begin(); i != pool.end(); ++i)
        (*i)->wait();
}


Job* ThreadPool::nextJob(void)
{
    Job* job = NULL;
    MutexLocker locker(&jobMutex);
    if (jobs.size() > 0)
    {
        job = jobs.front();
        jobs.pop_front();
    }
    return job;
}


void ThreadPool::setMaxThreadCount(unsigned int n)
{
    MutexLocker locker(&poolMutex);
	if (pool.size() > n)
	{
		int reduceCount = pool.size() - n;
		for (WorkerThreadList::iterator i = pool.begin(); (i != pool.end()) && (reduceCount > 0); ++i, --reduceCount)
		{
			if ((*i)->isWaiting())
			{
				eraseThread(i);
			}
			// if no thread is currently waiting, rely on jobDone()
			// to terminate inactive threads.
			requestedPoolSize = n;
		}
	}
	else
	{
		for (unsigned int i = pool.size(); i < n; ++i)
		{
			WorkerThread* newWorker = new WorkerThread(this);
            pool.push_back(newWorker);
#ifdef _DEBUG
            printf("Starting worker thread %d ...\n", i);
#endif
			newWorker->start();
			newWorker->trigger();
		}
	}
}
