#pragma once
#include <iostream>
#include <algorithm>
#include "fmod_errors.h"
#include "fmod.hpp"
class PAudio
{
public:
	PAudio(std::string file);
	void preCheck();
	void init();
	void play();
	void update();
	void FMODErrorCheck(FMOD_RESULT result);
	float* spectrum();
	~PAudio(void);
private:
	FMOD::System*		mSystem;
	FMOD::Sound*		mAudioStream;
	FMOD::Channel*		mChannel;
	int					mSampleSize; 
	float				*mSpecLeft, *mSpecRight, *mAvgSpec;
	FMOD_RESULT			mResult;		
	FMOD_SPEAKERMODE	mSpeakerMode; 
	FMOD_CAPS			mCaps; 
	unsigned int		mVersion; 
	int					mNumDrivers; 
	char				mName[256];
	std::string			mFile;
};

