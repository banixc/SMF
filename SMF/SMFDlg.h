
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
