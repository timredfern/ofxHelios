#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxHelios.h"

#define LASERGUI_POWEROFF -99

/* laser class including gui */

class lasergui : public ofxPanel, public ofxHelios {
public:
	void setup(int x,int y){
		ofxPanel::setup("laser","",x,y);
		add(power.set("power", true));
    	add(intensity.set("intensity", 30, 0, 255));
    	add(points.set("points", 30000, 0, 40000));
    	add(subdivide.set("subdivide", 15, 1, 100));
    	add(blank_num.set("blank points", 8, 0, 32));
    	add(max_angle.set("max angle", 15.0f, 1.0f, 90.0f));
	}
	void update(){
		ofxHelios::set_pts(points);
    	ofxHelios::set_subdivide(subdivide);
    	ofxHelios::set_blanknum(blank_num);
    	ofxHelios::set_maxangle(max_angle);
    	ofxHelios::set_intensity(intensity);
	}
	void drawgui(){
		ofxPanel::draw();
	}
	int draw(colourPolyline &line, int intensity=255){
		if (!power) numpts=LASERGUI_POWEROFF;
		else numpts=ofxHelios::draw(line,intensity);
		return numpts;
	}
    int draw(ofPolyline &line,ofColor colour=ofColor(255,255,255),int intensity=255){
    	if (!power) numpts=LASERGUI_POWEROFF;
		else numpts=ofxHelios::draw(line,colour,intensity);
		return numpts;
	}
    int draw(vector <ofPolyline> &lines,ofColor colour=ofColor(255,255,255),int intensity=255){
		if (!power) numpts=LASERGUI_POWEROFF;
		else numpts=ofxHelios::draw(lines,colour,intensity);
		return numpts;
	}
    int draw(vector <colourPolyline> &lines, int intensity=255){
		if (!power) numpts=LASERGUI_POWEROFF;
		else numpts=ofxHelios::draw(lines,intensity);
		return numpts;
	}
	int get_numpts(){return numpts;};
private:
		ofParameter<bool> power;
        ofParameter<int> intensity;
        ofParameter<int> points;
        ofParameter<int> subdivide;
        ofParameter<int> blank_num;
    	ofParameter<float> max_angle;
    	int numpts;
};