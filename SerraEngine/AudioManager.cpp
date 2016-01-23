#include "AudioManager.h"
#include "ErrorManager.h"

namespace SerraEngine {

int AudioManager::curChannel = 0;

void SoundEffect::play(int loops) const
{
	if (Mix_PlayChannel(-1, m_chunk, loops) == -1) {
		if (AudioManager::curChannel == AudioManager::maxChannels) AudioManager::curChannel = 0;
		if (Mix_PlayChannel(AudioManager::curChannel, m_chunk, loops) == -1) {
			fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
		}
		AudioManager::curChannel++;
	}
}

void Music::play(int loops) const
{
	if (Mix_PlayMusic(m_music, loops) == -1) fatalError("Mix_PlayMusic error: " + std::string(Mix_GetError()));
}

AudioManager::~AudioManager()
{
	destroy();
}

void AudioManager::init()
{
	if (m_isInitialized) fatalError("Audio Manager error: Trying to initialize Audio Manager twice!");
	//Can be bitwise combination of: MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) fatalError("Mix_Init error: " + std::string(Mix_GetError()));
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
	m_isInitialized = true;
	Mix_AllocateChannels(maxChannels);
}

void AudioManager::destroy()
{
	if (m_isInitialized) {
		m_isInitialized = false;
		Mix_AllocateChannels(0);
		for (auto &e : m_effectMap) Mix_FreeChunk(e.second);
		for (auto &m : m_musicMap) Mix_FreeMusic(m.second);
		m_effectMap.clear();
		m_musicMap.clear();
		Mix_CloseAudio();
		Mix_Quit();
	}
}

void AudioManager::setEffectsVolume(int v)
{
	for (auto e = m_effectMap.begin(), end = m_effectMap.end(); e != end; ++e) Mix_VolumeChunk(e->second, v);
}

void AudioManager::setMusicVolume(int v)
{
	Mix_VolumeMusic(v);
}

SoundEffect AudioManager::loadSoundEffect(const std::string& filePath)
{
	auto it = m_effectMap.find(filePath); //try find audio in cache
	SoundEffect effect;
	if (it == m_effectMap.end()) {
		auto chunk = Mix_LoadWAV(filePath.c_str());
		if (chunk == nullptr) fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
		m_effectMap[filePath] = chunk;
		effect.m_chunk = chunk;
		Mix_VolumeChunk(effect.m_chunk, m_effectsVolume);
	} else effect.m_chunk = it->second;
	return effect;
}

Music AudioManager::loadMusic(const std::string & filePath)
{
	auto it = m_musicMap.find(filePath); //try find audio in cache
	Music mus;
	if (it == m_musicMap.end()) {
		auto m = Mix_LoadMUS(filePath.c_str());
		if (m == nullptr) fatalError("Mix_LoadMusic error: " + std::string(Mix_GetError()));
		m_musicMap[filePath] = m;
		mus.m_music = m;
		Mix_VolumeMusic(m_musicVolume);
	}
	else mus.m_music = it->second;
	return mus;
}

}
