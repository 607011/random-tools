// $Id: ThreadPool.h,v 23b46739a27e 2009/02/27 15:25:22 oliver $
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __THREADPOOL_THREADPOOL_H_
#define __THREADPOOL_THREADPOOL_H_

#include <list>
#include <deque>

#if _MSC_VER >= 1500
#define HAS_MM_MONITOR
#include <intrin.h>
#endif

#include "Job.h"
#include "Mutex.h"
#include "Semaphore.h"

class WorkerThread;

typedef std::deque<Job*> JobQueue;
typedef std::list<WorkerThread*> WorkerThreadList;

class ThreadPool
{
public:
    /// ThreadPool constructor.
    /// Create a thread pool with the given number of worker threads.
    ThreadPool(int numThreads = DefaultThreadCount);

    /// ThreadPool destructor.
    /// Wait for all worker threads to return, clear job queue.
    /// This method is thread-safe.
    ~ThreadPool();

    void setMaxThreadCount(unsigned int);

    /// Add a job to the job queue.
    /// This method is thread-safe.
    void enqueue(Job*);

    /// Wait for worker threads to finish work
    void waitForFinished(void);

    /// Wait for worker threads to send ready event.
    void waitForTerminated(void);

    /// Start worker threads unless already running (\see start()).
    /// Then signal worker threads to take up their jobs (\see signalAll()).
    void run(void);

    /// Signal worker threads to quit (\see quitAll()),
    /// then trigger worker threads to process the job queue (\see signalAll()).
    void cancel(void);

	/// Return true if ThreadPool ended unexpectedly.
    bool isCanceled(void) const { return canceled; }

	/// Return next job in queue (returns NULL if queue is empty).
    Job* nextJob(void);

	/// This function is being called from a worker thread. 
	/// Calling this function tells the thread pool that the
	/// worker thread has finished *all* jobs, i.e. the job
	/// queue is empty.
	void jobsDone(void);

	/// This function is being called from a worker thread. 
	/// Calling this function tells the thread pool that the
	/// worker thread has finished a *single* job.
	void jobDone(WorkerThread*);

	/// Set priority of all threads in pool.
	void setPriority(int);

    enum _defnumthr { DefaultThreadCount = 4 };

private:
    WorkerThreadList pool;
    Semaphore ready;
	unsigned int requestedPoolSize;
	int numThreadsCompleted;
    Mutex jobMutex;
	Mutex poolMutex;
    std::deque<Job*> jobs;
	int numJobs0;
	bool canceled;
	bool finishedSignalSent;
	
    /// Trigger worker threads to process the job queue.
    void signalAll(void);

    /// Signal worker threads to quit by setting the doQuit flag.
    void quitAll(void);

	/// Terminate thread pointed to by WorkerThreadList::iterator
	/// and remove it from the pool. 
	/// This function is thread-safe.
	void eraseThread(WorkerThreadList::iterator i);
};

#endif // __THREADPOOL_THREADPOOL_H_
