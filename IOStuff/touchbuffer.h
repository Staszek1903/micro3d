#ifndef TOUCHBUFFER_H
#define TOUCHBUFFER_H

#include <vector>
#include "console.h"

class TouchEvent;
class KeyboardEvent;

class TouchHandler
{
public:
  virtual void handle(const TouchEvent & ev) = 0;
  virtual ~TouchHandler();
};

class KeyboardHandler
{
public:
	virtual void handle( const KeyboardEvent & ev) = 0;
    virtual ~KeyboardHandler();
};

class TouchEvent
{
public:
    enum State
    {
        BEGIN,
        SWIPE,
        END,
        NO_EVENT
    };

    TouchEvent(int x, int y, unsigned int finger, State state);
    TouchEvent();
    int x;
    int y;
    int finger;
    int state;
};

class KeyboardEvent
{
public:
	enum{
		PRESSED,
		RELEASED,
	};
	int state;
	int code;
	
	KeyboardEvent(int code, int state)
	: state(state), code(code){}	
};

class TouchBuffer
{
    static TouchBuffer * instance;
    TouchBuffer();

    std::vector <TouchHandler*> t_handlers;
    std::vector <KeyboardHandler*> k_handlers;
    TouchEvent current;

public:
    void emit(const TouchEvent &ev);
    void emit(const KeyboardEvent & ev);
    void addHandler(TouchHandler & h);
    void addHandler(KeyboardHandler & h);
    TouchEvent getCurrent();

    static TouchBuffer & get();
    static void release();
};

#endif // TOUCHBUFFER_H
