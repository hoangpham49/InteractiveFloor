#pragma once

#include "ofxOpenCv.h"
#include "ofxKinectNui.h"
#include "ofxKinectProjectorToolkit.h"
#include "ofxSecondWindow.h"



class ofxKinectProjectorCalibrator
{	
public:
	ofxKinectNui				kinect;			//Depth camera
	ofxKinectProjectorToolkit   kpt;

	ofxSecondWindow             secondWindow;
	ofFbo                       fboChessboard;
	ofxCvColorImage*            rgbImage;
	cv::Mat                     cvRgbImage;

	vector<ofVec2f>             currentProjectorPoints;
	vector<cv::Point2f>         cvPoints;
	vector<ofVec3f>             pairsKinect;
	vector<ofVec2f>             pairsProjector;

	string                      resultMessage;
	ofColor                     resultMessageColor;
	ofVec2f                     testPoint;

	int                         chessboardSize;
	int                         chessboardX;
	int                         chessboardY;
	int							mode;
	bool                        saved;	
public:
	void calibrate()
	{
		kpt.calibrate(pairsKinect, pairsProjector);
	}

	void drawChessboard(int x, int y, int chessboardSize);	
	void drawTestingPoint(ofVec2f projectedPoint);
	void addPointPair();
	void setup(int projector_solution_x, int projector_solution_y);
	void update();
	void draw();
	ofxKinectProjectorCalibrator(){};
	~ofxKinectProjectorCalibrator(){};
};

