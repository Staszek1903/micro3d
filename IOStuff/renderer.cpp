#include "renderer.h"

Renderer * Renderer::instance = nullptr;
sf::RenderWindow * Renderer::win_pointer = nullptr;

Renderer::Renderer()
{
	auto mode = sf::VideoMode::getDesktopMode();
    aspect_ratio = static_cast<float>(mode.height) / static_cast<float>(mode.width);
	
//	if(aspect_ratio >1.0f)
//	{
//		aspect_ratio = 1.0f/aspect_ratio;
//		view.setViewport(sf::FloatRect(0.0f,0.0f, 1.2f, aspect_ratio*aspect_ratio));
//	}
//	else
		view.setViewport(sf::FloatRect(0.0f,0.0f,1.2f,1.0f));
		
view.reset(sf::FloatRect(0,0,(PIXEL_WIDTH), (PIXEL_WIDTH*aspect_ratio)));
	win_pointer->setView(view);	
}

void Renderer::mapCtoP(float xi, float yi, float & xo,float & yo)
{
	auto out = win_pointer->mapCoordsToPixel(sf::Vector2f (xi,yi));
	xo = out.x;
	yo = out.y;
}

void Renderer::mapPtoC(float xi, float yi, float & xo,float & yo)
{
    auto out = win_pointer->mapPixelToCoords(sf::Vector2i(static_cast<int>(xi),static_cast<int>(yi)));
	xo = out.x;
	yo = out.y;
}
	
void Renderer::draw(sf::Drawable & d)
{
	win_pointer->draw(d);
}

void Renderer::addDrawable(sf::Drawable & d)
{
    drawables.push_back(&d);
}

void Renderer::removeDrawable(sf::Drawable &d)
{
    auto iter = std::find(drawables.begin(), drawables.end(), &d);
    drawables.erase(iter);
}

void Renderer::drawAll()
{
	for(auto d : drawables)
		draw(*d);
}

void Renderer::setWindow(sf::RenderWindow & win)
{
	win_pointer = &win;
}

Renderer& Renderer::get()
{
 	if(!instance)
 		instance = new Renderer();
 		
 	return *instance;
}

void Renderer::release()
{
	delete instance;
	instance = nullptr;
}
	
