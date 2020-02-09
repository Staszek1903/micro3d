#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "console.h"
#include "touchbuffer.h"
#include "renderer.h"
#include "touchable.h"
#include "programstage.h"
#include <SFML/Graphics.hpp>

/**
 * @brief The Program class abstract class for real time SFML application
 */
class Program 
{
	void input();
	void render();
protected:
	sf::RenderWindow win;
 	Touchable console_button;
 	
    /**
     * @brief halt_for_input functions stops the program oand awaits event to either close the window or continue the program
     */
    void halt_for_input();
	
public:
	Program();
    virtual ~Program();
	
    /**
     * @brief run runs the program. exits when window is closed
     */
	void run();

    /**
     * @return window reference
     */
    sf::RenderWindow & get_window();
};


#endif /* PROGRAM_H_ */
