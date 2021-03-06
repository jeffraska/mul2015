#include "AnimatedSprite.hpp"
#include "Weapon.h"
#include <SFML/Audio.hpp>

#ifndef CHARACTER_H
#define	CHARACTER_H

class Character {
public:

	enum TextureType {
		tCharacterWalk,
		/*tGunWalk,
		tGunFire*/
	};

	enum Direction {
		dLeft,
		dRight
	};

	enum WalkStatus {
		wStop,
		wWalk
	};

	enum JumpStatus
	{
		jNone,
		jJump,
		jFall
	};

	enum FireStatus {
		fHold,
		fFire
	};

	Character();
	~Character();
	void init(sf::Time frameTime = sf::seconds(0.2f), float speed = 15, int lives = 100);
	void setTexture(TextureType, Direction, const sf::Texture& texture, sf::IntRect, int);
	void go(Direction);
	void stop();

	void setWeapon(Weapon);

	void fire();
	void jump();
	void holdFire();
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f&);
	sf::Vector2f getPosition();
	sf::Vector2f getCenter();
	void animate(sf::Time t, float groundY);

	Direction getDirection() {
		return direction;
	};

	AnimatedSprite sprite;
	Weapon weapon;
	int lives;
private:
	Animation rightWalk;
	Animation leftWalk;

	Direction direction;
	WalkStatus wStatus;
	FireStatus fStatus;
	JumpStatus jStatus;

	sf::Vector2f position;

	sf::Time m_frameTime;
	sf::Time m_currentTime;
	float characterSpeed;
	sf::Time refreshRate;

	sf::Time jumpFrameTime;
	sf::Time jumpCurrentTime;
	float jumpProgress;

	sf::Sound shotSound;
};

#endif	/* CHARACTER_H */

