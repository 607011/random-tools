// $Id$
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
