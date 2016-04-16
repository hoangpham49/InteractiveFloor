#pragma once
#include "ofxCvTrackedBlob.h"

#define  MAX_NUM_CONTOURS_TO_FIND   128  // alther this if you think you will
// be looking for more....
#define  MAX_CONTOUR_LENGTH        1024  // alther this if you think your
// contours will be longer than this



class ofxCvBlobListener 
{
public:

	virtual void blobOn( const ofxCvTrackedBlob& b ) = 0;
	virtual void blobMoved( const ofxCvTrackedBlob& b ) = 0;
	virtual void blobOff( const ofxCvTrackedBlob& b ) = 0;

};

class ofxCvTouchListener 
{
public:
	virtual void touchOn( ofVec2f currPoint, ofVec2f prevPoint, int id, int order ) = 0;
	virtual void touchMoved( ofVec2f currPoint, ofVec2f prevPoint, int id, int order ) = 0;
	virtual void touchOff( ofVec2f currPoint, ofVec2f prevPoint, int id, int order ) = 0;

};

class ofxCvBlobTracker 
{
public:
	vector<ofxCvTrackedBlob>  blobs;

	ofxCvBlobTracker();	
	void trackBlobs( const vector<ofxCvBlob>& blobs );
	int findOrder( int id );  // order by which the present
	// blobs came into existence
	ofxCvTrackedBlob& getById( int id );  // returns a reference to the
	// corresponding blob in blobs vector
	void draw( float x, float y );  // draws all blobs

	ofEvent<ofxCvTrackedBlob> blobOn;
	ofEvent<ofxCvTrackedBlob> blobOff;
	ofEvent<ofxCvTrackedBlob> blobMoved;

protected:

	int currentID;
	int extraIDs;
	int numcheck;	
	int reject_distance_threshold;
	int ghost_frames;
	float minimumDisplacementThreshold;

	vector<vector<int> > matrix;
	vector<int> ids;
	vector<vector<ofxCvTrackedBlob> > history;

	inline void permute( int k );
	inline bool checkValid( int start );
	inline bool checkValidNew( int start );
};

