#pragma once

#include "ofxOpenCv.h"

class ofxKinectProjectorToolkit
{
	//dlib::matrix<double, 0, 11> A;
    //dlib::matrix<double, 0, 1> y;
    //dlib::matrix<double, 11, 1> x;
	cv::Mat A;
	cv::Mat y;
	cv::Mat x;
public:
	void calibrate(vector<ofVec3f> pairsKinect, vector<ofVec2f> pairsProjector);
    ofVec2f getProjectedPoint(ofVec3f worldPoint);
    vector<ofVec2f> getProjectedContour(vector<ofVec3f> *worldPoints);
    void loadCalibration(string path);
    void saveCalibration(string path);

	ofxKinectProjectorToolkit(void);
	~ofxKinectProjectorToolkit(void);
};

