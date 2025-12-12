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

    /**
     * @brief Construct the audio player without initializing OpenAL yet.
     *
     * `init()` must be called before any audio playback occurs.
     */
    OpenALAudioPlayer::OpenALAudioPlayer() = default;

    /**
     * @brief Destructor ensures OpenAL is properly shut down.
     */
    OpenALAudioPlayer::~OpenALAudioPlayer() {
        shutdown();
    }

    /**************************************************************************
     * Initialization
     **************************************************************************/

    /**
     * @brief Initialize the OpenAL device, context, and music source.
     *
     * @return True on success, false if initialization failed.
     *
     * Steps:
     *  - Opens the default device
     *  - Creates the OpenAL audio context
     *  - Makes the context current
     *  - Allocates a source dedicated to music playback
     */
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

    /**
     * @brief Shutdown OpenAL, clear buffers, and release audio resources.
     */
    void OpenALAudioPlayer::shutdown() {
        clearSoundCache();

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

    /**************************************************************************
     * Update (future expansion)
     **************************************************************************/

    /**
     * @brief Per-frame update hook.
     *
     * Currently unused, but reserved for:
     *  - streaming music buffers,
     *  - fade-in/fade-out logic,
     *  - dynamic reverb environments.
     */
    void OpenALAudioPlayer::update() {
        // Placeholder for future streaming/fading features
    }

    /**************************************************************************
     * Global Volume
     **************************************************************************/

    /**
     * @brief Set the engine-wide master volume [0..1].
     *
     * Applies to both music and SFX.
     */
    void OpenALAudioPlayer::setMasterVolume(float volume) {
        m_masterVolume = clamp(volume, 0.0f, 1.0f);
        applyGlobalVolume();
    }

    /**
     * @brief Apply master/music/SFX volume to all OpenAL sources.
     */
    void OpenALAudioPlayer::applyGlobalVolume() {
        alSourcef(m_musicSource, AL_GAIN, m_masterVolume * m_musicVolume);

        for (auto& [_, inst] : m_sfxCache) {
            alSourcef(inst.source, AL_GAIN, m_masterVolume * m_sfxVolume);
        }
    }

    /**************************************************************************
     * Music Control
     **************************************************************************/

    /**
     * @brief Load and begin playing music from a file.
     *
     * @param path Path to the audio file.
     * @param loop Whether the music should loop.
     */
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

    /**
     * @brief Stop the currently playing music.
     */
    void OpenALAudioPlayer::stopMusic(int) {
        alSourceStop(m_musicSource);
    }

    /**
     * @brief Pause or resume music playback.
     */
    void OpenALAudioPlayer::setMusicPaused(bool paused) {
        if (paused)
            alSourcePause(m_musicSource);
        else
            alSourcePlay(m_musicSource);
    }

    /**
     * @brief Check if music is actively playing.
     */
    bool OpenALAudioPlayer::isMusicPlaying() const {
        ALint state;
        alGetSourcei(m_musicSource, AL_SOURCE_STATE, &state);
        return (state == AL_PLAYING);
    }

    /**************************************************************************
     * Sound Effect Loading
     **************************************************************************/

    /**
     * @brief Load and cache a sound effect from file.
     *
     * Loads PCM data, creates an OpenAL buffer+source, and stores it in m_sfxCache.
     *
     * @return True on success.
     */
    bool OpenALAudioPlayer::loadSoundEffect(const std::string& name, const std::string& path) {
        if (m_sfxCache.find(name) != m_sfxCache.end())
            return true; // Already cached

        OpenALSoundEffectAsset decoder(path);

        std::vector<char> pcm;
        ALenum format = 0;
        ALsizei freq = 0;

        if (!decoder.decode(pcm, format, freq)) {
            std::cerr << "[OpenAL] Failed to decode SFX: " << path << "\n";
            return false;
        }

        SoundInstance inst;

        alGenBuffers(1, &inst.buffer);
        alBufferData(inst.buffer, format, pcm.data(), pcm.size(), freq);

        alGenSources(1, &inst.source);
        alSourcei(inst.source, AL_BUFFER, inst.buffer);
        alSourcef(inst.source, AL_GAIN, m_masterVolume * m_sfxVolume);

        m_sfxCache[name] = inst;
        return true;
    }

    /**************************************************************************
     * Sound Effect Playback
     **************************************************************************/

    /**
     * @brief Play a previously loaded sound effect.
     *
     * @param name Identifier given during `loadSoundEffect()`.
     * @param volume Additional volume multiplier.
     * @param loop Whether the SFX should loop.
     */
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

    /**
     * @brief Stop a single SFX, or all SFX if name is empty.
     */
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

    /**************************************************************************
     * SFX Volume
     **************************************************************************/

    void OpenALAudioPlayer::setSfxVolume(float volume) {
        m_sfxVolume = clamp(volume, 0.0f, 1.0f);
        applyGlobalVolume();
    }

    /**************************************************************************
     * Sound Effect Unloading
     **************************************************************************/

    /**
     * @brief Remove a sound effect from memory and delete its OpenAL buffers.
     */
    void OpenALAudioPlayer::unloadSoundEffect(const std::string& name) {
        auto it = m_sfxCache.find(name);
        if (it == m_sfxCache.end())
            return;

        alDeleteSources(1, &it->second.source);
        alDeleteBuffers(1, &it->second.buffer);

        m_sfxCache.erase(it);
    }

    /**
     * @brief Clear ALL cached SFX and delete associated OpenAL resources.
     */
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

    /**
     * @brief Set the listener's 3D position in the scene.
     */
    void OpenALAudioPlayer::setListenerPosition(const Vec3& pos) {
        alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    }

    /**
     * @brief Play a 3D sound from a specific world position.
     */
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

    /**
     * @brief Check for AL errors and print a message if one is found.
     *
     * @param context Human-readable label for where the error occurred.
     * @return True if an error was detected.
     */
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
