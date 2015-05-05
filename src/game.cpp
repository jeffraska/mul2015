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
	sf::Thread fontT(&game::loadFonts, this);
	fontT.launch();

	texT.wait();
	sndT.wait();
	musT.wait();
	fontT.wait();
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
		"machinegun",							// name (require name.png, nameshot.png, name.ogg)
		sf::Vector2f(40, 100),				// offset
		sf::IntRect(0, 0, 200, 100),		// walk right sprite coordinates
		10,									// number of walk right sprites
		sf::IntRect(0, 100, 200, 100),		// walk left sprite coordinates
		10,									// number of walk left sprites
		sf::IntRect(0, 200, 200, 100),		// fire right sprite coordinates
		10,									// number of fire right sprites
		sf::IntRect(0, 300, 200, 100),		// fire left sprite coordinates
		10,									// number of fire right sprites
		sf::IntRect(0, 0, 27, 34),			// shot right sprite coordinates
		1,									// number of shot right sprites
		sf::IntRect(0, 34, 27, 34),			// shot left sprite coordinates
		1,									// number of shot left sprites
		15,									// speed of shot
		sf::milliseconds(100),				// shot rate
		500									// max shot distance
		);

	Weapon laser(
		"laser",							// name (require name.png, nameshot.png, name.ogg)
		sf::Vector2f(40, 100),				// offset
		sf::IntRect(0, 0, 200, 100),		// walk right sprite coordinates
		10,									// number of walk right sprites
		sf::IntRect(0, 100, 200, 100),		// walk left sprite coordinates
		10,									// number of walk left sprites
		sf::IntRect(0, 200, 200, 100),		// fire right sprite coordinates
		10,									// number of fire right sprites
		sf::IntRect(0, 300, 200, 100),		// fire left sprite coordinates
		10,									// number of fire left sprites
		sf::IntRect(0, 0, 80, 34),			// shot right sprite coordinates
		10,									// number of shot right sprites
		sf::IntRect(0, 34, 80, 34),			// shot left sprite coordinates
		10,									// number of shot left sprites
		20,									// speed of shot
		sf::milliseconds(200),				// shot rate
		1000								// max shot distance
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
		if (!sound.loadFromFile(i->second))
		{
			cerr << "Cannot open " << i->second << endl;
			continue;
		}

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
		if (!m->openFromFile(i->second))
		{
			delete m;
			cerr << "Cannot open " << i->second << endl;
			continue;
		}
		m->setLoop(true);
		music.insert(std::pair<string, sf::Music*>(i->first, m));
	}
}

void game::loadFonts() {
	map<string, string> files = getFiles("textures/", ".ttf");

	for (map<string, string>::iterator i = files.begin(); i != files.end(); i++) {
		cout << "Opening " << i->second << endl;

		sf::Font f;

		if (!f.loadFromFile(i->second))
		{
			cerr << "Cannot open " << i->second << endl;
			continue;
		}

		fonts.insert(std::pair<string, sf::Font>(i->first, f));
	}
}