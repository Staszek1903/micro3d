#include "touchable.h"

Touchable::Touchable()
    :handler(nullptr)
{
    setFillColor(sf::Color::Black);
}

Touchable::Touchable(float x, float y, float w, float h)
	:handler(nullptr)
{
	TouchBuffer::get().addHandler(*this);
    Renderer::get().addDrawable(*this);
	//Renderer::get().addDrawable(*this);
	setPosition(x,y);
	create_rect(w,h);
    setFillColor(sf::Color::Black);
}

Touchable::Touchable(float x, float y, const std::vector<sf::Vector2f > &points)
:points(points), handler(nullptr)
{
    TouchBuffer::get().addHandler(*this);
    Renderer::get().addDrawable(*this);
    setPosition(x,y);
    update();
    setFillColor(sf::Color::Black);
}

void Touchable::create(float x, float y, float w, float h)
{
    TouchBuffer::get().addHandler(*this);
    Renderer::get().addDrawable(*this);
    //Renderer::get().addDrawable(*this);
    setPosition(x,y);
    create_rect(w,h);
    setFillColor(sf::Color::Black);
}
	
void Touchable::handle(const TouchEvent & ev)
{
	float x,y;
	Renderer::get().mapPtoC(ev.x,ev.y,x,y);
	auto b = getGlobalBounds();
		
	if(x>b.left&&x<(b.left+b.width)&& y>b.top&&y<(b.top+b.height))
	{
        if(handler)
        {
        	//Console::get()<<"jest H\n";
            handler(ev);
        } //else
       	//	Console::get()<<"ni ma H\n";     
	} 
}

std::size_t Touchable::getPointCount() const
{
	return points.size();
}

sf::Vector2f Touchable::getPoint(std::size_t i) const
{
	return points.at(i);
}

void Touchable::create_rect(float w, float h)
{
	points = { {0.0f,0.0f},
						{w, 0.0f},
						{w,h},
						{0.0f,h}};
	update();
}
	
void Touchable::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    const size_t size = points.size();
    sf::VertexArray array(sf::TriangleFan, size);
	
	states.transform = getTransform();
	states.texture = getTexture();
	
    for(size_t i=0; i<size; ++i)
	{
		array[i].position = points.at(i);
		array[i].color = getFillColor();
	}
	
    target.draw(array, states);
}

void Touchable::setHandler(std::function<void (const TouchEvent &)> h)
{
    handler = h;
}

Touchable::~Touchable()
{
    TouchBuffer::get().removeHandler(*this);
    Renderer::get().removeDrawable(*this);
}
