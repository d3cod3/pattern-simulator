#pragma once

#include "ofMain.h"

#include "ofxDatGui.h"
#include "ofxXmlSettings.h"

#include "ofxFolding.h"

#include "matrixAreas.h"

#define MAPPING_FILE "mapping.xml"


class ofApp : public ofBaseApp{

public:
    // MAIN WINDOW /////////////////////
    void setup();
    void update();
    void draw();
    void drawIntoFBO();

    // GUI WINDOW //////////////////////
    void setupGui();
    void drawGui(ofEventArgs & args);

    // MAPPING /////////////////////////
    void setupMapping();
    void drawMapping();
    void resetMappingPoints();
    void loadMappingPoints();
    void saveMappingPoints();
    void toggleDrawGrid();

    void gaussian_elimination(float *input, int n);
    void findHomography(ofPoint src[4], ofPoint dst[4], float homography[16]);
    void applyHomography();
    void applyGridMesh(int xCalib, int yCalib, int w, int h);

    // EXPORT
    void exportImage();

    // FULLSCREEN
    void toggleFullscreen();

    // KEYBOARD ////////////////////////
    void keyReleased(int key);

    // MOUSE ///////////////////////////
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    // GUI /////////////////////////////
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    void on2dPadEvent(ofxDatGui2dPadEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    void onMatrixEvent(ofxDatGuiMatrixEvent e);

    // GUI
    ofxDatGui* gui;


    // FOLDING PATTERNS
    ofFbo                   foldingFbo;
    ofFbo                   drawingFbo;
    ofShader				        shaderColorCorrection;
    ofPixels                foldingPixels;
    ofImage                 tempFrame;
    ofImage                 exportImg;
    matrixAreas             finalTextureMapping;
    int                     projectionScreenW;
    int                     projectionScreenH;
    int                     projectionPosX;
    int                     projectionPosY;
    int                     res;
    int                     gridRes;
    int                     realRes;
    bool                    saveMappingSettings;
    bool                    drawGrid;
    bool                    mFullscreen;

    FMHarmonicSquareWave    *waveX;
    SineWave                *fmod;
    FMTriangleWave          *amod;

    float mainFreq;
    float fmFreq;
    float amFreq;
    float freq;
    float ASCALE;
    float STEP;

    float zAngle;
    float yAngle;
    float meshZoom;
    float planeZoom;
    float planeDimX;
    float planeDimY;
    float D2;
    float AMP;

    unsigned long colCounter;
    unsigned long aPLength;
    ofColor uno;
    ofColor dos;
    ofColor tres;
    ofColor cuatro;
    ofColor cinco;
    ofColor seis;
    vector<ofColor> paleta;

    bool animate;

};
