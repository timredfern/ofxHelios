#pragma once
#include "ofPolyline.h"
#include "ofMain.h"


class colourPolyline: public ofPolyline {
	public:

		colourPolyline(){}

		colourPolyline(const ofPolyline line,const ofColor colour=ofColor(255,255,255)){
			for (auto& point:line){
				addVertex(point, colour);
			}
		}

		void clear(){
			ofPolyline::clear();
			pointColours.clear();
		}

		vector <ofDefaultVertexType> getTransformedVerts(ofMatrix4x4 xform);
		
		colourPolyline getTransformed(ofMatrix4x4 xform);

		void transform(ofMatrix4x4 xform);

		void addVertex( float x, float y);

		void addVertex( const ofPoint& p );

		void addVertex( float x, float y, const ofColor& c);

		void addVertex( float x, float y, int r, int g, int b);

		void addVertex( const ofPoint& p , int r, int g, int b);

		void addVertex( const ofPoint& p , const ofColor& c);

		void setColour(const ofColor& c);

		void draw();

		const ofColor getColourAt(const int i);

		void mix(colourPolyline& poly,float amount=1.0f);

	private:

		vector <ofColor> pointColours;

};