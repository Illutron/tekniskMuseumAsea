#pragma once

#include "ofMain.h"
#include "ofxIndustrialRobot.h"
#include "ofxCv.h"

#define CAPWIDTH 640
#define CAPHEIGHT 480
#define FACEHEIGHT 480

#define FACEWIDTH FACEHEIGHT*1.33

class ofApp : public ofBaseApp{
    
public:
    float maxX = 2100;
	float minX = 1390;
	float maxY = 1660;
	float minY = 1170;
	float maxZ = 750;

    bool facetrackerRunning = false;
    
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    int noFaceCount=0;
    float rate = 0;
    ofxIndustrialRobot * industrialRobot;
    void facetracker();
    ofVec3f idlePosition ;
    ofVec3f idleDirection;
    bool byteone[8];
	bool bytetwo[8];
	bool bytestatus[8];
    
    string motorStatusLabel[5];
    string panicStatus;
    bool firsttimefound = false;
    
    int resetting;
    
    bool unlockMotors;
    
    
    // face tracking
    
	ofVideoGrabber cam;
	
    
	cv::Mat translation, rotation;
	ofMatrix4x4 pose;
    
    ofxCv::ObjectFinder finder;
  

    unsigned long trackerTimer = 0;
    cv::Rect roi;
    bool tracked = false;
    	ofImage cropped;
    
    float x = 0;
    float y = 0;
    float width = 0;
   
};

