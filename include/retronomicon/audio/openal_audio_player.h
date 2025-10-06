#pragma once

#include "retronomicon/audio/IAudioPlayer.h"
#include <string>
#include <unordered_map>
#include <AL/al.h>
#include <AL/alc.h>

namespace retronomicon::opengl::audio {

    /**
     * @brief OpenAL implementation of IAudioPlayer
     */
    class OpenALAudioPlayer : public retronomicon::audio::IAudioPlayer {
    public:
        OpenALAudioPlayer();
        ~OpenALAudioPlayer() override;

        bool init() override;
        void shutdown() override;

        void playMusic(const std::string& path, bool loop = true) override;
        void stopMusic() override;
        void setMusicPaused(bool paused) override;

        void loadSound(const std::string& name, const std::string& path) override;
        void playSound(const std::string& name) override;
        void stopAllSounds() override;

    private:
        struct SoundData {
            ALuint buffer = 0;
            ALuint source = 0;
        };

        ALCdevice* device = nullptr;
        ALCcontext* context = nullptr;

        std::unordered_map<std::string, SoundData> soundCache;
        SoundData musicData;
        bool musicPlaying = false;
        bool initialized = false;

        bool loadWavFile(const std::string& path, ALuint& outBuffer);
        void clearCache();
    };

} // namespace retronomicon::opengl::audio
