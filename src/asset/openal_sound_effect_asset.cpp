#include "retronomicon/asset/openal_sound_effect_asset.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <filesystem>

// Enable stb_vorbis implementation (only in ONE .cpp file)
#define STB_VORBIS_IMPLEMENTATION
#include "stb_vorbis.c"

namespace retronomicon::opengl::asset {

    /**************************************************************************
     * Constructors
     **************************************************************************/

    OpenALSoundEffectAsset::OpenALSoundEffectAsset(const std::string& path)
        : OpenALSoundEffectAsset(path, std::filesystem::path(path).filename().string()) {}

    OpenALSoundEffectAsset::OpenALSoundEffectAsset(std::string path, std::string name)
        : SoundEffectAsset(std::move(path), std::move(name)) {}


    /**************************************************************************
     * Decoder Entry Point
     **************************************************************************/

    bool OpenALSoundEffectAsset::decode(
        std::vector<char>& outData,
        ALenum& outFmt,
        ALsizei& outFreq)
    {
        std::string ext = std::filesystem::path(m_path).extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        bool ok = false;

        if (ext == ".wav") {
            ok = loadWavFile(m_path, outData, outFmt, outFreq);
        }
        else if (ext == ".ogg") {
            ok = loadOggFile(m_path, outData, outFmt, outFreq);
        }
        else {
            std::cerr << "[OpenALSoundEffectAsset] Unsupported format: " << ext << "\n";
            return false;
        }

        if (!ok) {
            std::cerr << "[OpenALSoundEffectAsset] Failed to decode " << m_path << "\n";
        }

        return ok;
    }


    /**************************************************************************
     * WAV Loader (very simple PCM/Riff parser)
     **************************************************************************/

    bool OpenALSoundEffectAsset::loadWavFile(
        const std::string& path,
        std::vector<char>& data,
        ALenum& format,
        ALsizei& freq)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file) return false;

        char riff[4];
        file.read(riff, 4);
        if (std::strncmp(riff, "RIFF", 4) != 0) return false;

        // Channels
        file.seekg(22);
        short channels = 0;
        file.read(reinterpret_cast<char*>(&channels), 2);

        // Frequency
        int sampleRate = 0;
        file.read(reinterpret_cast<char*>(&sampleRate), 4);
        freq = static_cast<ALsizei>(sampleRate);

        // Bits per sample
        file.seekg(34);
        short bitsPerSample = 0;
        file.read(reinterpret_cast<char*>(&bitsPerSample), 2);

        // Data chunk
        file.seekg(40);
        char dataHeader[4];
        int dataSize = 0;
        file.read(dataHeader, 4);
        file.read(reinterpret_cast<char*>(&dataSize), 4);

        data.resize(dataSize);
        file.read(data.data(), dataSize);

        // Determine OpenAL format
        if (channels == 1)
            format = (bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
        else
            format = (bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;

        return true;
    }


    /**************************************************************************
     * OGG Loader (using stb_vorbis)
     **************************************************************************/

    bool OpenALSoundEffectAsset::loadOggFile(
        const std::string& path,
        std::vector<char>& data,
        ALenum& format,
        ALsizei& freq)
    {
        int channels = 0;
        int sampleRate = 0;
        short* output = nullptr;

        int samples = stb_vorbis_decode_filename(path.c_str(), &channels, &sampleRate, &output);
        if (samples < 0)
            return false;

        freq = static_cast<ALsizei>(sampleRate);

        if (channels == 1)
            format = AL_FORMAT_MONO16;
        else if (channels == 2)
            format = AL_FORMAT_STEREO16;
        else {
            free(output);
            return false;
        }

        size_t dataSize = samples * channels * sizeof(short);
        data.resize(dataSize);
        std::memcpy(data.data(), output, dataSize);

        free(output);
        return true;
    }

} // namespace retronomicon::opengl::asset
