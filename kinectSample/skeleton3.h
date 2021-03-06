#ifndef SKELETON_H_
#define SKELETON_H_

#include <Windows.h>
#include <Kinect.h>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>


template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != 0)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = 0;
	}
}

class Skeleton {
public:
	void skeletonTracking();
	void wideSquatAlgo(Joint joint[JointType::JointType_Count]); // 와이드 스쿼트 알고리즘(5/3)
	void drawSkeleton(cv::Mat canvas, Joint joint[JointType::JointType_Count]);
	const cv::Scalar GREEN = cv::Scalar(0, 255, 0);
	cv::Point changeCoordinates(Joint joint[JointType::JointType_Count], int type);
private:
	int colorWidth;                                   // width for RGB Image
	int colorHeight;
	int depthWidth;                                   // width for Depth Image
	int depthHeight;
	IKinectSensor* pSensor;
	IColorFrameSource* pColorSource;
	IBodyFrameSource* pBodySource;
	IDepthFrameSource* pDepthSource;
	IColorFrameReader* pColorReader;
	IBodyFrameReader* pBodyReader;
	IDepthFrameReader* pDepthReader;
	IFrameDescription* pColorDescription;
	IFrameDescription* pDepthDescription;
	ICoordinateMapper* pCoordinateMapper;
};


#endif /* SKELETON_H_ */
