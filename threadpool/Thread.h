// $Id: Thread.h,v 6aa0463352ef 2009/02/27 16:31:34 oliver $
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __THREADPOOL_THREAD_H_
#define __THREADPOOL_THREAD_H_

#ifdef _DEBUG
#include <cstdio>
#endif

#include <pthread.h>

#include "Mutex.h"


class Thread {
public:
    explicit Thread(void);
    ~Thread(void);

    bool isRunning(void) const;
    bool isFinished(void) const;
    bool isTerminated(void) const;

    void start(void);
    void finish(void);

    int wait(void);

    static void* _start(void*);
    static void _finish(void*);

protected:
    virtual void run(void)
    {
#ifdef _DEBUG
        printf("default run() method of thread %08x", id.p);
#endif
    }

private:
    pthread_t id;
    bool finished;
    bool running;
    bool terminated;
    mutable Mutex mutex;

};


#endif // __THREADPOOL_THREAD_H_
