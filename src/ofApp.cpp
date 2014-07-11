#include "ofApp.h"

using namespace ofxCv;


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    //cam.setDeviceID(0);
    cam.setDeviceID(0);
	cam.initGrabber(CAPWIDTH, CAPHEIGHT);
	tracker.setup();
    
    industrialRobot = new ofxIndustrialRobot(this);
    industrialRobot->setInput(ofxIndustrialRobot::Gravity);
    industrialRobot->gotoResetPosition();
    idlePosition = ofVec3f(1460, 1560, 0);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    
    if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
		position = tracker.getPosition();
		scale = tracker.getScale();
		orientation = tracker.getOrientation();
		rotationMatrix = tracker.getRotationMatrix();
	}
    
    
    //  industrialRobot->setGravityTarget(ofVec3f(1487.6, 666.2, 1038.6), ofVec3f(0.77, -0.44, 0.62));
    
    
	if(industrialRobot->getEmergencyStop()){
        //unlockMotors->setChecked(false);
	}
	
	//tjeckCurrentRobotController();
	
	//Check for changes in control mode (tilf¿jet af mads)
    /*	for(int i = 0; i < 4;i++)
     {
     if(inputRadio[i]->isChecked() && controlMode != i)
     {
     setControlMode(i);
     }
     }*/
	
	/*if(industrialRobot->thread.lock()){
     if(loadXmlButton->isDown()){
     industrialRobot->thread.converter->loadXml();
     loadXmlButton->setDown(false);
     }
     industrialRobot->thread.unlock();
     }*/
    
	if(industrialRobot->getSerial()->lock()){
		for(int i=0;i<8;i++){
			if(industrialRobot->getSerial()->returnedFlags[i])
				bytestatus[i] = true;
			else
				bytestatus[i] = false;
		}
		
		for(int i=0;i<5;i++){
			string l;
			if(industrialRobot->getSerial()->connected){
				l = "Motor "+ofToString(i,0)+": ";
				if(!byteone[7]){
					l+= "LOCKED ";
				}
				if(bytetwo[i]){
					if(bytestatus[i]){
						l+= "Resetting done...";
					} else {
						l+= "Resetting...";
					}
				} else if(bytestatus[i]){
					l+= "OK";
				} else {
					l+= "NOT OK";
				}
			} else {
				l = " ";
			}
			motorStatusLabel[i] = l;
		}
		
		if(!industrialRobot->thread.serial->connected){
			panicStatus = "Status: Serial not connected";
		}else if(bytestatus[7]){
			panicStatus = "Status: PANIC!!!";
			/*industrialRobot->thread.serial->unlock();
			 if(!warningShown){
			 QMessageBox msgBox;
			 msgBox.setText("Panic!");
			 msgBox.exec();
			 warningShown = true;
			 }
			 industrialRobot->thread.serial->lock();*/
		} else if(bytestatus[6]){
			panicStatus = ("Status: EMERGENCY STOP");
			/*industrialRobot->thread.serial->unlock();
			 if(!warningShown){
			 QMessageBox msgBox;
			 msgBox.setText("Emergency stop pressed. Release it, and unlock the robot.");
			 msgBox.exec();
			 warningShown = true;
			 }
			 industrialRobot->thread.serial->lock();*/
		} else {
			panicStatus = ("Status: OK");
            //			warningShown = false;
		}
		
		/*if(resetMotors->isDown()){
         resetting  = 1;
         }*/
		
		/*if(unlockMotors && !byteone[7]){
         byteone[7] = true;
         } else if(!unlockMotors && byteone[7]){
         byteone[7] = (false);
         }
         */
		industrialRobot->thread.serial->unlock();
	}
	
    /*	if(resetting > 0){
     ofSleepMillis(200);
     }
     */
    if(facetrackerRunning)
        facetracker();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    industrialRobot->visualizer->draw3d(0, 0, ofGetWidth()/2, ofGetHeight()/2);
    industrialRobot->visualizer->drawside( ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight()/2);
    industrialRobot->visualizer->drawtop( ofGetWidth()/2,  ofGetHeight()/2, ofGetWidth()/2, ofGetHeight()/2);
    // industrialRobot->visualizer->drawArm0()drawGraphs(x+(w/2.0)+2, y+(h/2.0)+2, w/2.0-2, h/2.0-2);
    
    bool * byteone = industrialRobot->thread.serial->sendFlags[0];
    bool * bytetwo = industrialRobot->thread.serial->sendFlags[1];
    bool * bytestatus = industrialRobot->thread.serial->returnedFlags;
    
    ofSetColor(255, 255, 255,255);
    for(int i=0;i<8;i++){
        if(!byteone[i]){
            ofNoFill();
        } else {
            ofFill();
        }
        ofRect(10+i*40, ofGetHeight()-(10+3*40), 30, 30);
        
        if(!bytetwo[i]){
            ofNoFill();
        } else {
            ofFill();
        }
        
        ofRect(10+i*40, ofGetHeight()-(10+2*40), 30, 30);
        if(!bytestatus[i]){
            ofNoFill();
        } else {
            ofFill();
        }
        ofRect(10+i*40, ofGetHeight()-(10+1*40), 30, 30);
    }
    for(int i=0;i<5;i++){
        ofDrawBitmapString(motorStatusLabel[i], 10+200*i, ofGetHeight()-(10+4*40));
    }
    ofDrawBitmapString(panicStatus, 10, ofGetHeight()-(10+5*40));
    
    
    cam.draw(0,0);
	if(tracker.getFound() ) {
        
        // cout<<tracker.getPosition()<<endl;
		ofSetLineWidth(1);
		//tracker.draw();
      //  cout <<"tracker X: " << tracker.getPosition().x <<", "<< CAPWIDTH<<endl;
      //  cout <<"tracker Y: " << tracker.getPosition().y <<", "<< CAPHEIGHT<<endl;
        //  tracker.getHaarRectangle().get
  		//easyCam.begin();
		ofSetupScreenOrtho(CAPWIDTH, CAPHEIGHT, OF_ORIENTATION_UNKNOWN, true, -1000, 1000);
		ofTranslate(CAPWIDTH / 2, CAPHEIGHT / 2);
		applyMatrix(rotationMatrix);
		ofScale(5,5,5);
		ofDrawAxis(scale);
		tracker.getObjectMesh().drawWireframe();
		//easyCam.end();
	}
    
    
    
}

void ofApp::facetracker()
{
    //cout << "noFaceCount: "<< noFaceCount << endl;
    noFaceCount++;
    
    if(noFaceCount<0)
        noFaceCount = 0;
    
    
    //1
    //If face detected: calculate its x, y, z position
    if(tracker.getFound() ) {
        
        noFaceCount -= 5;
      
        
        ofVec3f curDir = industrialRobot->getCurrentDir();
        ofVec3f curPos = industrialRobot->getCurrentTarget();
        firsttimefound = true; //legacy
        
        // Standard constraints
        float w = FACEWIDTH;
        float h = FACEHEIGHT;
        float factor = CAPWIDTH/FACEWIDTH;
      //  cout << tracker.getPosition().x << endl; //mads
        //? z + y pŒ ansigtet (kommer fra tracker
        float z = curDir.z + ( tracker.getPosition().x-w/2.0)*0.002*factor;
        float y = curDir.y - ( tracker.getPosition().y-h/2.0)*0.002*factor;
        
        // normalize??? Det vi gerne vil kigge pŒ (nyt target)
        ofVec3f targetDir = ofVec3f(0.9, y, z).normalized(); //Den vigtige en
        //? Armens nuv¾rende position/orientering
        ofVec3f armDir = curPos.normalized();
        
        // constraints
        if(targetDir.y > 0.7)
			targetDir.y = 0.7;
		if(targetDir.y < -0.7)
			targetDir.y = -0.7;
		if(targetDir.z > 0.4+armDir.z)
			targetDir.z = 0.4+armDir.z;
		if(targetDir.z < -0.4+armDir.z)
			targetDir.z = -0.4+armDir.z;
        
        //Tag robottens nuv¾rende position, og ¾ndre x (nyt target)
        float tx =   (maxX-300- industrialRobot->getCurrentTarget().x)*0.1 + industrialRobot->getCurrentTarget().x;
        ofVec3f targetPosition = curPos;
        ofVec3f target = targetPosition;
        target.x = tx;
        
        //More constraints
        if(target.x > maxX){
			target.x = maxX;
		}
		if(target.x < minX){
			target.x = minX;
		}
        
        
        //--//--//--//
        // bed robotten om at flytte sig hertil....
        if(noFaceCount < 40 )
        {
            
            if(industrialRobot->thread.lock()){
                if(industrialRobot->thread.coreData.reverseHeadPercent < 0.2 || industrialRobot->thread.coreData.reverseHeadPercent > 0.8){
                    
                    if( !industrialRobot->setGravityTarget(target, targetDir, 0.1, 2, ofxIndustrialRobotDefines::Up, false)){
                        cout<<"Could not add facetracker scale "<<target.x<<", "<<target.y<<",  "<<target.z<<"  -  "<<targetDir.x<<", "<<targetDir.y<<",  "<<targetDir.z<<endl;
                    }
                }
                industrialRobot->thread.unlock();
            }
        }
        
    }
    
    
    
    
    
    //2
    if(noFaceCount > 40){ // start searching
		if(rate > 60){
			rate = 30;
		}
		rate -=0.1;
		
		industrialRobot->lockVariant(false);
		
		
		if(industrialRobot->isRobotDirReady(0.1)){
			idleDirection = ofVec3f(2.0,ofRandom(-0.4, -0.05), ofRandom(-1.0, 1.0)).normalized();
			ofVec3f offsetPos = ofVec3f(ofRandom(-200,200), ofRandom(-200,400), ofRandom(-200,200));
			if( !industrialRobot->setGravityTarget(idlePosition+offsetPos, idleDirection, 0.05, 0.4, ofxIndustrialRobotDefines::Up)){
				cout<<"Could not add facetracker"<<endl;
			}
		}
	}
    else
     
     {
        rate += 0.1;
        //cout << "motion tracker counting";
        
        if(rate > 70){
            rate = 40;
        }
        
        industrialRobot->lockVariant(true);
        
        //	cout<<"face1"<<endl;
        //	if(robot->industrialRobot->thread.lock()){
        
        ofVec3f curTarget =  industrialRobot->getCurrentTarget();
        ofVec3f curDir =  industrialRobot->getCurrentDir();
        
        float tz = curTarget.z  - ( curTarget.normalized().z-curDir.z)*90.0;;
        
        if(tz > maxZ){
            tz = maxZ;
        }
        if(tz < -maxZ){
            tz = -maxZ;
        }
        
        float ty = curTarget.y  - ( -0.15-curDir.y)*200.0;;
        
        if(ty > maxY){
            ty = maxY;
        }
        if(ty < minY){
            ty = minY;
        }
        
        ofVec3f target = ofVec3f(industrialRobot->getCurrentGravityTarget().x,ty,tz);
        
        if( !industrialRobot->setGravityTarget(target, industrialRobot->getCurrentGravityDir(), 0.1, 1, ofxIndustrialRobotDefines::Up)){
            cout<<"Could not add facetracker  pos "<<industrialRobot->getCurrentGravityTarget().x<<"   "<<ty<<"   "<<tz<<endl;
        }
        //	robot->industrialRobot->thread.unlock();
    }
    
    
    
    
    //}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'a'){
        industrialRobot->resetRobot();
    }
    if(key == 'u'){
        //        industrialRobot->set
        if (facetrackerRunning) {
            facetrackerRunning = false;
        }
        industrialRobot->thread.serial->setUnlockFlag(!industrialRobot->thread.serial->unlockFlag());
    }
    
    if(key == '1'){
        byteone[0] = !byteone[0];
    }
    if(key == '2'){
        byteone[1] = !byteone[1];
    }
    
    if(key == '3'){
        byteone[2] = !byteone[2];
    }
    
    if(key == '4'){
        byteone[3] = !byteone[3];
    }
    
    if(key == '5'){
        byteone[4] = !byteone[4];
    }
    if(key == '6'){
        byteone[5] = !byteone[5];
    }
    if(key == '7'){
        byteone[6] = !byteone[6];
    }
    
    if(key == '8'){
        byteone[7] = !byteone[7];
    }
    
    if(key == 'q'){
        bytetwo[0] = !bytetwo[0];
    }
    if(key == 'w'){
        bytetwo[1] = !bytetwo[1];
    }
    
    if(key == 'e'){
        bytetwo[2] = !bytetwo[2];
    }
    
    if(key == 'r'){
        bytetwo[3] = !bytetwo[3];
    }
    
    if(key == 't'){
        bytetwo[4] = !bytetwo[4];
    }
    if(key == 'y'){
        bytetwo[5] = !bytetwo[5];
    }
    if(key == 'u'){
        bytetwo[6] = !bytetwo[6];
    }
    
    if(key == 'i'){
        bytetwo[7] = !bytetwo[7];
    }
    if(key == 'f'){
        facetrackerRunning = !facetrackerRunning;
    }
    
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
void ofApp::windowResized(int w, int h){
    
}

