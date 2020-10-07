#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>

#include <sstream>
#include <mutex>

class ConsoleData;

class Console
{
    static Console * instance;
    static sf::RenderWindow * win_pointer;
    static constexpr int content_size = 26;
    static constexpr int line_size = 80;
    static constexpr int character_size = 12;

	sf::Font font;
    sf::RenderTexture texture;
    sf::Sprite area;

    char content [content_size][line_size];
    int bottom = 2;
    int carriage =0;

    bool is_shown = true;
    bool need_redraw = true;
    float hide_level = 2.0f;

    Console(sf::RenderWindow &window);

public:
	sf::RenderWindow & window;

    /**
     *  @brief operator for outputing text daa to console
     *  @param console
     *  @param data any data that is convertable to text by stringstream
     *  @return console
     */
    template<typename T>
    friend Console & operator<<(Console & c, const T & data);
    friend Console & operator<<(Console & c, const sf::Vector2f &data);

    /**
     * @brief show shows or hides console
     * @param is
     */
    void show(bool is);

    /**
     * @brief draw internaly creates an graphical image of current console state
     */
	void draw();

    /**
     * @brief display draws grafical image on render window
     */
    void display();

    /**
     * @brief set_window sets render window which is to be passed to Console upon creation
     * @param window
     */
    static void set_window(sf::RenderWindow * window);

    /**
     * @brief get static console getter
     * @return instance of console
     */
    static Console & get();

    /**
     * @brief release frees the console data and deletes the instance
     */
    static void release();

    sf::Font &getFont();

private:
    void addContent(const std::string & text);
    void addChar(char c);
    void nextLine();
    void clearLine(int line);
};


template <typename T>
Console & operator<<(Console & c, const T &data)
{
    std::stringstream ss;
    std::string text;
    ss<<data;
    text = ss.str();

    c.addContent(text);

    return c;
}

Console &operator<<(Console &c, const sf::Vector2f &data);



#endif /* CONSOLE_H_ */
