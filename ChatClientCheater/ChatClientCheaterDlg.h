
// ChatClientCheaterDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CChatClientCheaterDlg dialog
class CChatClientCheaterDlg : public CDialog
{
// Construction
public:
	CChatClientCheaterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CHATCLIENTCHEATER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnDestroy();
	CComboBox m_cmbOptions;
	afx_msg void OnBnClickedCancel();
	CButton m_chkScheduler;
	afx_msg void OnBnClickedChkSchduler();
	CDateTimeCtrl m_DateTimeOption;
	//CDateTimeCtrl m_TimeOption;
	afx_msg void OnFileExit();
	afx_msg void OnHelpAbout();
	afx_msg void OnToolsSettings();
};
