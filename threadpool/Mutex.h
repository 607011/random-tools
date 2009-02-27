// $Id$
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.


#ifndef __THREADPOOL_MUTEX_H_
#define __THREADPOOL_MUTEX_H_

#include <pthread.h>


class Mutex {
    friend class WaitCondition;

public:
    inline explicit Mutex(void)
    {
        pthread_mutex_init(&mtx, NULL);
    }

    inline ~Mutex()
    {
        int rc = EBUSY;
        while (rc == EBUSY)
            rc = tryLock();
        pthread_mutex_unlock(&mtx);
        pthread_mutex_destroy(&mtx);
    }

    inline void lock(void)
    {
        pthread_mutex_lock(&mtx);
    }

    inline void unlock(void)
    {
        pthread_mutex_unlock(&mtx);
    }

    inline int tryLock(void)
    {
        return pthread_mutex_trylock(&mtx);
    }

private:
    pthread_mutex_t mtx;
};

#endif // __THREADPOOL_MUTEX_H_
