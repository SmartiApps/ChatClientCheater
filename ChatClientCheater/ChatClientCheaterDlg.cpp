
// ChatClientCheaterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatClientCheater.h"
#include "ChatClientCheaterDlg.h"
#include "Hook.h"
#include "hyperlink.h"
#include "SettingsDlg.h"
#include "Utils.h"
#pragma comment(lib,"Version.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	virtual BOOL OnInitDialog();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CHyperLink m_stwebLink;
	CHyperLink m_stEmail;
	CStatic m_stVersion;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_WEB, m_stwebLink);
	DDX_Control(pDX, IDC_ST_EMAIL, m_stEmail);
	DDX_Control(pDX, IDC_ST_VER, m_stVersion);
}
BOOL GetAppVersion( char *LibName, WORD *MajorVersion, WORD *MinorVersion, WORD *BuildNumber, WORD *RevisionNumber )
{
	DWORD dwHandle, dwLen;
	UINT BufLen;
	LPTSTR lpData;
	VS_FIXEDFILEINFO *pFileInfo;
	dwLen = GetFileVersionInfoSize( LibName, &dwHandle );
	if (!dwLen) 
		return FALSE;

	lpData = (LPTSTR) malloc (dwLen);
	if (!lpData) 
		return FALSE;

	if( !GetFileVersionInfo( LibName, dwHandle, dwLen, lpData ) )
	{
		free (lpData);
		return FALSE;
	}
	if( VerQueryValue( lpData, "\\", (LPVOID *) &pFileInfo, (PUINT)&BufLen ) ) 
	{
		*MajorVersion = HIWORD(pFileInfo->dwFileVersionMS);
		*MinorVersion = LOWORD(pFileInfo->dwFileVersionMS);
		*BuildNumber = HIWORD(pFileInfo->dwFileVersionLS);
		*RevisionNumber = LOWORD(pFileInfo->dwFileVersionLS);
		free (lpData);
		return TRUE;
	}
	free (lpData);
	return FALSE;
}
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString csVersion;

	WORD MajorVersion        =0; 
	WORD MinorVersion        =0; 
	WORD BuildNumber        =0; 
	WORD RevisionNumber        =0;

	TCHAR fileName[4096];
	GetModuleFileName(NULL,fileName,4096);
	//theApp.m_pszExeName is no good

	GetAppVersion(fileName,
		&MajorVersion,    
		&MinorVersion,    
		&BuildNumber,    
		&RevisionNumber);

	csVersion.Format (_T("%hu.%hu"),MajorVersion,MinorVersion);

	m_stVersion.SetWindowText(csVersion);
	m_stwebLink.SetURL("http://www.smartpricedeal.com/products/chatclientcheater.html");
	return TRUE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CChatClientCheaterDlg dialog




CChatClientCheaterDlg::CChatClientCheaterDlg(CWnd* pParent /*=NULL*/)
: CDialog(CChatClientCheaterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_LOGO);
}

void CChatClientCheaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_OPTIONS, m_cmbOptions);
	DDX_Control(pDX, IDC_CHK_SCHDULER, m_chkScheduler);
	DDX_Control(pDX, IDC_DATETIME_OPTION, m_DateTimeOption);
	//DDX_Control(pDX, IDC_DATETIMEPICKER2, m_TimeOption);
}

BEGIN_MESSAGE_MAP(CChatClientCheaterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CChatClientCheaterDlg::OnBnClickedBtnStart)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL, &CChatClientCheaterDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHK_SCHDULER, &CChatClientCheaterDlg::OnBnClickedChkSchduler)
	ON_COMMAND(ID_FILE_EXIT, &CChatClientCheaterDlg::OnFileExit)
	ON_COMMAND(ID_HELP_ABOUT, &CChatClientCheaterDlg::OnHelpAbout)
	ON_COMMAND(ID_TOOLS_SETTINGS, &CChatClientCheaterDlg::OnToolsSettings)
END_MESSAGE_MAP()


// CChatClientCheaterDlg message handlers

BOOL CChatClientCheaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_DateTimeOption.SetFormat("MMM dd, yyyy HH:mm:ss");
	m_chkScheduler.SetCheck(FALSE);
	OnBnClickedChkSchduler();

	m_cmbOptions.AddString("Lock");
	m_cmbOptions.AddString("Log off");
	m_cmbOptions.AddString("Shutdown");
	m_cmbOptions.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatClientCheaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatClientCheaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatClientCheaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CChatClientCheaterDlg::OnBnClickedBtnStart()
{
	if(m_chkScheduler.GetCheck()==TRUE)
	{
		COleDateTime cDateTime;
		COleDateTime cCurDateTime;

		m_DateTimeOption.GetTime(cDateTime);
		cCurDateTime=COleDateTime::GetCurrentTime();
		if(cDateTime<cCurDateTime)
		{
			AfxMessageBox("Please select the valid Date/Time");
			return;
		}

		g_nSelectedOption=m_cmbOptions.GetCurSel();
		if(g_nSelectedOption>0)
		{
			if(IDNO  == AfxMessageBox("Please save all your files before you proceed. Otherwise, you will lose it. Do you want to proceed?",MB_YESNO))
			{
				return;
			}
		}
		g_OleScheduleTime=cDateTime;
		g_bScheduler=TRUE;
	}
	if(CUtils::GetShortcutkey().GetLength()>0)
	{
		CString csPass=CUtils::GetShortcutkey().MakeUpper();
		g_chrShortcut=csPass.GetAt(0);
	}

	Sleep(2000);
	
	if((g_hDesktopHook = SetWindowsHookEx(WH_MOUSE_LL, KeyBoardMouseProc, NULL, 0)) == NULL)
	{
		// failed
	}

	if((g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardMouseProc, NULL, 0)) == NULL)
	{
		// failed
	}
	g_MouseThread=AfxBeginThread(MouseMoveThread,NULL);
	this->ShowWindow(FALSE);
}

void CChatClientCheaterDlg::OnDestroy()
{
	CDialog::OnDestroy();

	g_bMouseEvent=FALSE;
	if(g_hDesktopHook)
		UnhookWindowsHookEx(g_hDesktopHook);

	if(g_hKeyboardHook)
		UnhookWindowsHookEx(g_hKeyboardHook);

	WaitForSingleObject(g_MouseThread->m_hThread,5000);
}

void CChatClientCheaterDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CChatClientCheaterDlg::OnBnClickedChkSchduler()
{
	if(m_chkScheduler.GetCheck()==TRUE)
	{
		m_cmbOptions.EnableWindow();
		m_DateTimeOption.EnableWindow();
		//		m_TimeOption.EnableWindow();
	}
	else
	{
		m_cmbOptions.EnableWindow(FALSE);
		m_DateTimeOption.EnableWindow(FALSE);
		//	m_TimeOption.EnableWindow(FALSE);
	}
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CChatClientCheaterDlg::OnFileExit()
{
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}

void CChatClientCheaterDlg::OnHelpAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CChatClientCheaterDlg::OnToolsSettings()
{
	CSettingsDlg dlg;
	dlg.DoModal();
}
