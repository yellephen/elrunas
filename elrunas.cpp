#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    STARTUPINFOW Si = { sizeof(STARTUPINFOW) };
    PROCESS_INFORMATION Pi = { 0 };

    if (argc < 5) {
        printf("Usage: elrunas <username> <domain> <password> <executable>\n");
        return 1;
    }

    // Convert arguments to wide-character strings
    WCHAR wUsername[256], wDomain[256], wPassword[256], wExecutable[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, argv[1], -1, wUsername, 256);
    MultiByteToWideChar(CP_ACP, 0, argv[2], -1, wDomain, 256);
    MultiByteToWideChar(CP_ACP, 0, argv[3], -1, wPassword, 256);
    MultiByteToWideChar(CP_ACP, 0, argv[4], -1, wExecutable, MAX_PATH);

    BOOL result = CreateProcessWithLogonW(
        wUsername, wDomain, wPassword,
        LOGON_WITH_PROFILE, NULL, wExecutable,
        0, NULL, NULL, &Si, &Pi);

    if (!result)
    {
        printf("CreateProcessWithLogonW Failed (%d)\n", GetLastError());
        return 1;
    }

    WaitForSingleObject(Pi.hProcess, INFINITE);

    CloseHandle(Pi.hProcess);
    CloseHandle(Pi.hThread);

    return 0;
}
