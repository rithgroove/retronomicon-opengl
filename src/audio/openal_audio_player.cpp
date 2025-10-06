#include "retronomicon/opengl/audio/openal_audio_player.h"
#include <sndfile.h>
#include <iostream>

using namespace retronomicon::opengl::audio;

OpenALAudioPlayer::OpenALAudioPlayer() = default;

OpenALAudioPlayer::~OpenALAudioPlayer() {
    shutdown();
}

bool OpenALAudioPlayer::init() {
    device = alcOpenDevice(nullptr);
    if (!device) {
        std::cerr << "[OpenAL] Failed to open audio device\n";
        return false;
    }

    context = alcCreateContext(device, nullptr);
    if (!context || !alcMakeContextCurrent(context)) {
        std::cerr << "[OpenAL] Failed to create or set context\n";
        return false;
    }

    initialized = true;
    std::cout << "[OpenAL] Initialized successfully\n";
    return true;
}

void OpenALAudioPlayer::shutdown() {
    if (!initialized) return;

    stopAllSounds();
    clearCache();

    if (context) {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        context = nullptr;
    }

    if (device) {
        alcCloseDevice(device);
        device = nullptr;
    }

    initialized = false;
    std::cout << "[OpenAL] Shutdown complete\n";
}

void OpenALAudioPlayer::playMusic(const std::string& path, bool loop) {
    stopMusic();

    if (!loadWavFile(path, musicData.buffer)) {
        std::cerr << "[OpenAL] Failed to load music: " << path << "\n";
        return;
    }

    alGenSources(1, &musicData.source);
    alSourcei(musicData.source, AL_BUFFER, musicData.buffer);
    alSourcei(musicData.source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    alSourcePlay(musicData.source);

    musicPlaying = true;
}

void OpenALAudioPlayer::stopMusic() {
    if (!musicPlaying) return;
    alSourceStop(musicData.source);
    alDeleteSources(1, &musicData.source);
    alDeleteBuffers(1, &musicData.buffer);
    musicData = {};
    musicPlaying = false;
}

void OpenALAudioPlayer::setMusicPaused(bool paused) {
    if (!musicPlaying) return;
    if (paused)
        alSourcePause(musicData.source);
    else
        alSourcePlay(musicData.source);
}

void OpenALAudioPlayer::loadSound(const std::string& name, const std::string& path) {
    if (soundCache.find(name) != soundCache.end())
        return;

    ALuint buffer;
    if (!loadWavFile(path, buffer)) {
        std::cerr << "[OpenAL] Failed to load sound: " << path << "\n";
        return;
    }

    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    soundCache[name] = { buffer, source };
}

void OpenALAudioPlayer::playSound(const std::string& name) {
    auto it = soundCache.find(name);
    if (it == soundCache.end()) return;

    ALint state;
    alGetSourcei(it->second.source, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING)
        alSourceRewind(it->second.source);

    alSourcePlay(it->second.source);
}

void OpenALAudioPlayer::stopAllSounds() {
    for (auto& [name, snd] : soundCache)
        alSourceStop(snd.source);
    stopMusic();
}

void OpenALAudioPlayer::clearCache() {
    for (auto& [name, snd] : soundCache) {
        alDeleteSources(1, &snd.source);
        alDeleteBuffers(1, &snd.buffer);
    }
    soundCache.clear();
}

bool OpenALAudioPlayer::loadWavFile(const std::string& path, ALuint& outBuffer) {
    SF_INFO sfInfo;
    SNDFILE* sndFile = sf_open(path.c_str(), SFM_READ, &sfInfo);
    if (!sndFile) {
        std::cerr << "[OpenAL] Failed to open WAV file: " << path << "\n";
        return false;
    }

    std::vector<short> samples(sfInfo.frames * sfInfo.channels);
    sf_read_short(sndFile, samples.data(), static_cast<sf_count_t>(samples.size()));
    sf_close(sndFile);

    ALenum format = AL_NONE;
    if (sfInfo.channels == 1)
        format = AL_FORMAT_MONO16;
    else if (sfInfo.channels == 2)
        format = AL_FORMAT_STEREO16;
    else {
        std::cerr << "[OpenAL] Unsupported channel count: " << sfInfo.channels << "\n";
        return false;
    }

    alGenBuffers(1, &outBuffer);
    alBufferData(outBuffer, format, samples.data(),
                 static_cast<ALsizei>(samples.size() * sizeof(short)),
                 sfInfo.samplerate);

    return true;
}
