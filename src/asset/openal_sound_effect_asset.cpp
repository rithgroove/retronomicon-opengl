#include "retronomicon/asset/openal_sound_effect_asset.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cstring>

namespace retronomicon::asset {

    OpenALSoundEffectAsset::OpenALSoundEffectAsset(const std::string& path)
        : OpenALSoundEffectAsset(path, std::filesystem::path(path).filename().string())
    {
        // delegates to the two-parameter constructor
    }
    OpenALSoundEffectAsset::OpenALSoundEffectAsset(std::string path, std::string name)
        : SoundEffectAsset(std::move(path), std::move(name)) {}

    OpenALSoundEffectAsset::~OpenALSoundEffectAsset() {
        unload();
    }

    bool OpenALSoundEffectAsset::load() {
        if (m_buffer != 0)
            unload();

        alGenBuffers(1, &m_buffer);
        alGenSources(1, &m_source);

        std::vector<char> data;
        ALenum format;
        ALsizei freq;

        if (!loadWavFile(m_path, data, format, freq)) {
            std::cerr << "[OpenALSoundEffectAsset] Failed to load WAV: " << m_path << std::endl;
            return false;
        }

        alBufferData(m_buffer, format, data.data(), static_cast<ALsizei>(data.size()), freq);
        alSourcei(m_source, AL_BUFFER, m_buffer);
        alSourcef(m_source, AL_GAIN, m_volume);

        return true;
    }

    void OpenALSoundEffectAsset::unload() {
        if (m_source) {
            alDeleteSources(1, &m_source);
            m_source = 0;
        }
        if (m_buffer) {
            alDeleteBuffers(1, &m_buffer);
            m_buffer = 0;
        }
        m_playing = false;
    }

    void OpenALSoundEffectAsset::play(bool loop) {
        if (!m_source) return;

        alSourcei(m_source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcePlay(m_source);
        m_playing = true;
    }

    void OpenALSoundEffectAsset::stop() {
        if (!m_source) return;

        alSourceStop(m_source);
        m_playing = false;
    }

    bool OpenALSoundEffectAsset::loadWavFile(const std::string& path, std::vector<char>& data, ALenum& format, ALsizei& freq) {
        std::ifstream file(path, std::ios::binary);
        if (!file) return false;

        char riff[4];
        file.read(riff, 4);
        if (std::strncmp(riff, "RIFF", 4) != 0) return false;

        file.seekg(22);
        short channels;
        file.read(reinterpret_cast<char*>(&channels), 2);

        int sampleRate;
        file.read(reinterpret_cast<char*>(&sampleRate), 4);
        freq = sampleRate;

        file.seekg(34);
        short bitsPerSample;
        file.read(reinterpret_cast<char*>(&bitsPerSample), 2);

        char dataHeader[4];
        int dataSize;
        file.seekg(40);
        file.read(dataHeader, 4);
        file.read(reinterpret_cast<char*>(&dataSize), 4);

        data.resize(dataSize);
        file.read(data.data(), dataSize);

        if (channels == 1)
            format = (bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
        else
            format = (bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;

        return true;
    }

} // namespace retronomicon::asset
