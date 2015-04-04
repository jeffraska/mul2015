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
	playerRight.addFrame(sf::IntRect(0, 0, 300, 240));
	playerRight.addFrame(sf::IntRect(300, 0, 300, 240));
	playerRight.addFrame(sf::IntRect(600, 0, 300, 240));
	playerRight.addFrame(sf::IntRect(900, 0, 300, 240));
	playerRight.addFrame(sf::IntRect(1200, 0, 300, 240));
	playerRight.addFrame(sf::IntRect(1500, 0, 300, 240));
	playerRight.addFrame(sf::IntRect(1800, 0, 300, 240));
	playerRight.addFrame(sf::IntRect(2100, 0, 300, 240));
	playerRight.addFrame(sf::IntRect(2400, 0, 300, 240));
	playerRight.addFrame(sf::IntRect(2700, 0, 300, 240));

	playerLeft.setSpriteSheet(textures.at("player.png"));
	playerLeft.addFrame(sf::IntRect(0, 240, 300, 240));
	playerLeft.addFrame(sf::IntRect(300, 240, 300, 240));
	playerLeft.addFrame(sf::IntRect(600, 240, 300, 240));
	playerLeft.addFrame(sf::IntRect(900, 240, 300, 240));
	playerLeft.addFrame(sf::IntRect(1200, 240, 300, 240));
	playerLeft.addFrame(sf::IntRect(1500, 240, 300, 240));
	playerLeft.addFrame(sf::IntRect(1800, 240, 300, 240));
	playerLeft.addFrame(sf::IntRect(2100, 240, 300, 240));
	playerLeft.addFrame(sf::IntRect(2400, 240, 300, 240));
	playerLeft.addFrame(sf::IntRect(2700, 240, 300, 240));
}