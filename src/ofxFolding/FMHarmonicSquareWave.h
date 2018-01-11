#ifndef FMHARMONICSQUAREWAVE_H
#define FMHARMONICSQUAREWAVE_H


#include "AbstractWave.h"
#include "ConstantWave.h"

class FMHarmonicSquareWave : public AbstractWave {

public:
    FMHarmonicSquareWave(float theta, float freq, AbstractWave fmod);
    FMHarmonicSquareWave(float theta, float freq, float amp, float offset, ConstantWave fmod);
    FMHarmonicSquareWave(float theta, float freq, float amp, float offset, AbstractWave fmod);

    void pop();
    void push();
    void reset();
    float update();

private:
    AbstractWave _fmod;
    unsigned int maxHarmonics;

};

#endif // FMHARMONICSQUAREWAVE_H
