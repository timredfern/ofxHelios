#pragma once

#include "ofMain.h"
#include "ofxHelios.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void draw();

	ofxHelios laser;
};
