#ifndef OB_INSTANCE_SOUND_H_
#define OB_INSTANCE_SOUND_H_

#include "Instance.h"

namespace ob_instance{
	class Sound: public Instance{
		public:
			Sound();
			virtual ~Sound();

			void Pause();
			void Play();
			void Stop();

			void setSoundId(std::string newSoundId);

			DECLARE_CLASS(Sound);

			FMOD_CHANNEL* chan;

			OpenBlox::Sound* snd;
			std::string SoundId;
			bool Looped;

			static void register_lua_property_setters(lua_State* L);
			static void register_lua_property_getters(lua_State* L);
			static void register_lua_methods(lua_State* L);
	};
}
#endif
