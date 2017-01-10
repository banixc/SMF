#include "stdafx.h"
#include "OpenCVOp.h"

using namespace cv;

void OpenCVOp::showL()
{
	if (frameL.empty())
		return;
	Mat resized;
	resize(frameL, resized, resizeL);
	imshow(WIND_L, resized);
}

void OpenCVOp::showR()
{
	if (frameR.empty())
		return;
	Mat resized;
	resize(frameR, resized, resizeR);
	imshow(WIND_R, resized);
}

void OpenCVOp::showV()
{
	if (frameV.empty())
		return;
	Mat resized;
	resize(frameV, resized, resizeV);
	imshow(WIND_V, resized);
}

OpenCVOp::OpenCVOp()
{
	captureL = VideoCapture();
	captureR = VideoCapture();
}


OpenCVOp::~OpenCVOp()
{
}

int OpenCVOp::countCameras()
{
	VideoCapture temp_camera;
	int maxTested = 10;
	for (int i = 0; i < maxTested; i++) {
		VideoCapture temp_camera(i);
		bool res = (!temp_camera.isOpened());
		temp_camera.release();
		if (res)
		{
			return i;
		}
	}
	return maxTested;
}

bool OpenCVOp::openCamera(int l, int r)
{
	idL = l;
	idR = r;
	captureL.open(idL);
	captureR.open(idR);
	return captureL.isOpened() && captureR.isOpened();
}

void OpenCVOp::showCamera(bool flag)
{
	if (flag) {
		hThreadCamera = CreateThread(NULL, 0, ShowCameraThread, (LPVOID*)this, NULL, 0);
	}
	else {
		CloseHandle(hThreadCamera);
		captureL.release();
		captureR.release();
	}
}

void OpenCVOp::showVision(bool flag)
{
	if (flag) {
		hThreadVision = CreateThread(NULL, 0, ShowVisionThread, (LPVOID*)this, NULL, 0);
	}
	else {
		CloseHandle(hThreadVision);
	}
}

HWND OpenCVOp::bindWindow(const char * winname, int width, int height)
{
	namedWindow(winname);
	resizeWindow(winname, width, height);
	if (winname == WIND_L) {
		resizeL = Size(width, height);
	} 
	else if (winname == WIND_R) {
		resizeR = Size(width, height);
	}
	else if (winname == WIND_V) {
		resizeV = Size(width, height);
	}
	return (HWND)cvGetWindowHandle(winname);
}

DWORD OpenCVOp::ShowCameraThread(void * pArg)
{
	OpenCVOp* t = (OpenCVOp*)pArg;
	while (true) {
		t->captureL >> t->frameL;
		t->captureR >> t->frameR;
		t->showL();
		t->showR();
	}
	return 0;
}

DWORD OpenCVOp::ShowVisionThread(void * pArg)
{
	OpenCVOp* t = (OpenCVOp*)pArg;
	while (true) {

	}
	return 0;
}
