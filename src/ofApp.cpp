#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofEnableSmoothing();
    ofSetCircleResolution(200);

    // FOLDING PATTERN
    projectionScreenW = 1920;
    projectionScreenH = 1080;
    projectionPosX = 0;
    projectionPosY = 0;
    mFullscreen = false;

    drawingFbo.allocate(projectionScreenW,projectionScreenH,GL_RGBA32F_ARB);
    foldingFbo.allocate(projectionScreenW,projectionScreenH,GL_RGB);
    foldingPixels.allocate(projectionScreenW,projectionScreenH,OF_PIXELS_RGB);

    foldingFbo.begin();
    ofClear(255,255,255, 0);
    foldingFbo.end();
    drawingFbo.begin();
    ofClear(255,255,255, 0);
    drawingFbo.end();

    shaderColorCorrection.load("baseShader");

    shaderColorCorrection.begin();
    shaderColorCorrection.setUniform1f("tex_w",projectionScreenW);
    shaderColorCorrection.setUniform1f("tex_h",projectionScreenH);
    shaderColorCorrection.end();

    gridRes = 10;

    setupMapping();


    mainFreq = 1.2f;
    fmFreq = mainFreq*WAVE_PI;
    amFreq = mainFreq*1.029000f;
    freq = mainFreq;
    ASCALE = 6;
    STEP = 18;

    zAngle = 0.000000f;
    yAngle = 180.000000f;
    meshZoom = 1.000000f;
    planeZoom = 6.6f;
    planeDimX = ofGetWidth()*planeZoom;
    planeDimY = ofGetHeight()*planeZoom;
    D2 = planeDimX/2.0f;
    AMP = 20.000000f;

    colCounter = 0;
    aPLength = 0;
    uno = ofColor(0,0,0,255);
    dos = ofColor(255,255,255,255);
    tres = ofColor(0,0,0,255);
    cuatro = ofColor(255,255,255,255);
    cinco = ofColor(0,0,0,255);
    seis = ofColor(255,255,255,255);

    paleta.push_back(uno);
    paleta.push_back(dos);
    paleta.push_back(tres);
    paleta.push_back(cuatro);
    paleta.push_back(cinco);
    paleta.push_back(seis);

    aPLength = paleta.size();

    fmod = new SineWave(0,0,freq*ASCALE,0);
    amod = new FMTriangleWave(0,0,0.5f*ASCALE,0,ConstantWave(0));

    waveX = new FMHarmonicSquareWave(0,0,1.5,0,*fmod);

    animate = true;


}

//--------------------------------------------------------------
void ofApp::setupGui(){

    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_LEFT );
    gui->setTheme(new ofxDatGuiTheme(true));
    gui->addHeader(":: FOLDING SIMULATOR v. 0.0.1 ::");
    gui->addToggle("animate", true);
    gui->addBreak();
    ofxDatGuiFolder* folder = gui->addFolder("paleta", ofColor::white);
    folder->addColorPicker("** color 1", ofColor(0,0,0,255));
    folder->addLabel("");
    folder->addColorPicker("** color 2", ofColor(255,255,255,255));
    folder->addLabel("");
    folder->addColorPicker("** color 3", ofColor(0,0,0,255));
    folder->addLabel("");
    folder->addColorPicker("** color 4", ofColor(255,255,255,255));
    folder->addLabel("");
    folder->addColorPicker("** color 5", ofColor(0,0,0,255));
    folder->addLabel("");
    folder->addColorPicker("** color 6", ofColor(255,255,255,255));
    folder->expand();
    gui->addBreak();
    ofxDatGuiFolder* map_folder = gui->addFolder("mapping", ofColor::white);
    map_folder->addToggle("show");
    map_folder->addButton("load");
    map_folder->addButton("save");
    map_folder->addButton("reset");
    map_folder->expand();
    gui->addBreak();
    gui->addSlider("zoom", 0.4, 3.0, 1.0);
    gui->addSlider("rotation", 0, 360, 0);
    gui->addBreak();
    gui->addSlider("frequency", 0.0, WAVE_TWO_PI, 0);
    gui->addSlider("fm", 0.0, 1.0, 0);
    gui->addSlider("am", 0.0, 2.0, 0);
    gui->addSlider("amp", 0.0, 40, 20);
    gui->addBreak();
    gui->addButton("export image");
    gui->addBreak();
    gui->addFooter();


    gui->onButtonEvent(this, &ofApp::onButtonEvent);
    gui->onToggleEvent(this, &ofApp::onToggleEvent);
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->onTextInputEvent(this, &ofApp::onTextInputEvent);
    gui->on2dPadEvent(this, &ofApp::on2dPadEvent);
    gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
    gui->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    gui->onMatrixEvent(this, &ofApp::onMatrixEvent);

    ofSetBackgroundColor(0);
}

//--------------------------------------------------------------
void ofApp::update(){
     foldingFbo.getTexture().readToPixels(foldingPixels);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);

    drawingFbo.begin();
    //glPushAttrib(GL_ALL_ATTRIB_BITS);
    //glEnable(GL_BLEND);
    //glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    drawIntoFBO();
    //glDisable(GL_BLEND);
    //glPopAttrib();
    drawingFbo.end();

    foldingFbo.begin();
    // clear the FBO texture
    ofClear(0,0,0,255);
    // compute matrix homografy
    ofPushMatrix();
    applyHomography();
    ofPopMatrix();

    shaderColorCorrection.begin();
    shaderColorCorrection.setUniformTexture("tex0", drawingFbo, 0);
    // apply mapping mesh
    applyGridMesh(projectionPosX,projectionPosY,projectionScreenW,projectionScreenH);

    shaderColorCorrection.end();

    foldingFbo.end();

    foldingFbo.draw(projectionPosX,projectionPosY,projectionScreenW,projectionScreenH);

    // MAPPING
    drawMapping();
}

//--------------------------------------------------------------
void ofApp::drawIntoFBO(){
    ofEnableAlphaBlending();
    // draw FOLDING
    ofPushView();
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(ofGetWidth()*0.5f,ofGetHeight()*0.5f,0);
    ofRotateY(yAngle);
    ofRotateZ(zAngle);
    ofScale(meshZoom,meshZoom,meshZoom);

    colCounter = 0; // avoid mesh vertical flickering

    for (unsigned int y = 0;y<planeDimX-1; y+=STEP){
        waveX->push();
        fmod->push();
        amod->push();

        glBegin(GL_QUAD_STRIP);

        // paleta length control
        if (colCounter >= aPLength){
            colCounter = 0;
        }

        ofSetColor(paleta.at(colCounter));
        for (unsigned int x = 0;x<planeDimX-1;x+=STEP){
            float valueX = (waveX->update()+fmod->update()) * amod->update();
            glVertex3d(x - D2, y - STEP - D2, valueX * AMP);
            glVertex3d(x - D2, y - D2, valueX * AMP);
        }

        colCounter += 1;
        glEnd();

        waveX->pop();
        fmod->pop();
        amod->pop();
    }

    if(animate){
        waveX->update();
        fmod->update();
        amod->update();
    }

    ofPopMatrix();
    ofPopStyle();
    ofPopView();
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs & args){

}

//--------------------------------------------------------------
void ofApp::setupMapping(){

    saveMappingSettings = false;

    res = gridRes+1;
    realRes = res*res;
    finalTextureMapping.setup(projectionPosX,projectionPosY,projectionScreenW,projectionScreenH,realRes);

    finalTextureMapping.loadSettings(MAPPING_FILE);
    resetMappingPoints();

    drawGrid                = false;

}

//--------------------------------------------------------------
void ofApp::drawMapping(){
    if(drawGrid){
        ofPushMatrix();
        ofTranslate(0,0,0);
        ofScale(1.0,1.0,1.0);
        finalTextureMapping.drawMinimalGrid();
        ofPopMatrix();
    }
    if(saveMappingSettings){
        finalTextureMapping.saveSettings(MAPPING_FILE);
        saveMappingSettings = false;
    }
}

//--------------------------------------------------------------
void ofApp::resetMappingPoints(){
    // warped grid reset
    finalTextureMapping.reset(projectionScreenW,projectionScreenH);
    saveMappingSettings = true;
    finalTextureMapping.saveSettings(MAPPING_FILE);
    saveMappingSettings = false;
}

//--------------------------------------------------------------
void ofApp::loadMappingPoints(){
    char temp[256];
    // warped grid load points
    string URL;

    ofFileDialogResult res = ofSystemLoadDialog("Open Mapping File", false);
    if(res.bSuccess == true){
        URL = res.getPath();
        finalTextureMapping.loadSettings(URL);
        sprintf(temp,"OPEN MAPPING FILE: %s", URL.c_str());
        finalTextureMapping.saveSettings(MAPPING_FILE);
    }
}

//--------------------------------------------------------------
void ofApp::saveMappingPoints(){
    char temp[256];
    saveMappingSettings = true;
    // warped grid save points
    string folderURL;
    string fileName;

    ofFileDialogResult res = ofSystemSaveDialog(folderURL, "Save Mapping File");
    if(res.bSuccess == true){
        folderURL = res.getPath();
        sprintf(temp,"SAVE MAPPING FILE: %s", folderURL.c_str());
        finalTextureMapping.saveSettings(folderURL);
        finalTextureMapping.saveSettings(MAPPING_FILE);
    }
    saveMappingSettings = false;
}

//--------------------------------------------------------------
void ofApp::toggleDrawGrid(){
    drawGrid = !drawGrid;
}

//--------------------------------------------------------------
void ofApp::gaussian_elimination(float *input, int n){
    // ported to c from pseudocode in
    // http://en.wikipedia.org/wiki/Gaussian_elimination

    float * A = input;
    int i = 0;
    int j = 0;
    int m = n-1;
    while (i < m && j < n){
        // Find pivot in column j, starting in row i:
        int maxi = i;
        for(int k = i+1; k<m; k++){
            if(fabs(A[k*n+j]) > fabs(A[maxi*n+j])){
                maxi = k;
            }
        }
        if (A[maxi*n+j] != 0){
            //swap rows i and maxi, but do not change the value of i
            if(i!=maxi)
                for(int k=0; k<n; k++){
                    float aux = A[i*n+k];
                    A[i*n+k]=A[maxi*n+k];
                    A[maxi*n+k]=aux;
                }
            //Now A[i,j] will contain the old value of A[maxi,j].
            //divide each entry in row i by A[i,j]
            float A_ij=A[i*n+j];
            for(int k=0; k<n; k++){
                A[i*n+k]/=A_ij;
            }
            //Now A[i,j] will have the value 1.
            for(int u = i+1; u< m; u++){
                //subtract A[u,j] * row i from row u
                float A_uj = A[u*n+j];
                for(int k=0; k<n; k++){
                    A[u*n+k]-=A_uj*A[i*n+k];
                }
                //Now A[u,j] will be 0, since A[u,j] - A[i,j] * A[u,j] = A[u,j] - 1 * A[u,j] = 0.
            }

            i++;
        }
        j++;
    }

    //back substitution
    for(int i=m-2; i>=0; i--){
        for(int j=i+1; j<n-1; j++){
            A[i*n+m]-=A[i*n+j]*A[j*n+m];
        }
    }
}

//--------------------------------------------------------------
void ofApp::findHomography(ofPoint src[4], ofPoint dst[4], float homography[16]){
    // create the equation system to be solved
    //
    // from: Multiple View Geometry in Computer Vision 2ed
    //       Hartley R. and Zisserman A.
    //
    // x' = xH
    // where H is the homography: a 3 by 3 matrix
    // that transformed to inhomogeneous coordinates for each point
    // gives the following equations for each point:
    //
    // x' * (h31*x + h32*y + h33) = h11*x + h12*y + h13
    // y' * (h31*x + h32*y + h33) = h21*x + h22*y + h23
    //
    // as the homography is scale independent we can let h33 be 1 (indeed any of the terms)
    // so for 4 points we have 8 equations for 8 terms to solve: h11 - h32
    // after ordering the terms it gives the following matrix
    // that can be solved with gaussian elimination:

    float P[8][9]=
    {
        {-src[0].x, -src[0].y, -1,   0,   0,  0, src[0].x*dst[0].x, src[0].y*dst[0].x, -dst[0].x }, // h11
        {  0,   0,  0, -src[0].x, -src[0].y, -1, src[0].x*dst[0].y, src[0].y*dst[0].y, -dst[0].y }, // h12

        {-src[1].x, -src[1].y, -1,   0,   0,  0, src[1].x*dst[1].x, src[1].y*dst[1].x, -dst[1].x }, // h13
        {  0,   0,  0, -src[1].x, -src[1].y, -1, src[1].x*dst[1].y, src[1].y*dst[1].y, -dst[1].y }, // h21

        {-src[2].x, -src[2].y, -1,   0,   0,  0, src[2].x*dst[2].x, src[2].y*dst[2].x, -dst[2].x }, // h22
        {  0,   0,  0, -src[2].x, -src[2].y, -1, src[2].x*dst[2].y, src[2].y*dst[2].y, -dst[2].y }, // h23

        {-src[3].x, -src[3].y, -1,   0,   0,  0, src[3].x*dst[3].x, src[3].y*dst[3].x, -dst[3].x }, // h31
        {  0,   0,  0, -src[3].x, -src[3].y, -1, src[3].x*dst[3].y, src[3].y*dst[3].y, -dst[3].y }, // h32
    };

    gaussian_elimination(&P[0][0],9);

    // gaussian elimination gives the results of the equation system
    // in the last column of the original matrix.
    // opengl needs the transposed 4x4 matrix:
    float aux_H[]= { P[0][8],P[3][8],0,P[6][8], // h11  h21 0 h31
        P[1][8],P[4][8],0,P[7][8], // h12  h22 0 h32
        0      ,      0,0,0,       // 0    0   0 0
        P[2][8],P[5][8],0,1
    };      // h13  h23 0 h33

    for(int i=0; i<16; i++) homography[i] = aux_H[i];
}

//--------------------------------------------------------------
void ofApp::applyHomography(){
    //lets make a matrix for openGL
    //this will be the matrix that peforms the transformation
    float myMatrix[16];

    //we need our points as opencv points
    ofPoint cvsrc[4];
    ofPoint cvdst[4];

    //we set the warp coordinates
    //source coordinates as the dimensions of our FBO texture
    for(int i=0;i<4;i++){
        float x = (i % 4) / static_cast<float>(4-1);
        float y = (i / 4) / static_cast<float>(4-1);
        cvsrc[i].x = x*projectionScreenW;
        cvsrc[i].y = y*projectionScreenH;
    }

    for(int i=0; i<4;i++){
        cvdst[i].x = finalTextureMapping.points[i].x;
        cvdst[i].y = finalTextureMapping.points[i].y;
    }

    //figure out the warping!
    findHomography(cvsrc, cvdst, myMatrix);

    //finally lets multiply our matrix
    ofMultMatrix(ofMatrix4x4(myMatrix));
}

//--------------------------------------------------------------
void ofApp::applyGridMesh(int xCalib, int yCalib, int w, int h){
    float xStep = w/gridRes;
    float yStep = h/gridRes;

    glBegin(GL_QUADS);
    for(int x=0; x < gridRes; x++) {
        for(int y=0; y < gridRes; y++) {
            int revY =  y;
            ofPoint mp;

            mp = finalTextureMapping.points[(x)+(y+1)*res];
            glTexCoord2f(x*xStep, (revY+1)*yStep);
            glVertex3f(xCalib+mp.x,yCalib+mp.y,0);

            mp = finalTextureMapping.points[(x+1)+(y+1)*res];
            glTexCoord2f((x+1)*xStep, (revY+1)*yStep);
            glVertex3f(xCalib+mp.x,yCalib+mp.y,0);

            mp = finalTextureMapping.points[(x+1)+(y)*res];
            glTexCoord2f((x+1)*xStep, (revY)*yStep);
            glVertex3f(xCalib+mp.x,yCalib+mp.y,0);

            mp = finalTextureMapping.points[(x)+(y)*res];
            glTexCoord2f(x*xStep, (revY)*yStep);
            glVertex3f(xCalib+mp.x,yCalib+mp.y,0);

        }
    }
    glEnd();
}

//--------------------------------------------------------------
void ofApp::exportImage(){
    string folderURL;
    string fileName;

    char temp[256];

    ofFileDialogResult res = ofSystemSaveDialog(folderURL, "Save FOLDING image");
    if(res.bSuccess == true){
        folderURL = res.getPath();
        exportImg.setFromPixels(foldingPixels);
        sprintf(temp,"%s.tif", folderURL.c_str());
        exportImg.save(temp);
    }
}

//--------------------------------------------------------------
void ofApp::toggleFullscreen(){
    mFullscreen = !mFullscreen;
    ofSetFullscreen(mFullscreen);
    if(!mFullscreen){
        ofSetWindowShape(960,540);
        ofSetWindowPosition(276, 0);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'f'){
        toggleFullscreen();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    // MAPPING
    if(drawGrid){
        finalTextureMapping.mouseMoved(x,y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // MAPPING
    if(drawGrid){
        finalTextureMapping.mouseDragged(x,y);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // MAPPING
    if(drawGrid){
        finalTextureMapping.mousePressed(x,y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    // MAPPING
    if(drawGrid){
        finalTextureMapping.mouseReleased(x,y);
    }
}

//--------------------------------------------------------------
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e){
    //cout << "onButtonEvent: " << e.target->getLabel() << endl;
    if (e.target->is("load")){
        loadMappingPoints();
    }else if (e.target->is("save")){
        saveMappingPoints();
    }else if (e.target->is("reset")){
        resetMappingPoints();
    }else if(e.target->is("export image")){
        exportImage();
    }
}

//--------------------------------------------------------------
void ofApp::onToggleEvent(ofxDatGuiToggleEvent e){
    if (e.target->is("animate")){
        animate = !animate;
        gui->getToggle("animate")->setChecked(animate);
    }else if(e.target->is("show")){
        toggleDrawGrid();
    }
    //cout << "onToggleEvent: " << e.target->getLabel() << " " << e.checked << endl;
}

//--------------------------------------------------------------
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e){
    //cout << "onSliderEvent: " << e.target->getLabel() << " "; e.target->printValue();
    //if (e.target->is("datgui opacity")) gui->setOpacity(e.scale);
    if(e.target->is("zoom")){
        meshZoom = e.value;
    }else if(e.target->is("rotation")){
        zAngle = e.value;
    }else if(e.target->is("frequency")){
        waveX->setFrequency(e.value);
    }else if(e.target->is("fm")){
        fmod->setFrequency(e.value);
    }else if(e.target->is("am")){
        amod->setFrequency(e.value);
    }else if(e.target->is("amp")){
        AMP = e.value;
    }
}

//--------------------------------------------------------------
void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e){
    cout << "onTextInputEvent: " << e.target->getLabel() << " " << e.target->getText() << endl;
}

//--------------------------------------------------------------
void ofApp::on2dPadEvent(ofxDatGui2dPadEvent e){
    cout << "on2dPadEvent: " << e.target->getLabel() << " " << e.x << ":" << e.y << endl;
}

//--------------------------------------------------------------
void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e){
    cout << "onDropdownEvent: " << e.target->getLabel() << " Selected" << endl;
}

//--------------------------------------------------------------
void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e){
    //cout << "onColorPickerEvent: " << e.target->getLabel() << " " << e.target->getColor() << endl;
    //ofSetBackgroundColor(e.color);
    if (e.target->is("** color 1")){
        paleta[0].set(e.color);
    }else if (e.target->is("** color 2")){
        paleta[1].set(e.color);
    }else if (e.target->is("** color 3")){
        paleta[2].set(e.color);
    }else if (e.target->is("** color 4")){
        paleta[3].set(e.color);
    }else if (e.target->is("** color 5")){
        paleta[4].set(e.color);
    }else if (e.target->is("** color 6")){
        paleta[5].set(e.color);
    }
}

//--------------------------------------------------------------
void ofApp::onMatrixEvent(ofxDatGuiMatrixEvent e){
    cout << "onMatrixEvent " << e.child << " : " << e.enabled << endl;
    cout << "onMatrixEvent " << e.target->getLabel() << " : " << e.target->getSelected().size() << endl;
}
