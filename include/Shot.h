#include "AnimatedSprite.hpp"

#ifndef SHOT_H
#define	SHOT_H
#include "Character.h"

class Shot {
public:
	enum Direction {
		dLeft,
		dRight
	};

	Shot(int);
	void init(sf::Time frameTime = sf::seconds(0.2f), float speed = 15);
	void setTexture(Direction, const sf::Texture& texture, sf::IntRect, int);
	void go(Direction);

	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f&);
	sf::Vector2f getPosition();
	void animate(sf::Time t);
	bool hitTest(Character c);

	AnimatedSprite sprite;

	Animation rightShot;
	Animation leftShot;
	float maxDistance;
private:
	void destroyShot();

	Direction direction;

	sf::Vector2f position;

	sf::Time m_frameTime;
	sf::Time m_currentTime;
	float shotSpeed;
	sf::Time refreshRate;
	float totalDistance;

	int damage;
};

#endif	/* CHARACTER_H */
