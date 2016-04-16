#include "ofxKinectHandler.h"

ofxKinectHandler::ofxKinectHandler()
{
	setup();
}

ofxKinectHandler::~ofxKinectHandler()
{
	kinect.close();
	clear();
}

void ofxKinectHandler::clear()
{
	if(depthImage.isAllocated())
	{
		depthImage.clear();
	}
	if(backgroundDepth.isAllocated())
	{
		backgroundDepth.clear();
	}
	if(diffDepth.bAllocated)
	{
		diffDepth.clear();
	}
}

void ofxKinectHandler::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);	

	ofxKinectNui::InitSetting initSetting;	
	kinect.init(initSetting);
	kinect.setMirror(false);
	kinect.open();
	ofSetVerticalSync(true);

	if(kinect.isOpened())
	{
		depthWidth = 640, depthHeight = 480;
		diffDepth.allocate(depthWidth, depthHeight);

		nearDistance = 300;
		farDistance = 500;
		bLearnBackground = true;
		num = 0;	

		ofAddListener(blobTracker.blobOn, this, &ofxKinectHandler::_objectAdded);
		ofAddListener(blobTracker.blobMoved, this, &ofxKinectHandler::_objectMoved);
		ofAddListener(blobTracker.blobOff, this, &ofxKinectHandler::_objectDeleted);
	}
}

void ofxKinectHandler::update()
{
	if(!kinect.isOpened())
		return;

	kinect.update();

	if(bLearnBackground)
	{		
		backgroundDepth = kinect.getDistancePixels();
		bLearnBackground = false;
	}

	depthImage = kinect.getDistancePixels();	
	int length = depthWidth*depthHeight;
	unsigned short* pD = depthImage.getPixels();
	unsigned short* pB = backgroundDepth.getPixels();
	unsigned char* pDi = diffDepth.getPixels();
	int count = 0;
	for(int i = 0; i < length; i++, pD++, pB++, pDi++)
	{
		int inputZ = *pD;	
		int backgroundZ = *pB;
		
		int delta;
		//Check for zero values - it means that depth camera
		//does not compute distance in the pixel
		if ( inputZ != 0 && backgroundZ != 0 ) 
		{
			delta = backgroundZ - inputZ;
		}
		else 
		{
			delta = -1;
		}
		//Output value
		if ( ofInRange( delta, nearDistance, farDistance ) ) 
		{	
			int value = ofMap( delta, nearDistance, farDistance, 0, 255, true );
			*pDi = 255;
			count++;
		}
		else
			*pDi = 0;
		
	}
	diffDepth.flagImageChanged();

	num++;
	ofSaveImage(diffDepth.getPixelsRef(), "diff_" + ofToString(num) + ".jpg", ofImageQualityType::OF_IMAGE_QUALITY_MEDIUM);		

	contourFinder.findContours(diffDepth, 20, (depthWidth * depthHeight)/3, 10, true);

	if(contourFinder.nBlobs > 0)
		blobTracker.trackBlobs(contourFinder.blobs);
}

void ofxKinectHandler::threadedFunction()
{	

	while(isThreadRunning())
	{
		update();
	}	
}

void ofxKinectHandler::draw()
{		

}

void ofxKinectHandler::_objectAdded(ofxCvTrackedBlob& b) 
{      	
	int x = b.centroid.x, y = b.centroid.y;
	if( x > 0 && x < depthWidth && y > 0 && y < depthHeight )  
	{
		int id = b.id, order = blobTracker.findOrder(b.id);
		b.worldPoint = kinect.getWorldCoordinateFor(b.centroid.x, b.centroid.y);
		cout << "blobOn()-- " << "id = " << id << "  -order: " << order << endl;	
		ofNotifyEvent(objectAdded, b);
	}
	else
	{
		cout << "doBlobOn() event for blob: " << b.id << endl;
	}
}  

void ofxKinectHandler::_objectMoved(ofxCvTrackedBlob& b) 
{    
	int x = b.centroid.x, y = b.centroid.y;
	if( x > 0 && x < depthWidth && y > 0 && y < depthHeight ) 
	{
		int id = b.id, order = blobTracker.findOrder(b.id);
		b.worldPoint = kinect.getWorldCoordinateFor(b.centroid.x, b.centroid.y);
		cout << "blobMoved()-- " << "id = " << id << "  -order: " << order << endl;	
		ofNotifyEvent(objectMoved, b);
	}
	else
	{
		cout << "doBlobMoved() event for blob: " << b.id << endl;
	}

}  

void ofxKinectHandler::_objectDeleted(ofxCvTrackedBlob& b) 
{  	
	int x = b.centroid.x, y = b.centroid.y;
	if( x > 0 && x < depthWidth && y > 0 && y < depthHeight )  
	{
		int id = b.id, order = blobTracker.findOrder(b.id);
		b.worldPoint = kinect.getWorldCoordinateFor(b.centroid.x, b.centroid.y);
		cout << "blobOff()-- " << "id = " << id << "  -order: " << order << endl;	
		ofNotifyEvent(objectDeleted, b);
	}
	else
	{
		cout << "doBlobOff() event for blob: " << b.id << endl;
	}
}