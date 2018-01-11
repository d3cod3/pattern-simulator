#include "AbstractWave.h"

//--------------------------------------------------------------
AbstractWave::AbstractWave(){
    setPhase(0.0f);
    _freq = 0.0f;
    _amp = 1.0f;
    _offset = 0.0f;
}

//--------------------------------------------------------------
AbstractWave::AbstractWave(float paramFloat){
    setPhase(paramFloat);
    _freq = 0.0f;
    _amp = 1.0f;
    _offset = 0.0f;
}

//--------------------------------------------------------------
AbstractWave::AbstractWave(float paramFloat1, float paramFloat2){
    setPhase(paramFloat1);
    _freq = paramFloat2;
    _amp = 1.0f;
    _offset = 0.0f;
}

//--------------------------------------------------------------
AbstractWave::AbstractWave(float paramFloat1, float paramFloat2, float paramFloat3, float paramFloat4){
    setPhase(paramFloat1);
    _freq = paramFloat2;
    _amp = paramFloat3;
    _offset = paramFloat4;
}

//--------------------------------------------------------------
void AbstractWave::customSet(float paramFloat1, float paramFloat2, float paramFloat3, float paramFloat4){
    setPhase(paramFloat1);
    _freq = paramFloat2;
    _amp = paramFloat3;
    _offset = paramFloat4;
}

//--------------------------------------------------------------
void AbstractWave::setAmp(float amp){
    _amp = amp;
}

//--------------------------------------------------------------
float AbstractWave::getAmp(){
    return _amp;
}

//--------------------------------------------------------------
float AbstractWave::getValue(){
    return _value;
}

//--------------------------------------------------------------
float AbstractWave::getTheta(){
    return _phase;
}

//--------------------------------------------------------------
void AbstractWave::setTheta(float theta){
    _phase = theta;
    _origPhase = theta;
}

//--------------------------------------------------------------
void AbstractWave::reset(){
    _phase = _origPhase;
}

//--------------------------------------------------------------
float AbstractWave::getFrequency(){
    return _freq;
}

//--------------------------------------------------------------
void AbstractWave::setFrequency(float freq){
    _freq = freq;
}

//--------------------------------------------------------------
float AbstractWave::cyclePhase(float paramFloat){
    _phase = fmod((_phase + paramFloat),WAVE_TWO_PI);
    if (_phase < 0.0f) {
        _phase += WAVE_TWO_PI;
    }
    return _phase;
}

//--------------------------------------------------------------
void AbstractWave::pop(){
    if(!stateStack.empty()){
        WaveState localWaveState = stateStack.top();
        _phase = localWaveState.phase;
        _freq = localWaveState.frequency;
        _amp = localWaveState.amp;
        _offset = localWaveState.offset;

        stateStack.pop();
    }

}

//--------------------------------------------------------------
void AbstractWave::push(){
    WaveState localWaveState(_phase, _freq, _amp, _offset);
    stateStack.push(localWaveState);
}

//--------------------------------------------------------------
void AbstractWave::setPhase(float paramFloat){
    _phase = paramFloat;
    cyclePhase();
    _origPhase = paramFloat;
}

//--------------------------------------------------------------
float AbstractWave::update(){
    return _value;
}
