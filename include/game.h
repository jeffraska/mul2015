#include <list>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "AnimatedSprite.hpp"
#include "Character.h"
#include "Shot.h"

#ifndef GAME_H
#define	GAME_H

using namespace std;

class game {
public:

	static game& getInstance() {
		static game instance;
		static bool initialized;
		if (!initialized) {
			initialized = true;
			instance.init();
		}
		return instance;
	}

	map<string, sf::Texture> textures;
	map<string, sf::SoundBuffer> sounds;
	map<string, sf::Music*> music;
	Character player;
	map<string, Weapon> weapons;
	vector<Shot*> shots;
private:
	game();
	~game();
	game(game const&);
	void operator=(game const&);

	map<string, string> getFiles(string dir, string filetype);
	void loadTextures();
	void loadSounds();
	void loadMusic();
	void init();
};

#endif	/* GAME_H */
