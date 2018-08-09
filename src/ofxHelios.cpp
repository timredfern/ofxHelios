//
//  ofxHelios.cpp
// 
//
//  Created by Tim Redfern Nov 2017
// 
//

#include "ofxHelios.h"


int ofxHelios::draw(vector <ofPolyline> &lines,ofColor colour,int intensity){
    vector <colourPolyline> output;
    for (auto& line:lines){
        output.push_back(colourPolyline(line,colour));
    }
    return draw(output,intensity);
}

int ofxHelios::draw(ofPolyline &line,ofColor colour,int intensity){

    colourPolyline col=colourPolyline(line,colour);
    return draw(col,intensity);

}

int ofxHelios::draw(colourPolyline &line, int intensity){
    vector <colourPolyline> lines;
    lines.push_back(line);
    return draw(lines,intensity);
}

int ofxHelios::draw(vector <colourPolyline> &lines, int intensity){
    //todo: move to a thread
    //todo: inherit transform

    int xoffs=output_centre.x-(ofGetWidth()/2);
    int yoffs=output_centre.y-(ofGetHeight()/2);

    if (device!=OFXHELIOS_NODEVICE){
        while (!dac.GetStatus(device)); //timeout for this?

        vector <HeliosPoint> points;

        for (auto& line:lines){

            float dist=abs(prev_point.distance(line[0]));

            if (dist>SUBDIVIDE){
                //draw blanking points if required (only between shapes)

                for (float j=0;j<dist;j+=SUBDIVIDE){
                    float amt=j/dist;
                    points.push_back(HeliosPoint(
                        (uint16_t)(((prev_point.x*(1.0-amt))+(line[0].x*amt)+xoffs)),
                        (uint16_t)(((prev_point.y*(1.0-amt))+(line[0].y*amt)+yoffs)),
                        0,0,0,0)); //blank point
                }
            }
            for (int k=0;k<BLANK_NUM;k++){
                points.push_back(HeliosPoint(
                    (uint16_t)(line[0].x+xoffs),
                    (uint16_t)(line[0].y+yoffs),
                    0,0,0,0)); //blank point
            }
            int i;
            for (i=0;i<line.size()-1;i++){
                float dist=abs(ofPoint(line[i]).distance(ofPoint(line[i+1])));
                for (float j=0;j<dist;j+=SUBDIVIDE){

                    //draw way points
                    float amt=j/dist;
                    points.push_back(HeliosPoint(
                        (uint16_t)((line[i].x*(1.0-amt))+(line[i+1].x*amt)+xoffs),
                        (uint16_t)((line[i].y*(1.0-amt))+(line[i+1].y*amt)+yoffs),
                        (uint8_t)((((line.getColourAt(i).r*(1.0-amt))+(line.getColourAt(i+1).r*amt))*laserintensity)/255.0),
                        (uint8_t)((((line.getColourAt(i).g*(1.0-amt))+(line.getColourAt(i+1).g*amt))*laserintensity)/255.0),
                        (uint8_t)((((line.getColourAt(i).b*(1.0-amt))+(line.getColourAt(i+1).b*amt))*laserintensity)/255.0),
                        (uint8_t)intensity)
                    );
                }
                float angle=line.getDegreesAtIndex(i);
                if (angle>MAX_ANGLE||(i==line.size()-2)){

                    //dwell points to wait on a corner for laser to catch up
                    for (int l=0;l<((angle/180)*BLANK_NUM);l++){
                        points.push_back(HeliosPoint(
                            (uint16_t)(line[i+1].x+xoffs),
                            (uint16_t)(line[i+1].y+yoffs),
                            (uint8_t)(line.getColourAt(i+1).r*laserintensity/255.0),
                            (uint8_t)(line.getColourAt(i+1).g*laserintensity/255.0),
                            (uint8_t)(line.getColourAt(i+1).b*laserintensity/255.0),
                            (uint8_t)intensity)
                            );
                    }
                }
                prev_point=line[i+1];
                prev_colour=line.getColourAt(i+1);
            }
            
            for (int k=0;k<BLANK_NUM;k++){
                points.push_back(HeliosPoint(
                    (uint16_t)(prev_point.x+xoffs),
                    (uint16_t)(prev_point.y+yoffs),
                    (uint8_t)((prev_colour.r*laserintensity)/255.0),
                    (uint8_t)((prev_colour.g*laserintensity)/255.0),
                    (uint8_t)((prev_colour.b*laserintensity)/255.0),
                    (uint8_t)intensity)
                );    
            }
            
        }

        for (auto& p:points){ //clip scale
            p.x=min((uint16_t)0xfff,p.x);
            p.y=min((uint16_t)0xfff,p.y);
        }

        if (HELIOS_ERROR==dac.WriteFrame(device, pps, HELIOS_FLAGS_DEFAULT, &points[0], min(HELIOS_MAX_POINTS,(int)points.size()))){
            printf("ofxHelios: write error (%i,%i,%i,%i)\n",device, pps, HELIOS_FLAGS_DEFAULT, (int)points.size());
            return -1;
        }

        return points.size();
      
    }
    return -2;
}


    

void ofxHelios::threadedFunction(){

    while(isThreadRunning()) {
         
    }
}

