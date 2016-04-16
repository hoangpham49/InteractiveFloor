#pragma once

#include "ofMain.h"
#include "ofxKinectProjectorCalibrator.h"

const int PROJECTOR_RESOLUTION_X = 1280;
const int PROJECTOR_RESOLUTION_Y = 768;
const int KINECT_WIDTH = 640;
const int KINECT_HEIGHT = 480;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void allocate();
		void clear();

		void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void drawChessboard(int x, int y, int chessboardSize);	
		void drawTestingPoint(ofVec2f projectedPoint);
		void addPointPair();

		
	public:
		ofxKinectNui				kinect;			//Depth camera
		ofxKinectProjectorToolkit   kpt;

		ofxSecondWindow             secondWindow;
		ofFbo                       fboChessboard;
		ofxCvColorImage	            rgbImage;				
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

		unsigned short				nearClipping;		//Minimal distance in mm
		unsigned short				farClipping; 		//Maximal distance in mm
		ofShortPixels				depthImage;
		ofShortPixels				backgroundDepth;
		ofxCvGrayscaleImage			diffDepth;
		ofxCvContourFinder			contourFinderDepth;
		bool						bLearnBackground;
		
		int							num;
};
