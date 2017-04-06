#pragma once

#include <boost/noncopyable.hpp>
#include <SpheresEngine/Log.h>
#include <SpheresEngine/SoundEngine/PlayHandle.h>
#include <SpheresEngine/SoundEngine/SoundPtr.h>
#include <vector>

/**
 * Abstract class to play sound and music pieces
 */
class SoundEngineAbstract: boost::noncopyable {
public:

	/**
	 * Provide dtor to support inheritance
	 */
	virtual ~SoundEngineAbstract() = default;

	/**
	 * Play a sound.
	 * direction 0.0f is from center, -1.0f is from the very left ...
	 * @return a play handle that can be used to stop playing the sound piece early
	 */
	virtual PlayHandle playSound(SoundPtr snd, float direction = 0.0f) =0;

	/**
	 * Play a music piece
	 * @return a play handle that can be used to stop playing the sound piece early
	 */
	virtual PlayHandle playMusic(MusicPtr msc) = 0;

	/**
	 * Stop playing music or sound
	 * @param fadeOutTime fade out time in ms
	 * @param ph the playhandle which was returned when starting to play the piece
	 */
	virtual void stopPlay(PlayHandle const& ph, float fadeOutTime = -1.0f) = 0;

	/**
	 * Returns true if all sound output is currently muted
	 */
	bool isMuted() const {
		return m_mute;
	}

	/**
	 * Mute all sound output. This is useful on mobile platforms, when the app
	 * gets suspended.
	 */
	void setMuted(bool m) {
		m_mute = m;
	}

	/**
	 * pause the sound engine
	 */
	virtual void pauseSound() {
	}

	/**
	 * resume the sound engine
	 */
	virtual void resumeSound() {
	}

private:

	/**
	 * If true, no sound will be output
	 */
	bool m_mute = false;
};
