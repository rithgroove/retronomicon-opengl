#include "retronomicon/asset/openal_music_asset.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace retronomicon::asset {

    /***************************** Constructor *****************************/

    OpenALMusicAsset::OpenALMusicAsset(const std::string& path)
        : OpenALMusicAsset(path, std::filesystem::path(path).filename().string())
    {
        // delegates to the two-parameter constructor
    }

    OpenALMusicAsset::OpenALMusicAsset(const std::string& path, const std::string& name)
        : MusicAsset(path, name) {
    }

    /***************************** Destructor *****************************/

    OpenALMusicAsset::~OpenALMusicAsset() {
        unload();
    }

    /***************************** Load *****************************/

    bool OpenALMusicAsset::load() {
        unload(); // Clean up any previous state

        alGenBuffers(1, &m_buffer);
        alGenSources(1, &m_source);

        // For simplicity, assume PCM data loaded from .wav (you’ll likely replace with stb_vorbis or dr_wav)
        std::ifstream file(m_path, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to open music file: " << m_path << "\n";
            return false;
        }

        m_data.assign(std::istreambuf_iterator<char>(file), {});
        file.close();

        // In real implementation, decode WAV or OGG data here and call alBufferData()
        // Example (pseudo):
        // alBufferData(m_buffer, AL_FORMAT_STEREO16, decodedData, dataSize, sampleRate);

        alSourcei(m_source, AL_BUFFER, m_buffer);
        alSourcef(m_source, AL_GAIN, 1.0f);

        return true;
    }

    /***************************** Unload *****************************/

    void OpenALMusicAsset::unload() {
        if (m_source) {
            alDeleteSources(1, &m_source);
            m_source = 0;
        }
        if (m_buffer) {
            alDeleteBuffers(1, &m_buffer);
            m_buffer = 0;
        }
        m_data.clear();
        m_playing = false;
    }

    /***************************** Play *****************************/

    void OpenALMusicAsset::play(bool loop) {
        if (!m_source)
            return;

        m_loop = loop;
        alSourcei(m_source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcePlay(m_source);
        m_playing = true;
    }

    /***************************** Stop *****************************/

    void OpenALMusicAsset::stop() {
        if (m_source)
            alSourceStop(m_source);
        m_playing = false;
    }

    /***************************** To String *****************************/

    std::string OpenALMusicAsset::to_string() const {
        return "[OpenALMusicAsset]\n-name=" + m_name +
               "\n-path=" + m_path +
               "\n-playing=" + (m_playing ? "true" : "false") +
               "\n-loop=" + (m_loop ? "true" : "false") + "\n";
    }

} // namespace retronomicon::asset
