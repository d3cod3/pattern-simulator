#ifndef CONSTANTWAVE_H
#define CONSTANTWAVE_H

#include "AbstractWave.h"

class ConstantWave : public AbstractWave {

public:

    ConstantWave(float paramFloat);

    float update();

};

#endif // CONSTANTWAVE_H
