#include "fragmentworker.h"

std::mutex FragmentWorker::mutex;

void FragmentWorker::cthread_function()
{
    m3d::PixelArgSimple pixel_arg(*img, (Color){255,255,255});
    FragmentJob next_job;
    while (true) {
        {   // FETCH BLOCK
            std::lock_guard guard(mutex);
            while (fragment_queue.isEmpty()) {
                if(_stop_flag) break;
            }
            if(_stop_flag) break;
            next_job = fragment_queue.fethJob();
        }
        working.test_and_set();
        m3d::putPixelSimple(next_job.x, next_job.y, &pixel_arg);
        working.clear();

    }
}

FragmentWorker::FragmentWorker(JobQueue<FragmentJob> &fragment_queue, sf::Image *img, sf::Vector2u screen_size)
    :fragment_queue(fragment_queue), screen_size(screen_size), img(img) {}

FragmentWorker::FragmentWorker(const FragmentWorker & other)
    :fragment_queue(other.fragment_queue), screen_size(other.screen_size), img(other.img){}

FragmentWorker::~FragmentWorker(){ if(_thread) delete _thread; }

bool FragmentWorker::run()
{
    try{
        _thread = new std::thread(&FragmentWorker::cthread_function, this);
        //_id = id_counter++;
    }catch(...){
        return false;
    }
    return true;
}

void FragmentWorker::stop()
{
    _stop_flag = true;
}

void FragmentWorker::join()
{
    if(_thread)
        _thread->join();
}

bool FragmentWorker::isWorking()
{
    bool ret = working.test_and_set();
    if(!ret) working.clear();
    return ret;
}
