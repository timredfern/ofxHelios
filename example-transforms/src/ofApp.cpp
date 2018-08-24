#include "ofApp.h"

/*

    example-transforms

    ofxHelios draws using screen coordinates, centring the centre of
    the screen at the centre of the laser drawable area

    transform the polygons points to move the laser drawing 
    as ofxHelios doesn't inherit the transform (yet)

*/

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

colourPolyline makePolygon(int num,float diam){
    colourPolyline poly;
    float step=PI*2/num;
    for (int i=0;i<=num;i++){
        poly.addVertex(cos(step*i)*diam,sin(step*i)*diam,int(i*(255.0f/2))%256,int(i+1*(255.0f/2))%256,int(i+2*(255.0f/2))%256);
    }
    return poly;
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	ofSetColor(255,255,255);

    vector <colourPolyline> squares;

    for (int i=0;i<4;i++){
        ofMatrix4x4 m = ofMatrix4x4::newIdentityMatrix();
        m.rotateRad(ofGetElapsedTimef(),0,0,1);
        m.translate(ofGetWidth()/2+(cos(i*PI*0.5)*300),ofGetHeight()/2+(sin(i*PI*0.5)*300),0);
        colourPolyline square=makePolygon(6,400);
        square.transform(m);
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
