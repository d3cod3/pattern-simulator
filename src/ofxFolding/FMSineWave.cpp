#include "FMSineWave.h"

//--------------------------------------------------------------
FMSineWave::FMSineWave(float theta, float freq, AbstractWave fmod) : AbstractWave(theta, freq), _fmod(fmod){

}

//--------------------------------------------------------------
FMSineWave::FMSineWave(float theta, float freq, float amp, float offset, ConstantWave fmod) : AbstractWave(theta, freq, amp, offset), _fmod(fmod){

}

//--------------------------------------------------------------
FMSineWave::FMSineWave(float theta, float freq, float amp, float offset, AbstractWave fmod) : AbstractWave(theta, freq, amp, offset), _fmod(fmod){

}

//--------------------------------------------------------------
void FMSineWave::pop(){
    AbstractWave::pop();
    _fmod.pop();
}

//--------------------------------------------------------------
void FMSineWave::push(){
    AbstractWave::push();
    _fmod.push();
}

//--------------------------------------------------------------
void FMSineWave::reset(){
    AbstractWave::reset();
    _fmod.reset();
}

//--------------------------------------------------------------
float FMSineWave::update(){
    _value = (static_cast<float>(sin(_phase) * _amp) + _offset);
    cyclePhase(_freq + _fmod.update());
    return _value;
}
