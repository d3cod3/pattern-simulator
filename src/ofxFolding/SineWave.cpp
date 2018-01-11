#include "SineWave.h"

//--------------------------------------------------------------
SineWave::SineWave(float theta, float freq) : AbstractWave(theta, freq){

}

//--------------------------------------------------------------
SineWave::SineWave(float theta, float freq, float amp, float offset) : AbstractWave(theta, freq, amp, offset){

}

//--------------------------------------------------------------
float SineWave::update(){
    _value = (static_cast<float> (sin(_phase) * _amp) + _offset);
    cyclePhase(_freq);
    return _value;
}
