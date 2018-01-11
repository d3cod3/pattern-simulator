#ifndef AMFMSINEWAVE_H
#define AMFMSINEWAVE_H

#include "AbstractWave.h"

class AMFMSineWave : public AbstractWave {

public:
    AMFMSineWave(float theta, float freq, AbstractWave fmod, AbstractWave amod);
    AMFMSineWave(float theta, float freq, float offset, AbstractWave fmod, AbstractWave amod);

    void customSet(float paramFloat1, float paramFloat2, float paramFloat3, AbstractWave fmod, AbstractWave amod);
    float update();
    void setFMod(AbstractWave fmod);
    void setAMod(AbstractWave amod);
    AbstractWave getFMod();
    AbstractWave getAMod();

    void pop();
    void push();
    void reset();

private:
    AbstractWave _fmod;
    AbstractWave _amod;

};

#endif // AMFMSINEWAVE_H
