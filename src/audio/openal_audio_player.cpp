#include "retronomicon/audio/openal_audio_player.h"
#include "retronomicon/core/math/math_utils.h"
#include <iostream>

namespace retronomicon::audio {
    using retronomicon::core::math::clamp;
    OpenALAudioPlayer::OpenALAudioPlayer() = default;

    OpenALAudioPlayer::~OpenALAudioPlayer() {
        shutdown();
    }

    bool OpenALAudioPlayer::init() {
        m_device = alcOpenDevice(nullptr);
        if (!m_device) {
            std::cerr << "[OpenAL] Failed to open default audio device\n";
            return false;
        }

        m_context = alcCreateContext(m_device, nullptr);
        if (!m_context || !alcMakeContextCurrent(m_context)) {
            std::cerr << "[OpenAL] Failed to create or make context current\n";
            if (m_context) alcDestroyContext(m_context);
            alcCloseDevice(m_device);
            return false;
        }

        alGenSources(1, &m_musicSource);
        if (checkALError("alGenSources for music")) return false;

        std::cout << "[OpenAL] Initialized successfully\n";
        return true;
    }

    void OpenALAudioPlayer::shutdown() {
        for (auto& [name, instance] : m_sfxCache) {
            alDeleteSources(1, &instance.source);
            alDeleteBuffers(1, &instance.buffer);
        }
        m_sfxCache.clear();

        if (m_musicSource) {
            alDeleteSources(1, &m_musicSource);
            m_musicSource = 0;
        }

        if (m_context) {
            alcMakeContextCurrent(nullptr);
            alcDestroyContext(m_context);
            m_context = nullptr;
        }
        if (m_device) {
            alcCloseDevice(m_device);
            m_device = nullptr;
        }
        std::cout << "[OpenAL] Shutdown complete\n";
    }

    void OpenALAudioPlayer::update() {
        // For streaming or fade logic
        // TODO: Implement streaming buffer refill for long music tracks
    }

    /************************ GLOBAL ************************/

    void OpenALAudioPlayer::setMasterVolume(float volume) {
        m_masterVolume = clamp(volume, 0.0f, 1.0f);
        applyGlobalVolume();
    }

    float OpenALAudioPlayer::getMasterVolume() const {
        return m_masterVolume;
    }

    void OpenALAudioPlayer::applyGlobalVolume() {
        alSourcef(m_musicSource, AL_GAIN, m_masterVolume * m_musicVolume);
        for (auto& [name, instance] : m_sfxCache) {
            alSourcef(instance.source, AL_GAIN, m_masterVolume * m_sfxVolume);
        }
    }

    /************************ MUSIC ************************/

    void OpenALAudioPlayer::playMusic(const std::string& path, bool loop, int /*fadeInMs*/) {
        m_currentMusic = std::make_unique<OpenALMusicAsset>(path);
        if (!m_currentMusic->load()) {
            std::cerr << "[OpenAL] Failed to load music: " << path << "\n";
            return;
        }

        alSourcei(m_musicSource, AL_BUFFER, m_currentMusic->getBuffer());
        alSourcei(m_musicSource, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcef(m_musicSource, AL_GAIN, m_masterVolume * m_musicVolume);
        alSourcePlay(m_musicSource);
    }

    void OpenALAudioPlayer::stopMusic(int /*fadeOutMs*/) {
        alSourceStop(m_musicSource);
    }

    void OpenALAudioPlayer::setMusicPaused(bool paused) {
        if (paused)
            alSourcePause(m_musicSource);
        else
            alSourcePlay(m_musicSource);
    }

    bool OpenALAudioPlayer::isMusicPlaying() const {
        ALint state;
        alGetSourcei(m_musicSource, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }

    void OpenALAudioPlayer::setMusicVolume(float volume) {
        m_musicVolume = clamp(volume, 0.0f, 1.0f);
        alSourcef(m_musicSource, AL_GAIN, m_masterVolume * m_musicVolume);
    }

    float OpenALAudioPlayer::getMusicVolume() const {
        return m_musicVolume;
    }

    /************************ SFX ************************/

    bool OpenALAudioPlayer::loadSoundEffect(const std::string& name, const std::string& path) {
        if (m_sfxCache.find(name) != m_sfxCache.end()) return true;

        OpenALSoundEffectAsset asset(path);
        if (!asset.load()) {
            std::cerr << "[OpenAL] Failed to load SFX: " << path << "\n";
            return false;
        }

        SoundInstance instance;
        instance.buffer = asset.getBuffer();
        alGenSources(1, &instance.source);
        alSourcei(instance.source, AL_BUFFER, instance.buffer);
        alSourcef(instance.source, AL_GAIN, m_masterVolume * m_sfxVolume);

        m_sfxCache[name] = instance;
        return true;
    }

    void OpenALAudioPlayer::playSoundEffect(const std::string& name, float volume, bool loop) {
        auto it = m_sfxCache.find(name);
        if (it == m_sfxCache.end()) {
            std::cerr << "[OpenAL] Unknown SFX: " << name << "\n";
            return;
        }

        ALuint src = it->second.source;
        alSourcef(src, AL_GAIN, m_masterVolume * m_sfxVolume * volume);
        alSourcei(src, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcePlay(src);
    }

    void OpenALAudioPlayer::stopSoundEffect(const std::string& name) {
        if (name.empty()) {
            for (auto& [n, instance] : m_sfxCache)
                alSourceStop(instance.source);
        } else {
            auto it = m_sfxCache.find(name);
            if (it != m_sfxCache.end()) alSourceStop(it->second.source);
        }
    }

    void OpenALAudioPlayer::setSfxVolume(float volume) {
        m_sfxVolume = clamp(volume, 0.0f, 1.0f);
        applyGlobalVolume();
    }

    float OpenALAudioPlayer::getSfxVolume() const {
        return m_sfxVolume;
    }

    void OpenALAudioPlayer::unloadSoundEffect(const std::string& name) {
        auto it = m_sfxCache.find(name);
        if (it == m_sfxCache.end()) return;

        alDeleteSources(1, &it->second.source);
        alDeleteBuffers(1, &it->second.buffer);
        m_sfxCache.erase(it);
    }

    void OpenALAudioPlayer::clearSoundCache() {
        for (auto& [name, instance] : m_sfxCache) {
            alDeleteSources(1, &instance.source);
            alDeleteBuffers(1, &instance.buffer);
        }
        m_sfxCache.clear();
    }

    /************************ 3D AUDIO ************************/

    void OpenALAudioPlayer::setListenerPosition(const Vec3& pos) {
        alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    }

    void OpenALAudioPlayer::playSoundEffect3D(const std::string& name, const Vec3& pos, float volume, bool loop) {
        auto it = m_sfxCache.find(name);
        if (it == m_sfxCache.end()) return;

        ALuint src = it->second.source;
        alSource3f(src, AL_POSITION, pos.x, pos.y, pos.z);
        alSourcef(src, AL_GAIN, m_masterVolume * m_sfxVolume * volume);
        alSourcei(src, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcePlay(src);
    }

    /************************ INTERNAL ************************/

    bool OpenALAudioPlayer::checkALError(const std::string& context) {
        ALenum err = alGetError();
        if (err != AL_NO_ERROR) {
            std::cerr << "[OpenAL] Error in " << context << ": " << alGetString(err) << "\n";
            return true;
        }
        return false;
    }

} // namespace retronomicon::audio
