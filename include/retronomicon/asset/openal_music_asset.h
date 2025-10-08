#pragma once

#include "retronomicon/asset/music_asset.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <vector>

namespace retronomicon::asset {

    /**
     * @brief Implementation of MusicAsset using OpenAL.
     * 
     * Handles loading, playing, stopping, and managing long music tracks.
     * Typically used for background music.
     */
    class OpenALMusicAsset final : public MusicAsset {
    private:
        /** @brief OpenAL buffer ID. */
        ALuint m_buffer{0};

        /** @brief OpenAL source ID. */
        ALuint m_source{0};

        /** @brief Whether the music is currently playing. */
        bool m_playing{false};

        /** @brief Whether the music should loop when played. */
        bool m_loop{false};

        /** @brief Raw audio data loaded into memory. */
        std::vector<char> m_data;

    public:

        /**
         * @brief Construct an OpenALMusicAsset.
         *
         * @param path Path to the audio file.
        */
        OpenALMusicAsset(const std::string& path);
        
        /**
         * @brief Construct an OpenALMusicAsset.
         * @param path Path to the audio file.
         * @param name Name of the asset.
         */
        OpenALMusicAsset(const std::string& path, const std::string& name);

        /** @brief Destructor — cleans up OpenAL buffers and sources. */
        ~OpenALMusicAsset() override;

        /** @copydoc MusicAsset::load() */
        bool load() override;

        /** @copydoc MusicAsset::unload() */
        void unload() override;

        /** @copydoc MusicAsset::play() */
        void play(bool loop = true) override;

        /** @copydoc MusicAsset::stop() */
        void stop() override;

        /** @copydoc MusicAsset::to_string() */
        std::string to_string() const override;

        /***************************** Inline Setters & Getters *****************************/

        /** @brief Returns whether the music is currently playing. */
        inline bool is_playing() const noexcept { return m_playing; }

        /** @brief Returns whether the music is set to loop. */
        inline bool is_looping() const noexcept { return m_loop; }

        /** @brief Sets looping mode. */
        inline void set_loop(bool loop) noexcept { m_loop = loop; }

        /** @brief Returns the OpenAL source ID. */
        inline ALuint getSource() const noexcept { return m_source; }

        /** @brief Returns the OpenAL buffer ID. */
        inline ALuint getBuffer() const noexcept { return m_buffer; }
    };

} // namespace retronomicon::asset
