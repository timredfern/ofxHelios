//
//  ofxHelios.cpp
//
//  Created by Tim Redfern Nov 2017
//  Rewritten 2026 - v0.2.0
//

#include "ofxHelios.h"
#include <algorithm>
#include <chrono>

ofxHelios::ofxHelios() {}

ofxHelios::~ofxHelios() {
	if (isThreadRunning()) {
		stopThread();
		frameCondition.notify_all();
		waitForThread(true, 2000);
	}
	if (connected) {
		dac.Stop(deviceIndex);
		dac.CloseDevices();
	}
}

// mode: 0 = all, 1 = USB only, 2 = network only
int ofxHelios::openDevices(int mode, int devIdx) {
	int count = 0;
	switch (mode) {
		case 1:  count = dac.OpenDevicesOnlyUsb(); break;
		case 2:  count = dac.OpenDevicesOnlyNetwork(); break;
		default: count = dac.OpenDevices(); break;
	}

	numDevices = count;

	for (int i = 0; i < count; i++) {
		char name[32] = {0};
		dac.GetName(i, name);
		ofLogNotice("ofxHelios") << "v" << OFXHELIOS_VERSION
			<< " found DAC " << i << ": " << name
			<< " (firmware v" << dac.GetFirmwareVersion(i)
			<< ", " << (dac.GetIsUsb(i) ? "USB" : "network") << ")";
	}

	if (count == 0) {
		ofLogNotice("ofxHelios") << "v" << OFXHELIOS_VERSION << " no devices found";
		return 0;
	}

	if (devIdx >= count) {
		ofLogError("ofxHelios") << "device index " << devIdx
			<< " out of range (found " << count << " devices)";
		return count;
	}

	deviceIndex = devIdx;
	connected = true;
	startThread();

	ofLogNotice("ofxHelios") << "connected to device " << deviceIndex;
	return count;
}

int ofxHelios::setup(int devIdx) {
	return openDevices(0, devIdx);
}

int ofxHelios::setupUsb(int devIdx) {
	return openDevices(1, devIdx);
}

int ofxHelios::setupNetwork(int devIdx) {
	return openDevices(2, devIdx);
}

void ofxHelios::close() {
	if (isThreadRunning()) {
		stopThread();
		frameCondition.notify_all();
		waitForThread(true, 2000);
	}
	if (connected) {
		dac.Stop(deviceIndex);
		dac.CloseDevices();
		connected = false;
		deviceIndex = -1;
		numDevices = 0;
	}
}

bool ofxHelios::isConnected() const {
	return connected;
}

// --- Frame submission ---

int ofxHelios::sendFrame(std::vector<HeliosPointHighRes>& points) {
	if (!connected) return -1;
	if (points.empty()) return 0;

	int count = (int)points.size();

	{
		std::lock_guard<std::mutex> lock(bufferMutex);
		std::swap(backBuffer, points);
		newFrameAvailable = true;
	}
	frameCondition.notify_one();

	return count;
}

// --- Convenience draw methods ---

int ofxHelios::draw(colourPolyline& line) {
	std::vector<colourPolyline> lines;
	lines.push_back(line);
	return draw(lines);
}

int ofxHelios::draw(ofPolyline& line, ofColor colour) {
	colourPolyline col(line, colour);
	return draw(col);
}

int ofxHelios::draw(std::vector<ofPolyline>& lines, ofColor colour) {
	std::vector<colourPolyline> output;
	output.reserve(lines.size());
	for (auto& line : lines) {
		output.emplace_back(line, colour);
	}
	return draw(output);
}

int ofxHelios::draw(std::vector<colourPolyline>& lines) {
	if (!connected) return -1;

	ofxHeliosFrame::BuildParams params;
	params.subdivide = subdivide_;
	params.blankCount = blankCount_;
	params.maxAngle = maxAngle_;
	params.intensity = intensity_;
	params.outputCentre = outputCentre_;
	params.outputScale = outputScale_;
	params.screenWidth = ofGetWidth();
	params.screenHeight = ofGetHeight();
	params.maxPoints = getMaxPoints();

	auto points = ofxHeliosFrame::buildFrame(lines, params, buildState);
	return sendFrame(points);
}

// --- Parameters ---

void ofxHelios::setPps(int pps) {
	pps_ = std::clamp(pps, (int)HELIOS_MIN_PPS, getMaxPps());
}

int ofxHelios::getPps() const {
	return pps_;
}

void ofxHelios::setIntensity(float intensity) {
	intensity_ = std::clamp(intensity, 0.0f, 1.0f);
}

float ofxHelios::getIntensity() const {
	return intensity_;
}

void ofxHelios::setSubdivide(int n) {
	subdivide_ = std::max(1, n);
}

int ofxHelios::getSubdivide() const {
	return subdivide_;
}

void ofxHelios::setBlankCount(int n) {
	blankCount_ = std::max(0, n);
}

int ofxHelios::getBlankCount() const {
	return blankCount_;
}

void ofxHelios::setMaxAngle(float degrees) {
	maxAngle_ = std::max(0.0f, degrees);
}

float ofxHelios::getMaxAngle() const {
	return maxAngle_;
}

void ofxHelios::setOutputCentre(glm::vec2 c) {
	outputCentre_ = c;
}

glm::vec2 ofxHelios::getOutputCentre() const {
	return outputCentre_;
}

void ofxHelios::setOutputScale(float s) {
	outputScale_ = std::clamp(s, 0.0f, 1.0f);
}

float ofxHelios::getOutputScale() const {
	return outputScale_;
}

// --- Device queries ---

int ofxHelios::getNumDevices() const {
	return numDevices;
}

bool ofxHelios::isDeviceUsb() {
	if (!connected) return false;
	return dac.GetIsUsb(deviceIndex) == 1;
}

bool ofxHelios::isDeviceHighRes() {
	if (!connected) return false;
	return dac.GetSupportsHigherResolutions(deviceIndex) == 1;
}

std::string ofxHelios::getDeviceName() {
	if (!connected) return "";
	char name[32] = {0};
	dac.GetName(deviceIndex, name);
	return std::string(name);
}

std::string ofxHelios::getDeviceName(int index) {
	if (index < 0 || index >= numDevices) return "";
	char name[32] = {0};
	dac.GetName(index, name);
	return std::string(name);
}

int ofxHelios::getFirmwareVersion() {
	if (!connected) return -1;
	return dac.GetFirmwareVersion(deviceIndex);
}

int ofxHelios::getMaxPoints() {
	if (!connected) return HELIOS_MAX_POINTS;
	return isDeviceUsb() ? HELIOS_MAX_POINTS : HELIOS_MAX_POINTS_IDN;
}

int ofxHelios::getMaxPps() {
	if (!connected) return HELIOS_MAX_PPS;
	return isDeviceUsb() ? HELIOS_MAX_PPS : HELIOS_MAX_PPS_IDN;
}

int ofxHelios::getLastPointCount() const {
	return lastPointCount_;
}

float ofxHelios::getLaserFps() const {
	return measuredFps_.load();
}

// --- Background thread ---

void ofxHelios::threadedFunction() {
	using namespace std::chrono;
	steady_clock::time_point lastWriteTime;
	bool hasLastWrite = false;

	while (isThreadRunning()) {
		// Wait for a new frame or periodic wakeup
		{
			std::unique_lock<std::mutex> lock(bufferMutex);
			frameCondition.wait_for(lock, milliseconds(50), [this] {
				return newFrameAvailable.load() || !isThreadRunning();
			});

			if (!isThreadRunning()) break;

			if (newFrameAvailable) {
				std::swap(frontBuffer, backBuffer);
				newFrameAvailable = false;
			}
		}

		if (frontBuffer.empty()) continue;

		// Poll GetStatus with timeout
		auto deadline = steady_clock::now() + milliseconds(100);
		bool ready = false;
		while (steady_clock::now() < deadline && isThreadRunning()) {
			int status = dac.GetStatus(deviceIndex);
			if (status == 1) {
				ready = true;
				break;
			}
			if (status < 0) {
				ofLogError("ofxHelios") << "GetStatus error: " << status;
				break;
			}
			std::this_thread::sleep_for(milliseconds(1));
		}

		if (!ready || !isThreadRunning()) continue;

		int numPoints = std::min((int)frontBuffer.size(), getMaxPoints());
		int err = dac.WriteFrameHighResolution(
			deviceIndex,
			pps_.load(),
			HELIOS_FLAGS_DEFAULT,
			frontBuffer.data(),
			numPoints);

		if (err < 0) {
			ofLogError("ofxHelios") << "WriteFrameHighResolution error: " << err;
		} else {
			lastPointCount_ = numPoints;
			auto now = steady_clock::now();
			if (hasLastWrite) {
				float dt = duration<float>(now - lastWriteTime).count();
				if (dt > 0.0f) {
					measuredFps_ = 1.0f / dt;
				}
			}
			lastWriteTime = now;
			hasLastWrite = true;
		}
	}
}
