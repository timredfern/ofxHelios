#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxHelios.h"

class ofxHeliosGui : public ofxPanel {
public:

	void setup(ofxHelios& laser, int x = 10, int y = 10);
	void update();
	void drawGui();

	int draw(colourPolyline& line);
	int draw(ofPolyline& line, ofColor colour = ofColor(255, 255, 255));
	int draw(std::vector<colourPolyline>& lines);
	int draw(std::vector<ofPolyline>& lines, ofColor colour = ofColor(255, 255, 255));

	bool isPowered() const;
	int getLastPointCount() const;

private:

	ofxHelios* laser_ = nullptr;
	ofParameter<bool> power_{"power", true};
	ofParameter<float> intensity_{"intensity", 0.12f, 0.0f, 1.0f};
	ofParameter<int> points_{"points", 30000, 7, 100000};
	ofParameter<int> subdivide_{"subdivide", 15, 1, 100};
	ofParameter<int> blankCount_{"blank points", 8, 0, 32};
	ofParameter<float> maxAngle_{"max angle", 15.0f, 1.0f, 90.0f};
	int lastPointCount_ = 0;
};
