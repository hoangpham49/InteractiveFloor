#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//Depth camera setup
	ofSetLogLevel(OF_LOG_VERBOSE);	
	ofxKinectNui::InitSetting initSetting;	
	kinect.init(initSetting);
	kinect.setMirror(false);
	kinect.open();
	ofSetVerticalSync(true);

	allocate();
}

//--------------------------------------------------------------
void ofApp::allocate() {
	if(kinect.isOpened())
	{			
		chessboardSize = 300;
		chessboardX = 5;
		chessboardY = 4;

		rgbImage.allocate(KINECT_WIDTH, KINECT_HEIGHT);				
		diffDepth.allocate(KINECT_WIDTH, KINECT_HEIGHT);

		fboChessboard.allocate(PROJECTOR_RESOLUTION_X, PROJECTOR_RESOLUTION_Y, GL_RGBA);

		secondWindow.setup(
			"chessboard", 
			ofGetScreenWidth(), 
			0, 
			fboChessboard.getWidth(), 
			fboChessboard.getHeight(), 
			true);

		mode = 1;

		if(mode == 2)
			kpt.loadCalibration("calibration.xml");

		nearClipping = 300;
		farClipping = 3000;
		//kinect.setNearClippingDistance(nearClipping);
		//kinect.setFarClippingDistance(farClipping);
		bLearnBackground = true;
		num = 0;
	}
}

//--------------------------------------------------------------
void ofApp::clear() {
	if(rgbImage.bAllocated)
	{
		rgbImage.clear();
	}
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



//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();

	rgbImage.setFromPixels(kinect.getVideoPixels());

	/*
	if(bLearnBackground)
	{		
		backgroundDepth = kinect.getDistancePixels();
		bLearnBackground = false;
	}

	depthImage = kinect.getDistancePixels();	
	int length = KINECT_HEIGHT*KINECT_WIDTH;
	unsigned short* pD = depthImage.getPixels();
	unsigned short* pB = backgroundDepth.getPixels();
	unsigned char* pDi = diffDepth.getPixels();
	for(int i = 0; i < length; i++, pD++, pB++, pDi++)
	{
		int inputZ = *pD;	
		int backgroundZ = *pB;
		*pDi = *pB;
		
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
		if ( ofInRange( delta, nearClipping, farClipping ) ) 
		{	
			int value = ofMap( delta, nearClipping, farClipping, 0, 255, true );
			*pDi = 255;
		}
		else
			*pDi = 0;
			
	}
	diffDepth.flagImageChanged();

	num++;
	//ofSaveImage(diffDepth.getPixelsRef(), "diff_" + ofToString(num) + ".jpg", ofImageQualityType::OF_IMAGE_QUALITY_MEDIUM);		

	contourFinderDepth.findContours(diffDepth, 20, (KINECT_WIDTH * KINECT_HEIGHT)/3, 10, true);
	*/

	if(mode == 1)//calibrating		
	{							
		drawChessboard(ofGetMouseX(), ofGetMouseY(), chessboardSize);		
		//cvRgbImage = ofxCv::toCv(rgbImage->getPixelsRef());
		cvRgbImage = cv::Mat(rgbImage.getCvImage());
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
void ofApp::draw(){
	float ptSize;	
	rgbImage.draw(0, 0);
	
	//diffDepth.draw(0, 0);
	/*
	for(int i = 0; i < contourFinderDepth.nBlobs; i++)
	{
		contourFinderDepth.blobs[i].draw(0, 0);
	}
	*/
		

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
	default:
		break;
	}


	ofSetColor(255);
	secondWindow.begin();
	fboChessboard.draw(0, 0);
	secondWindow.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key==' '){
		addPointPair();
	} else if (key=='q') {
		chessboardSize -= 20;
	} else if (key=='w') {
		chessboardSize += 20;
	} else if (key=='c') {
		kpt.calibrate(pairsKinect, pairsProjector);
		mode = 2;
	} else if (key=='s') {
		kpt.saveCalibration("calibration.xml");
		saved = true;
	} else if (key=='l') {
		kpt.loadCalibration("calibration.xml");		
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if(mode == 2)//
		testPoint.set(min(x, (int)kinect.getDepthResolutionWidth() - 1), min(y, (int)kinect.getDepthResolutionHeight()-1));
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit(){ 
	kinect.close();
	clear();
}

//--------------------------------------------------------------
void ofApp::drawChessboard(int x, int y, int chessboardSize) 
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
void ofApp::drawTestingPoint(ofVec2f projectedPoint) 
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
void ofApp::addPointPair() 
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


