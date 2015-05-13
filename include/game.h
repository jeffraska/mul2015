#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Enemy.h"
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
	map<string, sf::Font> fonts;
	map<string, Weapon> weapons;

	vector<Shot*> shots;
	vector<Enemy> enemies;
	vector<sf::Text> dollarsTexts;
	vector<AnimatedSprite> explosions;
	vector<sf::Vector2i> ground;
	sf::Vector2i position;

	float dollars;
	void newEnemy(float, float);
	sf::Vector2i genGround(sf::Vector2i, sf::Vector2u);
	int groundCollision(sf::Vector2f);

	Player player;
	Enemy tankTemplate;
	Animation explosionAnimation;
	sf::Sound explosionSound;
private:
	game();
	~game();
	game(game const&);
	void operator=(game const&);

	map<string, string> getFiles(string dir, string filetype);
	void loadTextures();
	void loadSounds();
	void loadMusic();
	void loadFonts();
	void init();
};

#endif	/* GAME_H */
