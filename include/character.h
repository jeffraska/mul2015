#include "AnimatedSprite.hpp"

#ifndef CHARACTER_H
#define	CHARACTER_H

class character {
public:

	enum TextureType {
		tCharacterWalk,
		tGunWalk,
		tGunFire
	};

	enum Direction {
		dLeft,
		dRight
	};

	enum WalkStatus {
		wStop,
		wWalk
	};

	enum FireStatus {
		fHold,
		fFire
	};

	character();
	void init(sf::Time frameTime = sf::seconds(0.2f), float speed = 15);
	void setTexture(TextureType, Direction, const sf::Texture& texture, sf::IntRect, int);
	void go(Direction);
	void stop();
	void fire();
	void holdFire();
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f&);
	sf::Vector2f getPosition();
	void animate(sf::Time t);
	void move(float x);

	Direction getDirection() {
		return direction;
	};

	AnimatedSprite sprite;
	AnimatedSprite gun;
private:
	Animation rightWalk;
	Animation leftWalk;
	Animation gunRightWalk;
	Animation gunLeftWalk;
	Animation gunRightFire;
	Animation gunLeftFire;

	Direction direction;
	WalkStatus wStatus;
	FireStatus fStatus;

	sf::Vector2f position;

	sf::Time m_frameTime;
	sf::Time m_currentTime;
	float characterSpeed;
};

#endif	/* CHARACTER_H */

