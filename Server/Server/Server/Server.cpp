#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
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

typedef struct PlayerNumCheck //�α��� �ο��� üũ
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

// ���� �� �浹 ó�� �ڵ�
//void CollisionRect(COLOBJ objArr[]/*��ֹ� �迭*/, COLOBJ bulArr[]/*�Ѿ˱���ü �迭*/)
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
	// �� �簢���� ����, ���� �� �������� ���� ���Ѵ�.
	float fSumRadX = ((pDst.right - pDst.left) + (pSrc.right - pSrc.left)) * 0.5f;
	float fSumRadY = ((pDst.bottom - pDst.top) + (pSrc.bottom - pSrc.top)) * 0.5f;

	// �� �簢���� ����, ���� ������ �Ÿ��� ���Ѵ�.
	// fabs(X): X�� ���� ���� ���ϴ� �Լ�. <cmath>���� ����. 
	float fDistX = fabs(pDst.PosX - pSrc.PosX);
	float fDistY = fabs(pDst.PosY - pSrc.PosY);
	//const float fDist = D3DXVec3Length(&(pDst->GetInfo().vPos - pSrc->GetInfo().vPos));


	// ���� �� ������ ���� ���� �� �Ÿ����� Ŭ ����
	// ���� �� ������ ���� ���� �� �Ÿ����� Ŭ ��
	if (fSumRadX > fDistX && fSumRadY > fDistY)
	{
		// �� �簢���� ������ �� �İ�� ���̵� ���Ѵ�.
		*pMoveX = fSumRadX - fDistX;
		*pMoveY = fSumRadY - fDistY;

		return true;
	}

	return false;
}

// ���� �� �� �浹 �˰���
void CollisionRectEx(COLOBJ objArr[]/*��ֹ� �迭*/, COLOBJ playerArr[]/*�÷��̾� ��ġ �迭*/)
{
	// �з��� �Ÿ�
	float fMoveX = 0.f, fMoveY = 0.f;

	for (int i = 0; i < OBJ_NUM; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			if (CheckRect(objArr[i], playerArr[j], &fMoveX, &fMoveY))
			{
				// �İ�� ���̰� ª�� ������ pSrc�� �о��!
				if (fMoveX > fMoveY) // y������ �о
				{
					float fX = playerArr[j].PosX;
					float fY = playerArr[j].PosY;

					if (objArr[i].PosY > fY)
						fMoveY *= -1.f;

					p_Info[j].PosX = fX;
					p_Info[j].PosY = fY + fMoveY;
				}
				else // x������ �о
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

// ����- �÷��̾� �浹ó��
void CollisionRectWeapon(COLOBJ weaponArr[]/*weapon �迭*/, COLOBJ playerArr[]/*�÷��̾� �迭*/,bool bIsEpressed)
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
				// �÷��̾ ����� �浹���̴�
				//pSrc->SetIsOverlap(true);
				// 201201 ���⼭ ������� �÷��̾� ���̵� 1,2 e���ȴ��� ���;� �ҵ�?
				// �׸��� ���� money(����)�� �޾ƿ;���... ����ü ���� ����?
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
				// 201123 �ִ� �Ѿ� set �Լ� �߰�
				//pDst->IsDead();
			}
		}
	}
}




// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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


// ����� ���� ������ ���� �Լ�

int main(int argc, char* argv[])
{
	ZeroMemory(&objarr, sizeof(objarr));
	ZeroMemory(&weaponarr, sizeof(weaponarr));

    InitializeCriticalSection(&cs); // �Ӱ迵�� �ʱ�ȭ

	// objarr �޾ƿ�
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
    playernum.enterPlayerNum = 0;//�÷��̾� �ο��� �ʱ�ȭ
	ZeroMemory(&p_cols,sizeof(p_cols));


    waitPlayerEnterEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (waitPlayerEnterEvent == NULL) return 1;
   /* waitSendPlayerEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (waitSendPlayerEvent == NULL) return 1;*/
    // ���� �ʱ�ȭ
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
    // ������ ��ſ� ����� ����
    SOCKET client_sock;

    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE];
    HANDLE recvThread;

    initPlayerInfo(&p_Info[0],1);
    initPlayerInfo(&p_Info[1],2);

    printf("���� ����\n");
    while (1) {
        // accept()

        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

		printf("�÷��̾� %d ����\n", acceptCount);
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



    // ���� ����
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
    // Ŭ���̾�Ʈ ���� ���
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


