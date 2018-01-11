#include "AMFMSineWave.h"

//--------------------------------------------------------------
AMFMSineWave::AMFMSineWave(float theta, float freq, AbstractWave fmod, AbstractWave amod) : AbstractWave(theta, freq), _fmod(fmod), _amod(amod){

}

//--------------------------------------------------------------
AMFMSineWave::AMFMSineWave(float theta, float freq,float offset, AbstractWave fmod, AbstractWave amod) : AbstractWave(theta, freq, 1.0f, offset), _fmod(fmod), _amod(amod){

}

//--------------------------------------------------------------
void AMFMSineWave::customSet(float paramFloat1, float paramFloat2, float paramFloat3, AbstractWave fmod, AbstractWave amod){
    setPhase(paramFloat1);
    _freq = paramFloat2;
    _amp = 1.0f;
    _offset = paramFloat3;

    _fmod = fmod;
    _amod = amod;
}

//--------------------------------------------------------------
float AMFMSineWave::update(){
    _amp = _amod.update();
    _value = (_amp * static_cast<float>(sin(_phase)) + _offset);
    cyclePhase(_freq + _fmod.update());
    return _value;
}

//--------------------------------------------------------------
void AMFMSineWave::setFMod(AbstractWave fmod){
    _fmod = fmod;
}

//--------------------------------------------------------------
void AMFMSineWave::setAMod(AbstractWave amod){
    _amod = amod;
}


//--------------------------------------------------------------
AbstractWave AMFMSineWave::getFMod(){
    return _fmod;
}


//--------------------------------------------------------------
AbstractWave AMFMSineWave::getAMod(){
    return _amod;
}

//--------------------------------------------------------------
void AMFMSineWave::pop(){
    AbstractWave::pop();
    _fmod.pop();
    _amod.pop();
}

//--------------------------------------------------------------
void AMFMSineWave::push(){
    AbstractWave::push();
    _fmod.push();
    _amod.push();
}

//--------------------------------------------------------------
void AMFMSineWave::reset(){
    AbstractWave::reset();
    _fmod.reset();
    _amod.reset();
}

