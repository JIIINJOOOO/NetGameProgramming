#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#define PIE ((FLOAT) 3.141592654f)
#define PLAYERRECTWIDTH 13
#define	PLAYERRECTHEIGHT 24
#define SERVERPORT 9000
#define BUFSIZE    5
#define OBJ_NUM 1113
#define WEAPON_NUM 3
#define RIFLE_PRICE 1000
#define SMG_PRICE 500
#define SHOTGUN_PRICE 1500


#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>



using namespace std;


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
	int weaponID;
    int HP;
    int money;
    float angle;
}PINFO;
PlayerInfo p_Info[2];



typedef struct CollisionObj
{
	int objID;
	long left;
	long top;
	long right;
	long bottom;
	float PosX;
	float PosY;
}COLOBJ;
COLOBJ objarr[OBJ_NUM];
COLOBJ p_cols[2];
COLOBJ weaponarr[WEAPON_NUM];

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

void UpdatePlayerRect(COLOBJ& rect, PlayerInfo p_info)
{
	rect.PosX = p_info.PosX;
	rect.PosY = p_info.PosY;
	rect.left = LONG(p_info.PosX - PLAYERRECTWIDTH * 0.5f);
	rect.top = LONG(p_info.PosY - PLAYERRECTHEIGHT * 0.5f);
	rect.right = LONG(p_info.PosX + PLAYERRECTWIDTH * 0.5f);
	rect.bottom = LONG(p_info.PosY + PLAYERRECTHEIGHT * 0.5f);
}

// 서버 내 충돌 처리 코드
//void CollisionRect(COLOBJ objArr[]/*장애물 배열*/, COLOBJ bulArr[]/*총알구조체 배열*/)
//{
//	for (int i = 0; i < OBJ_NUM; ++i)
//	{
//		for (COLOBJ* pSrc : srcLst)
//		{
//			RECT rc = {};
//
//			const RECT dstRect = pDst->GetCollRect();
//			const RECT srcRect = pSrc->GetCollRect();
//
//			if (IntersectRect(&rc, &dstRect, &srcRect))
//			{
//				pDst->IsDead();
//				pSrc->IsDead();
//			}
//		}
//	}
//}

bool CheckRect(COLOBJ pDst, COLOBJ pSrc, float * pMoveX, float * pMoveY)
{
	// 두 사각형의 가로, 세로 축 반지름의 합을 구한다.
	float fSumRadX = ((pDst.right - pDst.left) + (pSrc.right - pSrc.left)) * 0.5f;
	float fSumRadY = ((pDst.bottom - pDst.top) + (pSrc.bottom - pSrc.top)) * 0.5f;

	// 두 사각형의 가로, 세로 중점의 거리를 구한다.
	// fabs(X): X의 절대 값을 구하는 함수. <cmath>에서 제공. 
	float fDistX = fabs(pDst.PosX - pSrc.PosX);
	float fDistY = fabs(pDst.PosY - pSrc.PosY);
	//const float fDist = D3DXVec3Length(&(pDst->GetInfo().vPos - pSrc->GetInfo().vPos));


	// 가로 축 반지름 합이 가로 축 거리보다 클 때와
	// 세로 축 반지름 합이 세로 축 거리보다 클 때
	if (fSumRadX > fDistX && fSumRadY > fDistY)
	{
		// 두 사각형이 겹쳤을 때 파고든 길이도 구한다.
		*pMoveX = fSumRadX - fDistX;
		*pMoveY = fSumRadY - fDistY;

		return true;
	}

	return false;
}

// 서버 내 벽 충돌 알고리즘
void CollisionRectEx(COLOBJ objArr[]/*장애물 배열*/, COLOBJ playerArr[]/*플레이어 위치 배열*/)
{
	// 밀려날 거리
	float fMoveX = 0.f, fMoveY = 0.f;

	for (int i = 0; i < OBJ_NUM; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			if (CheckRect(objArr[i], playerArr[j], &fMoveX, &fMoveY))
			{
				// 파고든 깊이가 짧은 쪽으로 pSrc를 밀어내자!
				if (fMoveX > fMoveY) // y축으로 밀어냄
				{
					float fX = playerArr[j].PosX;
					float fY = playerArr[j].PosY;

					if (objArr[i].PosY > fY)
						fMoveY *= -1.f;

					p_Info[j].PosX = fX;
					p_Info[j].PosY = fY + fMoveY;
				}
				else // x축으로 밀어냄
				{
					float fX = playerArr[j].PosX;
					float fY = playerArr[j].PosY;

					if (objArr[i].PosX > fX)
						fMoveX *= -1.f;

					p_Info[j].PosX = fX + fMoveX;
					p_Info[j].PosY = fY;
				}
			}
		}
	}
}

// 무기- 플레이어 충돌처리
void CollisionRectWeapon(COLOBJ weaponArr[]/*weapon 배열*/, COLOBJ playerArr[]/*플레이어 배열*/,bool bIsEpressed)
{
	for (int i = 0; i < WEAPON_NUM; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			RECT rc = {};

			const RECT weaponRect = { weaponArr[i].left,weaponArr[i].top,weaponArr[i].right,weaponArr[i].bottom };
			const RECT playerRect = { playerArr[j].left,playerArr[i].top,playerArr[i].right,playerArr[i].bottom };

			if (IntersectRect(&rc, &weaponRect, &playerRect))
			{
				//pSrc->SetWeaponID(pDst->GetWeaponID());
				// 플레이어가 무기와 충돌중이다
				//pSrc->SetIsOverlap(true);
				// 201201 여기서 스레드로 플레이어 아이디 1,2 e눌렸는지 얻어와야 할듯?
				// 그리고 총의 money(가격)도 받아와야함... 구조체 어케 할지?
				int iPrice = 0;
				int iWeaponType = 0;
				if (weaponArr[i].objID == 1)
				{
					iWeaponType = 1;
					iPrice = RIFLE_PRICE;
				}
				else if (weaponArr[i].objID == 2)
				{
					iWeaponType = 2;
					iPrice = SMG_PRICE;
				}
				else if (weaponArr[i].objID == 3)
				{
					iWeaponType = 3;
					iPrice = SHOTGUN_PRICE;
				}
				if (bIsEpressed && (p_Info[j].money >= iPrice))
				{
					//pSrc->SetWeaponID(pDst->GetWeaponID());
					p_Info[j].weaponID = iWeaponType;
					/*pSrc->SetWeaponMaxBul(pDst->GetWeaponMaxBul());
					pSrc->SetWeaponCurBul(pDst->GetWeaponMaxBul());*/
					//pSrc->SetMoney(pSrc->GetMoney() - pDst->GetMoney());
					p_Info[j].money -= iPrice;
				}
				// 201123 최대 총알 set 함수 추가
				//pDst->IsDead();
			}
		}
	}
}




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
        p_Info[0].angle = -180.f * atan2(mouseY - vDir.y - p_Info[0].PosY, mouseX - vDir.x - p_Info[0].PosX) / PIE;
    }
    else if (playerID == 2) 
    {
        p_Info[1].angle = -180.f * atan2(mouseY - vDir.y - p_Info[1].PosY, mouseX - vDir.x - p_Info[1].PosX) / PIE;
    }

}


void MovePlayer(Key keycode) 
{
    
    if (keycode.key_W_Press)
    {
        if (keycode.playerID == 1)
        {
            p_Info[0].PosY -= 1;
        }
        else if (keycode.playerID == 2)
        {
            p_Info[1].PosY -= 1;
        }
       
    }
        
    if (keycode.key_A_Press)
    {
        if (keycode.playerID == 1) 
        {
            p_Info[0].PosX -= 1;
        }
        else if (keycode.playerID == 2) 
        {
            p_Info[1].PosX -= 1;
        }
    }

    if (keycode.key_S_Press) 
    {
        if (keycode.playerID == 1)
            p_Info[0].PosY += 1;
        else if (keycode.playerID == 2)
            p_Info[1].PosY += 1;
    }

       
    if (keycode.key_D_Press) {

        if (keycode.playerID == 1)
        {
            p_Info[0].PosX += 1;
         
        }

        else if (keycode.playerID == 2)
        {
            p_Info[1].PosX += 1;
        }


    }


    
}

int recvn(SOCKET s, char* buf, int len, int flags);


// 사용자 정의 데이터 수신 함수

int main(int argc, char* argv[])
{
	ZeroMemory(&objarr, sizeof(objarr));
	ZeroMemory(&weaponarr, sizeof(weaponarr));

    InitializeCriticalSection(&cs); // 임계영역 초기화

	// objarr 받아옴
	FILE* f;
	f = fopen("objdata.txt", "r");
	for (int i = 0; i < 1113; ++i)
	{
		fscanf(f, "%d %ld %ld %ld %ld %f %f", &objarr[i].objID, &objarr[i].bottom, &objarr[i].left, &objarr[i].top, &objarr[i].right, &objarr[i].PosX, &objarr[i].PosY);
	}
	fclose(f);

	/*for (int i = 0; i < 1113; ++i)
	{
		printf("%d %ld %ld %ld %ld %f %f\n", objarr[i].objID, objarr[i].bottom, objarr[i].left, objarr[i].top, objarr[i].right, objarr[i].PosX, objarr[i].PosY);
	}*/

	FILE* fp;
	fp = fopen("weapondata.txt", "r");
	for (int i = 0; i < WEAPON_NUM; ++i)
	{
		fscanf(fp, "%d %ld %ld %ld %ld %f %f", &weaponarr[i].objID, &weaponarr[i].bottom, &weaponarr[i].left, &weaponarr[i].top, &weaponarr[i].right,
			&weaponarr[i].PosX, &weaponarr[i].PosY);
	}
	fclose(fp);
	/*for (int i = 0; i < WEAPON_NUM; ++i)
	{
		if(weaponarr[i].objID == 1)
			printf("rifle : %d %ld %ld %ld %ld %f %f\n", weaponarr[i].objID, weaponarr[i].bottom, weaponarr[i].left, weaponarr[i].top, 
				weaponarr[i].right, weaponarr[i].PosX, weaponarr[i].PosY);
	}*/

    int retval;
    playernum.enterPlayerNum = 0;//플레이어 인원수 초기화
	ZeroMemory(&p_cols,sizeof(p_cols));


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

    initPlayerInfo(&p_Info[0],1);
    initPlayerInfo(&p_Info[1],2);

    printf("서버 열림\n");
    while (1) {
        // accept()

        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

		printf("플레이어 %d 입장\n", acceptCount);
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
			if (currentThreadId == threadId[0]) 
			{
				UpdatePlayerRect(p_cols[0], p_Info[0]);
			}
			else if (currentThreadId == threadId[1])
			{
				UpdatePlayerRect(p_cols[1], p_Info[1]);
			}
          
			CollisionRectEx(objarr, p_cols);
			CollisionRectWeapon(weaponarr, p_cols, key.key_E_Press);
			LeaveCriticalSection(&cs);

            //std::cout << p_Info[0].PosX << "," << p_Info[0].PosY << std::endl;
           
            retval = send(client_sock, (char*)&p_Info[0], sizeof(PlayerInfo), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                return 0;
            }
            else if (retval == 0)
            {
                return 0;
            }
            retval = send(client_sock, (char*)&p_Info[1], sizeof(PlayerInfo), 0);
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


