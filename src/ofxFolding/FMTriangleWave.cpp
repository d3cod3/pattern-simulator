#include "FMTriangleWave.h"

//--------------------------------------------------------------
FMTriangleWave::FMTriangleWave(float theta, float freq, AbstractWave fmod) : AbstractWave(theta, freq), _fmod(fmod){

}

//--------------------------------------------------------------
FMTriangleWave::FMTriangleWave(float theta, float freq, float amp, float offset, ConstantWave fmod) : AbstractWave(theta, freq, amp, offset), _fmod(fmod){

}

//--------------------------------------------------------------
FMTriangleWave::FMTriangleWave(float theta, float freq, float amp, float offset, AbstractWave fmod) : AbstractWave(theta, freq, amp, offset), _fmod(fmod){

}

//--------------------------------------------------------------
void FMTriangleWave::pop(){
    AbstractWave::pop();
    _fmod.pop();
}

//--------------------------------------------------------------
void FMTriangleWave::push(){
    AbstractWave::push();
    _fmod.push();
}

//--------------------------------------------------------------
void FMTriangleWave::reset(){
    AbstractWave::reset();
    _fmod.reset();
}

//--------------------------------------------------------------
float FMTriangleWave::update(){
    _value = (2.0f * _amp * (abs(WAVE_PI - _phase) * 0.31830987F - 0.5F) + _offset);
    cyclePhase(_freq + _fmod.update());
    return _value;
}
