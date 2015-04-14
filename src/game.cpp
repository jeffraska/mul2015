#include <dirent.h>
#include <sys/types.h>
#include <iostream>
#include <cerrno>
#include "game.h"

// sf::Music and sf::SoundBuffer must be created separately in different threads
sf::Mutex OpenALMutex;

game::game() {
	sf::Thread texT(&game::loadTextures, this);
	texT.launch();
	sf::Thread sndT(&game::loadSounds, this);
	sndT.launch();
	sf::Thread musT(&game::loadMusic, this);
	musT.launch();

	texT.wait();
	sndT.wait();
	musT.wait();
}

game::~game() {
	for (int i = 0; i < music.size(); i++)
	{
		music.begin()->second->stop();
		delete music.begin()->second;
		music.erase(music.begin());
	}
}

void game::init() {
	// prepare player animation
	player.setTexture(
		Character::tCharacterWalk,
		Character::dRight,
		textures.at("player"),
		sf::IntRect(0, 0, 200, 250),
		10
		);

	player.setTexture(
		Character::tCharacterWalk,
		Character::dLeft,
		textures.at("player"),
		sf::IntRect(0, 250, 200, 250),
		10
		);

	Weapon machinegun(
		"machinegun",
		sf::Vector2f(40, 100),
		sf::IntRect(0, 0, 200, 100),
		10,
		sf::IntRect(0, 100, 200, 100),
		10,
		sf::IntRect(0, 200, 200, 100),
		10,
		sf::IntRect(0, 300, 200, 100),
		10,
		15,
		sf::milliseconds(100)
		);

	Weapon laser(
		"laser",
		sf::Vector2f(40, 100),
		sf::IntRect(0, 0, 200, 100),
		10,
		sf::IntRect(0, 100, 200, 100),
		10,
		sf::IntRect(0, 200, 200, 100),
		10,
		sf::IntRect(0, 300, 200, 100),
		10,
		20,
		sf::milliseconds(200)
		);

	weapons.insert(std::pair<string, Weapon>("machinegun", machinegun));
	weapons.insert(std::pair<string, Weapon>("laser", laser));

	player.weapon = weapons["machinegun"];
}

map<string, string> game::getFiles(string dir, string filetype) {
	map<string, string> files;

	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		cout << "Error (" << errno << ") opening " << dir << endl;
		return files;
	}

	while ((dirp = readdir(dp)) != NULL) {
		string f(dirp->d_name);

		if (f.rfind(filetype) == (f.size() - filetype.length())) {
			files.insert(std::pair<string, string>(f.substr(0, f.length() - filetype.length()), dir + f));
		}
	}
	closedir(dp);

	return files;
}

void game::loadTextures() {
	map<string, string> files = getFiles("textures/", ".png");

	for (map<string, string>::iterator i = files.begin(); i != files.end(); i++) {
		cout << "Opening " << i->second << endl;

		sf::Texture texture;
		texture.loadFromFile(i->second);
		texture.setSmooth(true);
		textures.insert(std::pair<string, sf::Texture>(i->first, texture));
	}
}

void game::loadSounds() {
	map<string, string> files = getFiles("sounds/", ".ogg");

	for (map<string, string>::iterator i = files.begin(); i != files.end(); i++) {
		cout << "Opening " << i->second << endl;

		OpenALMutex.lock();
		sf::SoundBuffer sound;
		OpenALMutex.unlock();
		sound.loadFromFile(i->second);
		sounds.insert(std::pair<string, sf::SoundBuffer>(i->first, sound));
	}
}

void game::loadMusic() {
	map<string, string> files = getFiles("music/", ".ogg");

	for (map<string, string>::iterator i = files.begin(); i != files.end(); i++) {
		cout << "Opening " << i->second << endl;

		sf::Music *m;
		OpenALMutex.lock();
		m = new sf::Music();
		OpenALMutex.unlock();
		m->openFromFile(i->second);
		m->setLoop(true);
		music.insert(std::pair<string, sf::Music*>(i->first, m));
	}
}