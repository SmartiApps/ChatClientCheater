// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatClientCheater.h"
#include "SettingsDlg.h"
#include "Utils.h"

// CSettingsDlg dialog

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialog)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSettingsDlg::IDD, pParent)
{

}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_PASS, m_edPass);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	ON_BN_CLICKED(ID_APPLY, &CSettingsDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDCANCEL, &CSettingsDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_ED_PASS, &CSettingsDlg::OnEnChangeEdPass)

END_MESSAGE_MAP()


// CSettingsDlg message handlers
BOOL CSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString csPass=CUtils::GetShortcutkey();
	if(csPass.GetLength()>1)
	{
		csPass=csPass.Left(1);
	}
	m_edPass.SetWindowText(csPass);
	return TRUE;
}

void CSettingsDlg::OnBnClickedApply()
{
	CString csPass;

	m_edPass.GetWindowText(csPass); 
	if(csPass.GetLength()==0)
	{
		AfxMessageBox("Please set a password ");
		return;
	}
	else
		CUtils::SetShortcutkey(csPass);
	OnCancel();
}

void CSettingsDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CSettingsDlg::OnEnChangeEdPass()
{
	CString csPass;

	m_edPass.GetWindowText(csPass); 
	if(csPass.GetLength()>1)
	{
		csPass=csPass.Left(1);
		m_edPass.SetWindowText(csPass);
	}
}

