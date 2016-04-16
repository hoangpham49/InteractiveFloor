#include "ofxKinectHandler.h"

ofxKinectHandler::ofxKinectHandler()
{
}

void ofxKinectHandler::setup()
{
	//omp_set_num_threads(4);
	kinect.setup();
	kinect.addDepthGenerator();
	kinect.addImageGenerator();
	kinect.setRegister(true);	//Enable alignment
	//of depth and color images
	kinect.start();			//Start depth camera to capture
	kinect.setUseDepthRawPixels(true);

	depthWidth = 640, depthHeight = 480;
	diffDepth.allocate(depthWidth, depthHeight);
	blobTracker.setListener(this);
	kpt.loadCalibration("calibration.xml");
	num = 0;

	//integral
	//quadTreeProcessor = new ofxQuadNodeProcessorByIntegral(depthWidth, depthHeight);
	//quadTreeProcessor->lowerFullness = 0.0004;
	//quadTreeProcessor->upperFullness = 0.25;
	int integralW = depthWidth + 1, integralH = depthHeight + 1;
	integralStep = depthWidth + 1;
	integralImage = new unsigned int[integralW*integralH];		
	for (int x = 0; x < integralW; x++)
	{
		integralImage[x] = 0;		
	}

	//first col
	for (int y = 0, idx = 0; y < integralH; y++, idx += integralStep)
	{
		integralImage[idx] = 0;		
	}
}

void ofxKinectHandler::threadedFunction()
{	
	
	while(isThreadRunning())
	{		
		if(bLearnBackground)
		{
			backgroundDepth = kinect.getDepthRawPixels();
			bLearnBackground = false;
		}

		vector<int> pDelta;
		
		int minV = 50;		//Minimal distance in mm
		int maxV = 100; 		//Maximal distance in mm
		//int minV = 250;		//Minimal distance in mm
		//int maxV = 500; 		//Maximal distance in mm

		ofShortPixels& input = kinect.getDepthRawPixels();				
		ofImage img; img.allocate(depthWidth, depthHeight, ofImageType::OF_IMAGE_GRAYSCALE);		
		//ofImage img1; img1.allocate(depthWidth, depthHeight, ofImageType::OF_IMAGE_GRAYSCALE);

		unsigned short* pB = backgroundDepth.getPixels();
		unsigned short* pIn = input.getPixels();
		unsigned char* pD = diffDepth.getPixels();

		unsigned int* pI = integralImage + integralStep + 1;
		unsigned int sum = 0;

		float t1 = ofGetElapsedTimef();

		for(int y = 0; y < depthHeight; y++, pI += integralStep)
		{	
			unsigned int* pRowI = pI;
			sum = 0;

			for(int x = 0; x < depthWidth; x++, pB++, pIn++, pD++, pRowI++ )
			{
				//int idx = x + depthWidth * y;
				//int inputZ = input.getPixels()[idx];	
				//int backgroundZ = backgroundDepth.getPixels()[idx];
				int inputZ = *pIn;	
				int backgroundZ = *pB;
							
				int delta = -1;				
				if ( inputZ != 0 && backgroundZ != 0 ) 
				{
					delta = backgroundZ - inputZ;
				}
					
				//img1.getPixels()[idx] = delta;
				
				//Output value
				/*
				if ( ofInRange( delta, minV, maxV ) ) 				
				{		
					*pD = 255;
					sum += 1;
					diffDepth.getPixels()[idx] = 255;
					img.getPixels()[idx] = diffDepth.getPixels()[idx];					
				}
				else
				{
					*pD = 0;
					sum += 0;
					diffDepth.getPixels()[idx] = 0;
					img.getPixels()[idx] = diffDepth.getPixels()[idx];
				}
				*/

				sum += ofInRange( delta, minV, maxV );				
				*pRowI = *(pRowI - integralStep) + sum;
			}
		}
		
		float t2 = ofGetElapsedTimef();
		
		//quadTree
		vector<ofRectangle> rois;
		//quadTreeProcessor->resetWhiteNode(depthWidth, depthHeight);		
		quadTree = new ofxQuadNode(0, 0, depthWidth, depthHeight);
		//quadTree->split(quadTreeProcessor);		
		quadTree->split(integralImage + integralStep + 1, integralStep, 0.0004, 0.25, rois);
		//quadTree->draw(img);
		delete quadTree;

		//img.saveImage("diff_" + to_string(num) + ".jpg", ofImageQualityType::OF_IMAGE_QUALITY_LOW);		
		//ofSaveImage(img1, "delta_" + to_string(num) + ".jpg", ofImageQualityType::OF_IMAGE_QUALITY_LOW);
		//num++;

		//rois = quadTreeProcessor->whiteNode;
		if(rois.size() > 0)
		{	
			int xmin = rois[0].x, xmax = rois[0].x;
			int ymin = rois[0].y, ymax = rois[0].y;
			float area = 0;
			vector<ofPoint> pts;
			for(int i = 0; i < rois.size(); i++)
			{
				xmin = min(xmin, (int)rois[i].x);
				xmax = max(xmax, (int)rois[i].x);
				ymin = min(ymin, (int)rois[i].y);
				ymax = max(ymax, (int)rois[i].y);
				area += rois[i].getArea();
				pts.push_back(rois[i].getCenter());
			}

			ofxCvBlob b;
			b.area = area;
			b.boundingRect = ofRectangle(xmin, ymin, xmax - xmin + 1, ymax - ymin + 1);
			b.centroid = b.boundingRect.getCenter();
			b.hole = false;
			b.pts = pts;
			b.nPts = pts.size();

			float t3 = ofGetElapsedTimef();
			
			vector<ofxCvBlob> blobs;
			blobs.push_back(b);
			blobTracker.trackBlobs(blobs);

			float t4 = ofGetElapsedTimef();

			cout << "Integral: " << t2 - t1 << ", Detect blob: " << t3 - t2 << ", Track blob: " << t4 - t3 << "\n";
		}
		

		/*
		contourFinder.findContours(diffDepth, 1000, 15000, 4, false, true);
		float t3 = ofGetElapsedTimef();

		if(contourFinder.nBlobs > 0)
			blobTracker.trackBlobs(contourFinder.blobs);

		float t4 = ofGetElapsedTimef();

		cout << "Integral: " << t2 - t1 << ", Detect blob: " << t3 - t2 << ", Track blob: " << t4 - t3 << "\n";
		*/
	}	
}

void ofxKinectHandler::draw()
{		
	
}

void ofxKinectHandler::blobOn(const ofxCvTrackedBlob& b) 
{      	
	if( listener != NULL ) 
	{
		int id = b.id, order = blobTracker.findOrder(b.id);
		cout << "blobOn()-- " << "id = " << id << "  -order: " << order << endl;	
		ofPoint currBlob = b.centroid;
		ofPoint prevBlob = b.centroid - b.deltaLoc;

		ofVec2f t = ofVec2f(min(depthWidth - 1, (int)currBlob.x), min(depthHeight - 1, (int)currBlob.y));
		ofVec3f worldPoint = kinect.cameraToWorld(t);
		ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);	

		ofVec2f prev_t = ofVec2f(min(depthWidth - 1, (int)prevBlob.x), min(depthHeight - 1, (int)prevBlob.y));
		ofVec3f prev_worldPoint = kinect.cameraToWorld(prev_t);
		ofVec2f prev_projectedPoint = kpt.getProjectedPoint(prev_worldPoint);	

		listener->touchOn(projectedPoint, prev_projectedPoint, id, order);
    }
	else
	{
        cout << "doBlobOn() event for blob: " << b.id << endl;
    }
}  

void ofxKinectHandler::blobMoved(const ofxCvTrackedBlob& b) 
{         
	if( listener != NULL ) 
	{
		int id = b.id, order = blobTracker.findOrder(b.id);
		cout << "blobMoved()-- " << "id = " << id << "  -order: " << order << endl;	
		ofPoint currBlob = b.centroid;
		ofPoint prevBlob = b.centroid - b.deltaLoc;

		ofVec2f t = ofVec2f(min(depthWidth - 1, (int)currBlob.x), min(depthHeight - 1, (int)currBlob.y));
		ofVec3f worldPoint = kinect.cameraToWorld(t);
		ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);	

		ofVec2f prev_t = ofVec2f(min(depthWidth - 1, (int)prevBlob.x), min(depthHeight - 1, (int)prevBlob.y));
		ofVec3f prev_worldPoint = kinect.cameraToWorld(prev_t);
		ofVec2f prev_projectedPoint = kpt.getProjectedPoint(prev_worldPoint);	

		listener->touchMoved(projectedPoint, prev_projectedPoint, id, order);
    }
	else
	{
        cout << "doBlobMoved() event for blob: " << b.id << endl;
    }
	
}  

void ofxKinectHandler::blobOff(const ofxCvTrackedBlob& b) 
{  	
	if( listener != NULL ) 
	{
		int id = b.id, order = blobTracker.findOrder(b.id);
		cout << "blobOff()-- " << "id = " << id << "  -order: " << order << endl;	
		ofPoint currBlob = b.centroid;
		ofPoint prevBlob = b.centroid - b.deltaLoc;

		ofVec2f t = ofVec2f(min(depthWidth - 1, (int)currBlob.x), min(depthHeight - 1, (int)currBlob.y));
		ofVec3f worldPoint = kinect.cameraToWorld(t);
		ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);	

		ofVec2f prev_t = ofVec2f(min(depthWidth - 1, (int)prevBlob.x), min(depthHeight - 1, (int)prevBlob.y));
		ofVec3f prev_worldPoint = kinect.cameraToWorld(prev_t);
		ofVec2f prev_projectedPoint = kpt.getProjectedPoint(prev_worldPoint);	

		listener->touchOff(projectedPoint, prev_projectedPoint, id, order);
    }
	else
	{
        cout << "doBlobOff() event for blob: " << b.id << endl;
    }
}