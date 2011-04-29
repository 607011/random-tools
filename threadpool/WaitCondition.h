// $Id: WaitCondition.h,v 6aa0463352ef 2009/02/27 16:31:34 oliver $
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __THREADPOOL_WAITCONDITION_H_
#define __THREADPOOL_WAITCONDITION_H_

#include <cstdlib>

#include <pthread.h>

#include "Mutex.h"

class WaitCondition {
public:
    inline explicit WaitCondition(void)
    {
        pthread_cond_init(&cond, NULL);
    }

    inline ~WaitCondition()
    {
        pthread_cond_destroy(&cond);
    }

    inline void wait(Mutex* mutex)
    {
        while(pthread_cond_wait(&cond, &(mutex->mtx)) != 0)
            ;
    }

    inline void wake(void)
    {
        pthread_cond_signal(&cond);
    }

    inline void wakeAll(void)
    {
        pthread_cond_broadcast(&cond);
    }

private:
    pthread_cond_t cond;

};

#endif // __THREADPOOL_WAITCONDITION_H_
