#include "GroundPanel.h"
#include "AnimatedSprite.hpp"


GroundPanel::GroundPanel(){
	//refreshRate = sf::milliseconds(75);
}

void GroundPanel::init(sf::Vector2f pos, int type_, int lives_){
	position = pos;
	size = sf::Vector2i(300, 120);
	type = type_;
	lives = lives_;
	sType = std::to_string(type_);
}

sf::Vector2f GroundPanel::getPosition(){
	return position;
}

sf::Vector2i GroundPanel::getSize(){
	return size;
}

int GroundPanel::getType(){
	return type;
}

std::string GroundPanel::getSType(){
	return sType;
}

int GroundPanel::getLives(){
	return lives;
}

GroundPanel::~GroundPanel()
{
}
