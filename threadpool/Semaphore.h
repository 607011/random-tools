// $Id$
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __THREADPOOL_SEMAPHORE_H_
#define __THREADPOOL_SEMAPHORE_H_

#include <cassert>

#include "Mutex.h"
#include "MutexLocker.h"
#include "WaitCondition.h"

class Semaphore {
public:
    inline explicit Semaphore(int n = 0) : avail(n)
    {
        assert(n >= 0);
    }

    inline ~Semaphore()
    {
    }

    inline void release(int n = 1)
    {
        assert(n >= 0);
        MutexLocker locker(&mutex);
        avail += n;
        cond.broadcast();
    }

    inline void acquire(int n = 1)
    {
        MutexLocker locker(&mutex);
        while (n > avail)
            cond.wait(&mutex);
        avail -= n;
    }

    inline int available() const
    {
        MutexLocker locker(&mutex);
        return avail;
    }



private:
    int avail;
    mutable Mutex mutex;
    WaitCondition cond;
};

#endif // __THREADPOOL_SEMAPHORE_H_
