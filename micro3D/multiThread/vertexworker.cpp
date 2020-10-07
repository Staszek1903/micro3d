#include "vertexworker.h"

int VertexWorker::id_counter = 0;
int VertexWorker::counter = 0;
std::mutex VertexWorker::mutex;

void VertexWorker::cthread_function()
{
    Color colors[3];
    Point3 vertices_projected[3];
    Point3 vertices_world[3];
    Point3 normal;
    LightPoint light_point;

    m3d::PixelArg pixel_arg(colors, vertices_projected, vertices_world, normal, light_point);
    pixel_arg.image = img;
    pixel_arg.z_buffer = z_buffer;
    pixel_arg.screen_size = screen_size;
            // uzuełnić wszystkie pola w ixel_arg !!!!!!!!!!!!!!!1

    m3d::PixelArgSimple pixel_arg_simple(*img, (Color){255,255,255});

    RasterFunc func_simple;
    func_simple.putPixel = /*make_fragment_job;*/ m3d::putPixelSimple;// m3d::putPixel;
    func_simple.arg = /*&fragment_queue;*/ &pixel_arg_simple;

    RasterFunc func;
    func.putPixel = m3d::putPixelZDebug;
    func.arg = &pixel_arg;

    VertexJob job;
    while (true) {
        {   // FETCH BLOCK
            std::lock_guard guard(mutex);
            while (vertex_queue.isEmpty()) {
                if(_stop_flag) break;
            }
            if(_stop_flag) break;
            job = vertex_queue.fethJob();
        }
//        std::mutex m;
//        m.lock();
//        Console::get()<<"V worker "<<_id<<" fetched job "<<(counter++)<<"\n";
//        m.unlock();
        _working_flag.store(true);

        pixel_arg.colors[0] = pixel_arg_simple.colors[0] = job.colors[0];
        pixel_arg.colors[1] = pixel_arg_simple.colors[1] = job.colors[1];
        pixel_arg.colors[2] = pixel_arg_simple.colors[2] = job.colors[2];


        Point3 v1 = job.triangle[0],
               v2 = job.triangle[1],
               v3 = job.triangle[2];
        v1 = transformPoint(v1, &(job.mvp));
        v2 = transformPoint(v2, &(job.mvp));
        v3 = transformPoint(v3, &(job.mvp));

        vertices_projected[0] = v1;
        vertices_projected[1] = v2;
        vertices_projected[2] = v3;

        vertices_world[0] = transformPoint(job.triangle[0], &job.model_matrix);
        vertices_world[1] = transformPoint(job.triangle[1], &job.model_matrix);
        vertices_world[2] = transformPoint(job.triangle[2], &job.model_matrix);

        normal = getTriangleNormal(v1,v2,v3);

        Point3 clip_input [3] = {v1,v2,v3};
        Point3 clip_result[64][3];
        int clip_size = clipToScreen(clip_input, 0.0f, 1.0f, clip_result);
        for(int j= 0; j<clip_size; ++j){
            v1 = toScreenSpace(clip_result[j][0], screen_size.x, screen_size.y);
            v2 = toScreenSpace(clip_result[j][1], screen_size.x, screen_size.y);
            v3 = toScreenSpace(clip_result[j][2], screen_size.x, screen_size.y);

            Triangle tr = make_triange(v1,v2,v3);
           // strokeTriangle(tr, func_simple);
            fillTriangleBres(v1,v2,v3, func);
        }
        _working_flag.store(false);
    }
}

VertexWorker::VertexWorker(JobQueue<VertexJob> &queue, JobQueue<FragmentJob> &fragment_queue, sf::Image *img, float *z_buff, sf::Vector2u screen_size)
    : vertex_queue(queue), fragment_queue(fragment_queue), screen_size(screen_size), img(img), z_buffer(z_buff) {}

VertexWorker::VertexWorker(const VertexWorker & other)
    : vertex_queue(other.vertex_queue), fragment_queue(other.fragment_queue), screen_size(other.screen_size), img(other.img),
      z_buffer(other.z_buffer)
{}

VertexWorker::~VertexWorker() { if(_thread) delete _thread; }

bool VertexWorker::run(){
    try{
        _thread = new std::thread(&VertexWorker::cthread_function, this);
        _id = id_counter++;
    }catch(...){
        return false;
    }
    return true;
}

void VertexWorker::stop()
{
    _stop_flag = true;
}

void VertexWorker::join()
{
    if(_thread)
        _thread->join();
}

bool VertexWorker::isWorking()
{
    return _working_flag.load();
}

void make_fragment_job(unsigned int x, unsigned int y, void *arg)
{
    static std::mutex m;
    FragmentJob job;
    job.x = x;
    job.y = y;

    auto * q = static_cast<JobQueue<FragmentJob>*> (arg);

    m.lock();
//    int s;
//    if((s = q->size())>1){
//        int a = q->size();
//        a = a+a;
//    }
    while(q->isFull()){}
    q->addJob(job);
    m.unlock();
    //m3d::putPixelSimple(x,y,arg);
}
