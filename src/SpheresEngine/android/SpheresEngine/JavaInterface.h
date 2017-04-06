#pragma once

#include <string>
#include <jni.h>
#include <android/native_activity.h>
#include <SpheresEngine/Log.h>

/**
 * Contains various static method to simplify C++ <-> Java calls
 */
class JavaTools {
public:
	/**
	 * Find the method id of a java method by name and signature. To search for the
	 * Java method of this signature:
	 *
	 * int loadImage (String name) {...}
	 *
	 * use
	 * name = "loadImage"
	 * signature "(Ljava/lang/String;)I"
	 */
	static jmethodID findJavaMethod(JNIEnv * env, jobject obj,
			std::string const& name, std::string const& signature) {

		jclass cls = env->GetObjectClass(obj);
		jmethodID mid = env->GetMethodID(cls, name.c_str(), signature.c_str());
		if (mid == 0) {
			logging::Fatal() << "Java Method " << name << " with signature "
					<< signature << " not found";
			return 0;
		}
		return mid;
	}

	/**
	 * Convert a std::string to a java string
	 * Note: the returned jstring must not be freed in any way, because it
	 * resides in the Java Mem space and will be handleded by the JVM mem management.
	 */
	static jstring toJavaString(JNIEnv * env, std::string const& st) {

		jstring jstr = env->NewStringUTF(st.c_str());
		if (jstr == 0) {
			logging::Fatal() << "NewStringUTF returned NULL";
			return nullptr;
		}
		return jstr;
	}

	/**
	 * Convert an std::string to a java string
	 */
	static void toStdString(JNIEnv *env, jstring jStr, std::string & sStr) {
		if (!jStr) {
			sStr.clear();
			return;
		}

		const char *s = env->GetStringUTFChars(jStr, NULL);
		sStr = s;
		env->ReleaseStringUTFChars(jStr, s);
	}
};

/**
 * JavaInterface implementation which does not need a native activity to work
 */
class JavaInterfaceNonNative {
public:
	/**
	 * Create the JavaInterface class
	 */
	JavaInterfaceNonNative(JNIEnv * env, jobject callingJavaObject) :
			m_env(env), m_javaActivity(callingJavaObject) {

	}

	/**
	 * start playing a sound
	 */
	int playSound(std::string const& soundName, float direction) {
		jmethodID midPlaySound = JavaTools::findJavaMethod(m_env,
				m_javaActivity, "playSound", "(Ljava/lang/String;F)I");
		jstring theSoundname = JavaTools::toJavaString(m_env, soundName);
		return m_env->CallIntMethod(m_javaActivity, midPlaySound, theSoundname);
	}

	/**
	 * start playing a music piece
	 */
	int playMusic(std::string const& musicName) {
		jmethodID midPlayMusic = JavaTools::findJavaMethod(m_env,
				m_javaActivity, "playMusic", "(Ljava/lang/String;)I");
		jstring theSoundname = JavaTools::toJavaString(m_env, musicName);
		m_env->CallIntMethod(m_javaActivity, midPlayMusic, theSoundname);
		return 0;
	}

	/**
	 * stop playing a sound or music piece
	 */
	void stopPlay(int playId, float fadeOutTime) {
		jmethodID midStopPlay = JavaTools::findJavaMethod(m_env, m_javaActivity,
				"stopPlay", "(I)V");
		m_env->CallVoidMethod(m_javaActivity, midStopPlay, playId);
	}

	/**
	 * load an image from the java resources
	 */
	int loadImage(std::string const& imageName) {
		jmethodID midLoadImage = JavaTools::findJavaMethod(m_env,
				m_javaActivity, "loadImage", "(Ljava/lang/String;)I");
		jstring theImageName = JavaTools::toJavaString(m_env, imageName);
		return m_env->CallIntMethod(m_javaActivity, midLoadImage, theImageName);
	}

	/**
	 * Free a texture which has been loaded
	 */
	void freeTexture(int textureId) {
		jmethodID mid = JavaTools::findJavaMethod(m_env, m_javaActivity,
				"freeTexture", "(I)V");
		m_env->CallVoidMethod(m_javaActivity, mid, textureId);
	}

	/**
	 * Resume playing all sounds
	 */
	void resumeSound() {
		jmethodID mid = JavaTools::findJavaMethod(m_env, m_javaActivity,
				"resumeSound", "()V");
		m_env->CallVoidMethod(m_javaActivity, mid);
	}

	/**
	 * Pause playing all sounds
	 */
	void pauseSound() {
		jmethodID mid = JavaTools::findJavaMethod(m_env, m_javaActivity,
				"pauseSound", "()V");
		m_env->CallVoidMethod(m_javaActivity, mid);
	}

	/**
	 * Load a text file from the android resources
	 */
	std::string loadTextFile(std::string const& textFileName) {
		std::string resultString;
		jstring res;
		jmethodID midLoadTextFile = JavaTools::findJavaMethod(m_env,
				m_javaActivity, "loadTextFile",
				"(Ljava/lang/String;)Ljava/lang/String;");
		jstring theTextFileName = JavaTools::toJavaString(m_env, textFileName);
		res = (jstring) m_env->CallObjectMethod(m_javaActivity, midLoadTextFile,
				theTextFileName);

		// convert to C++ string
		const char * utf8charResult = m_env->GetStringUTFChars(res, nullptr);
		resultString = std::string(utf8charResult);
		m_env->ReleaseStringUTFChars(res, utf8charResult);

		return resultString;
	}

private:
	/**
	 * Pointer to the java envirnoment, used to call java methods etc.
	 */
	JNIEnv * m_env = nullptr;

	/**
	 * Java side android activity which contains all the methods we want to call
	 */
	jobject m_javaActivity;
};
