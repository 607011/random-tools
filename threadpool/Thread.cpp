// $Id: Thread.cpp,v 23b46739a27e 2009/02/27 15:25:22 oliver $
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifdef _DEBUG
#include <cstdio>
#endif

#include <pthread.h>

#include "Thread.h"
#include "Mutex.h"
#include "MutexLocker.h"


Thread::Thread(void) : finished(false), running(false), terminated(false)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&id, &attr, _start, this);
    pthread_attr_destroy(&attr);
}


Thread::~Thread()
{
    finish();
}


void Thread::finish(void)
{
    mutex.lock();
    finished = true;
    mutex.unlock();
    // ...
}


void Thread::start(void)
{
    if (!running)
    {
        mutex.lock();
        running = true;
        finished = false;
        terminated = false;
        mutex.unlock();
    }
}


int Thread::wait(void)
{
    int rc = pthread_join(id, NULL);
    mutex.lock();
    running = false;
    terminated = true;
    finished = true;
    mutex.unlock();
    return rc;
}


void* Thread::_start(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    // pthread_cleanup_push(Thread::_finish, arg);
    Thread* thr = reinterpret_cast<Thread *>(arg);
#ifdef _DEBUG
    pthread_t id = pthread_self();
    printf("Thread %08x (%08x)\n", id.p, thr->id.p);
#endif
    thr->run();
    return arg;
}


void Thread::_finish(void* arg)
{
    // ...
}


bool Thread::isRunning(void) const
{
    MutexLocker locker(&mutex);
    return running;
}


bool Thread::isFinished(void) const
{
    MutexLocker locker(&mutex);
    return finished;
}


bool Thread::isTerminated(void) const
{
    MutexLocker locker(&mutex);
    return terminated;
}
