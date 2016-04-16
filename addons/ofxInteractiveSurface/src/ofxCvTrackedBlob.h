#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"

class ofxCvTrackedBlob : public ofxCvBlob
{
public:
	int id;

    ofPoint deltaLoc;
    ofPoint deltaLocTotal;
    ofPoint predictedPos;
	ofVec3f worldPoint;
    float deltaArea;


    // Used only by BlobTracker
    //
    bool markedForDeletion;
    int framesLeft;
    vector<float> error;
    vector<int> closest;  // ids of the closest points, sorted
public:

	int getLowestError();
	ofxCvTrackedBlob(const ofxCvBlob& b);    
	ofxCvTrackedBlob(void);
	~ofxCvTrackedBlob(void);
};

