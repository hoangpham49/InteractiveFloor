#pragma once
#include "ofMain.h"
#include "ofThread.h"
#include "ofxCvBlobTracker.h"
#include "ofxKinectProjectorToolkit.h"

class ofxScene : public ofThread
{		
public:	
		
	void setup(int _projW, int _projH);

	void touchOn(ofxCvTrackedBlob &b);
	void touchMoved(ofxCvTrackedBlob &b);
	void touchOff(ofxCvTrackedBlob &b);

	void start() { 
		//startThread(); 
	}
	void stop()  { 
		//stopThread();  
	}

	void threadedFunction();

	virtual void draw() = 0;
	virtual void update() = 0;
	

	ofxScene(int _projW, int _projH);
	~ofxScene();
protected:
	int touchX, touchY;
	int projectorWidth, projectorHeight;	
	ofxKinectProjectorToolkit kpt;
};

