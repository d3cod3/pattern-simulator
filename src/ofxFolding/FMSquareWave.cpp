#include "FMSquareWave.h"

//--------------------------------------------------------------
FMSquareWave::FMSquareWave(float theta, float freq, AbstractWave fmod) : AbstractWave(theta, freq), _fmod(fmod){

}

//--------------------------------------------------------------
FMSquareWave::FMSquareWave(float theta, float freq, float amp, float offset, ConstantWave fmod) : AbstractWave(theta, freq, amp, offset), _fmod(fmod){

}

//--------------------------------------------------------------
FMSquareWave::FMSquareWave(float theta, float freq, float amp, float offset, AbstractWave fmod) : AbstractWave(theta, freq, amp, offset), _fmod(fmod){

}

//--------------------------------------------------------------
void FMSquareWave::pop(){
    AbstractWave::pop();
    _fmod.pop();
}

//--------------------------------------------------------------
void FMSquareWave::push(){
    AbstractWave::push();
    _fmod.push();
}

//--------------------------------------------------------------
void FMSquareWave::reset(){
    AbstractWave::reset();
    _fmod.reset();
}

//--------------------------------------------------------------
float FMSquareWave::update(){
    _value = ((_phase / WAVE_TWO_PI < 0.5f ? 1 : -1) * _amp + _offset);
    cyclePhase(_freq + _fmod.update());
    return _value;
}
