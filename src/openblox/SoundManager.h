#ifndef OPENBLOX_SOUNDMANAGER_H_
#define OPENBLOX_SOUNDMANAGER_H_

#include "OpenBlox.h"

struct FMOD_SOUND;
struct FMOD_CHANNEL;

namespace OpenBlox{
	class Sound{
		public:
			Sound(FMOD_SOUND* nsnd);
			~Sound();
			FMOD_SOUND* getSnd();
		private:
			FMOD_SOUND* snd;
	};

	class SoundManager{
		public:
			static Sound* createSound(std::string asset);
			static FMOD_CHANNEL* playSound(Sound* snd, FMOD_CHANNEL* chan, bool isLooping = false);
			static void pauseChannel(FMOD_CHANNEL* chan);
			static void playChannel(FMOD_CHANNEL* chan);
			static void stopChannel(FMOD_CHANNEL* chan);
			static bool isPaused(FMOD_CHANNEL* chan);
			static int getLength(Sound* snd);
			static int getPos(FMOD_CHANNEL* chan);
			static void update();
			static void cleanup();

			DECLARE_STATIC_INIT(SoundManager);
	};
}
#endif
