
// SMFDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "OpenCVOp.h"

// CSMFDlg 对话框
class CSMFDlg : public CDialogEx
{
// 构造
public:
	CSMFDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMF_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	OpenCVOp op;

	// 生成的消息映射函数
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
	CButton m_bFromCamera;
	CButton m_bFromPic;
	CButton m_bFromFile;
	CStatusBar m_bar;
	CEdit m_eNum;
	CButton m_bCut;

	void initCombox();
	void initStatusBar();
	bool validParam();	
	int getInt(int nID);
	void showNextFrame();
	void ShowMat(cv::Mat & image, int IDC);

	void convertPoint(CPoint & p);
	void lockCameraButton();
	void lockCalibrateButton();
	void lockCalibrateButton(boolean lock);
	void lockParamsInput(boolean lock);
	void unlockCalibrateButton();

public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonFromCamera();
	afx_msg void OnBnClickedButtonFromPic();
	afx_msg void OnBnClickedButtonFromFile();
	afx_msg void OnBnClickedButtonCut();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);


};
