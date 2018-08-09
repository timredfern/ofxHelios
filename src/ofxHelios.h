//
//  ofxHelios.h
// 
//
//  Created by Tim Redfern Nov 2017
// 
// it would be good if ofxHelios could inherit the current transform   
// it would good if it could be resoution independent
//                      

#ifndef ofxHelios_h
#define ofxHelios_h
#define OFXHELIOS_VERSION 0.1
#define OFXHELIOS_NODEVICE -1
#include "ofMain.h"
#include "colourPolyline.h"

#define SUBDIVIDE 15
#define BLANK_NUM 8
#define MAX_ANGLE 15.0f

#include <HeliosDac.h>

class ofxHelios : public ofThread
{
public:
    
    ofxHelios(int _pps=20000,int _device = 0,int subdivide=SUBDIVIDE,int blank_num=BLANK_NUM,int max_angle=MAX_ANGLE)
    {
        int numdevices=dac.OpenDevices();
        for (int i=0;i<numdevices;i++){
            ofLogNotice() << "ofxHelios v "<<OFXHELIOS_VERSION<<": found laser DAC: firmware v "<<dac.GetFirmwareVersion(i);
        }
        if (!numdevices){
            ofLogNotice() << "ofxHelios v "<<OFXHELIOS_VERSION<<": no devices found";

        }
        if (_device>=numdevices){
            ofLogNotice() << "ofxHelios v "<<OFXHELIOS_VERSION<<": could not open device "<<_device;
            device=OFXHELIOS_NODEVICE;
        }
        else {
            device=_device;
            pps=_pps;
            //dac.SetShutter(device,true);
        }
        output_centre=ofPoint(0x7ff,0x7ff);
        prev_point=output_centre;
    }
    
    ~ofxHelios()
    {
        stopThread();
        dac.CloseDevices();
    }

    void set_centre(ofPoint c){
        ofPoint cp=c+ofPoint(0x7ff,0x7ff);
        if (cp!=output_centre){
            output_centre=cp;
            ofLogNotice() << "ofxHelios v "<<OFXHELIOS_VERSION<<": set output centre to "<<output_centre.x<<","<<output_centre.y;
        }
    }
    void set_pts(int n){
        if (n!=pps){
            pps=n;
            ofLogNotice() << "ofxHelios v "<<OFXHELIOS_VERSION<<": set point output to "<<pps;
        }
    }
    void set_intensity(int i){
        if (i!=laserintensity){
            laserintensity=i;
            ofLogNotice() << "ofxHelios v "<<OFXHELIOS_VERSION<<": set intensity to "<<laserintensity;
        }
    }
    void set_subdivide(int i){
        if (i!=subdivide){
            subdivide=i;
            ofLogNotice() << "ofxHelios v "<<OFXHELIOS_VERSION<<": set subdivide to "<<subdivide;
        }
    }
    void set_blanknum(int i){
        if (i!=blank_num){
            blank_num=i;
            ofLogNotice() << "ofxHelios v "<<OFXHELIOS_VERSION<<": set blank_num to "<<blank_num;
        }
    }
    void set_maxangle(float f){
        if (f!=max_angle){
            max_angle=f;
            ofLogNotice() << "ofxHelios v "<<OFXHELIOS_VERSION<<": set max_angle to "<<max_angle;
        }
    }
    int get_pts(){
        return pps;
    }
    int draw(colourPolyline &line, int intensity=255);
    int draw(ofPolyline &line,ofColor colour=ofColor(255,255,255),int intensity=255);
    int draw(vector <ofPolyline> &lines,ofColor colour=ofColor(255,255,255),int intensity=255);
    int draw(vector <colourPolyline> &lines, int intensity=255);

    void threadedFunction();

    //isReady()
    
    private:

        int device;
        HeliosDac dac;
        int pps;
        int laserintensity;

        int subdivide,blank_num;
        float max_angle;

        ofPoint output_centre;
        ofPoint prev_point;
        ofColor prev_colour;
    
};

#endif
