#define _USE_MATH_DEFINES

#include "Character.h"
#include "game.h"
#include <cmath>

Character::Character() {
	refreshRate = sf::milliseconds(75);
}

void Character::init(sf::Time frameTime, float speed, int lives) {
	direction = dRight;
	wStatus = wStop;
	fStatus = fHold;

	sprite.setFrameTime(refreshRate);
	sprite.setAnimation(rightWalk);
	sprite.stop();
	sprite.setLooped(true);

	weapon.sprite.setFrameTime(refreshRate);
	weapon.sprite.setAnimation(weapon.gunRightWalk);
	weapon.sprite.stop();
	weapon.sprite.setLooped(true);
	weapon.sprite.setPosition(weapon.weaponOffset);

	shotSound.setBuffer(game::getInstance().sounds[weapon.name]);
	shotSound.setVolume(50);

	m_frameTime = frameTime;
	characterSpeed = speed;

	jumpFrameTime = sf::milliseconds(10);
	jStatus = jFall;

	this->lives = lives;
}

void Character::setTexture(
	TextureType type,
	Direction dir,
	const sf::Texture& texture,
	sf::IntRect rect, int count) {
	Animation *anim = nullptr;

	if (type == tCharacterWalk) {
		if (dir == dRight)
			anim = &rightWalk;
		else
			anim = &leftWalk;
	}

	anim->setSpriteSheet(texture);
	anim->addXFrames(rect, count);
}

void Character::setWeapon(Weapon w) {
	weapon = w;
	weapon.sprite.setFrameTime(refreshRate);
	setPosition(getPosition());

	shotSound.setBuffer(game::getInstance().sounds[weapon.name]);

	if (wStatus == wWalk) {
		wStatus = wStop;
		go(direction);
	}
	else {
		stop();
	}

	if (fStatus == fFire) {
		fire();
	}
	else {
		holdFire();
	}
}

void Character::go(Direction d) {
	if (d != direction || wStatus == wStop) {
		direction = d;
		wStatus = wWalk;

		if (direction == dRight) {
			weapon.sprite.setPosition(position.x + weapon.weaponOffset.x, position.y + weapon.weaponOffset.y);
		}
		else {
			weapon.sprite.setPosition(position.x - weapon.weaponOffset.x, position.y + weapon.weaponOffset.y);
		}

		sprite.stop();

		if (d == dRight) {
			sprite.play(rightWalk);
		}
		else {
			sprite.play(leftWalk);
		}

		if (fStatus == fFire) {
			fire();
		}
		else {
			holdFire();
		}
	}
}

void Character::stop() {
	sprite.pause();
	wStatus = wStop;

	if (fStatus == fHold)
		holdFire();
}

void Character::fire() {
	fStatus = fFire;
	
	if (direction == dRight)
		weapon.sprite.play(weapon.gunRightFire);
	else
		weapon.sprite.play(weapon.gunLeftFire);
}

void Character::jump()
{
	if (jStatus != jJump)
	{ 
		jStatus = jJump;
		jumpCurrentTime = sf::Time::Zero;
		jumpProgress = 0;
	}
}

void Character::holdFire() {
	fStatus = fHold;
	weapon.m_currentTime = weapon.m_frameTime;

	if (direction == dRight)
		weapon.sprite.play(weapon.gunRightWalk);
	else
		weapon.sprite.play(weapon.gunLeftWalk);

	if (wStatus == wStop) {
		weapon.sprite.pause();
	}
}

void Character::setPosition(float x, float y) {
	position = sf::Vector2f(x, y);
	sprite.setPosition(position);

	sprite.setPosition(x, y);
	if (direction == dRight) {
		weapon.sprite.setPosition(x + weapon.weaponOffset.x, y + weapon.weaponOffset.y);
	}
	else {
		weapon.sprite.setPosition(x - weapon.weaponOffset.x, y + weapon.weaponOffset.y);
	}
}

void Character::setPosition(const sf::Vector2f& pos) {
	position = pos;
	sprite.setPosition(position);

	sprite.setPosition(pos.x, pos.y);
	if (direction == dRight) {
		weapon.sprite.setPosition(pos.x + weapon.weaponOffset.x, pos.y + weapon.weaponOffset.y);
	}
	else {
		weapon.sprite.setPosition(pos.x - weapon.weaponOffset.x, pos.y + weapon.weaponOffset.y);
	}
}

sf::Vector2f Character::getPosition() {
	return position;
}

void Character::animate(sf::Time deltaTime, float groundY) {
	if (wStatus == wWalk) {
		m_currentTime += deltaTime;
		if (m_currentTime >= m_frameTime) {
			m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_frameTime.asMicroseconds());

			if (direction == dRight)
				setPosition(position.x + characterSpeed, position.y);
			else
				setPosition(position.x - characterSpeed, position.y);
		}
	}

	if (fStatus == fFire)
	{
		weapon.m_currentTime += deltaTime;
		if (weapon.m_currentTime > weapon.m_frameTime)
		{
			weapon.m_currentTime = sf::microseconds(weapon.m_currentTime.asMicroseconds() % weapon.m_frameTime.asMicroseconds());

			game &g = game::getInstance();
			Shot *s = new Shot(weapon.damage);
			s->maxDistance = weapon.maxDistance + (rand() % static_cast<int>(weapon.maxDistance/10));
			s->setTexture(Shot::dRight, g.textures[weapon.name + "shot"], weapon.ShotRight, weapon.ShotRightCount);
			s->setTexture(Shot::dLeft, g.textures[weapon.name + "shot"], weapon.ShotLeft, weapon.ShotLeftCount);

			s->init(sf::milliseconds(10), weapon.shotSpeed);

			if (direction == dRight)
			{
				s->setPosition(position.x + weapon.weaponOffset.x + weapon.gunRightFire.getFrame(0).width, position.y + weapon.weaponOffset.y + s->leftShot.getFrame(0).height);
				s->go(Shot::dRight);
			}
			else
			{
				s->setPosition(position.x - weapon.weaponOffset.x - s->leftShot.getFrame(0).width, position.y + weapon.weaponOffset.y + s->leftShot.getFrame(0).height);
				s->go(Shot::dLeft);
			}

			g.shots.push_back(s);
			shotSound.play();

			g.dollars -= weapon.shotPrice;
		}
	}

	if (jStatus != jNone)
	{
		jumpCurrentTime += deltaTime;
		if (jumpCurrentTime >= jumpFrameTime)
		{
			jumpCurrentTime = sf::microseconds(jumpCurrentTime.asMicroseconds() % jumpFrameTime.asMicroseconds());

			float newY = position.y;

			if (jStatus == jJump)
			{
				jumpProgress += 5;

				newY -= 30*sin(jumpProgress * M_PI / 180);

				if (jumpProgress >= 90)	// end of jump on plane
				{
					jumpProgress = 0;
					jStatus = jFall;
				}
			} else if (jStatus == jFall)
			{
				newY += 20;

				if (newY > groundY - sprite.getAnimation()->getFrame(0).height)
				{
					newY = groundY - sprite.getAnimation()->getFrame(0).height;
					jumpProgress = 0;
					//jStatus = jNone;
				}
			}

			setPosition(position.x, newY);
		}
	}

	sprite.update(deltaTime);
	weapon.sprite.update(deltaTime);
}