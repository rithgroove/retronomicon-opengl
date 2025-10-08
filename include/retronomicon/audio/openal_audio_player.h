#pragma once

#include "retronomicon/audio/i_audio_player.h"
#include "retronomicon/asset/openal_sound_effect_asset.h"
#include "retronomicon/asset/openal_music_asset.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <unordered_map>
#include <string>
#include <memory>

namespace retronomicon::audio {

    using retronomicon::asset::OpenALMusicAsset;
    using retronomicon::asset::OpenALSoundEffectAsset;
    class OpenALAudioPlayer final : public IAudioPlayer {
    public:
        OpenALAudioPlayer();
        ~OpenALAudioPlayer() override;

        bool init() override;
        void shutdown() override;
        void update() override;

        /************** Global **************/
        void setMasterVolume(float volume) override;
        float getMasterVolume() const override;

        /************** Music **************/
        void playMusic(const std::string& path, bool loop = true, int fadeInMs = 0) override;
        void stopMusic(int fadeOutMs = 0) override;
        void setMusicPaused(bool paused) override;
        bool isMusicPlaying() const override;
        void setMusicVolume(float volume) override;
        float getMusicVolume() const override;

        /************** Sound Effects **************/
        bool loadSoundEffect(const std::string& name, const std::string& path) override;
        void playSoundEffect(const std::string& name, float volume = 1.0f, bool loop = false) override;
        void stopSoundEffect(const std::string& name = "") override;
        void setSfxVolume(float volume) override;
        float getSfxVolume() const override;
        void unloadSoundEffect(const std::string& name) override;
        void clearSoundCache() override;

        /************** 3D Audio (Optional) **************/
        void setListenerPosition(const Vec3& pos) override;
        void playSoundEffect3D(const std::string& name, const Vec3& pos, float volume = 1.0f, bool loop = false) override;

    private:
        // OpenAL handles
        ALCdevice* m_device = nullptr;
        ALCcontext* m_context = nullptr;

        // Cached buffers and sources for SFX
        struct SoundInstance {
            ALuint buffer = 0;
            ALuint source = 0;
        };
        std::unordered_map<std::string, SoundInstance> m_sfxCache;

        // Music
        std::unique_ptr<OpenALMusicAsset> m_currentMusic;
        ALuint m_musicSource = 0;

        // Volume controls
        float m_masterVolume = 1.0f;
        float m_musicVolume = 1.0f;
        float m_sfxVolume = 1.0f;

        // Internal helpers
        void applyGlobalVolume();
        bool checkALError(const std::string& context);
    };

} // namespace retronomicon::audio
