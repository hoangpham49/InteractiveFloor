#include "ofxKinectProjectorToolkit.h"


ofxKinectProjectorToolkit::ofxKinectProjectorToolkit(void)
{
}


ofxKinectProjectorToolkit::~ofxKinectProjectorToolkit(void)
{
}

void ofxKinectProjectorToolkit::calibrate(vector<ofVec3f> pairsKinect, vector<ofVec2f> pairsProjector) 
{
    int nPairs = pairsKinect.size();

    //A.set_size(nPairs*2, 11);
    //y.set_size(nPairs*2, 1);

	A = cv::Mat(nPairs*2, 11, CV_64F);
    y = cv::Mat(nPairs*2, 1, CV_64F);

    for (int i=0; i<nPairs; i++) {		
        //A(2*i, 0) = pairsKinect[i].x;
		A.at<double>(2*i, 0) = pairsKinect[i].x;
        //A(2*i, 1) = pairsKinect[i].y;
		A.at<double>(2*i, 1) = pairsKinect[i].y;
        //A(2*i, 2) = pairsKinect[i].z;
		A.at<double>(2*i, 2) = pairsKinect[i].z;
        //A(2*i, 3) = 1;
		A.at<double>(2*i, 3) = 1;
        //A(2*i, 4) = 0;
		A.at<double>(2*i, 4) = 0;
        //A(2*i, 5) = 0;
		A.at<double>(2*i, 5) = 0;
        //A(2*i, 6) = 0;
		A.at<double>(2*i, 6) = 0;
        //A(2*i, 7) = 0;
		A.at<double>(2*i, 7) = 0;
        //A(2*i, 8) = -pairsKinect[i].x * pairsProjector[i].x;
		A.at<double>(2*i, 8) = -pairsKinect[i].x * pairsProjector[i].x;
        //A(2*i, 9) = -pairsKinect[i].y * pairsProjector[i].x;
		A.at<double>(2*i, 9) = -pairsKinect[i].y * pairsProjector[i].x;
        //A(2*i, 10) = -pairsKinect[i].z * pairsProjector[i].x;
		A.at<double>(2*i, 10) = -pairsKinect[i].z * pairsProjector[i].x;

        //A(2*i+1, 0) = 0;
		A.at<double>(2*i+1, 0) = 0;
        //A(2*i+1, 1) = 0;
		A.at<double>(2*i+1, 1) = 0;
        //A(2*i+1, 2) = 0;
		A.at<double>(2*i+1, 2) = 0;
        //A(2*i+1, 3) = 0;
		A.at<double>(2*i+1, 3) = 0;
        //A(2*i+1, 4) = pairsKinect[i].x;
		A.at<double>(2*i+1, 4) = pairsKinect[i].x;
        //A(2*i+1, 5) = pairsKinect[i].y;
		A.at<double>(2*i+1, 5) = pairsKinect[i].y;
        //A(2*i+1, 6) = pairsKinect[i].z;
		A.at<double>(2*i+1, 6) = pairsKinect[i].z;
        //A(2*i+1, 7) = 1;
		A.at<double>(2*i+1, 7) = 1;
        //A(2*i+1, 8) = -pairsKinect[i].x * pairsProjector[i].y;
		A.at<double>(2*i+1, 8) = -pairsKinect[i].x * pairsProjector[i].y;
        //A(2*i+1, 9) = -pairsKinect[i].y * pairsProjector[i].y;
		A.at<double>(2*i+1, 9) = -pairsKinect[i].y * pairsProjector[i].y;
        //A(2*i+1, 10) = -pairsKinect[i].z * pairsProjector[i].y;
		A.at<double>(2*i+1, 10) = -pairsKinect[i].z * pairsProjector[i].y;
        
        //y(2*i, 0) = pairsProjector[i].x;
		y.at<double>(2*i, 0) = pairsProjector[i].x;
        //y(2*i+1, 0) = pairsProjector[i].y;
		y.at<double>(2*i+1, 0) = pairsProjector[i].y;
    }
    
	cv::solve(A, y, x, cv::DECOMP_QR);

    //dlib::qr_decomposition<dlib::matrix<double, 0, 11> > qrd(A);
    //x = qrd.solve(y);
}

ofVec2f ofxKinectProjectorToolkit::getProjectedPoint(ofVec3f worldPoint) 
{
    float a = x.at<double>(0, 0)*worldPoint.x + x.at<double>(1, 0)*worldPoint.y + x.at<double>(2, 0)*worldPoint.z + x.at<double>(3,0);
    float b = x.at<double>(4, 0)*worldPoint.x + x.at<double>(5, 0)*worldPoint.y + x.at<double>(6, 0)*worldPoint.z + x.at<double>(7,0);
    float c = x.at<double>(8, 0)*worldPoint.x + x.at<double>(9, 0)*worldPoint.y + x.at<double>(10, 0)*worldPoint.z + 1;
    ofVec2f projectedPoint(a/c, b/c);
    return projectedPoint;
}

void ofxKinectProjectorToolkit::loadCalibration(string path){
    ofXml xml;
    xml.load(path);    
    xml.setTo("CALIBRATION");
	x = cv::Mat(11, 1, CV_64F);
    for (int i=0; i<11; i++) 
	{
		//x.push_back(xml.getValue<float>("COEFF"+ofToString(i)));		
		string str = xml.getValue<string>("COEFF"+ofToString(i));
		x.at<double>(i, 0) = ofBinaryToDouble(str);
    }
}

void ofxKinectProjectorToolkit::saveCalibration(string path){
    ofXml xml;
    xml.addChild("CALIBRATION");
    xml.setTo("CALIBRATION");
    for (int i=0; i<11; i++) 
	{
        ofXml coeff;
		coeff.addValue("COEFF"+ofToString(i), ofToBinary(x.at<double>(i, 0)));
        xml.addXml(coeff);
    }
    xml.setToParent();
    xml.save(path);
}