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
			textures.insert(std::pair<string, sf::Texture>(f, texture));
		}
	}
	closedir(dp);

	// prepare right player animation
	playerRight.setSpriteSheet(textures.at("player.png"));
	playerRight.addFrame(sf::IntRect(0, 64, 32, 32));
	playerRight.addFrame(sf::IntRect(32, 64, 32, 32));
	playerRight.addFrame(sf::IntRect(64, 64, 32, 32));

	playerLeft.setSpriteSheet(textures.at("player.png"));
	playerLeft.addFrame(sf::IntRect(0, 32, 32, 32));
	playerLeft.addFrame(sf::IntRect(32, 32, 32, 32));
	playerLeft.addFrame(sf::IntRect(64, 32, 32, 32));
}