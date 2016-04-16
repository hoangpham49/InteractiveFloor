#pragma once
#include "ofMain.h"
#include "ofThread.h"
#include "ofxBounce.h"
#include "ofxRipples.h"
#include "ofxScene.h"

class ofxWaterEffect : public ofxScene
{		
public:	
	
	void draw();	
	void update();

	ofxWaterEffect(int _projW, int _projH);
	~ofxWaterEffect();
protected:
	ofxBounce   bounce;
	ofxRipples  rip;
};

