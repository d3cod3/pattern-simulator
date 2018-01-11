#include "ConstantWave.h"

//--------------------------------------------------------------
ConstantWave::ConstantWave(float paramFloat) : AbstractWave(){

    _value = paramFloat;

}

//--------------------------------------------------------------
float ConstantWave::update(){
    return _value;
}
