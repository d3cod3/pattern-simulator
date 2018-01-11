#ifndef SINEWAVE_H
#define SINEWAVE_H

#include "AbstractWave.h"


class SineWave : public AbstractWave {

public:

    SineWave(float theta, float freq);
    SineWave(float theta, float freq, float amp, float offset);

    float update();

};

#endif // SINEWAVE_H
