#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofEnableSmoothing();
    ofSetCircleResolution(200);

    // FOLDING PATTERN
    projectionScreenW = 1920;
    projectionScreenH = 1080;

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
    applyHomography();

    shaderColorCorrection.begin();
    shaderColorCorrection.setUniformTexture("tex0", drawingFbo, 0);
    // apply mapping mesh
    applyGridMesh(0,0,projectionScreenW,projectionScreenH);

    shaderColorCorrection.end();

    foldingFbo.end();

    foldingFbo.draw(0,0,projectionScreenW,projectionScreenH);

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
    finalTextureMapping.setup(0,0,1920,1080,realRes);

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
void ofApp::applyHomography(){
    //lets make a matrix for openGL
    //this will be the matrix that peforms the transformation
    GLfloat myMatrix[16];

    //we set it to the default - 0 translation
    //and 1.0 scale for x y z and w
    for(int i = 0; i < 16; i++){
        if(i % 5 != 0) myMatrix[i] = 0.0;
        else myMatrix[i] = 1.0;
    }

    //we need our points as opencv points
    CvPoint2D32f cvsrc[realRes];
    CvPoint2D32f cvdst[realRes];

    //we set the warp coordinates
    //source coordinates as the dimensions of our FBO texture
    for(int i=0;i<realRes;i++){
        float x = (i % res) / static_cast<float>(res-1);
        float y = (i / res) / static_cast<float>(res-1);
        cvsrc[i].x = x*projectionScreenW;
        cvsrc[i].y = y*projectionScreenH;
    }

    for(int i=0; i<realRes;i++){
        cvdst[i].x = finalTextureMapping.points[i].x;
        cvdst[i].y = finalTextureMapping.points[i].y;
    }

    //we create a matrix that will store the results
    //from openCV - this is a 3x3 2D matrix that is
    //row ordered
    CvMat * translate = cvCreateMat(3,3,CV_32FC1);

    //for the more accurate method we need to create
    //a couple of matrixes that just act as containers
    //to store our points  - the nice thing with this
    //method is you can give it more than four points!

    CvMat* src_mat = cvCreateMat( realRes, 2, CV_32FC1 );
    CvMat* dst_mat = cvCreateMat( realRes, 2, CV_32FC1 );

    //copy our points into the matrixes
    cvSetData( src_mat, cvsrc, sizeof(CvPoint2D32f));
    cvSetData( dst_mat, cvdst, sizeof(CvPoint2D32f));

    //figure out the warping!
    cvFindHomography(src_mat, dst_mat, translate);

    //get the matrix as a list of floats
    float *matrix = translate->data.fl;

    //we need to copy these values
    //from the 3x3 2D openCV matrix which is row ordered
    //
    // ie:   [0][1][2] x
    //       [3][4][5] y
    //       [6][7][8] w

    //to openGL's 4x4 3D column ordered matrix
    //        x   y   z   w
    // ie:   [0] [4] [8][12]
    //       [1] [5] [9][13]
    //       [2] [6][10][14]
    //		 [3] [7][11][15]
    //

    myMatrix[0]		= matrix[0];
    myMatrix[4]		= matrix[1];
    myMatrix[12]	= matrix[2];

    myMatrix[1]		= matrix[3];
    myMatrix[5]		= matrix[4];
    myMatrix[13]	= matrix[5];

    myMatrix[3]		= matrix[6];
    myMatrix[7]		= matrix[7];
    myMatrix[15]	= matrix[8];

    //finally lets multiply our matrix
    glMultMatrixf(myMatrix);
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
void ofApp::keyReleased(int key){
    if(key == 'f'){
        ofToggleFullscreen();
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
