#include "PAudio.h"


PAudio::PAudio(std::string file)
{
	mFile		= file;
	mSampleSize = 64; 
	mSpecLeft	= new float[mSampleSize];
	mSpecRight	= new float[mSampleSize];
	mAvgSpec	= new float[mSampleSize];
	preCheck();
	init(); 

}

PAudio::~PAudio(void)
{
	mResult = mSystem->release();
	FMODErrorCheck(mResult);
}

void PAudio::FMODErrorCheck(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		std::cout<<"FMOD error! ("<<result<<") "<<FMOD_ErrorString(result)<<std::endl;
		std::cin.get();
		exit(-1);
	}
}

void PAudio::preCheck()
{
	//Create FMOD interface object
	mResult = FMOD::System_Create(&mSystem);
	FMODErrorCheck(mResult);

	//Check version 
	mResult = mSystem->getVersion(&mVersion);
	FMODErrorCheck(mResult);
	if(mVersion < FMOD_VERSION)
	{
		std::cout<<"Error! You are using an old version of FMOD "<<mVersion<<". This program requires "<<FMOD_VERSION<<std::endl;
		std::cin.get();
		exit(-1);
	}

	//get number of sound cards
	mResult = mSystem->getNumDrivers(&mNumDrivers);
	FMODErrorCheck(mResult);

	//disable sound if no drivers detected
	if(mNumDrivers == 0){
		mResult = mSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		FMODErrorCheck(mResult);
	}else{
		std::cout<<"Number of sound drivers: "<<mNumDrivers<<std::endl;

		//get the capabilities of the default sound card
		mResult = mSystem->getDriverCaps(0, &mCaps, 0, &mSpeakerMode);
		FMODErrorCheck(mResult);

		//set the speaker mode to match that in the control panel
		mResult = mSystem->setSpeakerMode(mSpeakerMode);
		FMODErrorCheck(mResult);


		//Increase buffer size if hardware acceleration is off
		if(mCaps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			mResult = mSystem->setDSPBufferSize(1024, 10);
			FMODErrorCheck(mResult);
		}

		//Get name of driver
		mResult = mSystem->getDriverInfo(0, mName, 256, 0);
		FMODErrorCheck(mResult);

		std::cout<<"Name of Driver: "<<mName<<std::endl;
		//SigmaTel sound driver crackles so switching to PCM floating point output to avoid it. 
		if(strstr(mName, "SigmaTel"))
		{
			mResult = mSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			FMODErrorCheck(mResult);
		}
	}


}

void PAudio::init()
{
	mResult = mSystem->init(100, FMOD_INIT_NORMAL, 0);

	//if the speaker mode is not supported we revert to stereo
	if(mResult == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		mResult = mSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		FMODErrorCheck(mResult);
		mResult = mSystem->init(100, FMOD_INIT_NORMAL, 0);
	}
	FMODErrorCheck(mResult);

	mResult = mSystem->createStream(mFile.c_str(), FMOD_DEFAULT, 0, &mAudioStream);
	FMODErrorCheck(mResult);
}

void PAudio::play()
{
	mResult = mSystem->playSound(FMOD_CHANNEL_FREE, mAudioStream, false, &mChannel);
	FMODErrorCheck(mResult);
}

void PAudio::update()
{
	mSystem->update();
	
	mChannel->getSpectrum(mSpecLeft, mSampleSize, 0, FMOD_DSP_FFT_WINDOW_RECT);
	mChannel->getSpectrum(mSpecRight, mSampleSize, 1, FMOD_DSP_FFT_WINDOW_RECT);


	for(int i = 0; i < mSampleSize ; i++)
		mAvgSpec[i] = (mSpecLeft[i] + mSpecRight[i]) / 2 ;

	//Find max volume
		auto maxIterator = std::max_element(&mAvgSpec[0], &mAvgSpec[mSampleSize]);
	float maxVol = *maxIterator;
	// Normalize
	if (maxVol != 0)
		std::transform(&mAvgSpec[0], &mAvgSpec[mSampleSize], &mAvgSpec[0], [maxVol] (float dB) -> float { return dB / maxVol; });
}

float* PAudio::spectrum()
{
	return mAvgSpec;
}