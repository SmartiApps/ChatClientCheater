
HHOOK g_hDesktopHook; //  global
HHOOK g_hKeyboardHook; //  global
BOOL g_bMouseEvent=TRUE;
char g_chrShortcut='s';
BOOL g_bScheduler=FALSE;
COleDateTime g_OleScheduleTime;
int g_nSelectedOption=-1;
enum 
{
	eLock=0,
	eLogOff,
	eShutdown
};
CWinThread *g_MouseThread=NULL;
int g_nXPos=0;
int g_nYpos=0;

void SendMouseMoveMessage()
{
	INPUT ip;

	ip.type = INPUT_MOUSE;
	ip.mi.dx = 0;
	ip.mi.dy = 0;
	ip.mi.mouseData = 0;
	ip.mi.dwFlags = (MOUSEEVENTF_MOVE || MOUSEEVENTF_ABSOLUTE);
	ip.mi.time = 0;
	ip.mi.dwExtraInfo = GetMessageExtraInfo();
	SendInput(1, &ip, sizeof(ip));
}
void DoScheduledProcess()
{
	COleDateTime cCurDateTime=COleDateTime::GetCurrentTime();
	if(cCurDateTime>g_OleScheduleTime)
	{
		if(g_nSelectedOption==eLock)
		{
			LockWorkStation() ;
			AfxGetMainWnd()->PostMessage(WM_CLOSE);
		}
		else if(g_nSelectedOption==eLogOff)
		{
			system("shutdown -l -f");
		}
		else if(g_nSelectedOption==eShutdown)
		{
			system("shutdown -s -f -t 00");
		}

	}
}
UINT MouseMoveThread(LPVOID lpMsg)
{
	while(g_bMouseEvent)
	{
		SendMouseMoveMessage();
		if(g_bScheduler==TRUE)
		{
			DoScheduledProcess();
		}
		Sleep(5000);
	}
	return 0;
}
LRESULT CALLBACK KeyBoardMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0)
		return CallNextHookEx(g_hDesktopHook, nCode, wParam, lParam);
	KBDLLHOOKSTRUCT *msg = (KBDLLHOOKSTRUCT*)lParam;
	if(nCode == HC_ACTION)
	{

		if(wParam == WM_LBUTTONDOWN ||wParam == WM_RBUTTONDOWN ||wParam ==WM_MOUSEMOVE )
		{
			LPMOUSEHOOKSTRUCT lpMouse=NULL;
			lpMouse=(LPMOUSEHOOKSTRUCT)lParam;
			if(lpMouse)
			{
				if(g_nXPos==0 && g_nYpos==0)
				{
					g_nXPos=lpMouse->pt.x;
					g_nYpos=lpMouse->pt.y;
				}
				if(g_nXPos!=lpMouse->pt.x||	g_nYpos!=lpMouse->pt.y)
				{
					g_bMouseEvent=FALSE;
					//Lock Computer
					LockWorkStation() ;
					AfxGetMainWnd()->PostMessage(WM_CLOSE);
				}
			}
		}
		else if (  wParam == WM_KEYUP || wParam ==WM_KEYDOWN) 
		{
			if ( msg->vkCode ==g_chrShortcut ) 
			{
				//If U is pressed Remain Unlocked and Exit 
				//Else lock computer
				g_bMouseEvent=FALSE;
				AfxGetMainWnd()->PostMessage(WM_CLOSE);

			}
			else
			{
				g_bMouseEvent=FALSE;
				LockWorkStation() ;
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
			}
		}   
	}

	return CallNextHookEx(g_hDesktopHook, nCode, wParam, lParam);
}