// $Id$

#ifdef WIN32
#include <Windows.h>
#endif

#include <cstdio>
#include <cstdlib>


#include "ThreadPool.h"
#include "WorkerThread.h"

class MyJob : public Job
{
public:
    MyJob(int i) : d(i)
    {
        printf("Job %d initialising.\n", d);
    }
    ~MyJob()
    {
        printf("Job %d ready.\n", d);
    }
    void run(void)
    {
        printf("This is job %d.\n", d);
    }
private:
    int d;
};


int main(int argc, char* argv[])
{
    int poolSize = ThreadPool::DefaultThreadCount;
    if (argc > 2)
        poolSize = atoi(argv[1]);
    ThreadPool pool(poolSize);

    for (int i = 0; i < 23; ++i)
        pool.enqueue(new MyJob(i+1));

    printf("Main thread waiting for jobs to finish ...\n");
    pool.waitForFinished();

    printf("\nReady.\n");
    return 0;
}
