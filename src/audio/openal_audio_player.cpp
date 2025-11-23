#include "retronomicon/audio/openal_audio_player.h"
#include "retronomicon/asset/openal_sound_effect_asset.h"
#include "retronomicon/math/math_utils.h"

#include <iostream>
#include <fstream>

namespace retronomicon::opengl::audio {

    using retronomicon::math::clamp;
    using retronomicon::audio::Vec3;

    /**************************************************************************
     * Constructor / Destructor
     **************************************************************************/

    OpenALAudioPlayer::OpenALAudioPlayer() = default;

    OpenALAudioPlayer::~OpenALAudioPlayer() {
        shutdown();
    }

    /**************************************************************************
     * Initialization
     **************************************************************************/

    bool OpenALAudioPlayer::init() {
        m_device = alcOpenDevice(nullptr);
        if (!m_device) {
            std::cerr << "[OpenAL] Failed to open audio device\n";
            return false;
        }

        m_context = alcCreateContext(m_device, nullptr);
        if (!m_context || !alcMakeContextCurrent(m_context)) {
            std::cerr << "[OpenAL] Failed to create or activate audio context\n";
            if (m_context) alcDestroyContext(m_context);
            alcCloseDevice(m_device);
            m_device = nullptr;
            return false;
        }

        alGenSources(1, &m_musicSource);
        if (checkALError("Generate music source"))
            return false;

        std::cout << "[OpenAL] Audio initialized\n";
        return true;
    }

    /**************************************************************************
     * Shutdown
     **************************************************************************/

    void OpenALAudioPlayer::shutdown() {
        // Clear SFX
        clearSoundCache();

        // Delete music source
        if (m_musicSource) {
            alDeleteSources(1, &m_musicSource);
            m_musicSource = 0;
        }

        // Destroy context/device
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

    /**************************************************************************
     * Update
     **************************************************************************/

    void OpenALAudioPlayer::update() {
        // Future:
        // - streaming music
        // - fade in/out
    }

    /**************************************************************************
     * Global Volume
     **************************************************************************/

    void OpenALAudioPlayer::setMasterVolume(float volume) {
        m_masterVolume = clamp(volume, 0.0f, 1.0f);
        applyGlobalVolume();
    }

    float OpenALAudioPlayer::getMasterVolume() const {
        return m_masterVolume;
    }

    void OpenALAudioPlayer::applyGlobalVolume() {
        // Apply to music
        alSourcef(m_musicSource, AL_GAIN, m_masterVolume * m_musicVolume);

        // Apply to SFX
        for (auto& [_, inst] : m_sfxCache) {
            alSourcef(inst.source, AL_GAIN, m_masterVolume * m_sfxVolume);
        }
    }

    /**************************************************************************
     * Music Control
     **************************************************************************/

    void OpenALAudioPlayer::playMusic(const std::string& path, bool loop, int) {
        m_currentMusic = std::make_unique<OpenALMusicAsset>(path);

        if (!m_currentMusic->load()) {
            std::cerr << "[OpenAL] Failed to load music: " << path << "\n";
            return;
        }

        ALuint buffer = m_currentMusic->getBuffer();

        alSourcei(m_musicSource, AL_BUFFER, buffer);
        alSourcei(m_musicSource, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcef(m_musicSource, AL_GAIN, m_masterVolume * m_musicVolume);
        alSourcePlay(m_musicSource);
    }

    void OpenALAudioPlayer::stopMusic(int) {
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
        return (state == AL_PLAYING);
    }

    void OpenALAudioPlayer::setMusicVolume(float volume) {
        m_musicVolume = clamp(volume, 0.0f, 1.0f);
        alSourcef(m_musicSource, AL_GAIN, m_masterVolume * m_musicVolume);
    }

    float OpenALAudioPlayer::getMusicVolume() const {
        return m_musicVolume;
    }

    /**************************************************************************
     * Sound Effect Loading
     **************************************************************************/

    bool OpenALAudioPlayer::loadSoundEffect(const std::string& name, const std::string& path) {
        // Already cached?
        if (m_sfxCache.find(name) != m_sfxCache.end())
            return true;

        // Use decoder-only asset
        OpenALSoundEffectAsset decoder(path);

        std::vector<char> pcm;
        ALenum format = 0;
        ALsizei freq = 0;

        if (!decoder.decode(pcm, format, freq)) {
            std::cerr << "[OpenAL] Failed to decode SFX: " << path << "\n";
            return false;
        }

        SoundInstance inst;

        // Generate AL buffer
        alGenBuffers(1, &inst.buffer);
        alBufferData(inst.buffer, format, pcm.data(), pcm.size(), freq);

        // Generate AL source
        alGenSources(1, &inst.source);
        alSourcei(inst.source, AL_BUFFER, inst.buffer);
        alSourcef(inst.source, AL_GAIN, m_masterVolume * m_sfxVolume);

        m_sfxCache[name] = inst;
        return true;
    }

    /**************************************************************************
     * Sound Effect Playback
     **************************************************************************/

    void OpenALAudioPlayer::playSoundEffect(const std::string& name, float volume, bool loop) {

        std::cout<< "[OpenAL] playSoundEffect: " <<name << std::endl;
        auto it = m_sfxCache.find(name);
        if (it == m_sfxCache.end()) {
            std::cerr << "[OpenAL] Unknown SFX: " << name << "\n";
            return;
        }

        ALuint src = it->second.source;


        std::cout<< "[OpenAL] playSoundEffect2: " <<name << std::endl;
        alSourcef(src, AL_GAIN, m_masterVolume * m_sfxVolume * volume);
        alSourcei(src, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcePlay(src);

        std::cout<< "[OpenAL] playSoundEffect3: " <<name << std::endl;
    }

    void OpenALAudioPlayer::stopSoundEffect(const std::string& name) {
        if (name.empty()) {
            for (auto& [_, inst] : m_sfxCache)
                alSourceStop(inst.source);
            return;
        }

        auto it = m_sfxCache.find(name);
        if (it != m_sfxCache.end())
            alSourceStop(it->second.source);
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
        if (it == m_sfxCache.end())
            return;

        alDeleteSources(1, &it->second.source);
        alDeleteBuffers(1, &it->second.buffer);

        m_sfxCache.erase(it);
    }

    void OpenALAudioPlayer::clearSoundCache() {
        for (auto& [_, inst] : m_sfxCache) {
            alDeleteSources(1, &inst.source);
            alDeleteBuffers(1, &inst.buffer);
        }
        m_sfxCache.clear();
    }

    /**************************************************************************
     * 3D Positional Audio
     **************************************************************************/

    void OpenALAudioPlayer::setListenerPosition(const Vec3& pos) {
        alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    }

    void OpenALAudioPlayer::playSoundEffect3D(const std::string& name,
                                             const Vec3& pos,
                                             float volume,
                                             bool loop)
    {
        auto it = m_sfxCache.find(name);
        if (it == m_sfxCache.end())
            return;

        ALuint src = it->second.source;

        alSource3f(src, AL_POSITION, pos.x, pos.y, pos.z);
        alSourcef(src, AL_GAIN, m_masterVolume * m_sfxVolume * volume);
        alSourcei(src, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);

        alSourcePlay(src);
    }

    /**************************************************************************
     * Error Checking
     **************************************************************************/

    bool OpenALAudioPlayer::checkALError(const std::string& context) {
        ALenum err = alGetError();
        if (err != AL_NO_ERROR) {
            std::cerr << "[OpenAL] Error in " << context << ": "
                      << alGetString(err) << "\n";
            return true;
        }
        return false;
    }

} // namespace retronomicon::opengl::audio
