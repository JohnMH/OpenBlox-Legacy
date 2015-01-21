#include "Sound.h"

namespace ob_instance{
	DEFINE_CLASS(Sound, true, false);

	Sound::Sound() : Instance(){
		Name = ClassName;

		SoundId = "";
		snd = NULL;

		Looped = false;

		chan = NULL;
	}

	Sound::~Sound(){
		if(snd){
			delete snd;
		}
	}

	void Sound::Pause(){
		if(chan){
			OpenBlox::SoundManager::pauseChannel(chan);
		}
	}

	void Sound::Play(){
		if(snd){
			int len = OpenBlox::SoundManager::getLength(snd);
			bool newPlay = false;
			if(chan){
				int pos = OpenBlox::SoundManager::getPos(chan);
				if(len > pos){
					OpenBlox::SoundManager::playChannel(chan);
				}else{
					newPlay = true;
				}
			}else{
				newPlay = true;
			}
			if(newPlay){
				chan = OpenBlox::SoundManager::playSound(snd, chan, Looped);
			}
		}
	}

	void Sound::Stop(){
		if(chan){
			OpenBlox::SoundManager::stopChannel(chan);
		}
	}

	void Sound::setSoundId(std::string newSoundId){
		if(newSoundId != SoundId){
			SoundId = newSoundId;

			snd = OpenBlox::SoundManager::createSound(SoundId);

			std::vector<ob_type::VarWrapper> args = std::vector<ob_type::VarWrapper>();
			args.push_back(ob_type::VarWrapper("SoundId"));

			Changed->Fire(args);
		}
	}

	Instance* Sound::cloneImpl(){
		Sound* newGuy = new Sound;
		newGuy->Name = Name;

		newGuy->SoundId = SoundId;
		newGuy->snd = snd;

		return newGuy;
	}

	void Sound::register_lua_property_getters(lua_State* L){
		luaL_Reg props[]{
			{"SoundId", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(Sound* s = dynamic_cast<Sound*>(inst)){
					lua_pushstring(L, s->SoundId.c_str());
					return 1;
				}
				return 0;
			}},
			{NULL, NULL}
		};
		luaL_register(L, NULL, props);

		Instance::register_lua_property_getters(L);
	}

	void Sound::register_lua_property_setters(lua_State* L){
		luaL_Reg props[]{
				{"SoundId", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(Sound* s = dynamic_cast<Sound*>(inst)){
					std::string newSoundId = std::string(luaL_checkstring(L, 2));
					s->setSoundId(newSoundId);
					return 0;
				}
				return 0;
			}},
			{"Looped", [](lua_State* L)->int{
				Instance* inst = checkInstance(L, 1);
				if(Sound* s = dynamic_cast<Sound*>(inst)){
					bool newVal = false;
					if(lua_isboolean(L, 2)){
						newVal = lua_toboolean(L, 2);
					}else if(!lua_isnoneornil(L, 2)){
						newVal = true;
					}
					if(newVal != s->Looped){
						s->Looped = newVal;

						std::vector<ob_type::VarWrapper> args = std::vector<ob_type::VarWrapper>();
						args.push_back(ob_type::VarWrapper("Looped"));

						s->Changed->Fire(args);
					}
					return 0;
				}
				return 0;
			}},
			{NULL, NULL}
		};
		luaL_register(L, NULL, props);

		Instance::register_lua_property_setters(L);
	}

	void Sound::register_lua_methods(lua_State* L){
			luaL_Reg methods[]{
				{"Pause", [](lua_State* L)->int{
					Instance* inst = checkInstance(L, 1);
					if(Sound* s = dynamic_cast<Sound*>(inst)){
						s->Pause();
						return 0;
					}
					return luaL_error(L, COLONERR, "Pause");
				}},
				{"Play", [](lua_State* L)->int{
					Instance* inst = checkInstance(L, 1);
					if(Sound* s = dynamic_cast<Sound*>(inst)){
						s->Play();
						return 0;
					}
					return luaL_error(L, COLONERR, "Play");
				}},
				{"Stop", [](lua_State* L)->int{
					Instance* inst = checkInstance(L, 1);
					if(Sound* s = dynamic_cast<Sound*>(inst)){
						s->Stop();
						return 0;
					}
					return luaL_error(L, COLONERR, "Stop");
				}},
				{NULL, NULL}
			};
			luaL_register(L, NULL, methods);

			Instance::register_lua_methods(L);
		}
}
