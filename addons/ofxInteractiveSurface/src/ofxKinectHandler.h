#pragma once
#include "ofMain.h"
#include "ofThread.h"
#include "ofEvents.h"
#include "ofxKinectNui.h"
#include "ofxCvBlobTracker.h"


class ofxKinectHandler : public ofThread
{		
public:
	ofEvent<ofxCvTrackedBlob> objectAdded;
	ofEvent<ofxCvTrackedBlob> objectMoved;
	ofEvent<ofxCvTrackedBlob> objectDeleted;

	void start() { 
		//startThread(); 
	}
	void stop()  { 
		//stopThread();  
	}

	void threadedFunction();
	void update();
	void draw();
	void setup();
	void clear();

	void _objectAdded(ofxCvTrackedBlob &_blob);
	void _objectMoved(ofxCvTrackedBlob &_blob);
	void _objectDeleted(ofxCvTrackedBlob &_blob);


	ofxKinectHandler();
	~ofxKinectHandler();
	
protected:	
	int								num;//debug
	bool							bLearnBackground;
	int								depthWidth, depthHeight;
	int								nearDistance, farDistance;
		
	ofShortPixels					backgroundDepth, depthImage;
	ofxCvGrayscaleImage				diffDepth;
	ofxCvContourFinder				contourFinder;
	ofxCvBlobTracker				blobTracker;
	ofxKinectNui					kinect;			//Depth camera
	
	

	
};


