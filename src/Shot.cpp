#include "Shot.h"
#include "game.h"
#include <iostream>
#include <cmath>

Shot::Shot(int dmg, bool hitEnemy) {
	refreshRate = sf::milliseconds(75);

	damage = dmg;
	this->hitEnemy = hitEnemy;
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
	Animation *anim = nullptr;

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
	game &g = game::getInstance();

	m_currentTime += deltaTime;
	if (m_currentTime >= m_frameTime) {
		m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_frameTime.asMicroseconds());

		if (direction == dRight)
			setPosition(position.x + shotSpeed, position.y);
		else
			setPosition(position.x - shotSpeed, position.y);

		totalDistance += shotSpeed;
	}

	sprite.update(deltaTime);

	vector<Enemy>::iterator i;

	if (hitEnemy)
	{
		for (i = g.enemies.begin(); i != g.enemies.end(); ++i)
		{
			if (hitTest(*i))
			{
				char scoreStr[10];

				i->lives -= damage;
				g.dollars += damage;

				// new dollars text
				/*sf::Text dollars;
				dollars.setFont(g.fonts["Jose"]);
				sprintf_s(scoreStr, 10, "%d", damage);
				dollars.setString(scoreStr);
				dollars.setColor(sf::Color::Black);
				sf::Vector2f offset;
				offset.x = i->sprite.getAnimation()->getFrame(0).width / 2;
				offset.y = -60;
				dollars.setPosition(i->getPosition() + offset);
				dollars.setCharacterSize(72);

				g.dollarsTexts.push_back(dollars);*/

				if (i->lives <= 0)
				{
					i->holdFire();
					i->stop();

					// show explosion
					AnimatedSprite as;
					as.setPosition(
						i->getPosition().x + ((i->sprite.getAnimation()->getFrame(0).width - g.explosionAnimation.getFrame(0).width) / 2),
						i->getPosition().y + ((i->sprite.getAnimation()->getFrame(0).height - g.explosionAnimation.getFrame(0).height) / 2)
					);
					as.setAnimation(g.explosionAnimation);
					as.setLooped(false);
					as.setFrameTime(sf::milliseconds(75));
					as.play();
					g.explosions.push_back(as);

					g.explosionSound.play();
					g.enemies.erase(i);
				}

				destroyShot();
				return;
			}
		}
	}
	else
	{
		if (hitTest(g.player))
		{
			g.player.lives -= damage;

			if (g.player.lives <= 0)
			{
				g.gameOver = true;
				g.player.stop();
				g.player.holdFire();

				g.deathSound.play();

				for (i = g.enemies.begin(); i != g.enemies.end(); ++i)
				{
					i->holdFire();
					i->stop();
				}
			}

			destroyShot();
			return;
		}
	}

	if (totalDistance > maxDistance)
	{
		destroyShot();
	}
}

bool Shot::hitTest(Character c)
{
	return (position.x >= c.getPosition().x && position.x <= c.getPosition().x + c.sprite.getAnimation()->getFrame(0).width &&
		position.y >= c.getPosition().y && position.y <= c.getPosition().y + c.sprite.getAnimation()->getFrame(0).height);
}

void Shot::destroyShot()
{
	game &g = game::getInstance();
	vector<Shot*>::iterator  s = find(g.shots.begin(), g.shots.end(), this);
	g.shots.erase(s);
	delete this;
}