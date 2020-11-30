#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#define PIE ((FLOAT) 3.141592654f)


#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>



using namespace std;
#define SERVERPORT 9000
#define BUFSIZE    5

CRITICAL_SECTION cs;


static DWORD threadId[2];
int acceptCount = 0;
struct Key
{
    bool key_W_Press;
    bool key_A_Press;
    bool key_S_Press;
    bool key_D_Press;
    bool key_R_Press;
    bool key_E_Press;
    bool Mouse_R_Press;
    bool Mouse_L_Press;
    int playerID;
    float mouseX;
    float mouseY;
};
Key key;

typedef struct PlayerInfo
{
    int PosX;
    int PosY;
    int playerID;
    int HP;
    int money;
    float angle;
};
PlayerInfo p1_info;
PlayerInfo p2_info;

struct myVector2D
{
    float x;
    float y;
};

myVector2D vDir = { 1.0f, 0.0f };
myVector2D vLook = { 1.0f, 0.0f };
HANDLE waitPlayerEnterEvent;

typedef struct PlayerNumCheck //로그인 인원수 체크
{
    int enterPlayerNum;
    int playerID;
};

PlayerNumCheck playernum;

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


void initPlayerInfo(PlayerInfo* pInfo, int playerNum)
{
    if (playerNum == 1)
    {
        pInfo->playerID = 1;
        pInfo->PosX = 200;
        pInfo->PosY = 350;
    }
    else 
    {
        pInfo->playerID = 2;
        pInfo->PosX = 850;
        pInfo->PosY = 350;
    }

    pInfo->HP = 100;
    pInfo->money = 1000;
    pInfo->angle = 0.0f;
}

void RotatePlayer(float mouseX, float mouseY, int playerID) 
{
    if (playerID == 1) 
    {
        p1_info.angle = -180.f * atan2(mouseY - vDir.y - p1_info.PosY, mouseX - vDir.x - p1_info.PosX) / PIE;
    }
    else if (playerID == 2) 
    {
        p2_info.angle = -180.f * atan2(mouseY - vDir.y - p2_info.PosY, mouseX - vDir.x - p2_info.PosX) / PIE;
    }

}


void MovePlayer(Key keycode) 
{
    
    if (keycode.key_W_Press)
    {
        if (keycode.playerID == 1)
        {
            p1_info.PosY -= 1;
        }
        else if (keycode.playerID == 2)
        {
            p2_info.PosY -= 1;
        }
       
    }
        
    if (keycode.key_A_Press)
    {
        if (keycode.playerID == 1) 
        {
            p1_info.PosX -= 1;
        }
        else if (keycode.playerID == 2) 
        {
            p2_info.PosX -= 1;
        }
    }

    if (keycode.key_S_Press) 
    {
        if (keycode.playerID == 1)
            p1_info.PosY += 1;
        else if (keycode.playerID == 2)
            p2_info.PosY += 1;
    }

       
    if (keycode.key_D_Press) {

        if (keycode.playerID == 1)
        {
            p1_info.PosX += 1;
         
        }

        else if (keycode.playerID == 2)
        {
            p2_info.PosX += 1;
        }


    }


    
}

int recvn(SOCKET s, char* buf, int len, int flags);


// 사용자 정의 데이터 수신 함수

int main(int argc, char* argv[])
{

    InitializeCriticalSection(&cs); // 임계영역 초기화

    int retval;
    playernum.enterPlayerNum = 0;//플레이어 인원수 초기화
    waitPlayerEnterEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (waitPlayerEnterEvent == NULL) return 1;
   /* waitSendPlayerEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (waitSendPlayerEvent == NULL) return 1;*/
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

    initPlayerInfo(&p1_info,1);
    initPlayerInfo(&p2_info,2);

    printf("서버 열림\n");
    while (1) {
        // accept()

        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }


        recvThread = CreateThread(NULL, 0, RecvFromClient, (LPVOID)client_sock, 0, &threadId[acceptCount]);
        acceptCount = (acceptCount + 1) % 2;

        if (recvThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(recvThread); }


     
    }


    CloseHandle(waitPlayerEnterEvent);
   // CloseHandle(waitSendPlayerEvent);

    DeleteCriticalSection(&cs);

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
    DWORD currentThreadId = GetCurrentThreadId();
    // 클라이언트 정보 얻기
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
    
    playernum.enterPlayerNum++;
    if (currentThreadId == threadId[0]) {
        //std::cout << playernum.enterPlayerNum << std::endl;
        retval = send(client_sock, (char*)&playernum, sizeof(PlayerNumCheck), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            return 0;
        }
        else if (retval == 0)
        {
            return 0;
        }

        WaitForSingleObject(waitPlayerEnterEvent, INFINITE);

        playernum.playerID = 1;
        //std::cout << playernum.enterPlayerNum << std::endl;
        retval = send(client_sock, (char*)&playernum, sizeof(PlayerNumCheck), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            return 0;
        }
        else if (retval == 0)
        {
            return 0;
        }
    }

	if (currentThreadId == threadId[1]) {
        playernum.playerID = 2;
		//std::cout << playernum.enterPlayerNum << std::endl;
		retval = send(client_sock, (char*)&playernum, sizeof(PlayerNumCheck), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			return 0;
		}
		else if (retval == 0)
		{
			return 0;
		}

        SetEvent(waitPlayerEnterEvent);
	}

    

	while (true) {

        if (playernum.enterPlayerNum > 1) {
            
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
            EnterCriticalSection(&cs);
            MovePlayer(key);
            RotatePlayer(key.mouseX, key.mouseY, key.playerID);
            LeaveCriticalSection(&cs);
          
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
            retval = send(client_sock, (char*)&p2_info, sizeof(PlayerInfo), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                return 0;
            }
            else if (retval == 0)
            {
                return 0;
            }

           

        }

	}
    


    // closesocket()
    closesocket(client_sock);


    return 0;
}


