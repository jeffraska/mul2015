#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Shot.h"
#include "GroundPanel.h"

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
	vector<GroundPanel> ground;
	sf::Vector2i position;
	bool barrier;

	float dollars;
	void newEnemy(float, float);
	void genGround(sf::Vector2u, int);
	int groundCollision(sf::Vector2f, int);
	int randomGenerator();

	Player player;
	Enemy tankTemplate;
	Animation explosionAnimation;
	sf::Sound explosionSound;
	sf::Sound deathSound;
	bool gameOver;
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
