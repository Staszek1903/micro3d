#include "program.h"
 Program::Program():win(sf::VideoMode::getDesktopMode(), "title")
{
    //win.setMouseCursorVisible(false);
	Console::set_window(&win);
	Console::get();
    Console::get().show(false);
    Renderer::setWindow(win);
	Renderer::get();

    console_button.create(10, 50, 50,
                          50);

    console_button.setFillColor(sf::Color(0, 0, 0, 128));
    console_button.setHandler([](const TouchEvent & ev){
        static bool shown = false;
        if (ev.state == TouchEvent::END)
        {
            shown = !shown;
            //Console::get() << "Console BUTTON" << shown << "\n";
            Console::get().show(shown);
        }
    });

    win.setFramerateLimit(60);
    Renderer::get().addDrawable(console_button);

	// auto mode = sf::VideoMode::getDesktopMode();
	// float aspect_ratio = (float)mode.height / (float)mode.width; 
	// Console::get()<<"aspect_ratio: "<<aspect_ratio<<" Pixelsize:
	// "<<PIXEL_WIDTH<<" "<<(PIXEL_WIDTH*aspect_ratio)<<"\n" << "width:
	// "<<mode.width << " height: "<<mode.height<<"\n";;
}

Program::~Program()
{
	Console::release();
	TouchBuffer::release();
}

void Program::run()
{
	// int tick = 0;
	while (win.isOpen())
	{
		// Console::get()<<"tick: "<<tick<<'\n';
		// tick++;
		try
		{
			ProgramStage::switch_stage();
			input();
            ProgramStage::update_stage(1.0/30.0);
			render();
		}
		catch(std::exception & exc)
		{

            //error_generate(exc.what());
			Console::get() << "EXCEPTION:\n " << exc.what() << "\n PRESS ANY KEY TO PROCEED\n";
			render();
			halt_for_input();
		}

    }

    /// PROGRAM STAGE RELEASE !!!!!!!!!!!!!!!!!!!!
}

sf::RenderWindow &Program::get_window()
{
    return win;
}

void Program::input()
{
	static int mousePressed = 0;
	sf::Event ev;
	while (win.pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:
			win.close();
			break;

		case sf::Event::TouchBegan:
			TouchBuffer::get().
				emit(TouchEvent(ev.touch.x, ev.touch.y, ev.touch.finger, TouchEvent::BEGIN));
			break;

		case sf::Event::TouchMoved:
			TouchBuffer::get().
				emit(TouchEvent(ev.touch.x, ev.touch.y, ev.touch.finger, TouchEvent::SWIPE));
			break;

		case sf::Event::TouchEnded:
			TouchBuffer::get().
				emit(TouchEvent(ev.touch.x, ev.touch.y, ev.touch.finger, TouchEvent::END));
			break;

		case sf::Event::MouseButtonPressed:
			TouchBuffer::get().
				emit(TouchEvent
					 (ev.mouseButton.x, ev.mouseButton.y, ev.mouseButton.button,
					  TouchEvent::BEGIN));
			mousePressed++;
			break;

		case sf::Event::MouseMoved:
			if (mousePressed > 0)
				TouchBuffer::get().
					emit(TouchEvent(ev.mouseMove.x, ev.mouseMove.y, 0, TouchEvent::SWIPE));
			break;

		case sf::Event::MouseButtonReleased:
			TouchBuffer::get().
				emit(TouchEvent
					 (ev.mouseButton.x, ev.mouseButton.y, ev.mouseButton.button, TouchEvent::END));
			mousePressed--;
			break;

		case sf::Event::KeyPressed:
			TouchBuffer::get().emit(KeyboardEvent(ev.key.code, KeyboardEvent::PRESSED));
            if(ev.key.code == sf::Keyboard::F12){
                    static bool shown = false;
                    shown = !shown;
                    Console::get().show(shown);
            }else if (ev.key.code == sf::Keyboard::Escape){
                win.close();
            }
			break;

		case sf::Event::KeyReleased:
			TouchBuffer::get().emit(KeyboardEvent(ev.key.code, KeyboardEvent::RELEASED));
			break;


		default:
			break;
		}

		ProgramStage::input_stage(ev);
	}
}

void Program::render()
{
	win.clear(sf::Color::Black);
	sf::RectangleShape shape;
	shape.setFillColor(sf::Color(80, 80, 10));
	shape.setPosition(0, 0);
	shape.setSize(sf::Vector2f(1000, 1000));
	win.draw(shape);

	ProgramStage::render_stage(win);
	Renderer::get().drawAll();
	Console::get().display();
	win.display();
}

void Program::halt_for_input()
{
	sf::Event ev;
	while (true)
		while (win.pollEvent(ev))
		{
			if (ev.type == sf::Event::KeyPressed)
			{
				if (ev.key.code == sf::Keyboard::Escape)
					win.close();
				else
					return;
			}
			else if (ev.type == sf::Event::TouchBegan)
				return;
		}
}
