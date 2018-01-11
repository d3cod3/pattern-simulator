#ifndef ABSTRACTWAVE_H
#define ABSTRACTWAVE_H

#define WAVE_PI         3.1415927f
#define WAVE_TWO_PI     6.2831855f

#include "ofMain.h"

#include "WaveState.h"

class AbstractWave {

public:

    AbstractWave();
    AbstractWave(float paramFloat);
    AbstractWave(float paramFloat1, float paramFloat2);
    AbstractWave(float paramFloat1, float paramFloat2, float paramFloat3, float paramFloat4);

    void customSet(float paramFloat1, float paramFloat2, float paramFloat3, float paramFloat4);

    void setAmp(float amp);
    float getAmp();
    float getValue();
    float getTheta();
    void setTheta(float theta);
    void reset();
    float getFrequency();
    void setFrequency(float freq);

    float cyclePhase(float paramFloat=0);
    void pop();
    void push();
    void setPhase(float paramFloat);

    float update();


protected:

    float _phase;
    float _origPhase;
    float _freq;
    float _amp;
    float _offset;
    float _value;

    stack<WaveState> stateStack;

};

#endif // ABSTRACTWAVE_H
