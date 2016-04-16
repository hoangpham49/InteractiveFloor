#include "ofxWaterEffect.h"

ofxWaterEffect::ofxWaterEffect(int _projW, int _projH) : ofxScene(_projW, _projH)
{
	bounce.allocate(projectorWidth, projectorHeight);
	rip.allocate(projectorWidth, projectorHeight);
	ofImage background;
    background.loadImage("fondo.jpg");
	background.resize(projectorWidth, projectorHeight);
	bounce.setTexture(background.getTextureReference(), 1);
}


ofxWaterEffect::~ofxWaterEffect()
{
}



void ofxWaterEffect::draw()
{	
	ofBackground(0);
	ofSetColor(255,255);

	bounce.draw(0,0);
	ofDrawBitmapString("ofxBounce", 15,15);
}

void ofxWaterEffect::update()
{
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	rip.begin();
    ofFill();
    ofSetColor(ofNoise( ofGetFrameNum() ) * 255 * 5, 255);
	ofEllipse(touchX, touchY, 10,10);
    rip.end();
    rip.update();
	
	bounce << rip;
}

