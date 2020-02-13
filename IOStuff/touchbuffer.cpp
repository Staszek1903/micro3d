#include "touchbuffer.h"

TouchBuffer * TouchBuffer::instance = nullptr;

TouchHandler::~TouchHandler()
{}

KeyboardHandler::~KeyboardHandler()
{}

TouchBuffer::TouchBuffer()
{

}

void TouchBuffer::emit(const TouchEvent &ev)
{
    current = ev;
    
    //Console::get()<<"handlers: "<<t_handlers.size()<<"\n";
    
    for(auto * h : t_handlers)
    	h->handle(ev);
}

void TouchBuffer::emit(const KeyboardEvent & ev)
{
	for(auto * h: k_handlers)
		h->handle(ev);
}

void TouchBuffer::addHandler( TouchHandler & h)
{
	TouchHandler *  ptr = &h; 
    t_handlers.push_back(ptr);
}

void TouchBuffer::removeHandler(TouchHandler &h)
{
    TouchHandler *  ptr = &h;
    auto iter = std::find(t_handlers.begin(), t_handlers.end(), ptr);
    t_handlers.erase(iter);
}

void TouchBuffer::addHandler( KeyboardHandler & h)
{
	KeyboardHandler * ptr = &h; 
	k_handlers.push_back(ptr);
}

void TouchBuffer::removeHandler(KeyboardHandler &h)
{
    KeyboardHandler *  ptr = &h;
    auto iter = std::find(k_handlers.begin(), k_handlers.end(), ptr);
    k_handlers.erase(iter);
}

TouchEvent TouchBuffer::getCurrent()
{
    return current;
}

TouchBuffer &TouchBuffer::get()
{
    if(!instance)
        instance = new TouchBuffer();
    return *instance;
}

void TouchBuffer::release()
{
    if(instance)
    {
        delete instance;
        instance = nullptr;
    }
}

TouchEvent::TouchEvent(int x, int y, unsigned int finger, State state)
    :x(x), y(y), finger(static_cast<int>(finger)), state(state)
{}

TouchEvent::TouchEvent()
    :x(0),y(0), finger(0), state(NO_EVENT)
{}
