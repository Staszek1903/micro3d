#ifndef JOBQUEUE_H
#define JOBQUEUE_H

#include <mutex>

template <class JobType, int LENGTH = 256>
class JobQueue
{
    JobType job_array[LENGTH];
    volatile int next_job = 0;
    volatile int last_job = 0;
    volatile int _size = 0;


    std::mutex synch_mutex;

public:
    JobQueue(){};

    bool isFull();
    bool isEmpty();

    int size();

    /**
     * @brief addJob - adds fob to queue
     *                  BLOCKS if queue full
     * @param job
     */
    void addJob(const JobType & job);

    /**
     * @brief fethJob
     * @return next job, null if queue empty
     */
    JobType fethJob();

    void close();
};


template<class JobType, int LENGTH>
bool JobQueue<JobType,LENGTH>::isFull()
{
    //std::lock_guard quard (synch_mutex);
    return (_size == LENGTH);
}

template<class JobType, int LENGTH>
bool JobQueue<JobType,LENGTH>::isEmpty()
{
    std::lock_guard quard (synch_mutex);
    return (_size == 0);
}

template<class JobType, int LENGTH>
int JobQueue<JobType,LENGTH>::size()
{
    std::lock_guard quard (synch_mutex);
    return _size;
}

template<class JobType, int LENGTH>
void JobQueue<JobType,LENGTH>::addJob(const JobType &job)
{
    //while(isFull()){}
    std::lock_guard quard (synch_mutex);
    if(_size == LENGTH) throw std::runtime_error(std::string("adding to full queue in ") + __FILE__);
    job_array[last_job++] = job;
    ++_size;
    last_job %= LENGTH;
}

template<class JobType, int LENGTH>
JobType JobQueue<JobType,LENGTH>::fethJob()
{
    //while(isEmpty()){}
    std::lock_guard quard (synch_mutex);
    if(_size == 0) throw std::runtime_error(std::string("fetch from empty job queue") + __FILE__);
    JobType & ret = job_array[next_job++];
    --_size;
    next_job %= LENGTH;

    return ret;
}

#endif // JOBQUEUE_H
