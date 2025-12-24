#pragma once

#include "retronomicon/audio/i_audio_player.h"
#include "retronomicon/asset/openal_sound_effect_asset.h"
#include "retronomicon/asset/openal_music_asset.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <unordered_map>
#include <string>
#include <memory>

namespace retronomicon::opengl::audio {

    using retronomicon::opengl::asset::OpenALMusicAsset;
    using retronomicon::opengl::asset::OpenALSoundEffectAsset;

    /**
     * @class OpenALAudioPlayer
     * @brief OpenAL backend implementation of the IAudioPlayer interface.
     *
     * This class provides an OpenAL-based audio backend supporting:
     *  - Music playback (single active track)
     *  - Sound effect playback with caching
     *  - Global, music, and SFX volume control
     *  - Optional 3D positional audio
     *
     * Responsibilities:
     *  - Manage OpenAL device and context lifecycle
     *  - Own OpenAL sources and buffers
     *  - Translate engine-level audio commands into OpenAL calls
     *
     * This class is intended to be backend-specific and not exposed
     * directly to gameplay code.
     */
    class OpenALAudioPlayer : public retronomicon::audio::IAudioPlayer {
    public:
        /**
         * @brief Constructs the audio player.
         *
         * Does not create the OpenAL device or context.
         * Call init() before using the player.
         */
        OpenALAudioPlayer();

        /**
         * @brief Destroys the audio player.
         *
         * Automatically shuts down OpenAL resources if still initialized.
         */
        ~OpenALAudioPlayer() override;

        /***************************** Initialization *****************************/

        /**
         * @brief Initializes the OpenAL device and context.
         *
         * @return true if initialization succeeds, false otherwise.
         */
        bool init() override;

        /**
         * @brief Shuts down the OpenAL context and releases all resources.
         */
        void shutdown() override;

        /***************************** Global *****************************/

        /**
         * @brief Updates internal audio state.
         *
         * Intended for streaming updates, fade logic, or cleanup.
         * Should be called once per frame.
         */
        void update() override;

        /**
         * @brief Sets the master volume.
         *
         * @param volume Volume multiplier in range [0.0, 1.0].
         */
        void setMasterVolume(float volume) override;

        /**
         * @brief Gets the current master volume.
         *
         * @return Master volume multiplier.
         */
        float getMasterVolume() const override;

        /***************************** Music *****************************/

        /**
         * @brief Plays a music track.
         *
         * Stops any currently playing music.
         *
         * @param path Path to the music asset.
         * @param loop Whether the music should loop.
         * @param fadeInMs Fade-in duration in milliseconds (optional).
         */
        void playMusic(const std::string& path,
                       bool loop = true,
                       int fadeInMs = 0) override;

        /**
         * @brief Stops the currently playing music.
         *
         * @param fadeOutMs Fade-out duration in milliseconds (optional).
         */
        void stopMusic(int fadeOutMs = 0) override;

        /**
         * @brief Pauses or resumes music playback.
         *
         * @param paused True to pause, false to resume.
         */
        void setMusicPaused(bool paused) override;

        /**
         * @brief Checks whether music is currently playing.
         *
         * @return true if music is playing, false otherwise.
         */
        bool isMusicPlaying() const override;

        /**
         * @brief Sets the music volume.
         *
         * @param volume Volume multiplier in range [0.0, 1.0].
         */
        void setMusicVolume(float volume) override;

        /**
         * @brief Gets the music volume.
         *
         * @return Music volume multiplier.
         */
        float getMusicVolume() const override;

        /***************************** Sound Effects *****************************/

        /**
         * @brief Loads a sound effect into the cache.
         *
         * @param name Logical name used to reference the sound.
         * @param path Path to the sound effect asset.
         * @return true if loading succeeds, false otherwise.
         */
        bool loadSoundEffect(const std::string& name,
                             const std::string& path) override;

        /**
         * @brief Plays a cached sound effect.
         *
         * @param name Sound effect name.
         * @param volume Volume multiplier.
         * @param loop Whether the sound should loop.
         */
        void playSoundEffect(const std::string& name,
                             float volume = 1.0f,
                             bool loop = false) override;

        /**
         * @brief Stops a specific sound effect or all sound effects.
         *
         * @param name Name of the sound to stop. Empty string stops all.
         */
        void stopSoundEffect(const std::string& name = "") override;

        /**
         * @brief Sets the global sound effect volume.
         *
         * @param volume Volume multiplier in range [0.0, 1.0].
         */
        void setSfxVolume(float volume) override;

        /**
         * @brief Gets the global sound effect volume.
         *
         * @return SFX volume multiplier.
         */
        float getSfxVolume() const override;

        /**
         * @brief Unloads a sound effect from the cache.
         *
         * @param name Name of the sound effect to unload.
         */
        void unloadSoundEffect(const std::string& name) override;

        /**
         * @brief Clears all cached sound effects.
         */
        void clearSoundCache() override;

        /***************************** 3D Audio (optional) *****************************/

        /**
         * @brief Sets the listener position for 3D audio.
         *
         * @param pos World-space listener position.
         */
        void setListenerPosition(const retronomicon::audio::Vec3& pos) override;

        /**
         * @brief Plays a 3D positional sound effect.
         *
         * @param name Sound effect name.
         * @param pos World-space sound position.
         * @param volume Volume multiplier.
         * @param loop Whether the sound should loop.
         */
        void playSoundEffect3D(const std::string& name,
                               const retronomicon::audio::Vec3& pos,
                               float volume = 1.0f,
                               bool loop = false) override;

    private:
        /***************************** OpenAL Context *****************************/

        /** OpenAL device handle */
        ALCdevice*  m_device  = nullptr;

        /** OpenAL context handle */
        ALCcontext* m_context = nullptr;

        /***************************** SFX Cache *****************************/

        /**
         * @struct SoundInstance
         * @brief Represents a single sound effect instance.
         */
        struct SoundInstance {
            /** OpenAL buffer storing audio data */
            ALuint buffer = 0;

            /** OpenAL source used for playback */
            ALuint source = 0;
        };

        /** Cached sound effects indexed by logical name */
        std::unordered_map<std::string, SoundInstance> m_sfxCache;

        /***************************** Music *****************************/

        /** Currently loaded music asset */
        std::unique_ptr<OpenALMusicAsset> m_currentMusic;

        /** OpenAL source used for music playback */
        ALuint m_musicSource = 0;

        /***************************** Volume *****************************/

        /** Master volume multiplier */
        float m_masterVolume = 1.0f;

        /** Music volume multiplier */
        float m_musicVolume  = 1.0f;

        /** Sound effects volume multiplier */
        float m_sfxVolume    = 1.0f;

        /***************************** Internal Helpers *****************************/

        /**
         * @brief Applies master, music, and SFX volume to OpenAL sources.
         */
        void applyGlobalVolume();

        /**
         * @brief Checks for OpenAL errors.
         *
         * @param context Description of the calling context for logging.
         * @return true if an error occurred, false otherwise.
         */
        bool checkALError(const std::string& context);
    };

} // namespace retronomicon::opengl::audio
