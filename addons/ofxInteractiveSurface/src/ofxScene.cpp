#include "ofxScene.h"

ofxScene::ofxScene(int _projW, int _projH)
{
	setup(_projW, _projH);
}


ofxScene::~ofxScene()
{
}


void ofxScene::setup(int _projW, int _projH)
{
	projectorWidth = _projW;
	projectorHeight = _projH;

	ofSetFrameRate(60);
	ofEnableAlphaBlending();
	ofSetWindowShape(projectorWidth, projectorHeight);
	
	kpt.loadCalibration("calibration.xml");
}


void ofxScene::touchOn( ofxCvTrackedBlob &b ) 
{   
	cout << "touchOn()-- " << "id = " << b.id << endl;
	ofVec2f projectedPoint = kpt.getProjectedPoint(b.worldPoint);	
	touchX = ofMap(projectedPoint.x, 0, 1, 0, projectorWidth);
	touchY = ofMap(projectedPoint.y, 0, 1, 0, projectorHeight);
	
}  

void ofxScene::touchMoved( ofxCvTrackedBlob &b ) 
{       
	cout << "touchMoved()-- " << "id = " << b.id << endl;
	ofVec2f projectedPoint = kpt.getProjectedPoint(b.worldPoint);	
	touchX = ofMap(projectedPoint.x, 0, 1, 0, projectorWidth);
	touchY = ofMap(projectedPoint.y, 0, 1, 0, projectorHeight);
}

void ofxScene::touchOff( ofxCvTrackedBlob &b ) 
{  	
	cout << "touchOff()-- " << "id = " << b.id << endl;
}


void ofxScene::threadedFunction()
{
	while(isThreadRunning())
	{
		update();
	}
}

