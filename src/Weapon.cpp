#include "Weapon.h"
#include "game.h"

Weapon::Weapon() {
}

Weapon::Weapon(
	string n,
	sf::Vector2f offset,
	sf::IntRect walkR,
	int walkRCount,
	sf::IntRect walkL,
	int walkLCount,
	sf::IntRect fireR,
	int fireRCount,
	sf::IntRect fireL,
	int fireLCount,
	float shotS,
	sf::Time rate) {
	name = n;
	setTexture(tGunWalk, dRight, game::getInstance().textures[name], walkR, walkRCount);
	setTexture(tGunWalk, dLeft, game::getInstance().textures[name], walkL, walkLCount);
	setTexture(tGunFire, dRight, game::getInstance().textures[name], fireR, fireRCount);
	setTexture(tGunFire, dLeft, game::getInstance().textures[name], fireL, fireLCount);
	weaponOffset = offset;
	shotSpeed = shotS;
	m_frameTime = rate;
}

void Weapon::setTexture(
	TextureType type,
	Direction dir,
	const sf::Texture& texture,
	sf::IntRect rect, int count) {
	Animation *anim = NULL;

	if (type == tGunWalk) {
		if (dir == dRight)
			anim = &gunRightWalk;
		else
			anim = &gunLeftWalk;
	}
	else if (type == tGunFire) {
		if (dir == dRight)
			anim = &gunRightFire;
		else
			anim = &gunLeftFire;
	}

	anim->setSpriteSheet(texture);
	anim->addXFrames(rect, count);
}