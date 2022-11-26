#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxHelios.h"

/* laser class including gui */

class lasergui : public ofxPanel, public ofxHelios {
public:
	void setup(int x,int y){
		ofxPanel::setup("laser","",x,y);
		add(laser_power.set("power", true));
    	add(laser_intensity.set("intensity", 30, 0, 255));
    	add(laser_points.set("points", 30000, 0, 40000));
    	add(laser_subdivide.set("subdivide", 15, 1, 100));
    	add(laser_blank_num.set("blank points", 8, 0, 32));
    	add(laser_max_angle.set("max angle", 15.0f, 1.0f, 90.0f));
	}
	void update(){
		ofxHelios::set_pts(laser_points);
    	ofxHelios::set_subdivide(laser_subdivide);
    	ofxHelios::set_blanknum(laser_blank_num);
    	ofxHelios::set_maxangle(laser_max_angle);
    	ofxHelios::set_intensity(laser_intensity);
	}
	void drawgui(){
		ofxPanel::draw();
	}
	int draw(colourPolyline &line, int intensity=255){
		return ofxHelios::draw(line,intensity);
	}
    int draw(ofPolyline &line,ofColor colour=ofColor(255,255,255),int intensity=255){
		return ofxHelios::draw(line,colour,intensity);
	}
    int draw(vector <ofPolyline> &lines,ofColor colour=ofColor(255,255,255),int intensity=255){
		return ofxHelios::draw(lines,colour,intensity);
	}
    int draw(vector <colourPolyline> &lines, int intensity=255){
		return ofxHelios::draw(lines,intensity);
	}
private:
		ofParameter<bool> laser_power;
        ofParameter<int> laser_intensity;
        ofParameter<int> laser_points;
        ofParameter<int> laser_subdivide;
        ofParameter<int> laser_blank_num;
    	ofParameter<float> laser_max_angle;
};