#pragma once

#include "retronomicon/asset/sound_effect_asset.h"
#include "retronomicon/core/math/math_utils.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <vector>
#include <iostream>

namespace retronomicon::asset {

    using retronomicon::core::math::clamp;
    /**
     * @brief SoundEffectAsset implementation using OpenAL.
     * 
     * Represents a short sound effect (SFX) loaded into an OpenAL buffer and played via a source.
     * Intended for UI clicks, footsteps, etc.
     */
    class OpenALSoundEffectAsset final : public SoundEffectAsset {
    public:

        /**
         * @brief Construct an OpenALSoundEffectAsset.
         *
         * @param path Path to the audio file.
        */
        OpenALSoundEffectAsset(const std::string& path);

        /**
         * @brief Construct a new OpenALSoundEffectAsset.
         * 
         * @param path The file path to the sound effect.
         * @param name The display name of this asset.
         */
        OpenALSoundEffectAsset(std::string path, std::string name);

        /**
         * @brief Destroy the OpenALSoundEffectAsset and free OpenAL resources.
         */
        ~OpenALSoundEffectAsset() override;

        /**
         * @brief Load the sound effect from file into an OpenAL buffer.
         * 
         * @return true if successfully loaded, false otherwise.
         */
        bool load() override;

        /**
         * @brief Unload and delete the OpenAL buffer/source.
         */
        void unload() override;

        /**
         * @brief Play the sound effect.
         * 
         * @param loop Whether to loop playback (default: false).
         */
        void play(bool loop = false) override;

        /**
         * @brief Stop the sound effect if playing.
         */
        void stop() override;

        /***************************** Inline Getter/Setter *****************************/

        /**
         * @brief Set the playback volume (gain).
         * @param volume Volume between 0.0f (silent) and 1.0f (full).
         */
        inline void setVolume(float volume) {
            m_volume = clamp(volume, 0.0f, 1.0f);
            if (m_source)
                alSourcef(m_source, AL_GAIN, m_volume);
        }
        
        /**
         * @brief Get the current playback volume.
         */
        inline float getVolume() const { return m_volume; }

        /**
         * @brief Check whether the sound is currently playing.
         */
        inline bool isPlaying() const { return m_playing; }

        /**
         * @brief Get the OpenAL source handle.
         */
        inline ALuint getSource() const { return m_source; }

        /**
         * @brief Get the OpenAL buffer handle.
         */
        inline ALuint getBuffer() const { return m_buffer; }

    private:
        /**
         * @brief Load WAV data manually (very minimal parser for PCM WAV).
         * 
         * @param path Path to file
         * @param data Out buffer for audio data
         * @param format Out AL format
         * @param freq Out frequency
         * @return true if loaded, false otherwise.
         */
        bool loadWavFile(const std::string& path, std::vector<char>& data, ALenum& format, ALsizei& freq);

    private:
        ALuint m_buffer = 0;   ///< OpenAL buffer ID
        ALuint m_source = 0;   ///< OpenAL source ID
        bool m_playing = false;///< Whether currently playing
        float m_volume = 1.0f; ///< Volume (0.0 - 1.0)
    };

} // namespace retronomicon::asset
