#include "AudioOutput.h"

AudioOutput::AudioOutput(unsigned int sampleRate, unsigned int bufferFrames)
    : sampleRate(sampleRate), bufferFrames(bufferFrames), bufferPosition(0), isPlaying(false) {
        RtAudio::StreamParameters parameters;
        parameters.deviceId = dac.getDefaultOutputDevice();
        parameters.nChannels = 1;
        parameters.firstChannel = 0;

        unsigned int sampleRate = this->sampleRate;
        unsigned int bufferFrames = this->bufferFrames;

        dac.openStream(&parameters, nullptr, RTAUDIO_FLOAT64, sampleRate, &bufferFrames, &AudioOutput::audioCallback, this);
}

AudioOutput::~AudioOutput() {
    if (dac.isStreamOpen()) {
        dac.closeStream();
    }
}

void AudioOutput::play(const std::vector<double>& data) {
    buffer = data;
    bufferPosition = 0;
    isPlaying = true;
    dac.startStream();
}

void AudioOutput::stop() {
    if (dac.isStreamRunning()) {
        dac.stopStream();
    }
    isPlaying = false;
}

int AudioOutput::audioCallback(void* outputBuffer, void* /*inputBuffer*/, unsigned int nBufferFrames,
    double /*streamTime*/, RtAudioStreamStatus /*status*/, void* UserData) {
        AudioOutput* audio = static_cast<AudioOutput*>(UserData);
        double* buffer = static_cast<double*>(outputBuffer);

        for (unsigned int i = 0; i < nBufferFrames; ++i) {
            if (audio->bufferPosition < audio->buffer.size()) {
                buffer[i] = audio->buffer[audio->bufferPosition++];
            } else {
                buffer[i] = 0.0;
                audio->isPlaying = false;
                return 1;
            }
        }
    return 0;
}