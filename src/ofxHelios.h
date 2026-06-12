//
//  ofxHelios.h
//
//  Created by Tim Redfern Nov 2017
//  Rewritten 2026 - v0.2.0
//

#pragma once

#define OFXHELIOS_VERSION "0.2.0"

#include "ofMain.h"
#include "colourPolyline.h"
#include "ofxHeliosFrame.h"
#include <HeliosDac.h>
#include <atomic>
#include <mutex>
#include <condition_variable>

class ofxHelios : public ofThread
{
public:

	ofxHelios();
	~ofxHelios();

	// Device lifecycle.
	int scan();           // scan all, return device count
	int scanUsb();        // scan USB only
	int scanNetwork();    // scan network only
	bool connect(int deviceIndex = 0);  // connect to a scanned device

	int setup(int deviceIndex = 0);     // scan + connect (convenience)
	int setupUsb(int deviceIndex = 0);
	int setupNetwork(int deviceIndex = 0);
	void close();
	bool isConnected() const;

	// Raw frame submission. Non-blocking: swaps points into back-buffer
	// for the background thread to send. Returns point count or -1 on error.
	int sendFrame(std::vector<HeliosPointHighRes>& points);

	// Convenience: build frame from polylines and submit.
	int draw(colourPolyline& line);
	int draw(ofPolyline& line, ofColor colour = ofColor(255, 255, 255));
	int draw(std::vector<colourPolyline>& lines);
	int draw(std::vector<ofPolyline>& lines, ofColor colour = ofColor(255, 255, 255));

	// Parameters
	void setPps(int pps);
	int getPps() const;
	void setIntensity(float intensity);  // 0.0 - 1.0
	float getIntensity() const;
	void setSubdivide(int n);
	int getSubdivide() const;
	void setBlankCount(int n);
	int getBlankCount() const;
	void setMaxAngle(float degrees);
	float getMaxAngle() const;
	void setOutputCentre(glm::vec2 c);
	glm::vec2 getOutputCentre() const;
	void setOutputScale(float s);
	float getOutputScale() const;

	// Device queries
	int getNumDevices() const;
	bool isDeviceUsb();
	bool isDeviceHighRes();
	std::string getDeviceName();
	std::string getDeviceName(int index);
	int getFirmwareVersion();
	int getMaxPoints();
	int getMaxPps();
	int getLastPointCount() const;
	float getLaserFps() const;

private:

	void threadedFunction() override;
	int scanDevices(int mode);
	int openDevices(int mode, int deviceIndex);

	HeliosDac dac;
	int deviceIndex = -1;
	int numDevices = 0;
	std::atomic<bool> connected{false};
	std::vector<std::string> deviceNames_;

	// Double buffer for thread communication
	std::vector<HeliosPointHighRes> frontBuffer;
	std::vector<HeliosPointHighRes> backBuffer;
	std::mutex bufferMutex;
	std::condition_variable frameCondition;
	std::atomic<bool> newFrameAvailable{false};

	// Parameters (pps is atomic - read by background thread)
	std::atomic<int> pps_{30000};
	float intensity_ = 1.0f;
	int subdivide_ = 15;
	int blankCount_ = 8;
	float maxAngle_ = 15.0f;
	glm::vec2 outputCentre_{0, 0};
	float outputScale_ = 1.0f;

	// Frame building state (main thread only)
	ofxHeliosFrame::BuildState buildState;

	std::atomic<int> lastPointCount_{0};
	std::atomic<float> measuredFps_{0.0f};
};
