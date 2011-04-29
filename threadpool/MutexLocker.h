// $Id: MutexLocker.h,v 23b46739a27e 2009/02/27 15:25:22 oliver $
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __THREADPOOL_MUTEXLOCKER_H_
#define __THREADPOOL_MUTEXLOCKER_H_

#include "Mutex.h"

class MutexLocker {
public:
    inline explicit MutexLocker(Mutex* mutex) : mtx(mutex)
    {
        if (mtx != NULL)
            mtx->lock();
    }

    inline ~MutexLocker(void)
    {
        if (mtx != NULL)
            mtx->unlock();
    }

private:
    Mutex* mtx;
};


#endif // __THREADPOOL_MUTEXLOCKER_H_
