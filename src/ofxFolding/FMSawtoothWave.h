#ifndef FMSAWTOOTHWAVE_H
#define FMSAWTOOTHWAVE_H


#include "AbstractWave.h"
#include "ConstantWave.h"

class FMSawtoothWave : public AbstractWave {

public:
    FMSawtoothWave(float theta, float freq, AbstractWave fmod);
    FMSawtoothWave(float theta, float freq, float amp, float offset, ConstantWave fmod);
    FMSawtoothWave(float theta, float freq, float amp, float offset, AbstractWave fmod);

    void pop();
    void push();
    void reset();
    float update();

private:
    AbstractWave _fmod;

};

#endif // FMSAWTOOTHWAVE_H
