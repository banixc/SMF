
// SMFDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "OpenCVOp.h"

#define TEXT_OPEN L"��"
#define TEXT_CLOSE L"�ر�"
#define TEXT_OPEN_FAILED L"������ͷʧ�ܣ���������豸��"
// CSMFDlg �Ի���
class CSMFDlg : public CDialogEx
{
// ����
public:
	CSMFDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMF_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	OpenCVOp op;
	bool openingCamera = false;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CComboBox m_cSelectL;
	CComboBox m_cSelectR;
	CEdit m_eGridH;
	CEdit m_eGridW;
	CEdit m_eReally;
	CButton m_bOpen;
	CButton m_bCalibrate;
	CButton m_bFromCamera;
	CButton m_bFromPic;
	CButton m_bFromFile;
	CStatusBar m_bar;

	void initCombox();
	void initStatusBar();
	bool validParam();	
	int getInt(int nID);

	void showNextFrame();


public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonFromCamera();
	afx_msg void OnBnClickedButtonFromPic();
	afx_msg void OnBnClickedButtonFromFile();
	afx_msg void OnBnClickedButtonCut();

protected:
	CEdit m_eNum;
	CButton m_bCut;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void ShowMat(cv::Mat & image, int IDC);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void convertPoint(CPoint & p);
	void lockCameraButton();
	void lockCalibrateButton();
	void lockCalibrateButton(boolean lock);
	void unlockCalibrateButton();
};
