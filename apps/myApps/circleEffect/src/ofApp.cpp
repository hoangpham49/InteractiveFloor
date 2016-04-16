#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	kinectHandler = new ofxKinectHandler();
	//scene = new ofxCircleEffect(PROJECTOR_RESOLUTION_X, PROJECTOR_RESOLUTION_Y);
	scene = new ofxWaterEffect(PROJECTOR_RESOLUTION_X, PROJECTOR_RESOLUTION_Y);

	ofAddListener(kinectHandler->objectAdded, scene, &ofxScene::touchOn);
	ofAddListener(kinectHandler->objectMoved, scene, &ofxScene::touchMoved);
	ofAddListener(kinectHandler->objectDeleted, scene, &ofxScene::touchOff);

	kinectHandler->start();
	scene->start();
	
}

//--------------------------------------------------------------
void ofApp::update(){
	kinectHandler->update();
	scene->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	scene->draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
	delete kinectHandler;
	delete scene;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
