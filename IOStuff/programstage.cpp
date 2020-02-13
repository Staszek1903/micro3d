#include "programstage.h"
#include "console.h"

std::unique_ptr<ProgramStage> ProgramStage::next;

std::unique_ptr<ProgramStage> ProgramStage::current;

ProgramStage::ProgramStage()
{}

ProgramStage::~ProgramStage()
{}

void ProgramStage::switch_stage()
{
	//Console::get()<<"switching stage\n";
	if(next)
	{
		next->init();
		if(current)
			current->release();
		current.swap(next);
        next.reset();
	}
}

void ProgramStage::input_stage(sf::Event &ev)
{
	if(current)
		current->input(ev);
}

void ProgramStage::update_stage(double dt)
{
	if(current)	
		current->update(dt);
}

void ProgramStage::render_stage(sf::RenderWindow & win)
{
	if(current)
		current->render(win);
}
