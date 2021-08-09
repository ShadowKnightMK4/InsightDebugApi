#include "Process.h"

bool Process::Lockdown()
{
	HANDLE Self;

	HANDLE RestrictedSelf;
	TOKEN_PRIVILEGES *Priv;
	DWORD AllotMe = 0;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_DUPLICATE | TOKEN_QUERY, &Self))
	{
		GetTokenInformation(Self, TokenGroupsAndPrivileges, nullptr, 0, &AllotMe);

		if (AllotMe != 0)
		{
			Priv = (TOKEN_PRIVILEGES*)malloc(AllotMe + sizeof(TOKEN_PRIVILEGES));
			if (Priv)
			{
				memset(Priv, 0, AllotMe + sizeof(TOKEN_PRIVILEGES));
				if (GetTokenInformation(Self, TokenGroupsAndPrivileges, Priv, AllotMe, &AllotMe))
				{
		//			CreateRestrictedToken(Self, DISABLE_MAX_PRIVILEGE, )
				}
			}
		}
		CloseHandle(Self);
	}
	return false;
}
