#ifndef FRAGMENTWORKER_H
#define FRAGMENTWORKER_H

#include <thread>
#include "jobqueue.h"
#include "fragmentjob.h"
#include "../../IOStuff/console.h"
#include <atomic>

//#include "../micro3d.h"
//#include "../rasterization.h"
//#include "../model.h"
#include "../shaders.h"
//#include "../renderstate.h"


class FragmentWorker
{
    JobQueue<FragmentJob> & fragment_queue;
    std::thread * _thread = nullptr;
    void cthread_function();
    bool _stop_flag = false;
    std::atomic_flag working{false};

    sf::Vector2u screen_size;
    sf::Image * img;

    static std::mutex mutex;

public:
    FragmentWorker(JobQueue<FragmentJob> & fragment_queue, sf::Image * img, sf::Vector2u screen_size);
    FragmentWorker(const FragmentWorker&other);
    ~FragmentWorker();

    bool run();
    void stop();
    void join();
    bool isWorking();
};

#endif // FRAGMENTWORKER_H
