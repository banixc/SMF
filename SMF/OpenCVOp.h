#pragma once
#include <opencv2\opencv.hpp>

using namespace cv;

class OpenCVOp
{
protected:
	HANDLE hThreadCamera;
	HANDLE hThreadVision;

	void showL();
	void showR();
	void showV();
public:
	OpenCVOp();
	~OpenCVOp();

	int idL, idR;
	VideoCapture captureL, captureR;
	Mat frameL, frameR, frameV;
	Size resizeL, resizeR, resizeV;

	int countCameras();
	bool openCamera(int l, int r);
	void showCamera(bool flag);
	void showVision(bool flag);
	HWND bindWindow(const char * winname, int width, int height);

	static DWORD WINAPI ShowCameraThread(void *pArg);	//显示摄像机
	static DWORD WINAPI ShowVisionThread(void *pArg);	//显示视差图
};

