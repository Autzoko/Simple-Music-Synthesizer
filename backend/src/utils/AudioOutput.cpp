#include "AudioOutput.h"
#include <stdexcept>
#include <iostream>

AudioOutput::AudioOutput(Synthesizer& synth, unsigned int sampleRate, unsigned int bufferSize)
    : synthesizer(synth), sampleRate(sampleRate), bufferSize(bufferSize) {
        PaError err = Pa_Initialize();

        if (err != paNoError) {
            std::cerr << "Port Audio Initializatio Failed" << std::endl;
            throw std::runtime_error("Failed to initialized portaudio: " + std::string(Pa_GetErrorText(err)));
        }

        PaStreamParameters outputParameters;
        outputParameters.device = Pa_GetDefaultOutputDevice();
        if (outputParameters.device == paNoDevice) {
            std::cerr << "No default output device available." << std::endl;
            throw std::runtime_error("No default output device available.");
        }
        outputParameters.channelCount = 1;
        outputParameters.sampleFormat = paFloat32;
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = nullptr;

        err = Pa_OpenStream(
            &stream,
            nullptr,
            &outputParameters,
            sampleRate,
            bufferSize,
            paClipOff,
            audioCallback,
            this
        );

        if (err != paNoError) {
            Pa_Terminate();
            std::cerr << "Failed to open audio stream: " << Pa_GetErrorText(err) << std::endl;
            throw std::runtime_error("Failed to open stream: " + std::string(Pa_GetErrorText(err)));
        }

}

AudioOutput::~AudioOutput() {
    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }
    Pa_Terminate();
}

void AudioOutput::start() {
    PaError err = Pa_StartStream(stream);
    if (err != paNoError) {
        throw std::runtime_error("Failed to start audio stream: " + std::string(Pa_GetErrorText(err)));
    }
}

void AudioOutput::stop() {
    if (stream) {
        Pa_StopStream(stream);
    }
}

int AudioOutput::audioCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
    void* userData) {
        auto* audioOutput = static_cast<AudioOutput*>(userData);
        auto* buffer = static_cast<float*>(outputBuffer);

        std::vector<double> audioData(framesPerBuffer);

        try {
            audioOutput->synthesizer.generateAudio(audioData, framesPerBuffer);

            for (unsigned int i = 0; i < framesPerBuffer; ++i) {
                buffer[i] = static_cast<float>(audioData[i]);
            }

            return paContinue;
        } catch (const std::exception& e) {
            std::cerr << "Audio callback exception: " << e.what() << std::endl;
            return paAbort;
        }
}