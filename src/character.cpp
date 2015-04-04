#include "character.h"
#include <iostream>

sf::Vector2f gunOffset(40, 100);

character::character() {

}

void character::init(sf::Time frameTime, float speed) {
	direction = dRight;
	wStatus = wStop;
	fStatus = fHold;

	sprite.setFrameTime(sf::milliseconds(75));
	sprite.setAnimation(rightWalk);
	sprite.stop();
	sprite.setLooped(true);

	gun.setFrameTime(sf::milliseconds(75));
	gun.setAnimation(gunRightWalk);
	gun.stop();
	gun.setLooped(true);

	gun.setPosition(gunOffset);

	m_frameTime = frameTime;
	characterSpeed = speed;
}

void character::setTexture(
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
	} else if (type == tGunWalk) {
		if (dir == dRight)
			anim = &gunRightWalk;
		else
			anim = &gunLeftWalk;
	} else if (type == tGunFire) {
		if (dir == dRight)
			anim = &gunRightFire;
		else
			anim = &gunLeftFire;
	}

	anim->setSpriteSheet(texture);
	anim->addXFrames(rect, count);
}

void character::go(Direction d) {
	if (d != direction || wStatus == wStop) {
		direction = d;
		wStatus = wWalk;

		if (direction == dRight) {
			gun.setPosition(position.x + gunOffset.x, position.y + gunOffset.y);
		} else {
			gun.setPosition(position.x - gunOffset.x, position.y + gunOffset.y);
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

void character::stop() {
	sprite.pause();
	wStatus = wStop;

	if (fStatus == fHold)
		holdFire();
}

void character::fire() {
	fStatus = fFire;

	if (direction == dRight)
		gun.play(gunRightFire);
	else
		gun.play(gunLeftFire);
}

void character::holdFire() {
	fStatus = fHold;

	if (direction == dRight)
		gun.play(gunRightWalk);
	else
		gun.play(gunLeftWalk);

	if (wStatus == wStop) {
		gun.pause();
	}
}

void character::setPosition(float x, float y) {
	position = sf::Vector2f(x, y);
	sprite.setPosition(position);

	sprite.setPosition(x, y);
	if (direction == dRight) {
		gun.setPosition(x + gunOffset.x, y + gunOffset.y);
	} else {
		gun.setPosition(x - gunOffset.x, y + gunOffset.y);
	}
}

void character::setPosition(const sf::Vector2f& pos) {
	position = pos;
	sprite.setPosition(position);

	sprite.setPosition(pos.x, pos.y);
	if (direction == dRight) {
		gun.setPosition(pos.x + gunOffset.x, pos.y + gunOffset.y);
	} else {
		gun.setPosition(pos.x - gunOffset.x, pos.y + gunOffset.y);
	}
}

sf::Vector2f character::getPosition() {
	return position;
}

void character::animate(sf::Time deltaTime) {
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
	gun.update(deltaTime);
}