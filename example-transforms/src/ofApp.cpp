#include "ofApp.h"

int MAX_POINTS=20000;
int LASER_INTENSITY=37;


//--------------------------------------------------------------
void testApp::setup(){
	laser.set_pts(MAX_POINTS);
    laser.set_intensity(LASER_INTENSITY);

}

//--------------------------------------------------------------
void testApp::update(){
    

}

ofPolyline polyLineTransform(const ofPolyline& poly, ofMatrix4x4 xform){
    ofPolyline tempPoly;
    for (auto& p:poly){
        tempPoly.addVertex(ofVec3f(p)*xform);
    }
    return tempPoly;
}

ofPolyline makePolygon(int num,float diam){
    ofPolyline poly;
    float step=PI*2/num;
    for (int i=0;i<=num;i++){
        poly.addVertex(cos(step*i)*diam,sin(step*i)*diam);
    }
    return poly;
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	ofSetColor(255,255,255);

    vector <ofPolyline> squares;

    for (int i=0;i<4;i++){
        ofMatrix4x4 m = ofMatrix4x4::newIdentityMatrix();
        m.rotateRad(ofGetElapsedTimef(),0,0,1);
        m.translate(ofGetWidth()/2+(cos(i*PI*0.5)*300),ofGetHeight()/2+(sin(i*PI*0.5)*300),0);
        ofPolyline square=polyLineTransform(makePolygon(4,400),m);
        square.draw();
        squares.push_back(square);
    }

    int num = laser.draw(squares);

    if (num>0){
        ofSetWindowTitle(ofToString(ofGetFrameRate(), 2)+" fps laser points: "+ofToString(num));
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



	/// int i = 1;
	/// while ( i<bounds.size()) {
	/// 	float angle = atan2(line[i-1].y - line[i].y, line[i-1].x - line[i].x);  
	/// 	bounds[i].set(bounds[i-1].x - cos(angle) * 20, bounds[i-1].y - sin(angle) * 20);
	/// 	i++;
	/// }

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	//line.addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

	//line.clear();
	//current=-100;
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
