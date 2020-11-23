#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


#define SERVERPORT 9000
#define BUFSIZE    5

static DWORD threadId[2];

struct Key
{
    char ckey;
};
Key key;

typedef struct PlayerInfo
{
    int PosX;
    int PosY;
    int playerID;
    int HP;
    int money;
};
PlayerInfo p1_info;

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}


DWORD WINAPI RecvFromClient(LPVOID arg);
DWORD WINAPI SendToClient(LPVOID arg);

void initPlayerInfo(PlayerInfo* pInfo)
{
    pInfo->PosX = 400;
    pInfo->PosY = 350;
    pInfo->HP = 100;
    pInfo->money = 1000;
}


void movePlayer(char keycode) 
{
    switch (keycode)
    {
    case 'W':
        p1_info.PosY -= 1;
        std::cout << "w : 값 바뀜 -> " << p1_info.PosX << "," << p1_info.PosY << std::endl;

        break;
    case 'A':
        p1_info.PosX -= 1;
        std::cout << "a : 값 바뀜 -> " << p1_info.PosX << "," << p1_info.PosY << std::endl;

        break;
    case 'S':
        p1_info.PosY += 1;
        std::cout << "s : 값 바뀜  -> " << p1_info.PosX << "," << p1_info.PosY << std::endl;

        break;
    case 'D':
        p1_info.PosX += 1;
        std::cout << "d : 값 바뀜 -> " << p1_info.PosX << "," << p1_info.PosY << std::endl;

        break;
    default:
        break;
    }
}

int recvn(SOCKET s, char* buf, int len, int flags);


// 사용자 정의 데이터 수신 함수

int main(int argc, char* argv[])
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");
    // 데이터 통신에 사용할 변수
    SOCKET client_sock;

    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE];
    HANDLE recvThread;
    HANDLE sendThread;

    initPlayerInfo(&p1_info);
    p1_info.playerID = 1;

    printf("서버 열림\n");
    while (1) {
        // accept()

        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }


        recvThread = CreateThread(NULL, 0, RecvFromClient, (LPVOID)client_sock, 0,NULL);
        
        if (recvThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(recvThread); }


     
    }



    // closesocket()
    closesocket(listen_sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}

int recvn(SOCKET s, char* buf, int len, int flags)
{
    int received;
    char* ptr = buf;
    int left = len;

    while (left > 0) {
        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR)
            return SOCKET_ERROR;
        else if (received == 0)
            break;
        left -= received;
        ptr += received;
    }

    return (len - left);
}

DWORD WINAPI RecvFromClient(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[3];
    

    // 클라이언트 정보 얻기
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    while (true) {
        retval = recv(client_sock, (char*)&key, sizeof(Key), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            return 0;
        }
        else if (retval == 0)
        {
            return 0;
        }

        //std::cout << key.ckey << std::endl;
        movePlayer(key.ckey);

        //std::cout << p1_info.PosX << "," << p1_info.PosY << std::endl;

        retval = send(client_sock, (char*)&p1_info, sizeof(PlayerInfo), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            return 0;
        }
        else if (retval == 0)
        {
            return 0;
        }
    }

;

    // closesocket()
    closesocket(client_sock);


    return 0;
}


