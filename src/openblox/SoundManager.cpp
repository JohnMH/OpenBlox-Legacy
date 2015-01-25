#include "SoundManager.h"

#include <fmod.h>
#include <fmod_errors.h>

namespace OpenBlox{
	Sound::Sound(FMOD_SOUND* nsnd){
		snd = nsnd;
	}

	Sound::~Sound(){
		FMOD_Sound_Release(snd);
	}

	FMOD_SOUND* Sound::getSnd(){
		return snd;
	}

	static FMOD_SYSTEM* system;

	bool FMOD_Err(FMOD_RESULT result, std::string funcName){
		if(result != FMOD_OK){
			LOGI("[FMOD] (%s) Error: %s", funcName.c_str(), FMOD_ErrorString(result));
			return true;
		}
		return false;
	}

	STATIC_INIT(SoundManager){
		FMOD_RESULT result;

		result = FMOD_System_Create(&system);
		if(FMOD_Err(result, "FMOD_System_Create")){return;}

		unsigned int version;
		result = FMOD_System_GetVersion(system, &version);
		if(FMOD_Err(result, "FMOD_System_GetVersion")){return;}

		if(version < FMOD_VERSION){
			LOGI("[FMOD] An old version of FMOD has been detected.");
		}

		int numDrivers = 0;
		result = FMOD_System_GetNumDrivers(system, &numDrivers);
		if(FMOD_Err(result, "FMOD_System_GetNumDrivers")){return;}

		FMOD_CAPS caps;
		FMOD_SPEAKERMODE speakerMode;

		if(numDrivers == 0){
			LOGI("[FMOD] No sound drivers detected. Sound will be disabled.");

			result = FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_NOSOUND);
			if(FMOD_Err(result, "FMOD_System_SetOutput")){return;}
		}else{
			//Get capabilities of default sound driver
			result = FMOD_System_GetDriverCaps(system, 0, &caps, 0, &speakerMode);
			if(FMOD_Err(result, "FMOD_System_GetDriverCaps")){return;}

			result = FMOD_System_SetSpeakerMode(system, speakerMode);
			if(FMOD_Err(result, "FMOD_System_SetSpeakerMode")){return;}

			if(caps & FMOD_CAPS_HARDWARE_EMULATED){
				result = FMOD_System_SetDSPBufferSize(system, 1024, 10);
				if(FMOD_Err(result, "FMOD_System_SetDSPBufferSize")){return;}
			}

			char name[256];
			result = FMOD_System_GetDriverInfo(system, 0, name, 256, NULL);
			if(FMOD_Err(result, "FMOD_System_GetDriverInfo")){return;}

			//Fix for SigmaTel sound drivers (crackle if format is PCM 16-bit)
			if(strstr(name, "SigmaTel")){
				result = FMOD_System_SetSoftwareFormat(system, 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
				if(FMOD_Err(result, "FMOD_System_setSoftwareFormat")){return;}
			}
		}

		result = FMOD_System_Init(system, 100, FMOD_INIT_NORMAL, NULL);
		if(FMOD_Err(result, "FMOD_System_Init")){return;}
	}

	Sound* SoundManager::createSound(std::string asset){
		asset_response_body* resp = AssetLocator::getAsset(asset);
		if(resp){
			FMOD_SOUND* snd;

			FMOD_CREATESOUNDEXINFO* exinfo = new FMOD_CREATESOUNDEXINFO();
			exinfo->cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
			exinfo->length = resp->size;

			FMOD_RESULT result = FMOD_System_CreateSound(system, resp->data, FMOD_SOFTWARE | FMOD_OPENMEMORY, exinfo, &snd);
			if(FMOD_Err(result, "FMOD_System_CreateSound")){return NULL;}
			return new Sound(snd);
		}
		return NULL;
	}

	FMOD_CHANNEL* SoundManager::playSound(Sound* snd, FMOD_CHANNEL* chan, bool isLooping){
		if(snd){
			FMOD_SOUND* nsnd = snd->getSnd();

			FMOD_RESULT result;

			if(isLooping){
				result = FMOD_Sound_SetMode(nsnd, FMOD_LOOP_NORMAL);
				if(FMOD_Err(result, "FMOD_Sound_SetMode")){return NULL;}
				result = FMOD_Sound_SetLoopCount(nsnd, -1);
				if(FMOD_Err(result, "FMOD_Sound_SetLoopCount")){return NULL;}
			}else{
				result = FMOD_Sound_SetMode(nsnd, FMOD_LOOP_OFF);
				if(FMOD_Err(result, "FMOD_Sound_SetMode")){return NULL;}
			}

			FMOD_CHANNEL* chan;
			result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, nsnd, false, &chan);
			if(FMOD_Err(result, "FMOD_System_PlaySound")){return NULL;}

			return chan;
		}
		return NULL;
	}

	void SoundManager::pauseChannel(FMOD_CHANNEL* chan){
		if(chan){
			FMOD_RESULT result = FMOD_Channel_SetPaused(chan, true);
			if(FMOD_Err(result, "FMOD_Channel_SetPaused")){return;}
		}
	}

	void SoundManager::playChannel(FMOD_CHANNEL* chan){
		if(chan){
			FMOD_RESULT result = FMOD_Channel_SetPaused(chan, false);
			if(FMOD_Err(result, "FMOD_Channel_SetPaused")){return;}
		}
	}

	void SoundManager::stopChannel(FMOD_CHANNEL* chan){
		if(chan){
			FMOD_RESULT result = FMOD_Channel_Stop(chan);
			if(FMOD_Err(result, "FMOD_Channel_Stop")){return;}
		}
	}

	bool SoundManager::isPaused(FMOD_CHANNEL* chan){
		if(chan){
			FMOD_BOOL isIt = false;
			FMOD_RESULT result = FMOD_Channel_GetPaused(chan, &isIt);
			if(FMOD_Err(result, "FMOD_Channel_GetPaused")){return false;}
			return isIt;
		}
		return false;
	}

	int SoundManager::getLength(Sound* snd){
		if(snd){
			FMOD_SOUND* nsnd = snd->getSnd();

			unsigned int len;

			FMOD_RESULT result = FMOD_Sound_GetLength(nsnd, &len, FMOD_TIMEUNIT_MS);
			if(FMOD_Err(result, "FMOD_Sound_GetLength")){return -1;}

			return len;
		}
		return -1;
	}

	int SoundManager::getPos(FMOD_CHANNEL* chan){
		if(chan){
			unsigned int pos;

			FMOD_RESULT result = FMOD_Channel_GetPosition(chan, &pos, FMOD_TIMEUNIT_MS);
			if(FMOD_Err(result, "FMOD_Channel_GetPosition")){return -1;}

			return pos;
		}
		return -1;
	}

	void SoundManager::update(){
		FMOD_RESULT result = FMOD_System_Update(system);
		if(FMOD_Err(result, "FMOD_System_Update")){return;}
	}

	void SoundManager::cleanup(){
		FMOD_RESULT result = FMOD_System_Release(system);
		if(FMOD_Err(result, "FMOD_System_Release")){return;}
	}
}
