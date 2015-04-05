#include "AnimatedSprite.hpp"
#include <iostream>

#ifndef WEAPON_H
#define	WEAPON_H

using namespace std;

class Weapon {
public:

	enum TextureType {
		tGunWalk,
		tGunFire
	};

	enum Direction {
		dLeft,
		dRight
	};

	Weapon();
	Weapon(string name, sf::Vector2f offset, sf::IntRect walkR, int walkRCount, sf::IntRect walkL, int walkLCount, sf::IntRect fireR, int fireRCount, sf::IntRect fireL, int fireLCount);
	void setTexture(TextureType, Direction, const sf::Texture& texture, sf::IntRect, int);

	AnimatedSprite sprite;
	Animation gunRightWalk;
	Animation gunLeftWalk;
	Animation gunRightFire;
	Animation gunLeftFire;

	string name;
	sf::Vector2f weaponOffset;
};

#endif	/* WEAPON_H */

