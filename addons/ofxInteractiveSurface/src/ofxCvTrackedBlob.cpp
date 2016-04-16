#include "ofxCvTrackedBlob.h"


ofxCvTrackedBlob::ofxCvTrackedBlob(void)
{
	id = -1;
	area = 0.0f;
	length = 0.0f;
	deltaArea = 0.0f;
	markedForDeletion = false;
	framesLeft = 0;
}


ofxCvTrackedBlob::~ofxCvTrackedBlob(void)
{
}

ofxCvTrackedBlob::ofxCvTrackedBlob( const ofxCvBlob& b ) 
{
	area = b.area;
	length = b.length;
	boundingRect = b.boundingRect;
	centroid = b.centroid;
	hole = b.hole;
	pts = b.pts;

	id = -1;
	deltaArea = 0.0f;
	markedForDeletion = false;
	framesLeft = 0;
}

int ofxCvTrackedBlob::getLowestError() 
{
	int best=-1;
	float best_v=99999.0f;
	for( unsigned int i=0; i<error.size(); i++ ) 
	{
		if( error[i] < best_v ) 
		{
			best = i;
			best_v = error[i];
		}
	}
	return best;
}