#include "ofApp.h"

/*
    example-fractalzoom

    Nested pentagons that zoom in continuously, creating an infinite
    fractal tunnel effect. Each level is scaled by 1/phi (golden ratio)
    and rotated 36 degrees. The zoom loops seamlessly because every
    level is self-similar.

    Colours cycle through the spectrum per level.
*/

static const float PHI = (1.0f + sqrt(5.0f)) / 2.0f;
static const int NUM_LEVELS = 10;
static const int SIDES = 5;
static const float BASE_RADIUS = 450.0f;

colourPolyline makePentagon(float radius, float rotation, ofColor colour) {
	colourPolyline poly;
	float step = TWO_PI / SIDES;
	for (int i = 0; i <= SIDES; i++) {
		float angle = step * i + rotation;
		poly.addVertex(
			cos(angle) * radius,
			sin(angle) * radius,
			colour);
	}
	return poly;
}

void ofApp::setup() {
	laser.setup();
	laser.setPps(40000);
	laser.setIntensity(0.5f);
	laser.setSubdivide(24);
	laser.setBlankCount(12);
	laserGui.setup(laser);
}

void ofApp::update() {
	laserGui.update();
}

void ofApp::draw() {
	ofBackground(0);




	float cx = ofGetWidth() / 2.0f;
	float cy = ofGetHeight() / 2.0f;

	// Zoom cycles over log(phi) so it loops seamlessly
	float t = ofGetElapsedTimef() * 0.4f;
	float zoomPhase = fmod(t, 1.0f);
	float zoom = pow(PHI, zoomPhase);

	std::vector<colourPolyline> lines;

	for (int level = -1; level < NUM_LEVELS; level++) {
		// Each level is 1/phi smaller and rotated 36 degrees
		float levelScale = pow(1.0f / PHI, level) * zoom;
		float radius = BASE_RADIUS * levelScale;

		// Skip levels that are too large or too small to see
		if (radius > 1200.0f || radius < 5.0f) continue;

		// depth is continuous across the wrap point:
		// at phase=1, level N has depth N-1, matching level N-1 at phase=0
		float depth = level - zoomPhase;

		float rotation = depth * glm::radians(36.0f);

		// Colour cycles through hue per depth level
		float hue = fmod(depth * 47.0f + 2550.0f, 255.0f);
		ofColor colour;
		colour.setHsb(hue, 230, 255);

		// Fade out levels that are very large (about to exit) or very small
		float fade = 1.0f;
		if (radius > 800.0f) fade = ofMap(radius, 800.0f, 1200.0f, 1.0f, 0.0f, true);
		if (radius < 30.0f) fade = ofMap(radius, 5.0f, 30.0f, 0.0f, 1.0f, true);
		colour.r *= fade;
		colour.g *= fade;
		colour.b *= fade;

		colourPolyline pent = makePentagon(radius, rotation, colour);

		// Translate to screen centre
		ofMatrix4x4 m;
		m.makeIdentityMatrix();
		m.translate(cx, cy, 0);
		pent.transform(m);

		pent.draw();
		lines.push_back(pent);
	}

	int num = laser.draw(lines);



	ofSetColor(255);
	ofDrawBitmapString(
		ofToString(laserGui.getLaserFps(), 1) + " laser fps  points: " + ofToString(num),
		20, 20);

	laserGui.draw();

}
