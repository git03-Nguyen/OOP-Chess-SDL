#pragma once
#include "SDL_mixer.h"
#include <string>
#include <iostream>
using namespace std;

class SoundManager {
private:
	bool isMuted = false;

	Mix_Music* bgMenu;
	Mix_Music* bgPvP;
	Mix_Music* bgPvE;
	Mix_Music* bgReplay;
	Mix_Music* bgEnd;

	Mix_Music* playing;
	
	Mix_Chunk* efMove;
	Mix_Chunk* efCapture;
	Mix_Chunk* efClickBtn;


	int themeMusicVolume;
	int eventMusicVolume;
public:
	SoundManager();
	~SoundManager();

	void setThemeMusicVolume(int themeMusicVolume);
	void setEventMusicVolume(int eventMusicVolume);

	int getThemeMusicVolume() const;
	int getEventMusicVolume() const;

	void playEndMusic();
	void playMainMenuMusic();
	void playPvPMusic();
	void playPvEMusic();
	void playReplayMusic();

	void playPieceMoveSound();
	void playCapturingSound();
	void playClickBtnSound();

	void muteXunmute();

};
