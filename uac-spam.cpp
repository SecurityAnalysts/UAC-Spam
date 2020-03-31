bool IsElevated(){
    DWORD dwSize = 0;
    HANDLE hToken = NULL;
    BOOL bReturn = FALSE;
 
    TOKEN_ELEVATION tokenInformation;
     
    if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
        return FALSE;
 
    if(GetTokenInformation(hToken, TokenElevation, &tokenInformation, sizeof(TOKEN_ELEVATION), &dwSize))
    {
        bReturn = (BOOL)tokenInformation.TokenIsElevated;
    }
 
    CloseHandle(hToken);
    return bReturn;
}

bool ElevateNow(){
	bool bAlreadyRunningAsAdministrator = IsElevated();
	if(!bAlreadyRunningAsAdministrator)
	{
		char szPath[MAX_PATH];
		if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
		{
			SHELLEXECUTEINFO sei = { sizeof(sei) };
			
			sei.lpVerb = "runas";
			sei.lpFile = szPath;
			sei.hwnd = NULL;
			sei.nShow = SW_NORMAL;

			if (!ShellExecuteEx(&sei))
			{
				DWORD dwError = GetLastError();
				
				if (dwError == ERROR_CANCELLED){
					CreateThread(0,0,(LPTHREAD_START_ROUTINE)ElevateNow,0,0,0);
				}
				
			}else{

				exit (EXIT_FAILURE);
			
			}
		}
	}
}
