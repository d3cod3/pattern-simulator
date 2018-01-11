#ifndef FMTRIANGLEWAVE_H
#define FMTRIANGLEWAVE_H

#include "AbstractWave.h"
#include "ConstantWave.h"

class FMTriangleWave : public AbstractWave {

public:
    FMTriangleWave(float theta, float freq, AbstractWave fmod);
    FMTriangleWave(float theta, float freq, float amp, float offset, ConstantWave fmod);
    FMTriangleWave(float theta, float freq, float amp, float offset, AbstractWave fmod);

    void pop();
    void push();
    void reset();
    float update();

private:
    AbstractWave _fmod;

};

#endif // FMTRIANGLEWAVE_H
