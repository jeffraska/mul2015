#include <SFML/Graphics.hpp>
#include "AnimatedSprite.hpp"

#ifndef GROUNDPANEL_H
#define	GROUNDPANEL_H

using namespace std;


#pragma once
class GroundPanel
{
public:
	GroundPanel();
	~GroundPanel();

	sf::Vector2f position;
	sf::Vector2i size;
	int type;
	int lives;
	sf::Sprite sprite;
	std::string sType;

	sf::Vector2f getPosition();
	sf::Vector2i getSize();
	int getType();
	int getLives();
	void init(sf::Vector2f, int, int);
	std::string getSType();

private:
	
};
#endif	/* GROUNDPANEL_H */
