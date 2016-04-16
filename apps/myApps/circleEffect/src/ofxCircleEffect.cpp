#include "ofxCircleEffect.h"

ofxCircleEffect::ofxCircleEffect(int _projW, int _projH) : ofxScene(_projW, _projH)
{
	rip.allocate(projectorWidth, projectorHeight);
}


ofxCircleEffect::~ofxCircleEffect()
{
}



void ofxCircleEffect::draw()
{	
	ofBackground(0);
	ofSetColor(255,255);

	rip.draw(0,0);
	ofDrawBitmapString("ofxRipples ( damping = " + ofToString(rip.damping) + " )", 15,15);
}

void ofxCircleEffect::update()
{
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	rip.begin();
	ofFill();
	ofSetColor(ofNoise( ofGetFrameNum() ) * 255 * 5, 255);
	ofEllipse(touchX, touchY, 10,10);
	rip.end();
	rip.update();
}

