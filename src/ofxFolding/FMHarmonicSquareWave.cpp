#include "FMHarmonicSquareWave.h"

//--------------------------------------------------------------
FMHarmonicSquareWave::FMHarmonicSquareWave(float theta, float freq, AbstractWave fmod) : AbstractWave(theta, freq), _fmod(fmod){
    maxHarmonics = 3;
}

//--------------------------------------------------------------
FMHarmonicSquareWave::FMHarmonicSquareWave(float theta, float freq, float amp, float offset, ConstantWave fmod) : AbstractWave(theta, freq, amp, offset), _fmod(fmod){
    maxHarmonics = 3;
}

//--------------------------------------------------------------
FMHarmonicSquareWave::FMHarmonicSquareWave(float theta, float freq, float amp, float offset, AbstractWave fmod) : AbstractWave(theta, freq, amp, offset), _fmod(fmod){
    maxHarmonics = 3;
}

//--------------------------------------------------------------
void FMHarmonicSquareWave::pop(){
    AbstractWave::pop();
    _fmod.pop();
}

//--------------------------------------------------------------
void FMHarmonicSquareWave::push(){
    AbstractWave::push();
    _fmod.push();
}

//--------------------------------------------------------------
void FMHarmonicSquareWave::reset(){
    AbstractWave::reset();
    _fmod.reset();
}

//--------------------------------------------------------------
float FMHarmonicSquareWave::update(){
    _value = 0.0f;
    for (unsigned int i = 1; i <= maxHarmonics; i += 2) {
        _value = (static_cast<float>(_value + 1.0f / i * static_cast<float>(sin(i * _phase))));
    }
    _value *= _amp;
    _value += _offset;
    cyclePhase(_freq + _fmod.update());
    return _value;
}
