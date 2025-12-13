#pragma once

#include "retronomicon/asset/sound_effect_asset.h"
#include <string>
#include <vector>
#include <iostream>
#include <AL/al.h>

namespace retronomicon::opengl::asset {

    /**
     * @brief Decoder-only sound effect asset for OpenAL backends.
     *
     * This class is responsible for **decoding audio files only**
     * (e.g. WAV, OGG) into raw PCM data.
     *
     * ⚠️ Important design rule:
     *  - This class does **NOT** create or own any OpenAL buffers or sources.
     *  - All OpenAL objects are owned and managed by `OpenALAudioPlayer`.
     *
     * This separation ensures:
     *  - clean ownership boundaries,
     *  - predictable lifetime management,
     *  - backend-agnostic asset handling.
     */
    class OpenALSoundEffectAsset final
        : public retronomicon::asset::SoundEffectAsset {
    public:

        /**
         * @brief Construct a decoder-only sound effect asset.
         *
         * @param path Path to the audio file (WAV or OGG).
         */
        explicit OpenALSoundEffectAsset(const std::string& path);

        /**
         * @brief Construct with explicit file path and asset name.
         *
         * @param path Path to the audio file.
         * @param name Asset identifier.
         */
        OpenALSoundEffectAsset(std::string path, std::string name);

        /**
         * @brief Destructor.
         *
         * No OpenAL cleanup is required because this class does not
         * own buffers or sources.
         */
        ~OpenALSoundEffectAsset() override = default;

        /**
         * @brief Decode audio data into raw PCM format.
         *
         * Supported formats:
         *  - WAV
         *  - OGG
         *
         * The decoded PCM data is intended to be uploaded into
         * an OpenAL buffer by `OpenALAudioPlayer`.
         *
         * @param outData Output buffer containing raw PCM bytes.
         * @param outFmt  OpenAL format (e.g. `AL_FORMAT_MONO16`).
         * @param outFreq Sampling rate in Hz.
         *
         * @return true if decoding succeeded, false otherwise.
         */
        bool decode(std::vector<char>& outData,
                    ALenum& outFmt,
                    ALsizei& outFreq);

        /**
         * @name Disabled Playback Interface
         *
         * These overrides exist only to satisfy the SoundEffectAsset interface.
         * Playback and resource ownership are intentionally delegated to
         * OpenALAudioPlayer.
         */
        ///@{
        bool load() override { return true; }
        void unload() override {}
        void play(bool loop = false) override {}
        void stop() override {}
        ///@}

    private:
        /**
         * @brief Decode a WAV audio file.
         *
         * @param path   Path to WAV file.
         * @param data   Output PCM buffer.
         * @param format OpenAL format.
         * @param freq   Sample rate.
         */
        bool loadWavFile(const std::string& path,
                         std::vector<char>& data,
                         ALenum& format,
                         ALsizei& freq);

        /**
         * @brief Decode an OGG/Vorbis audio file.
         *
         * @param path   Path to OGG file.
         * @param data   Output PCM buffer.
         * @param format OpenAL format.
         * @param freq   Sample rate.
         */
        bool loadOggFile(const std::string& path,
                         std::vector<char>& data,
                         ALenum& format,
                         ALsizei& freq);
    };

} // namespace retronomicon::opengl::asset
