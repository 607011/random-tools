// $Id: Thread_p.h,v 23b46739a27e 2009/02/27 15:25:22 oliver $
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __THREADPOOL_THREAD_P_H_
#define __THREADPOOL_THREAD_P_H_

#include <pthread.h>

class ThreadPrivate {

public:
    ThreadPrivate(void);
    ~ThreadPrivate();
    pthread_t id;
    pthread_cond_t done;
    static void *start(void* arg);
    static void finish(void* arg);

    bool running;
    bool finished;
    bool terminated;
};


#endif // __THREADPOOL_THREAD_P_H_
