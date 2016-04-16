#pragma once

#include "ofMain.h"
#include "ofxCircleEffect.h"
#include "ofxWaterEffect.h"
#include "ofxKinectHandler.h"
const int PROJECTOR_RESOLUTION_X = 1280;
const int PROJECTOR_RESOLUTION_Y = 768;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
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

		ofxKinectHandler	*kinectHandler;
		ofxScene			*scene;
	
};
