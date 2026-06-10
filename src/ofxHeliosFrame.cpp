#include "ofxHeliosFrame.h"
#include <algorithm>
#include <cmath>

namespace ofxHeliosFrame {

static uint16_t clamp16(float v) {
	if (v < 0.0f) return 0;
	if (v > 65535.0f) return 65535;
	return (uint16_t)v;
}

HeliosPointHighRes screenToLaser(
	const glm::vec2& screenPoint,
	float scale,
	const glm::vec2& centreOffset,
	int screenWidth,
	int screenHeight,
	const ofColor& colour,
	float intensity)
{
	float cx = (screenPoint.x - screenWidth * 0.5f + centreOffset.x) * scale + 0x7FFF;
	float cy = (screenPoint.y - screenHeight * 0.5f + centreOffset.y) * scale + 0x7FFF;

	HeliosPointHighRes p;
	p.x = clamp16(cx);
	p.y = clamp16(cy);
	p.r = clamp16(colour.r / 255.0f * intensity * 65535.0f);
	p.g = clamp16(colour.g / 255.0f * intensity * 65535.0f);
	p.b = clamp16(colour.b / 255.0f * intensity * 65535.0f);
	return p;
}

HeliosPointHighRes screenToLaserBlank(
	const glm::vec2& screenPoint,
	float scale,
	const glm::vec2& centreOffset,
	int screenWidth,
	int screenHeight)
{
	float cx = (screenPoint.x - screenWidth * 0.5f + centreOffset.x) * scale + 0x7FFF;
	float cy = (screenPoint.y - screenHeight * 0.5f + centreOffset.y) * scale + 0x7FFF;

	HeliosPointHighRes p;
	p.x = clamp16(cx);
	p.y = clamp16(cy);
	p.r = 0;
	p.g = 0;
	p.b = 0;
	return p;
}

std::vector<HeliosPointHighRes> buildFrame(
	const std::vector<colourPolyline>& lines,
	const BuildParams& params,
	BuildState& state)
{
	std::vector<HeliosPointHighRes> points;
	points.reserve(params.maxPoints);

	float scale = 65535.0f / std::max(params.screenWidth, params.screenHeight);

	// Initialise prev_point to screen centre on first call
	if (!state.initialised) {
		state.prevPoint = glm::vec2(params.screenWidth * 0.5f, params.screenHeight * 0.5f);
		state.initialised = true;
	}

	for (const auto& line : lines) {
		if (line.size() < 2) continue;

		glm::vec2 firstPt(line[0].x, line[0].y);

		// Blank travel from previous point to first vertex of this shape
		float dist = glm::distance(state.prevPoint, firstPt);
		if (dist > params.subdivide) {
			for (float j = 0; j < dist; j += params.subdivide) {
				float amt = j / dist;
				glm::vec2 interpPt = glm::mix(state.prevPoint, firstPt, amt);
				points.push_back(screenToLaserBlank(
					interpPt, scale, params.outputCentre,
					params.screenWidth, params.screenHeight));
				if ((int)points.size() >= params.maxPoints) goto done;
			}
		}

		// Blank dwell at first vertex
		for (int k = 0; k < params.blankCount; k++) {
			points.push_back(screenToLaserBlank(
				firstPt, scale, params.outputCentre,
				params.screenWidth, params.screenHeight));
			if ((int)points.size() >= params.maxPoints) goto done;
		}

		// Walk each segment
		for (int i = 0; i < (int)line.size() - 1; i++) {
			glm::vec2 p1(line[i].x, line[i].y);
			glm::vec2 p2(line[i + 1].x, line[i + 1].y);
			ofColor c1 = line.getColourAt(i);
			ofColor c2 = line.getColourAt(i + 1);

			float segDist = glm::distance(p1, p2);

			// Interpolated points along segment
			for (float j = 0; j < segDist; j += params.subdivide) {
				float amt = j / segDist;
				glm::vec2 interpPt = glm::mix(p1, p2, amt);
				ofColor interpCol(
					(uint8_t)(c1.r * (1.0f - amt) + c2.r * amt),
					(uint8_t)(c1.g * (1.0f - amt) + c2.g * amt),
					(uint8_t)(c1.b * (1.0f - amt) + c2.b * amt));

				points.push_back(screenToLaser(
					interpPt, scale, params.outputCentre,
					params.screenWidth, params.screenHeight,
					interpCol, params.intensity));
				if ((int)points.size() >= params.maxPoints) goto done;
			}

			// Dwell points at corners or end of polyline
			float angle = line.getDegreesAtIndex(i);
			if (angle > params.maxAngle || i == (int)line.size() - 2) {
				int dwellCount = (int)((angle / 180.0f) * params.blankCount);
				for (int l = 0; l < dwellCount; l++) {
					points.push_back(screenToLaser(
						p2, scale, params.outputCentre,
						params.screenWidth, params.screenHeight,
						c2, params.intensity));
					if ((int)points.size() >= params.maxPoints) goto done;
				}
			}

			state.prevPoint = p2;
			state.prevColour = c2;
		}

		// Trailing dwell at last point (lit, not blank)
		for (int k = 0; k < params.blankCount; k++) {
			points.push_back(screenToLaser(
				state.prevPoint, scale, params.outputCentre,
				params.screenWidth, params.screenHeight,
				state.prevColour, params.intensity));
			if ((int)points.size() >= params.maxPoints) goto done;
		}
	}

done:
	return points;
}

} // namespace ofxHeliosFrame
