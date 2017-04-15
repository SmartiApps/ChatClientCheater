#pragma once
#include <vector>
using namespace std;
class CUtils
{
public:
	CUtils(void);
	~CUtils(void);
	static void GetAllFiles(LPCTSTR lpPath, CStringArray &csaFileArray);
	 static int ReadFromFile(LPCTSTR lpcFileName, CString &cContents);
	static BOOL DoesFileExist(LPCTSTR lpcFileName );
	static int WriteToFile(LPCTSTR lpcFileName, LPCTSTR lpcText);
	static int AppendToFile(LPCTSTR lpFileName, LPCTSTR lpData);
	static BOOL OpenFolderDialog(CString& csFolderPath);

	static int SplitString(const CString& input, const CString& delimiter, vector<CString>& results);
	//Settings
	static CString GetShortcutkey();
	static void SetShortcutkey(LPCTSTR lpSKey);
	static CString GetAppDataPath();
	static CString GetSettingsPath();
	static CString  m_csAppDataPath;
	static CString  m_csSettingsPath;
};
