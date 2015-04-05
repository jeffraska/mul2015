#include "Character.h"
#include <iostream>
#include <cmath>

Character::Character() {
	refreshRate = sf::milliseconds(75);
}

void Character::init(sf::Time frameTime, float speed) {
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

	m_frameTime = frameTime;
	characterSpeed = speed;
}

void Character::setTexture(
		TextureType type,
		Direction dir,
		const sf::Texture& texture,
		sf::IntRect rect, int count) {
	Animation *anim;

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

	if (wStatus == wWalk) {
		wStatus = wStop;
		go(direction);
	} else {
		stop();
	}

	if (fStatus == fFire) {
		fire();
	} else {
		holdFire();
	}
}

void Character::go(Direction d) {
	if (d != direction || wStatus == wStop) {
		direction = d;
		wStatus = wWalk;

		if (direction == dRight) {
			weapon.sprite.setPosition(position.x + weapon.weaponOffset.x, position.y + weapon.weaponOffset.y);
		} else {
			weapon.sprite.setPosition(position.x - weapon.weaponOffset.x, position.y + weapon.weaponOffset.y);
		}

		sprite.stop();

		if (d == dRight) {
			sprite.play(rightWalk);
		} else {
			sprite.play(leftWalk);
		}

		if (fStatus == fFire) {
			fire();
		} else {
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

void Character::holdFire() {
	fStatus = fHold;

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
	} else {
		weapon.sprite.setPosition(x - weapon.weaponOffset.x, y + weapon.weaponOffset.y);
	}
}

void Character::setPosition(const sf::Vector2f& pos) {
	position = pos;
	sprite.setPosition(position);

	sprite.setPosition(pos.x, pos.y);
	if (direction == dRight) {
		weapon.sprite.setPosition(pos.x + weapon.weaponOffset.x, pos.y + weapon.weaponOffset.y);
	} else {
		weapon.sprite.setPosition(pos.x - weapon.weaponOffset.x, pos.y + weapon.weaponOffset.y);
	}
}

sf::Vector2f Character::getPosition() {
	return position;
}

void Character::animate(sf::Time deltaTime) {
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

	sprite.update(deltaTime);
	weapon.sprite.update(deltaTime);
}