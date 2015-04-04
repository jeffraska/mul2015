#include <dirent.h>
#include <sys/types.h>
#include <iostream>
#include <cerrno>
#include "game.h"

const char *game::textureDir = "textures/";

game::game() {
	// load all textures
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(textureDir)) == NULL) {
		cout << "Error(" << errno << ") opening " << textureDir << endl;
		return;
	}

	while ((dirp = readdir(dp)) != NULL) {
		string f(dirp->d_name);

		if (f.rfind(".png") == (f.size() - 4)) {
			cout << "Opening " << textureDir + f << endl;
			sf::Texture texture;
			texture.loadFromFile(textureDir + f);
			texture.setSmooth(true);
			textures.insert(std::pair<string, sf::Texture>(f, texture));
		}
	}
	closedir(dp);

	// prepare player animation
	player.setTexture(
			character::tCharacterWalk,
			character::dRight,
			textures.at("player.png"),
			sf::IntRect(0, 0, 200, 250),
			10
			);

	player.setTexture(
			character::tCharacterWalk,
			character::dLeft,
			textures.at("player.png"),
			sf::IntRect(0, 250, 200, 250),
			10
			);

	player.setTexture(
			character::tGunWalk,
			character::dRight,
			textures.at("machinegun.png"),
			sf::IntRect(0, 0, 200, 100),
			10
			);

	player.setTexture(
			character::tGunWalk,
			character::dLeft,
			textures.at("machinegun.png"),
			sf::IntRect(0, 100, 200, 100),
			10
			);

	player.setTexture(
			character::tGunFire,
			character::dRight,
			textures.at("machinegun.png"),
			sf::IntRect(0, 200, 200, 100),
			10
			);

	player.setTexture(
			character::tGunFire,
			character::dLeft,
			textures.at("machinegun.png"),
			sf::IntRect(0, 300, 200, 100),
			10
			);
}