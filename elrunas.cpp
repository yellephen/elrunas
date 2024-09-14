#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    STARTUPINFOW Si = { sizeof(STARTUPINFOW) };
    PROCESS_INFORMATION Pi = { 0 };

    if (argc < 4) {
        printf("Usage: elrunas <username>@<host|domain> <password> <executable>\n");
        return 1;
    }

    // Convert arguments to wide-character strings
    WCHAR wUsername[256], wDomain[256], wPassword[256], wExecutable[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, argv[1], -1, wUsername, 256);
    MultiByteToWideChar(CP_ACP, 0, argv[2], -1, wPassword, 256);
    MultiByteToWideChar(CP_ACP, 0, argv[3], -1, wExecutable, MAX_PATH);

    BOOL result = CreateProcessWithLogonW(
        wUsername, NULL, wPassword,
        0, NULL, wExecutable,
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
