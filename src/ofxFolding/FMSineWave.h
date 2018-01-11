#ifndef FMSINEWAVE_H
#define FMSINEWAVE_H

#include "AbstractWave.h"
#include "ConstantWave.h"

class FMSineWave : public AbstractWave {

public:
    FMSineWave(float theta, float freq, AbstractWave fmod);
    FMSineWave(float theta, float freq, float amp, float offset, ConstantWave fmod);
    FMSineWave(float theta, float freq, float amp, float offset, AbstractWave fmod);

    void pop();
    void push();
    void reset();
    float update();

private:
    AbstractWave _fmod;

};

#endif // FMSINEWAVE_H
