#include <list>
#include <map>
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.hpp"
#include "character.h"

#ifndef GAME_H
#define	GAME_H

using namespace std;

class game {
public:

	static game& getInstance() {
		static game instance;
		return instance;
	}

	map<string, sf::Texture> textures;
	character player;
private:
	game();
	game(game const&);
	void operator=(game const&);
	static const char *textureDir;
};

#endif	/* GAME_H */

