#ifndef RENDERER_H_
#define RENDERER_H_

#include <SFML/Graphics.hpp>
#define PIXEL_WIDTH 1200.0f
#include <vector>

/**
 * @brief The Renderer class is a singleton class that takes care of render transformations of objects
 */
class Renderer
{
	static Renderer * instance;
	static sf::RenderWindow * win_pointer;
	
	sf::View view;
	float aspect_ratio= 0.0f;
	
	std::vector <sf::Drawable*> drawables;
	
	Renderer();
public:
    /**
     * @brief setWindow sets a window reference which is to be passed to instance upun its creation
     * @param win
     */
	static void setWindow(sf::RenderWindow & win);

    /**
     * @brief get singleton getter
     * @return renderer instance
     */
	static Renderer& get();

    /**
     * @brief release frees renderer data and deletes its instance
     */
	static void release();
	
    /**
     * @brief mapCtoP maps camera coordinates to coresponding picel on a screen
     * @param xi X input
     * @param yi Y input
     * @param xo X output
     * @param yo Y output
     */
	void mapCtoP(float xi, float yi, float & xo,float & yo);

    /**
     * @brief mapCtoP maps camera coordinates to coresponding picel on a screen
     * @param xi X input
     * @param yi Y input
     * @param xo X output
     * @param yo Y output
     */
    void mapPtoC(float xi, float yi, float & xo,float & yo);
	
	
    /**
     * @brief addDrawable adds drawable object to array
     * @param d
     */
	void addDrawable(sf::Drawable & d);
    void removeDrawable(sf::Drawable & d);

    /**
     * @brief drawAll draw all added drawable object
     */
	void drawAll();

    /**
     * @brief draw draws a single object
     * @param d
     */
	void draw(sf::Drawable & d);	
};

#endif
