#include "ui.h"

constexpr int def_pos_x = 150;
constexpr int def_pos_y = 500;
constexpr int w  = 50;

const Description default_ui[5] =
{
	 // { pos:{x,y}, vert:{ {x1,y1}, {x2,y2}, ...}  }  
	{  {def_pos_x,def_pos_y -60},  { {-w,-w},{w,-w},{0,w} }  },
	{  {def_pos_x+60, def_pos_y},  { {w,-w},{w,w},{-w,0} }  },
	{  {def_pos_x, def_pos_y+60},  { {w,w},{-w,w},{0,-w} }  },
	{  {def_pos_x-60, def_pos_y},  { {-w,w},{-w,-w},{w,0} }  },
	
	{  {900,500},  { {-w,-w},{w,-w},{w,w},{-w,w } }  }
};

UI::UI(){}

UI::~UI()
{
	for(auto t: nodes)
		if(t) delete t;
}

void UI::addNode( Touchable * t )
{
	nodes.push_back(t);
	Renderer::get().addDrawable(*t);
}

void UI::createDefault()
{
	for(int i=0; i<5; ++i)
	{
		auto t = new Touchable(default_ui[i].pos.x, default_ui[i].pos.y, default_ui[i].vert);
        Console::get()<<" Default UI elem added "<<i<<"\n";
			t->setFillColor(sf::Color(255,255,255,128));
	addNode(t);		
	
	}
}

void UI::setHandler(size_t t, std::function<void(const TouchEvent & ev)> h)
{
	if(t >= nodes.size()) return;
	nodes.at(t)->setHandler(h);
}
