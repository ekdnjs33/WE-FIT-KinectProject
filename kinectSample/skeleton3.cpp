
#include "skeleton3.h"
#include <time.h> // �ǽð� �ð� ������ ���� (4/30)
#include <fstream>// file (5/1)
#include <iostream>
#include <math.h>
#include <string>
#define CURL_STATICLIB // http ��� �����ϴ� curl ���̺귯�� ���� (6/1)
#include <stdio.h> 
#include <curl/curl.h> 

#define radian 57.2958

#pragma comment(lib, "wldap32.lib") 
#pragma comment(lib, "ws2_32.lib") 

CURL *curl;
CURLcode res;

using namespace std;

clock_t sclock, nclock; // sclock(����), nclock(����: 1/1000��)
time_t t = time(NULL); // msec (5/1)
struct tm tm = *localtime(&t);
int tail = 0;

ofstream out("test.txt");// file (5/1)

cv::Point Skeleton::changeCoordinates(Joint joint[JointType::JointType_Count], int type)
{
	ColorSpacePoint colorSpacePoint = { 0 };
	pCoordinateMapper->MapCameraPointToColorSpace(joint[type].Position, &colorSpacePoint);
	int x = static_cast<int>(colorSpacePoint.X);
	int y = static_cast<int>(colorSpacePoint.Y);

	return cv::Point(x, y);
}

void Skeleton::drawSkeleton(cv::Mat canvas, Joint joint[JointType::JointType_Count])
{
	cv::line(canvas, changeCoordinates(joint, JointType_Head), changeCoordinates(joint, JointType_Neck), GREEN, 3); //�Ӹ�(3)���� ��(2)
	cv::line(canvas, changeCoordinates(joint, JointType_Neck), changeCoordinates(joint, JointType_SpineShoulder), GREEN, 3); //�񿡼� ��� ô��(20)
	cv::line(canvas, changeCoordinates(joint, JointType_SpineShoulder), changeCoordinates(joint, JointType_ShoulderLeft), GREEN, 3); //��� ô�߿��� ���� ���(4)
	cv::line(canvas, changeCoordinates(joint, JointType_SpineShoulder), changeCoordinates(joint, JointType_ShoulderRight), GREEN, 3); //��� ô�߿��� ������ ���(8)
	cv::line(canvas, changeCoordinates(joint, JointType_SpineShoulder), changeCoordinates(joint, JointType_SpineMid), GREEN, 3); //��� ô�߿��� ô�� �߰�(1)
	cv::line(canvas, changeCoordinates(joint, JointType_ShoulderLeft), changeCoordinates(joint, JointType_ElbowLeft), GREEN, 3); //���� ������� ���� �Ȳ�ġ(5)
	cv::line(canvas, changeCoordinates(joint, JointType_ShoulderRight), changeCoordinates(joint, JointType_ElbowRight), GREEN, 3); //������ ������� ������ �Ȳ�ġ(9)
	cv::line(canvas, changeCoordinates(joint, JointType_ElbowLeft), changeCoordinates(joint, JointType_WristLeft), GREEN, 3); //���� �Ȳ�ġ���� ���� �ո�(6)
	cv::line(canvas, changeCoordinates(joint, JointType_ElbowRight), changeCoordinates(joint, JointType_WristRight), GREEN, 3); //������ �Ȳ�ġ���� ������ �ո�(10)
	cv::line(canvas, changeCoordinates(joint, JointType_WristLeft), changeCoordinates(joint, JointType_HandLeft), GREEN, 3); //���� �ո񿡼� ���� ��(7)
	cv::line(canvas, changeCoordinates(joint, JointType_WristRight), changeCoordinates(joint, JointType_HandRight), GREEN, 3); //������ �ո񿡼� ������ ��(11)
	cv::line(canvas, changeCoordinates(joint, JointType_HandLeft), changeCoordinates(joint, JointType_HandTipLeft), GREEN, 3); //���� �տ��� ���� �հ���(21)
	cv::line(canvas, changeCoordinates(joint, JointType_HandRight), changeCoordinates(joint, JointType_HandTipRight), GREEN, 3); //������ �տ��� ������ �հ���(23)
	cv::line(canvas, changeCoordinates(joint, JointType_WristLeft), changeCoordinates(joint, JointType_ThumbLeft), GREEN, 3); //���� �ո񿡼� ���� ����(22)
	cv::line(canvas, changeCoordinates(joint, JointType_WristRight), changeCoordinates(joint, JointType_ThumbRight), GREEN, 3); //������ �ո񿡼� ������ ����(24)
	cv::line(canvas, changeCoordinates(joint, JointType_SpineMid), changeCoordinates(joint, JointType_SpineBase), GREEN, 3); //ô�� �߰����� ô�� ��(0)
	cv::line(canvas, changeCoordinates(joint, JointType_SpineBase), changeCoordinates(joint, JointType_HipLeft), GREEN, 3); //ô�� ������ ���� ������(12)
	cv::line(canvas, changeCoordinates(joint, JointType_SpineBase), changeCoordinates(joint, JointType_HipRight), GREEN, 3); //ô�� ������ ������ ������(16)
	cv::line(canvas, changeCoordinates(joint, JointType_HipLeft), changeCoordinates(joint, JointType_KneeLeft), GREEN, 3); //���� �����̿��� ���� ����(13)
	cv::line(canvas, changeCoordinates(joint, JointType_HipRight), changeCoordinates(joint, JointType_KneeRight), GREEN, 3); //������ �����̿��� ������ ����(17)
	cv::line(canvas, changeCoordinates(joint, JointType_KneeLeft), changeCoordinates(joint, JointType_AnkleLeft), GREEN, 3); //���� �������� ���� �߸�(14)
	cv::line(canvas, changeCoordinates(joint, JointType_KneeRight), changeCoordinates(joint, JointType_AnkleRight), GREEN, 3); //������ ���ÿ��� ������ �߸�(18)
	cv::line(canvas, changeCoordinates(joint, JointType_AnkleLeft), changeCoordinates(joint, JointType_FootLeft), GREEN, 3); //���� �߸񿡼� ���� ��(15)
	cv::line(canvas, changeCoordinates(joint, JointType_AnkleRight), changeCoordinates(joint, JointType_FootRight), GREEN, 3); //������ �߸񿡼� ������ ��(19)
}


void Skeleton::skeletonTracking()
{
	cv::setUseOptimized(true);

	// sensor
	HRESULT hResult = S_OK;
	hResult = GetDefaultKinectSensor(&pSensor);
	if (FAILED(hResult))
	{
		std::cerr << "Error: GetDefaultKinectSensor" << std::endl;
		exit(-1);
	}

	hResult = pSensor->Open();
	if (FAILED(hResult))
	{
		std::cerr << "Error: IKinectSensor::Open()" << std::endl;
		exit(-1);
	}

	// Source
	hResult = pSensor->get_ColorFrameSource(&pColorSource); //
	if (FAILED(hResult))
	{
		std::cerr << "Error: IKinectSensor::get_ColorFrameSource()" << std::endl;
		exit(-1);
	}

	hResult = pSensor->get_BodyFrameSource(&pBodySource);
	if (FAILED(hResult))
	{
		std::cerr << "Error: IKinectSensor::get_BodyFrameSource()" << std::endl;
		exit(-1);
	}

	hResult = pSensor->get_DepthFrameSource(&pDepthSource);
	if (FAILED(hResult))
	{
		std::cerr << "Error: IKinectSensor::get_DepthFrameSource()" << std::endl;
		exit(-1);
	}

	// Reader
	hResult = pColorSource->OpenReader(&pColorReader);
	if (FAILED(hResult))
	{
		std::cerr << "Error: IColorFrameSource::OpenReader()" << std::endl;
		exit(-1);
	}

	hResult = pBodySource->OpenReader(&pBodyReader);
	if (FAILED(hResult))
	{
		std::cerr << "Error: IBodyFrameSource::OpenReader()" << std::endl;
		exit(-1);
	}

	hResult = pDepthSource->OpenReader(&pDepthReader);
	if (FAILED(hResult))
	{
		std::cerr << "Error: IDepthFrameSource::OpenReader()" << std::endl;
		exit(-1);
	}

	// Description
	hResult = pColorSource->get_FrameDescription(&pColorDescription);
	if (FAILED(hResult))
	{
		std::cerr << "Error: IColorFrameSource::get_FrameDescription()" << std::endl;
		exit(-1);
	}

	hResult = pDepthSource->get_FrameDescription(&pDepthDescription);
	if (FAILED(hResult))
	{
		std::cerr << "Error: IDepthFrameSource::get_FrameDescription()" << std::endl;
		exit(-1);
	}

	colorWidth = 0;
	colorHeight = 0;
	depthWidth = 0;
	depthHeight = 0;
	pColorDescription->get_Width(&colorWidth);   // 1920
	pColorDescription->get_Height(&colorHeight); // 1080
	pDepthDescription->get_Width(&depthWidth);   // 512
	pDepthDescription->get_Height(&depthHeight); // 424
	unsigned int colorbufferSize = colorWidth * colorHeight * 4 * sizeof(unsigned char);
	unsigned int depthbufferSize = depthWidth * depthHeight * 4 * sizeof(unsigned char);

	cv::Mat colorBufferMat(colorHeight, colorWidth, CV_8UC4);
	cv::Mat depthBufferMat(depthHeight, depthWidth, CV_16UC1);
	cv::Mat bodyMat(colorHeight / 2, colorWidth / 2, CV_8UC4);
	cv::Mat depthMat(depthHeight, depthWidth, CV_8UC1);
	std::string colorWinName = "Skeleton RGB";
	std::string depthWinName = "Skeleton Depth";
	cv::namedWindow(colorWinName);
	cv::namedWindow(depthWinName);

	// Color Table
	cv::Vec3b color[BODY_COUNT]; //6����� ī��Ʈ ����
	color[0] = cv::Vec3b(255, 0, 0);
	color[1] = cv::Vec3b(0, 255, 0);
	color[2] = cv::Vec3b(0, 0, 255);
	color[3] = cv::Vec3b(255, 255, 0);
	color[4] = cv::Vec3b(255, 0, 255);
	color[5] = cv::Vec3b(0, 255, 255);

	// Range (Range of Depth is 500-8000[mm] ���� ����, Range of Detection is 500-45000[mm] ���� ����) 
	unsigned short min = 0;
	unsigned short max = 0;
	pDepthSource->get_DepthMinReliableDistance(&min);
	pDepthSource->get_DepthMaxReliableDistance(&max);
	std::cout << "Range: " << min << " - " << max << std::endl;

	// Coordinate Mapper
	hResult = pSensor->get_CoordinateMapper(&pCoordinateMapper);
	if (FAILED(hResult))
	{
		std::cerr << "Error: IKinectSensor::get_CoordinateMapper()" << std::endl;
		exit(-1);
	}

	while (1)
	{
		/* 1/100�� ������ ���� (5/1) */
		nclock = clock();
		if (nclock - sclock >= (CLOCKS_PER_SEC / 100)) // clock/100�ʿ� ũ�ų� ������
		{
			tail++;
			if (tail == 100)// 1�ʰ� ������
			{
				tail = 0;
				sclock = clock();
			}
		}

		// Frame
		IColorFrame* pColorFrame = 0;
		IDepthFrame* pDepthFrame = 0;
		hResult = pDepthReader->AcquireLatestFrame(&pDepthFrame);
		if (SUCCEEDED(hResult))
		{
			hResult = pDepthFrame->AccessUnderlyingBuffer(&depthbufferSize, reinterpret_cast<UINT16**>(&depthBufferMat.data));
			if (SUCCEEDED(hResult))
				depthBufferMat.convertTo(depthMat, CV_8U, -255.0f / 8000.0f, 255.0f);
		}
		hResult = pColorReader->AcquireLatestFrame(&pColorFrame);
		if (SUCCEEDED(hResult))
		{
			hResult = pColorFrame->CopyConvertedFrameDataToArray(colorbufferSize, reinterpret_cast<BYTE*>(colorBufferMat.data), ColorImageFormat::ColorImageFormat_Bgra);
			if (SUCCEEDED(hResult))
				cv::resize(colorBufferMat, bodyMat, cv::Size(), 0.5, 0.5);
		}
		// SafeRelease(pColorFrame);

		IBodyFrame* pBodyFrame = 0;
		hResult = pBodyReader->AcquireLatestFrame(&pBodyFrame);
		if (SUCCEEDED(hResult))
		{
			IBody* pBody[BODY_COUNT] = { 0 };
			hResult = pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, pBody);
			if (SUCCEEDED(hResult))
			{
				for (int count = 0; count < BODY_COUNT; count++)
				{
					BOOLEAN bTracked = false;
					hResult = pBody[count]->get_IsTracked(&bTracked);
					if (SUCCEEDED(hResult) && bTracked)
					{
						Joint joint[JointType::JointType_Count];
						hResult = pBody[count]->GetJoints(JointType::JointType_Count, joint);
						if (SUCCEEDED(hResult) && TrackingConfidence_High)
						{
							// Joints
							for (int type = 0; type < JointType::JointType_Count; type++)
							{
								cv::Point jointPoint = changeCoordinates(joint, type);
								if ((jointPoint.x >= 0) && (jointPoint.y >= 0) && (jointPoint.x < colorWidth) && (jointPoint.y < colorHeight))
								{
									cv::circle(colorBufferMat, jointPoint, 8, static_cast<cv::Scalar>(color[count]), -1, CV_AA);
								}
							}
							drawSkeleton(colorBufferMat, joint);
							//wideSquatAlgo(joint);// �Լ� �Է� (5/3)
						}

						// Lean : ����
						PointF amount;
						hResult = pBody[count]->get_Lean(&amount);

						if (SUCCEEDED(hResult)) {
							// �ǽð� �ð� ������ ���� (4/30)
							std::cout << "[time] " << tm.tm_year + 1900 << "-" << tm.tm_mon + 1 << "-" << tm.tm_mday << " " << tm.tm_hour << ":" << tm.tm_min << ":" << tm.tm_sec << ":" << tail << std::endl;

							cv::Point jointPoint[25];
							for (int type = 0; type < JointType::JointType_Count; type++)
							{
								jointPoint[type] = changeCoordinates(joint, type);
							}
							//0-1-20
							double v1_x = jointPoint[1].x - jointPoint[0].x; //P8.x-P9.x
							double v1_y = jointPoint[1].y - jointPoint[0].y;
							double v2_x = jointPoint[1].x - jointPoint[20].x;
							double v2_y = jointPoint[1].y - jointPoint[20].y;
							double d1 = sqrt(pow((v1_x), 2) + pow((v1_y), 2));
							double d2 = sqrt(pow((v2_x), 2) + pow((v2_y), 2));

							double backAngle = acos((v1_x * v2_x + v1_y * v2_y) / (d1*d2)) * radian;

							//4-5-6
							double v3_x = jointPoint[9].x - jointPoint[8].x; //P8.x-P9.x
							double v3_y = jointPoint[9].y - jointPoint[8].y;
							double v4_x = jointPoint[9].x - jointPoint[10].x;
							double v4_y = jointPoint[9].y - jointPoint[10].y;
							double d3 = sqrt(pow((v3_x), 2) + pow((v3_y), 2));
							double d4 = sqrt(pow((v4_x), 2) + pow((v4_y), 2));

							double leftArm = acos((v3_x * v4_x + v3_y * v4_y) / (d3*d4)) * radian;

							//8-9-10
							double v9_x = jointPoint[9].x - jointPoint[8].x; //P8.x-P9.x
							double v9_y = jointPoint[9].y - jointPoint[8].y;
							double v11_x = jointPoint[9].x - jointPoint[10].x;
							double v11_y = jointPoint[9].y - jointPoint[10].y;
							double d9 = sqrt(pow((v9_x), 2) + pow((v9_y), 2));
							double d11 = sqrt(pow((v11_x), 2) + pow((v11_y), 2));

							double rightArm = acos((v9_x * v11_x + v9_y * v11_y) / (d9*d11)) * radian;

							// �ǽð� ���̷��� ������ ��ǥ ���� (4/23)

							// kinect-server�� �ð� �� skeleton ��ǥ ������ post (6/1)
							/* Now specify the POST data */
							char buf[200];
							sprintf(buf, "userId=white&backAngle=%.2f&rightArm=%.2f&leftArm=%.2f&timestamp=%d-%02d-%02d %02d:%02d:%02d.%03d", backAngle, rightArm, leftArm, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, tail);
							//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf); // ������ post (6/1)
							// ������ ���� ������ ���� (7/2)

							/* In windows, this will init the winsock stuff */
							curl_global_init(CURL_GLOBAL_ALL);

							/* get a curl handle */
							curl = curl_easy_init();
							if (curl) {
								/* First set the URL that is about to receive our POST. This URL can
								just as well be a https:// URL if that is what should receive the
								data. */
								curl_easy_setopt(curl, CURLOPT_URL, "http://10.200.27.150:8080/joints");
								/* Now specify the POST data */
								curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);

								/* Perform the request, res will get the return code */
								res = curl_easy_perform(curl);
								/* Check for errors */
								if (res != CURLE_OK)
									fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

								/* always cleanup */
								curl_easy_cleanup(curl);

								curl_global_cleanup();

								std::cout << "backAngle: " << backAngle << " rightArm: " << rightArm << " leftArm: " << leftArm << std::endl;

							}

						}


						//if (SUCCEEDED(hResult))
						//   std::cout << "amount: " << amount.X << ", " << amount.Y << std::endl;
						// std::�� ���� �ܼ� â�� ���, �Ⱦ��� ���������
					}
				}
				cv::resize(colorBufferMat, bodyMat, cv::Size(), 0.5, 0.5);
			}
			for (int count = 0; count < BODY_COUNT; count++)
			{
				SafeRelease(pBody[count]);
			}
		}
		SafeRelease(pColorFrame);
		SafeRelease(pBodyFrame);
		SafeRelease(pDepthFrame);

		// save color image, depth image
		cv::imshow(colorWinName, bodyMat);
		cv::imshow(depthWinName, depthMat);

		if (cv::waitKey(10) == VK_ESCAPE)
			break;
	}

	SafeRelease(pColorSource);
	SafeRelease(pBodySource);
	SafeRelease(pColorReader);
	SafeRelease(pBodyReader);
	SafeRelease(pColorDescription);
	SafeRelease(pCoordinateMapper);
	SafeRelease(pDepthSource);
	SafeRelease(pDepthReader);
	SafeRelease(pDepthDescription);
	if (pSensor)
		pSensor->Close();
	SafeRelease(pSensor);
	cv::destroyAllWindows();
}
