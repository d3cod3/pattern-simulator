#include "FMSawtoothWave.h"

//--------------------------------------------------------------
FMSawtoothWave::FMSawtoothWave(float theta, float freq, AbstractWave fmod) : AbstractWave(theta, freq), _fmod(fmod){

}

//--------------------------------------------------------------
FMSawtoothWave::FMSawtoothWave(float theta, float freq, float amp, float offset, ConstantWave fmod) : AbstractWave(theta, freq, amp, offset), _fmod(fmod){

}

//--------------------------------------------------------------
FMSawtoothWave::FMSawtoothWave(float theta, float freq, float amp, float offset, AbstractWave fmod) : AbstractWave(theta, freq, amp, offset), _fmod(fmod){

}

//--------------------------------------------------------------
void FMSawtoothWave::pop(){
    AbstractWave::pop();
    _fmod.pop();
}

//--------------------------------------------------------------
void FMSawtoothWave::push(){
    AbstractWave::push();
    _fmod.push();
}

//--------------------------------------------------------------
void FMSawtoothWave::reset(){
    AbstractWave::reset();
    _fmod.reset();
}

//--------------------------------------------------------------
float FMSawtoothWave::update(){
    _value = ((_phase / WAVE_TWO_PI * 2.0f - 1.0f) * _amp + _offset);
    cyclePhase(_freq + _fmod.update());
    return _value;
}

