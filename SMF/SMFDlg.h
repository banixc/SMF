
// SMFDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "OpenCVOp.h"

#define TEXT_OPEN L"打开"
#define TEXT_CLOSE L"关闭"
#define TEXT_OPEN_FAILED L"打开摄像头失败，请检查你的设备！"
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
	bool openingCamera = false;

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
	CButton m_bCalibrate;
	CButton m_bFromCamera;
	CButton m_bFromPic;
	CButton m_bFromVideo;
	CStatusBar m_bar;

	bool openCamera(bool flag);
	void initCombox();
	void initStatusBar();
	void bindCVWindow(int nID, const char * winname);
	bool validParam();	
	int getInt(int nID);
public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonCalibrate();
	afx_msg void OnBnClickedButtonFromCamera();
	afx_msg void OnBnClickedButtonFromPic();
	afx_msg void OnBnClickedButtonFromVideo();
	afx_msg void OnBnClickedButtonCut();
protected:
	CEdit m_eNum;
	CButton m_bCut;
};
