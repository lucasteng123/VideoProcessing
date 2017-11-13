#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxOsc.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"
#include "ofxNDI.h"

class videoCaptureTrack {
    
    ofxFaceTracker tracker;
    ofVideoGrabber video;
    public:
    int vidW, vidH;
    void setup(int, int, int);
    void update();
    void drawVideo(int, int);
    void trackingDraw();
    vector<float> getTrackingData();
    
    
};


class ofApp : public ofBaseApp{
    
	public:
        //----- gui section
        bool showGui;
        const int RELATIVEMOVE = 0;
        ofxDatGui* gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
        ofxDatGuiButton* connectToServerButton;
        ofxDatGuiFolder* serverSection;
        ofxDatGuiLabel* serverConnectedLabel;
        ofxDatGuiFolder* cameraSection;
        ofxDatGuiMatrix* activeCamera;
        ofxDatGuiFRM* framerate;
        
        //-----\gui section
    
        //video capture
    
        struct VideoFeedNDI {
            //ndi
            ofxNDIreceiver ndiReciever;
            
        };
        struct VideoFeedWebcam {
            ofVideoGrabber videoSource;
            bool faceDetected = false;
            ofxFaceTracker tracker;
        };
        struct VideoFeedBlackmagic{
           
           
        };
        vector<VideoFeedWebcam> feeds;
        vector<ofxFaceTracker> trackers;
        ofxFaceTracker tracker1;
        ofxFaceTracker tracker2;
    
    
        int currentSelectedCamera=0;

        //----- OSC section
        ofxOscReceiver oscReciever;
        string server;
        int serverPort;
        bool connectedToServer;
        int clientID;
    
        ofxOscSender oscSenderBroadcast;
        ofxOscSender oscServer;
    
        //-----/OSC section
		void setup();
		void update();
		void draw();
        void exit();
    
        void serverConnect();
        void connectToServer(string ip, int port, int clientid);
    
        void sendCommand(int command, int values[], int camera);
    
        void buttonPressed(ofxDatGuiButtonEvent e);
        void matrixPressed(ofxDatGuiMatrixEvent e);
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
		
};


