#ifndef WAVESTATE_H
#define WAVESTATE_H


class WaveState {

public:

    WaveState(float paramFloat1, float paramFloat2, float paramFloat3, float paramFloat4);

    float phase;
    float frequency;
    float amp;
    float offset;

};

#endif // WAVESTATE_H
