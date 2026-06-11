#include "ofApp.h"
/*
    example-colourpolyline

    colourPolyline subclasses openFrameworks polyline to add per-vertex colours

    colourPolyline can be drawn to the screen or by ofxHelios

*/
int MAX_POINTS=30000;
int NUM_POINTS=128;
float LASER_INTENSITY=0.5f;
int CIRCLE_RADIUS=256;

//--------------------------------------------------------------
void testApp::setup(){
	laser.setup();
	laser.setPps(MAX_POINTS);
	laser.setIntensity(LASER_INTENSITY);
}

//--------------------------------------------------------------
void testApp::update(){
    

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	ofSetColor(255,255,255);

    float pfactor=(PI*2)/NUM_POINTS;

    vector <colourPolyline> lines;

    for (int k=0;k<2;k++){
        for (int j=0;j<2;j++){

            colourPolyline line;

            for (int i=0;i<NUM_POINTS+1;i++){
                line.addVertex(
                    ((cos(float(i)*pfactor)+1)*CIRCLE_RADIUS)+(ofGetWidth()/2)+(-(CIRCLE_RADIUS+200)+(k*400)),
                    ((sin(float(i)*pfactor)+1)*CIRCLE_RADIUS)+(ofGetHeight()/2)+(-(CIRCLE_RADIUS+200)+(j*400)),
                    (sin(float(i+(ofGetElapsedTimef()*30*j))*pfactor)+1)*127,
                    (sin(float(i+(ofGetElapsedTimef()*57*k))*pfactor)+1)*127,
                    (sin(float(i+(ofGetElapsedTimef()*101*(j+k)))*pfactor)+1)*127
                    );
            }
            lines.push_back(line);
            line.draw();
        }
    }

    int num = laser.draw(lines);

    if (num>0){
        ofSetWindowTitle(ofToString(laser.getLaserFps(), 1) + " laser fps  points: " + ofToString(num));
    }
    else {
        ofSetWindowTitle("laser error ");
    }

    

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
