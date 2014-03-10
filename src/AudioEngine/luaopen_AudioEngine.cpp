#include <luaT/luaT.h>
#include "AudioComponent.h"
#include "CppBase/StringUtil.h"
#include "AudioStopListener.h"
#include "AudioComponentSystem.h"

using namespace luaT;

luaT_defMetaData(xihad::audio::AudioComponent, false);
// luaT_defMetaData(xihad::audio::AudioStopListener, false);
luaT_defMetaData(xihad::audio::AudioComponentSystem, false);
namespace xihad { namespace audio 
{
	using namespace ngn;
	
	static irrklang::E_STREAM_MODE popStreamMode(lua_State* L, int idx)
	{
		if (lua_gettop(L) >= idx)
		{
			const char* modes[] = { "AutoDetect", "Streaming", "NoStreaming", 0 };
			luaT_variable(L, idx, const char*, mode);
			int midx = StringUtil::select(mode, modes);
			if (midx != -1)
				return (irrklang::E_STREAM_MODE) midx;
			else 
				luaL_argerror(L, idx, "Error stream mode");
		}
		
		return irrklang::ESM_AUTO_DETECT;
	}
	
	// TODO
	static int playMusic(lua_State* L)
	{
		luaT_variable(L, 1, AudioComponent*, comp);
		luaT_variable(L, 2, const char*, fname);
		auto mode = popStreamMode(L, 3);
		
		comp->playMusic(fname, mode);
		return 0;
	}

	static int playSound(lua_State* L)
	{
		luaT_variable(L, 1, AudioComponent*, comp);
		luaT_variable(L, 2, const char*, fname);
		auto mode = popStreamMode(L, 3);
		
		comp->playSound(fname, mode);
		return 0;
	}

// 	static int addAudioStopListener(lua_State* L)
// 	{
// 		class LuaAudioStopListener : public AudioStopListener
// 		{
// 		public:
// 			virtual void onSoundStopped(AudioComponent* sound, E_STOP_EVENT_CAUSE reason)
// 			{
// 				
// 			}
// 		};
// 	}
// 
// 	static int removeAudioStopListener(lua_State* L)
// 	{
// 
// 	}


	int luaopen_AudioComponents(lua_State* L)
	{
		luaT_defRegsBgn(audio)
			luaT_mnamedfunc(AudioComponent, setPaused),
			luaT_mnamedfunc(AudioComponent, isPaused),
			luaT_mnamedfunc(AudioComponent, stopAudio),
			luaT_mnamedfunc(AudioComponent, getVolume),
			luaT_mnamedfunc(AudioComponent, setVolume),
			luaT_mnamedfunc(AudioComponent, setPan),
			luaT_mnamedfunc(AudioComponent, getPan),
			luaT_mnamedfunc(AudioComponent, isLooped),
			luaT_mnamedfunc(AudioComponent, setLooped),
			luaT_mnamedfunc(AudioComponent, isFinished),
			luaT_mnamedfunc(AudioComponent, setMinDistance),
			luaT_mnamedfunc(AudioComponent, getMinDistance),
			luaT_mnamedfunc(AudioComponent, setMaxDistance),
			luaT_mnamedfunc(AudioComponent, getMaxDistance),
			luaT_mnamedfunc(AudioComponent, getPlayPosition),
			luaT_mnamedfunc(AudioComponent, setPlayPosition),
			luaT_mnamedfunc(AudioComponent, getPlayLength),
			luaT_mnamedfunc(AudioComponent, setPlaybackSpeed),
			luaT_mnamedfunc(AudioComponent, getPlaybackSpeed),
			luaT_lnamedfunc(playMusic),
			luaT_lnamedfunc(playSound),
			
			// TODO 
			// AudioListener
		luaT_defRegsEnd
		MetatableFactory<AudioComponent, Component>::create(L, audio, 0);

		luaT_defRegsBgn(system)
			luaT_mnamedfunc(AudioComponentSystem, stopAllAudios),
			luaT_mnamedfunc(AudioComponentSystem, setAllAudiosPaused),
			luaT_mnamedfunc(AudioComponentSystem, setListenerPosition),
			luaT_mnamedfunc(AudioComponentSystem, setSoundVolume),
			luaT_mnamedfunc(AudioComponentSystem, getSoundVolume),
			luaT_mnamedfunc(AudioComponentSystem, setDefault3DSoundMinDistance),
			luaT_mnamedfunc(AudioComponentSystem, getDefault3DSoundMinDistance),
			luaT_mnamedfunc(AudioComponentSystem, setDefault3DSoundMaxDistance),
			luaT_mnamedfunc(AudioComponentSystem, getDefault3DSoundMaxDistance),
		luaT_defRegsEnd
		MetatableFactory<AudioComponentSystem>::create(L, system, 0);

		return 0;
	}
}}


