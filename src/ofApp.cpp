#include "ofApp.h"
using namespace ofxCv;

//videoCaptureMethods
void videoCaptureTrack::setup(int src, int w, int h){
    video.setDeviceID(src);
    video.initGrabber(w, h);
    vidW=w;
    vidH=h;
    tracker.setup();
    tracker.setAttempts(1);
    tracker.setClamp(3.);
    tracker.setIterations(5);
}
void videoCaptureTrack::update(){
    video.update();
    if(video.isFrameNew()){
        tracker.update(toCv(video));
    }
}

void videoCaptureTrack::drawVideo(int x, int y){
    video.draw(x,y);
}
void videoCaptureTrack::trackingDraw(){
    tracker.draw();
}

vector<float> videoCaptureTrack::getTrackingData(){
    ofVec2f position = tracker.getPosition();
    float * posptr = position.getPtr();
    vector<float> values;
    values.push_back(*(posptr));
    values.push_back(*(posptr+1));
    values.push_back(tracker.getScale());
    return values;
}


//--------------------------------------------------------------
void ofApp::setup(){
    videoCaptureTrack vct;
   
    try{
    inputs.push_back(vct);
    inputs[0].setup(0,320,240);
    } catch(exception& e) {
        std::cout << e.what();
    }
    //set up window
    ofSetWindowTitle("Video Processing");
    ofSetWindowShape(512, 512);
    ofSetFrameRate(60);
    ofBackgroundHex(0x1c1c1c);
    gui->onButtonEvent(this, &ofApp::buttonPressed);
    gui->onMatrixEvent(this, &ofApp::matrixPressed);
    serverSection = gui->addFolder("SERVER", ofColor::darkRed);
    
    serverConnectedLabel = serverSection->addLabel("Not Connected to Server");
    connectToServerButton = serverSection->addButton("Connect to Server");
    
    cameraSection=gui->addFolder("CAMERA",ofColor::darkBlue);
    activeCamera=cameraSection->addMatrix("Active Camera", inputs.size());
    activeCamera->setRadioMode(true);
    
    //Networking
    try {
        oscReciever.setup(7891);
        connectedToServer = false;
        oscSenderBroadcast.setup("127.0.0.1", 7892);
        ofSetLogLevel(OF_LOG_VERBOSE);
        serverConnect();
    } catch (exception& e) {
        std::cout << e.what();
    }
   
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //osc handler
    while (oscReciever.hasWaitingMessages()) {
        ofxOscMessage m;
        oscReciever.getNextMessage(&m);
        ofLog(OF_LOG_VERBOSE, "Recieved OSC Message with address:" + ofToString(m.getAddress()) );
        if(m.getAddress() == "/registerServer"){
            connectToServer(m.getArgAsString(0), m.getArgAsInt(1), m.getArgAsInt(2));
        }
    }
    
    //update cameras
    inputs[0].update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    ofTranslate(inputs[currentSelectedCamera].vidW, inputs[currentSelectedCamera].vidH);
    inputs[currentSelectedCamera].drawVideo(0, 0);
    inputs[currentSelectedCamera].trackingDraw();
    ofPopMatrix();
}

void ofApp::exit(){
    
}

void ofApp::serverConnect(){
    ofxOscMessage messageToSend;
    messageToSend.setAddress("/registerClient");
    messageToSend.addStringArg("127.0.0.1");
    messageToSend.addIntArg(7891);
    oscSenderBroadcast.sendMessage(messageToSend);
}

void ofApp::connectToServer(string ip, int port, int clientid){
    server = ip;
    serverPort = port;
    clientID = clientid;
    oscServer.setup(server, serverPort);
    connectedToServer = true;
    ofLog(OF_LOG_NOTICE, "Connected To Server at address:" +server+",  and port:"+ofToString(serverPort));
    serverConnectedLabel->setLabel("Connected to " + server);
    
}

void ofApp::sendCommand(int command, int values[], int camera){
    switch (command) {
        case 0:{
            //relativemove
            ofxOscMessage m;
            m.setAddress("/relativeMovement");
            m.addIntArg(values[0]);
            m.addIntArg(values[1]);
            m.addIntArg(camera);
            oscServer.sendMessage(m);
            break;
        }
        default:{
            ofLog(OF_LOG_ERROR, "Invalid command passed");
            break;
        }
    }
}


//------------------------ EVENTS ------------------------------
//--------------------------------------------------------------
void ofApp::buttonPressed(ofxDatGuiButtonEvent e){
    if (e.target == connectToServerButton) {
        if(!connectedToServer) serverConnect();
    }
}
void ofApp::matrixPressed(ofxDatGuiMatrixEvent e){
    if(e.target == activeCamera){
        vector<int> selected = activeCamera->getSelected();
        currentSelectedCamera = selected[0];
    }
}

void ofApp::keyPressed(int key){
    if(key == 's') showGui = !showGui;
    int up[] = {0,-10};
    int down[] = {0,10};
    int left[] = {-10,0};
    int right[] = {10,0};
    
    if(key == OF_KEY_UP) sendCommand(RELATIVEMOVE, up, 0);
    if(key == OF_KEY_DOWN) sendCommand(RELATIVEMOVE, down, 0);
    if(key == OF_KEY_LEFT) sendCommand(RELATIVEMOVE, left, 0);
    if(key == OF_KEY_RIGHT) sendCommand(RELATIVEMOVE, right, 0);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
