// $Id: Job.h,v 23b46739a27e 2009/02/27 15:25:22 oliver $
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __THREADPOOL_JOB_H_
#define __THREADPOOL_JOB_H_

class Job {

    friend class WorkerThread;

public:
    Job(void) { }
    virtual ~Job() { }

protected:
    virtual void run(void) = 0;

};


#endif // __THREADPOOL_JOB_H_
