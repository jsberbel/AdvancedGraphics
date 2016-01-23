#pragma once
#include <SDL/SDL_mixer.h>
#include <string>
#include <map>

//http://jcatki.no-ip.org:8080/SDL_mixer/SDL_mixer_frame.html

namespace SerraEngine {

class SoundEffect
{
	Mix_Chunk* m_chunk = nullptr;
public:
	explicit SoundEffect() = default;
	~SoundEffect() = default;
	void play(int loops = 0) const; //if loops = -1 -> loop forever, else play it loops times
	friend class AudioManager;
};

class Music
{
	Mix_Music* m_music = nullptr;
public:
	explicit Music() = default;
	~Music() = default;
	void play(int loops = -1) const; //if loops = -1 -> loop forever, else play it loops times
	static void pause();
	static void stop();
	static void resume();
	friend class AudioManager;
};

class AudioManager
{
	std::map <std::string, Mix_Chunk*> m_effectMap;
	std::map <std::string, Mix_Music*> m_musicMap;
	bool m_isInitialized = false;
	int effectsVolume = static_cast<int>(MIX_MAX_VOLUME * 0.3f);
	int musicVolume = static_cast<int>(MIX_MAX_VOLUME * 0.5f);
public:
	static const int maxChannels = 16;
	static int curChannel; //0

	explicit AudioManager() = default;
	~AudioManager();

	void init();
	void destroy();

	void setEffectsVolume(int v);
	static void setMusicVolume(int v);

	SoundEffect loadSoundEffect(const std::string &filePath);
	Music loadMusic(const std::string &filePath);
};

}
