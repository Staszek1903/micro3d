#ifndef UI_H_
#define UI_H_
#include <SFML/Graphics.hpp>
#include <vector>

#include "touchable.h"
#include "renderer.h"

struct Description
{
	sf::Vector2f pos;
	std::vector <sf::Vector2f> vert;
};

class UI
{
	std::vector <Touchable* > nodes;
public:
	UI();
	~UI();
	void addNode(Touchable * t );
	void createDefault();
    size_t getSize(){return nodes.size();}
	
    void setHandler(size_t t, std::function<void(const TouchEvent &)> h);
	
	enum Default
	{
		UP,
		RIGHT,
		DOWN,
		LEFT,
		BUTTON
	};
};

#endif /* DFAULTUI_H_ */
