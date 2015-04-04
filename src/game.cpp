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

	// prepare right player animation
	playerRight.setSpriteSheet(textures.at("player.png"));
	playerRight.addXFrames(sf::IntRect(0, 0, 300, 240), 10);

	playerLeft.setSpriteSheet(textures.at("player.png"));
	playerLeft.addXFrames(sf::IntRect(0, 240, 300, 240), 10);
}