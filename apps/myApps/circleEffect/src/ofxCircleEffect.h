#pragma once
#include "ofMain.h"
#include "ofThread.h"
#include "ofxRipples.h"
#include "ofxScene.h"

class ofxCircleEffect : public ofxScene
{		
public:	
	
	void draw();	
	void update();

	ofxCircleEffect(int _projW, int _projH);
	~ofxCircleEffect();
protected:
	ofxRipples  rip;
};

