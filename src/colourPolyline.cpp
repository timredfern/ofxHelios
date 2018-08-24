#include "colourPolyline.h"

vector <ofDefaultVertexType> colourPolyline::getTransformedVerts(ofMatrix4x4 xform){
	auto verts=getVertices();
	vector <ofDefaultVertexType> temp;
    for (auto& v:verts){
        temp.push_back(ofVec3f(v)*xform);
    }
    return temp;
}

void colourPolyline::transform(ofMatrix4x4 xform){
	auto temp=getTransformedVerts(xform);
    clear();
    addVertices(temp);
}

colourPolyline colourPolyline::getTransformed(ofMatrix4x4 xform){
	colourPolyline transformed = *this;
	transformed.transform(xform);
	return transformed;
}

void colourPolyline::addVertex( float x, float y){
	addVertex(ofPoint(x,y));
}

void colourPolyline::addVertex( const ofPoint& p ){
	addVertex(p, ofColor(255,255,255));
}

void colourPolyline::addVertex( float x, float y, const ofColor& c){
	addVertex(ofPoint(x,y),c);
}

void colourPolyline::addVertex( float x, float y, int r, int g, int b){
	addVertex(ofPoint(x,y),ofColor(r,g,b));
}

void colourPolyline::addVertex( const ofPoint& p , int r, int g, int b){
	addVertex(p,ofColor(r,g,b));
}

void colourPolyline::addVertex( const ofPoint& p , const ofColor& c){
	ofPolyline::addVertex(p);
	pointColours.push_back(c);
}

void colourPolyline::setColour(const ofColor& c){
	for (auto i=pointColours.begin();i!=pointColours.end();i++){
		*i=c;
	}
}

const ofColor colourPolyline::getColourAt(const int i){
	return pointColours[i];
}

void colourPolyline::draw(){
	for (int i=0;i<ofPolyline::size()-1;i++){
		ofSetColor(pointColours[i]);
		ofPolyline l;
		l.addVertex(ofPolyline::operator[](i)); 
		l.addVertex(ofPolyline::operator[](i+1));
		l.draw();
		//todo - cache
	}
}

void colourPolyline::mix(colourPolyline& poly,float amount){
	for (int i=0;i<min(poly.size(),ofPolyline::size());i++){
		ofPoint p1=ofPolyline::operator[](i);
		ofPolyline::operator[](i)=ofPoint(
			(p1.x*(1.0f-amount))+(poly[i].x*amount),
			(p1.y*(1.0f-amount))+(poly[i].y*amount)
			);
		pointColours[i]=(poly.pointColours[i]*amount)+
			(pointColours[i]*(1.0f-amount));
	}
}