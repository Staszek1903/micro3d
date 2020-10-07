#ifndef VERTEXTHREAD_H
#define VERTEXTHREAD_H

#include <thread>
#include <atomic>

#include "jobqueue.h"
#include "vertexjob.h"
#include "fragmentjob.h"
#include "../../IOStuff/console.h"

#include "../micro3d.h"
#include "../rasterization.h"
#include "../model.h"
#include "../shaders.h"
#include "../renderstate.h"

void make_fragment_job(unsigned int x, unsigned int y, void * arg);

class VertexWorker
{
    JobQueue<VertexJob> & vertex_queue;
    JobQueue<FragmentJob> & fragment_queue;
    std::thread * _thread = nullptr;
    void cthread_function();

    bool _stop_flag = false;
    std::atomic_bool _working_flag{false};

    static int id_counter;
    int _id;

    sf::Vector2u screen_size;
    sf::Image * img;
    float * z_buffer;

    static std::mutex mutex;

public:
    VertexWorker(JobQueue<VertexJob> & queue, JobQueue<FragmentJob> & fragment_queue, sf::Image * img, float * z_buff,
                 sf::Vector2u screen_size);
    VertexWorker(const VertexWorker&);
    ~VertexWorker();

    bool run();
    void stop();
    void join();
    bool isWorking();

    static int counter;
};

#endif // VERTEXTHREAD_H
