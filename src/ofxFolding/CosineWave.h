#ifndef COSINEWAVE_H
#define COSINEWAVE_H

#include "AbstractWave.h"

class CosineWave : public AbstractWave{

public:

    CosineWave(float theta, float freq);
    CosineWave(float theta, float freq, float amp, float offset);

    float update();

};

#endif // COSINEWAVE_H
