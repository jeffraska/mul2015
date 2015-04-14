#include "AnimatedSprite.hpp"

#ifndef SHOT_H
#define	SHOT_H

class Shot {
public:
	enum Direction {
		dLeft,
		dRight
	};

	Shot();
	void init(sf::Time frameTime = sf::seconds(0.2f), float speed = 15);
	void setTexture(Direction, const sf::Texture& texture, sf::IntRect, int);
	void go(Direction);

	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f&);
	sf::Vector2f getPosition();
	void animate(sf::Time t);
	void move(float x);

	AnimatedSprite sprite;

	Animation rightShot;
	Animation leftShot;
private:
	Direction direction;

	sf::Vector2f position;

	sf::Time m_frameTime;
	sf::Time m_currentTime;
	float shotSpeed;
	sf::Time refreshRate;
};

#endif	/* CHARACTER_H */
