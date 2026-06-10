#pragma once

#include "ofMain.h"
#include "colourPolyline.h"
#include <HeliosDac.h>
#include <vector>

namespace ofxHeliosFrame {

	struct BuildParams {
		int subdivide = 15;        // screen-pixel distance between interpolated points
		int blankCount = 8;        // blank/dwell points at shape boundaries
		float maxAngle = 15.0f;    // degrees; corners sharper than this get dwell points
		float intensity = 1.0f;    // 0.0-1.0 global multiplier, baked into RGB
		glm::vec2 outputCentre{0, 0};  // screen-space offset from centre
		int screenWidth = 1024;
		int screenHeight = 768;
		int maxPoints = HELIOS_MAX_POINTS;
	};

	struct BuildState {
		glm::vec2 prevPoint{0, 0};     // screen coords, persists between frames
		ofColor prevColour{0, 0, 0};
		bool initialised = false;
	};

	// Convert polylines to laser points. Updates state for cross-frame blanking.
	std::vector<HeliosPointHighRes> buildFrame(
		const std::vector<colourPolyline>& lines,
		const BuildParams& params,
		BuildState& state);

	// Map a screen-space point to a 16-bit DAC coordinate.
	// Screen centre maps to DAC centre (0x7FFF). The larger screen dimension
	// fills the full DAC range, preserving aspect ratio.
	HeliosPointHighRes screenToLaser(
		const glm::vec2& screenPoint,
		float scale,
		const glm::vec2& centreOffset,
		int screenWidth,
		int screenHeight,
		const ofColor& colour,
		float intensity);

	// Blank version (colour = 0)
	HeliosPointHighRes screenToLaserBlank(
		const glm::vec2& screenPoint,
		float scale,
		const glm::vec2& centreOffset,
		int screenWidth,
		int screenHeight);
}
