#include "CosineWave.h"

//--------------------------------------------------------------
CosineWave::CosineWave(float theta, float freq) : AbstractWave(theta, freq){

}

//--------------------------------------------------------------
CosineWave::CosineWave(float theta, float freq, float amp, float offset) : AbstractWave(theta, freq, amp, offset){

}

//--------------------------------------------------------------
float CosineWave::update(){
    _value = (static_cast<float> (cos(_phase) * _amp) + _offset);
    cyclePhase(_freq);
    return _value;
}
