#pragma once

#include <SpheresEngine/Cpp11.h>
#include <SpheresEngine/JavaInterface.h>
#include <SpheresEngine/SoundEngine/SoundEngineAbstract.h>

/**
 * Sound engine for android systems
 */
class SoundEngine final: public SoundEngineAbstract {
public:

	/*
	 * Create the sound engine
	 */
	SoundEngine() :
			SoundEngineAbstract(), m_javaInterface(nullptr) {
		logging::Info() << "Android Sound Engine initialized ";

	}

	/**
	 * Start playing a sound
	 */
	PlayHandle playSound(SoundPtr snd, float direction = 0.0f) override {
		if (isMuted())
			return PlayHandle(0);

		auto hndl = m_javaInterface->playSound(snd, direction);
		return PlayHandle(hndl);
	}

	/**
	 * Start playing a music piece
	 */
	PlayHandle playMusic(MusicPtr msc) override {
		if (isMuted())
			return PlayHandle(0);

		auto hndl = m_javaInterface->playMusic(msc);
		return PlayHandle(hndl);
	}

	/**
	 * stop playing music or sound
	 */
	void stopPlay(PlayHandle const& ph, float fadeOutTime = -1.0f) override {
		m_javaInterface->stopPlay(ph, fadeOutTime);
	}

	/**
	 * Resume playing all sounds android
	 */
	void resumeSound() override {
		m_javaInterface->resumeSound();
	}

	/**
	 * Pause all sound playback on android
	 */
	void pauseSound() override {
		m_javaInterface->pauseSound();
	}

	/**
	 * The the java interface class which is currently used
	 */
	void setJavaInterface(JavaInterface * ji) {
		m_javaInterface = ji;
	}

private:

	/**
	 * Stores the java interface for calls into the java API
	 */
	JavaInterface * m_javaInterface;
};

