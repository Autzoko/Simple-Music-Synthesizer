# Simple Music Synthesizer

This is a simple music synthesizer project.

## Libraries

- Qt6: to support GUI.
- RtMIDI: to support MIDI input.
- PortAudio: to support sound output.

## Basic Structure

- Oscillators: contains 4 basic oscillator, genrate 5 different types of waveform, can adjust oscillator weight and detune.
- Filter: contains 3 basic filters(low-pass, high-pass, band-pass), can adjust cutoff and resonance.
- Envelope Generator: generate basic linear ADSR envelope, each stage value can be adjusted.
- Effects: contains 7 basic effects.