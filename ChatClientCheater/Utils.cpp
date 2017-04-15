#include "StdAfx.h"
#include "Definations.h"
#include "Utils.h"


CString  CUtils::m_csAppDataPath;
CString  CUtils::m_csSettingsPath;

CUtils::CUtils(void)
{
}

CUtils::~CUtils(void)
{
}
void CUtils::GetAllFiles(LPCTSTR lpPath,CStringArray &csaFileArray)
{
	CFileFind finder;

	// build a string with wildcards
	CString strWildcard(lpPath);
	strWildcard += _T("\\*.*");

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!

		if (finder.IsDots())
			continue;

		// if it's a directory, recursively search it

		if (finder.IsDirectory())
		{
			CString str = finder.GetFilePath();
			csaFileArray.Add(str );
			GetAllFiles(str,csaFileArray);
		}
		else
		{
			CString str = finder.GetFilePath();
			csaFileArray.Add(str );
		}
	}

	finder.Close();
}


BOOL CUtils::OpenFolderDialog(CString& csFolderPath)
{
	BOOL bRet =FALSE;
	BROWSEINFO bi; 
	ZeroMemory(&bi, sizeof(bi)); 
	TCHAR szDisplayName[MAX_PATH]; 
	szDisplayName[0] = ' '; 

	bi.hwndOwner = NULL; 
	bi.pidlRoot = NULL; 
	bi.pszDisplayName = szDisplayName; 
	bi.lpszTitle = _T("Please select a folder:"); 
	bi.ulFlags = BIF_RETURNONLYFSDIRS; 
	bi.lParam = NULL; 
	bi.iImage = 0; 

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi); 
	TCHAR szPathName[MAX_PATH]; 
	if (NULL != pidl) 
	{ 
		bRet = SHGetPathFromIDList(pidl,szPathName); 
		if(bRet)
		{
			csFolderPath=szPathName;
		}
	}
	return bRet;
}
int CUtils::ReadFromFile(LPCTSTR lpcFileName, CString &cContents)
{
	if( !lpcFileName || lpcFileName[0]=='\0' )
	{
		return -1;
	}	
	CFile cf;
	CFileStatus status;
	TCHAR *szText = NULL;
	CFileException cfe;

	try
	{
		if(!cf.Open(lpcFileName, CFile::modeRead|CFile::shareDenyWrite, &cfe))
		{
			//WriteLog("GetContentsFromFile - Open file error");
			return -1;
		}

		cf.GetStatus(status);
		szText = new TCHAR[(UINT)status.m_size + 1];
		cf.Read(szText,(UINT)status.m_size);
		szText[status.m_size]=0;
		cContents = szText;

		if(szText)
		{
			delete[] szText;
			szText = NULL;
		}	
		cf.Close();

	}
	catch(CFileException *ce)
	{
		ce->Delete();
		cf.Close();		
		return -1;
	}


	return 1;
}

BOOL CUtils::DoesFileExist(LPCTSTR lpcFileName )
{
	if( !lpcFileName || lpcFileName[0]=='\0' )
	{
		return FALSE;
	}

	CFileStatus status;
	BOOL bRet = FALSE;

	bRet = CFile::GetStatus (lpcFileName, status );
	return bRet;
}

int CUtils::WriteToFile(LPCTSTR lpcFileName, LPCTSTR lpcText)
{
	if( !lpcFileName || lpcFileName[0]=='\0' )
	{
		return -1;
	}
	if(!lpcText || lpcText[0]=='\0') return -1;


	CFile cf;
	CFileException cfe;

	try
	{
		if(!cf.Open(lpcFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyWrite, &cfe))
		{
			return 0;
		}

		cf.Write(lpcText, lstrlen(lpcText) );
		cf.Close();
	}
	catch(CFileException *ce)
	{
		ce->Delete();
		cf.Close();

		return -1;
	}

	return 1;
}

int CUtils::AppendToFile(LPCTSTR lpFileName, LPCTSTR lpData)
{
	CString csFileName = lpFileName;
	CString csData = lpData;
	if(csFileName.IsEmpty() || csData.IsEmpty())
	{
		return -1;
	}


	CStdioFile cf;
	if(	cf.Open(lpFileName,CFile::modeCreate| CFile::modeWrite|CFile::modeNoTruncate)<=0)
	{
		return -1;
	}
	cf.SeekToEnd();
	cf.WriteString(csData+ "\n");

	cf.Close();
	return 1;
}

CString CUtils::GetAppDataPath()
{
	if(!m_csAppDataPath.IsEmpty())
		return m_csAppDataPath;

	CString csPath;
	TCHAR szPath[MAX_PATH];
	int nSize=0;
	SHGetFolderPath(NULL,CSIDL_LOCAL_APPDATA,NULL,nSize,szPath);
	//To do later
	csPath=szPath;
	csPath=csPath+"\\"+COMPANY_NAME+"\\";

	if(FALSE==DoesFileExist(csPath))
		CreateDirectory(csPath,NULL);

	csPath=csPath+APPLICATION_NAME+"\\";

	if(FALSE==DoesFileExist(csPath))
	{

		CreateDirectory(csPath,NULL);
	}

	m_csAppDataPath =csPath;

	return m_csAppDataPath;
}
CString GetStringFromIni(const char* const szPath, const char* const szKey, const char* const szValue)
{
	CString csPath;

	GetPrivateProfileString(szPath,szKey,"",csPath.GetBuffer(),1024,szPath);
	return csPath;
}
CString CUtils::GetShortcutkey()
{
	LPCTSTR lpPath=GetSettingsPath();
	TCHAR szSkey[2];
	GetPrivateProfileString("Settings","Shortcut","",szSkey,2,lpPath);
	return szSkey;
}
void CUtils::SetShortcutkey(LPCTSTR lpSKey)
{
	LPCTSTR lpPath=GetSettingsPath();
	WritePrivateProfileString("Settings","Shortcut",lpSKey,lpPath);	
}

CString CUtils::GetSettingsPath()
{
	if(!m_csSettingsPath.IsEmpty())
		return m_csSettingsPath;
	m_csSettingsPath=GetAppDataPath()+SETTINGS_PATH;
	if(!DoesFileExist(m_csSettingsPath))
		SetShortcutkey("s");

	return m_csSettingsPath;
}

int CUtils::SplitString(const CString& input, const CString& delimiter, vector<CString>& results)
{	
	results.clear();

	if(input.IsEmpty())
		return 0;

	int iPos = 0;
	int newPos = -1;
	int sizeS2 = delimiter.GetLength();
	int isize = input.GetLength();


	CArray<INT, int> positions;

	newPos = input.Find (delimiter, 0);

	if( newPos < 0 ) 
	{ 
		results.push_back(input);
		return 0; 
	}
	else if(newPos==0)
	{
		int nPos=1;
		while(newPos==0 && input.GetLength()>0)
		{
			newPos = input.Find (delimiter, nPos);
			if(nPos==1 && newPos==-1 && input.GetLength()>1)
			{
				results.push_back(input.Mid(1));
			}
			nPos++;
		}
	}


	int numFound = 0;

	while( newPos > iPos )
	{
		numFound++;
		positions.Add(newPos);
		iPos = newPos;
		newPos = input.Find (delimiter, iPos+sizeS2+1);
	}
	int nDelimitterPos=-1;
	for( int i=0; i < positions.GetSize(); i++ )
	{
		nDelimitterPos=positions[i] ;
		CString s;
		if( i == 0 )
			s = input.Mid( i,nDelimitterPos );
		else
		{
			int offset = positions[i-1] + sizeS2;
			if( offset < isize )
			{
				if( i == positions.GetSize() )
					s = input.Mid(offset);
				else if( i > 0 )
					s = input.Mid( positions[i-1] + sizeS2, 
					positions[i] - positions[i-1] - sizeS2 );
			}
		}
		if( s.GetLength() > 0 )
			results.push_back(s);
	}
	if(input.GetLength()>nDelimitterPos)
	{
		results.push_back(input.Mid(nDelimitterPos+delimiter.GetLength()));
	}
	return numFound;
}