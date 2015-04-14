#include "Shot.h"
#include <iostream>
#include <cmath>

Shot::Shot() {
	refreshRate = sf::milliseconds(75);
}

void Shot::init(sf::Time frameTime, float speed) {
	direction = dRight;

	sprite.setFrameTime(refreshRate);
	sprite.setAnimation(rightShot);
	sprite.stop();
	sprite.setLooped(true);

	m_frameTime = frameTime;
	shotSpeed = speed;
}

void Shot::setTexture(
	Direction dir,
	const sf::Texture& texture,
	sf::IntRect rect, int count) {
	Animation *anim = NULL;

	if (dir == dRight)
		anim = &rightShot;
	else
		anim = &leftShot;

	anim->setSpriteSheet(texture);
	anim->addXFrames(rect, count);
}

void Shot::go(Direction d) {
	direction = d;

	if (d == dRight) {
		sprite.play(rightShot);
	}
	else {
		sprite.play(leftShot);
	}
}

void Shot::setPosition(float x, float y) {
	position = sf::Vector2f(x, y);
	sprite.setPosition(position);
}

void Shot::setPosition(const sf::Vector2f& pos) {
	position = pos;
	sprite.setPosition(position);
}

sf::Vector2f Shot::getPosition() {
	return position;
}

void Shot::animate(sf::Time deltaTime) {
	m_currentTime += deltaTime;
	if (m_currentTime >= m_frameTime) {
		m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_frameTime.asMicroseconds());

		if (direction == dRight)
			setPosition(position.x + shotSpeed, position.y);
		else
			setPosition(position.x - shotSpeed, position.y);
	}

	sprite.update(deltaTime);
}