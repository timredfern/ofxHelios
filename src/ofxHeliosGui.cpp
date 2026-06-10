#include "ofxHeliosGui.h"

void ofxHeliosGui::setup(ofxHelios& laser, int x, int y) {
	laser_ = &laser;
	ofxPanel::setup("laser", "", x, y);
	add(power_);
	add(intensity_);
	add(points_);
	add(subdivide_);
	add(blankCount_);
	add(maxAngle_);
}

void ofxHeliosGui::update() {
	if (!laser_) return;
	laser_->setPps(points_);
	laser_->setIntensity(intensity_);
	laser_->setSubdivide(subdivide_);
	laser_->setBlankCount(blankCount_);
	laser_->setMaxAngle(maxAngle_);
}

void ofxHeliosGui::drawGui() {
	ofxPanel::draw();
}

int ofxHeliosGui::draw(colourPolyline& line) {
	if (!power_) { lastPointCount_ = 0; return 0; }
	lastPointCount_ = laser_->draw(line);
	return lastPointCount_;
}

int ofxHeliosGui::draw(ofPolyline& line, ofColor colour) {
	if (!power_) { lastPointCount_ = 0; return 0; }
	lastPointCount_ = laser_->draw(line, colour);
	return lastPointCount_;
}

int ofxHeliosGui::draw(std::vector<colourPolyline>& lines) {
	if (!power_) { lastPointCount_ = 0; return 0; }
	lastPointCount_ = laser_->draw(lines);
	return lastPointCount_;
}

int ofxHeliosGui::draw(std::vector<ofPolyline>& lines, ofColor colour) {
	if (!power_) { lastPointCount_ = 0; return 0; }
	lastPointCount_ = laser_->draw(lines, colour);
	return lastPointCount_;
}

bool ofxHeliosGui::isPowered() const {
	return power_;
}

int ofxHeliosGui::getLastPointCount() const {
	return lastPointCount_;
}
