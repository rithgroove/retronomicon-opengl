#pragma once

#include "retronomicon/asset/sound_effect_asset.h"
#include <string>
#include <vector>
#include <iostream>
#include <AL/al.h>

namespace retronomicon::opengl::asset {

    /**
     * @brief A sound effect asset that ONLY decodes audio data (WAV/OGG)
     *        and does NOT create any OpenAL buffers or sources.
     *
     * The OpenALAudioPlayer owns OpenAL buffers/sources.
     */
    class OpenALSoundEffectAsset final : public retronomicon::asset::SoundEffectAsset {
    public:

        /**
         * @brief Construct a decoder-only sound effect asset.
         * @param path Path to audio file.
         */
        OpenALSoundEffectAsset(const std::string& path);

        /**
         * @brief Construct with explicit path + asset name.
         */
        OpenALSoundEffectAsset(std::string path, std::string name);

        /**
         * @brief Destructor — no OpenAL cleanup because we no longer own AL objects.
         */
        ~OpenALSoundEffectAsset() override = default;

        /**
         * @brief Decode WAV or OGG audio data into PCM memory.
         *
         * OpenALAudioPlayer will upload this PCM data into an AL buffer.
         *
         * @param outData  The returned PCM audio bytes.
         * @param outFmt   The OpenAL format (AL_FORMAT_MONO16, etc.)
         * @param outFreq  Sampling rate
         * @return true if decoding succeeded.
         */
        bool decode(std::vector<char>& outData, ALenum& outFmt, ALsizei& outFreq);

        /**
         * @brief Dummy overrides (no OpenAL source/buffer anymore).
         */
        bool load() override { return true; }
        void unload() override {}
        void play(bool loop = false) override {}
        void stop() override {}

    private:
        // Internal decoders
        bool loadWavFile(const std::string& path,
                         std::vector<char>& data,
                         ALenum& format,
                         ALsizei& freq);

        bool loadOggFile(const std::string& path,
                         std::vector<char>& data,
                         ALenum& format,
                         ALsizei& freq);
    };

} // namespace retronomicon::opengl::asset
