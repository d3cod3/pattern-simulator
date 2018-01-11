#ifndef FMSQUAREWAVE_H
#define FMSQUAREWAVE_H


#include "AbstractWave.h"
#include "ConstantWave.h"

class FMSquareWave : public AbstractWave {

public:
    FMSquareWave(float theta, float freq, AbstractWave fmod);
    FMSquareWave(float theta, float freq, float amp, float offset, ConstantWave fmod);
    FMSquareWave(float theta, float freq, float amp, float offset, AbstractWave fmod);

    void pop();
    void push();
    void reset();
    float update();

private:
    AbstractWave _fmod;

};

#endif // FMSQUAREWAVE_H
