#pragma once

#include "ofMain.h"
#include "ofxHeliosGui.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	ofxHelios laser;
	ofxHeliosGui laserGui;
};
