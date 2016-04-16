#include "ofxKinectProjectorCalibrator.h"


//--------------------------------------------------------------
void ofxKinectProjectorCalibrator::setup(int projector_solution_x, int projector_solution_y)
{
	//Depth camera setup
	ofSetLogLevel(OF_LOG_VERBOSE);	
	ofxKinectNui::InitSetting initSetting;	
	kinect.init(initSetting);
	kinect.open();
	ofSetVerticalSync(true);

	chessboardSize = 300;
	chessboardX = 5;
	chessboardY = 4;

	rgbImage = new ofxCvColorImage();
	rgbImage->allocate(640, 480);

	fboChessboard.allocate(projector_solution_x, projector_solution_y, GL_RGBA);

	secondWindow.setup(
		"chessboard", 
		ofGetScreenWidth(), 
		0, 
		fboChessboard.getWidth(), 
		fboChessboard.getHeight(), 
		true);

	mode = 2;

	if(mode == 2)
		kpt.loadCalibration("calibration.xml");


}

//--------------------------------------------------------------
void ofxKinectProjectorCalibrator::update()
{

	kinect.update();		
	rgbImage->setFromPixels(kinect.getVideoPixels());

	if(mode == 1)//calibrating		
	{							
		drawChessboard(ofGetMouseX(), ofGetMouseY(), chessboardSize);		
		//cvRgbImage = ofxCv::toCv(rgbImage->getPixelsRef());
		cvRgbImage = cv::Mat(rgbImage->getCvImage());
		cv::Size patternSize = cv::Size(chessboardX-1, chessboardY-1);
		int chessFlags = cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_FAST_CHECK;
		bool foundChessboard = findChessboardCorners(cvRgbImage, patternSize, cvPoints, chessFlags);
		if(foundChessboard) 
		{
			cv::Mat gray;
			cvtColor(cvRgbImage, gray, CV_RGB2GRAY);
			cornerSubPix(gray, cvPoints, cv::Size(11, 11), cv::Size(-1, -1),
				cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(cvRgbImage, patternSize, cv::Mat(cvPoints), foundChessboard);
			//addPointPair();
		}		
	}
	else if(mode == 2)//testing
	{		
		ofVec2f t = ofVec2f(min(kinect.getDepthResolutionWidth()-1, (int)testPoint.x), min(kinect.getDepthResolutionHeight()-1, (int)testPoint.y));
		ofVec3f worldPoint = kinect.getWorldCoordinateFor(t.x, t.y);
		ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);
		drawTestingPoint(projectedPoint);
	}

}

//--------------------------------------------------------------
void ofxKinectProjectorCalibrator::draw()
{

	float ptSize;	
	rgbImage->draw(0, 0);
	kinect.drawDepth(10, 490, 320, 240);
	ofSetColor(0);

	switch(mode)
	{
	case 1://calib						
		ofDrawBitmapString("Position the chessboard using the mouse.", 340, 510);
		ofDrawBitmapString("Adjust the size of the chessboard using the 'q' and 'w' keys.", 340, 530);
		ofDrawBitmapString("Press the spacebar to save a set of point pairs.", 340, 550);
		ofDrawBitmapString("Press the 'c' key to calibrate.", 340, 570);
		ofSetColor(resultMessageColor);
		ofDrawBitmapString(resultMessage, 340, 610);
		ofSetColor(0);
		ofDrawBitmapString(ofToString(pairsKinect.size())+" point pairs collected.", 340, 630);
		break;
	case 2://testing		
		ofDrawBitmapString("Click on the image to test a point in the RGB image.", 340, 510);
		ofDrawBitmapString("The projector should place a green dot on the corresponding point.", 340, 530);
		ofDrawBitmapString("Press the 's' key to save the calibration.", 340, 550);
		if (saved) {
			ofDrawBitmapString("Calibration saved.", 340, 590);
		}
		ofSetColor(255, 0, 0);
		//float ptSize = ofMap(cos(ofGetFrameNum()*0.1), -1, 1, 3, 40);
		ptSize = 20;
		ofCircle(testPoint.x, testPoint.y, ptSize);
		break;		
	}


	ofSetColor(255);
	secondWindow.begin();
	fboChessboard.draw(0, 0);
	secondWindow.end();

}


//--------------------------------------------------------------
void ofxKinectProjectorCalibrator::drawChessboard(int x, int y, int chessboardSize) 
{
	float w = chessboardSize / chessboardX;
	float h = chessboardSize / chessboardY;
	currentProjectorPoints.clear();
	fboChessboard.begin();
	ofBackground(255);
	ofSetColor(0);
	ofTranslate(x, y);
	for (int j=0; j<chessboardY; j++) 
	{
		for (int i=0; i<chessboardX; i++) 
		{
			int x0 = ofMap(i, 0, chessboardX, 0, chessboardSize);
			int y0 = ofMap(j, 0, chessboardY, 0, chessboardSize);
			if (j>0 && i>0) 
			{
				currentProjectorPoints.push_back(ofVec2f(
					ofMap(x+x0, 0, fboChessboard.getWidth(), 0, 1),
					ofMap(y+y0, 0, fboChessboard.getHeight(), 0, 1)
					));

				//currentProjectorPoints.push_back(ofVec2f(x0, y0));
			}
			if ((i+j)%2==0) 
				ofRect(x0, y0, w, h);
		}
	}
	ofSetColor(255);
	fboChessboard.end();
}

//--------------------------------------------------------------
void ofxKinectProjectorCalibrator::drawTestingPoint(ofVec2f projectedPoint) 
{
	//float ptSize = ofMap(sin(ofGetFrameNum()*0.1), -1, 1, 3, 40);
	float ptSize = 10;
	fboChessboard.begin();
	ofBackground(255);
	ofSetColor(0, 255, 0);

	ofCircle(
		ofMap(projectedPoint.x, 0, 1, 0, fboChessboard.getWidth()),
		ofMap(projectedPoint.y, 0, 1, 0, fboChessboard.getHeight()),
		ptSize);

	//ofCircle(projectedPoint.x, projectedPoint.y, ptSize);
	ofSetColor(255);
	fboChessboard.end();
}

//--------------------------------------------------------------
void ofxKinectProjectorCalibrator::addPointPair() 
{

	int nDepthPoints = 0;
	for (int i=0; i < cvPoints.size(); i++) 
	{
		ofVec3f worldPoint = kinect.getWorldCoordinateFor(cvPoints[i].x, cvPoints[i].y);//getWorldCoordinateAt(cvPoints[i].x, cvPoints[i].y);
		if (worldPoint.z > 0)   nDepthPoints++;
	}

	if (nDepthPoints == (chessboardX-1)*(chessboardY-1)) 
	{
		for (int i=0; i<cvPoints.size(); i++) 
		{
			ofVec3f worldPoint = kinect.getWorldCoordinateFor(cvPoints[i].x, cvPoints[i].y);
			pairsKinect.push_back(worldPoint);
			pairsProjector.push_back(currentProjectorPoints[i]);
		}
		resultMessage = "Added " + ofToString((chessboardX-1)*(chessboardY-1)) + " points pairs.";
		resultMessageColor = ofColor(0, 255, 0);
	}
	else 
	{
		resultMessage = "Points not added because not all chessboard\npoints' depth known. Try re-positionining.";
		resultMessageColor = ofColor(255, 0, 0);
	}

}