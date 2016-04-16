#pragma once
#include "ofMain.h"
#include "ofThread.h"
#include "ofEvents.h"
#include "ofxOpenNI.h"
#include "ofxKinectProjectorToolkit.h"
#include "ofxCvBlobTracker.h"
#include "ofxQuadNodeProcessorByIntegral.h"
#include <omp.h>

class ofxKinectHandler : public ofThread, public ofxCvBlobListener
{		
public:
	void start()
	{
		startThread();
	}

	void stop()
	{
		stopThread();
	}

	void threadedFunction();
	void draw();
	void setup();
	void setListener(ofxCvTouchListener* _listener)
	{
		listener = _listener;
	}

	void blobOn( const ofxCvTrackedBlob& b );  
	void blobMoved( const ofxCvTrackedBlob& b );      
	void blobOff( const ofxCvTrackedBlob& b ); 

	ofxKinectHandler();
	~ofxKinectHandler()
	{
		if(listener)
			delete listener;
		if(quadTree)
			delete quadTree;
		//if(quadTreeProcessor)
			//delete quadTreeProcessor;
	}

protected:	
	int num;//debug
	bool							bLearnBackground;
	int								depthWidth, depthHeight;
	//ofxQuadNodeProcessorByIntegral*	quadTreeProcessor;
	unsigned int*					integralImage;
	int								integralStep;
	ofxQuadNode*					quadTree;
	ofShortPixels					backgroundDepth;
	ofxCvGrayscaleImage				diffDepth;
	ofxCvContourFinder				contourFinder;
	ofxCvBlobTracker				blobTracker;
	ofxOpenNI						kinect;			//Depth camera
	ofxKinectProjectorToolkit		kpt;
	ofxCvTouchListener*				listener;
};


