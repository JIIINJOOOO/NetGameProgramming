#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#define PIE ((FLOAT) 3.141592654f)
#define PLAYER_RECT_WIDTH 13
#define	PLAYER_RECT_HEIGHT 24
#define SHOTGUN_RECT_HEIGHT 2
#define SHOTGUN_RECT_WIDTH 4
#define RIFLE_RECT_HEIGHT 1
#define RIFLE_RECT_WIDTH 4
#define SMG_RECT_HEIGHT 1
#define SMG_RECT_WIDTH 2
#define SERVERPORT 9000
#define BUFSIZE    5
#define OBJ_NUM 1138
#define WEAPON_NUM 6
#define RIFLE_PRICE 1000
#define SMG_PRICE 500
#define SHOTGUN_PRICE 1500
#define RIFLE_BULLET 5
#define SMG_BULLET 10
#define SHOTGUN_BULLET 2
#define FPS 30
#define MAX_BULLETS 10

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>


bool p1_isBuy = false;
bool p2_isBuy = false;
float fTimer = 0.f;

float timer = 0;//
using namespace std;
int nowTime;
CRITICAL_SECTION cs;


static DWORD threadId[2];
int acceptCount = 0;
#pragma pack(push,1)
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
#pragma pack(pop)

Key key;
Key tempKey;
#pragma pack(push,1)
typedef struct BulletInfo
{
	bool IsDead;
	float StartPosX;
	float StartPosY;
	float PosX;
	float PosY;
	float DirX;
	float DirY;
	float speed;
	float range;
	int damage;
};
#pragma pack(pop)

#pragma pack(push,1)
typedef struct PlayerInfo
{
	bool IsDead;
	float PosX;
	float PosY;
	float DirX;
	float DirY;
    int playerID;
	int weaponID;
	int CurBulletNum;
	int MaxBulletNum;
    int HP;
    int money;
    float angle;
	BulletInfo bullets[MAX_BULLETS];

}PINFO;
#pragma pack(pop)

PlayerInfo p_Info[2];


#pragma pack(push,1)
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
#pragma pack(pop)

COLOBJ objarr[OBJ_NUM];
COLOBJ p_cols[2];
COLOBJ weaponarr[WEAPON_NUM];
COLOBJ bulletarr_1[MAX_BULLETS];
COLOBJ bulletarr_2[MAX_BULLETS];



#pragma pack(push,1)
typedef struct PlayerNumCheck //�α��� �ο��� üũ
{
    int enterPlayerNum;
    int playerID;
};
#pragma pack(pop)
PlayerNumCheck playernum;

struct myVector2D
{
	float x;
	float y;
};

myVector2D vDir[2] = { {1.0f, 0.0f},{1.0f, 0.0f} };
//myVector2D vLook = { 1.0f, 0.0f };
HANDLE waitPlayerEnterEvent;


void CalcDirVec(int playerID, float p_posX, float p_posY, float m_posX, float m_posY) 
{
	double dirX = m_posX - p_posX;
	double dirY = m_posY - p_posY;

	double dist = sqrt( pow(dirX, 2.0) + pow(dirY, 2.0));

	p_Info[playerID].DirX = dirX / dist;
	p_Info[playerID].DirY = dirY / dist;


}


void UpdatePlayerRect(COLOBJ& rect, PlayerInfo p_info)
{
	rect.PosX = p_info.PosX;
	rect.PosY = p_info.PosY;
	rect.left = LONG(p_info.PosX - PLAYER_RECT_WIDTH * 0.5f);
	rect.top = LONG(p_info.PosY - PLAYER_RECT_HEIGHT * 0.5f);
	rect.right = LONG(p_info.PosX + PLAYER_RECT_WIDTH * 0.5f);
	rect.bottom = LONG(p_info.PosY + PLAYER_RECT_HEIGHT * 0.5f);
}

void UpdateBulletRect(COLOBJ bulletrects[], BulletInfo bullets[])
{
	for (int i = 0; i < MAX_BULLETS; ++i)
	{
		bulletrects[i].PosX = bullets[i].PosX;
		bulletrects[i].PosY = bullets[i].PosY;
		bulletrects[i].left = LONG(bullets[i].PosX - PLAYER_RECT_WIDTH * 0.5f);
		bulletrects[i].top = LONG(bullets[i].PosY - PLAYER_RECT_HEIGHT * 0.5f);
		bulletrects[i].right = LONG(bullets[i].PosX + PLAYER_RECT_WIDTH * 0.5f);
		bulletrects[i].bottom = LONG(bullets[i].PosY + PLAYER_RECT_HEIGHT * 0.5f);
	}
}



// ���� �� �浹 ó�� �ڵ�
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
// ���� �� �Ѿ� �浹 ó�� �ڵ�
void CollisionRect(COLOBJ objArr[]/*��ֹ� �迭*/, COLOBJ bulArr[]/*�Ѿ˱���ü �迭*/, BulletInfo bullets[])
{
	float fMoveX = 0.f, fMoveY = 0.f;
	for (int i = 0; i < OBJ_NUM; ++i)
	{
		for (int j = 0; j < MAX_BULLETS; ++j)
		{
			/*RECT rc = {};

			const RECT dstRect = pDst->GetCollRect();
			const RECT srcRect = pSrc->GetCollRect();*/
			//if (IntersectRect(&rc, &dstRect, &srcRect))
			if (!bullets[j].IsDead)
			{
				if (CheckRect(objArr[i], bulArr[j], &fMoveX, &fMoveY))
				{
					/*pDst->IsDead();
					pSrc->IsDead();*/
					bullets[j].IsDead = true;
				}
			}
		}
	}
}

// ���� �� �÷��̾�-�Ѿ� �浹
void CollisionRectPlayerBul(COLOBJ playerArr/*�÷��̾�*/, COLOBJ bulArr[]/*�Ѿ˱���ü �迭*/, BulletInfo bullets[], PlayerInfo* p_pInfo)
{
	float fMoveX = 0.f, fMoveY = 0.f;
	for (int i = 0; i < OBJ_NUM; ++i)
	{
		for (int j = 0; j < MAX_BULLETS; ++j)
		{
			/*RECT rc = {};

			const RECT dstRect = pDst->GetCollRect();
			const RECT srcRect = pSrc->GetCollRect();*/
			//if (IntersectRect(&rc, &dstRect, &srcRect))
			if (!bullets[j].IsDead)
			{
				if (CheckRect(playerArr, bulArr[j], &fMoveX, &fMoveY))
				{
					/*pDst->IsDead();
					pSrc->IsDead();*/
					if (p_pInfo->HP > 0)
					{
						p_pInfo->HP -= bullets[j].damage;
						
					}
					else
					{
						p_pInfo->IsDead = true;
					}
					bullets[j].IsDead = true;
				}
			}
		}
	}
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

    float fMoveX = 0.f, fMoveY = 0.f;

	for (int i = 0; i < WEAPON_NUM; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			//RECT rc = {};

			/*const RECT weaponRect = { weaponArr[i].left,weaponArr[i].top,weaponArr[i].right,weaponArr[i].bottom };
			const RECT playerRect = { playerArr[j].left,playerArr[i].top,playerArr[i].right,playerArr[i].bottom };*/

			//if (IntersectRect(&rc, &weaponRect, &playerRect))
            if(CheckRect(weaponArr[i],playerArr[j],&fMoveX,&fMoveY))
			{
               
				//pSrc->SetWeaponID(pDst->GetWeaponID());
				// �÷��̾ ����� �浹���̴�
				//pSrc->SetIsOverlap(true);
				// 201201 ���⼭ ������� �÷��̾� ���̵� 1,2 e���ȴ��� ���;� �ҵ�?
				// �׸��� ���� money(����)�� �޾ƿ;���... ����ü ���� ����?
				int iPrice = 0;
				int iWeaponType = 0;
				int iMaxBulletNum = 0;
				if (weaponArr[i].objID == 1)
				{
					iWeaponType = 1;
					iPrice = RIFLE_PRICE;
					iMaxBulletNum = RIFLE_BULLET;
				}
				else if (weaponArr[i].objID == 2)
				{
					iWeaponType = 2;
					iPrice = SMG_PRICE;
					iMaxBulletNum = SMG_BULLET;
				}
				else if (weaponArr[i].objID == 3)
				{
					iWeaponType = 3;
					iPrice = SHOTGUN_PRICE;
					iMaxBulletNum = SHOTGUN_BULLET;
				}
				if (bIsEpressed && (p_Info[j].money >= iPrice)&&!p1_isBuy&&j==0)
				{
                    //cout << j << "buy Weapon" <<i << endl;
					//pSrc->SetWeaponID(pDst->GetWeaponID());
					p_Info[j].weaponID = iWeaponType;
					/*pSrc->SetWeaponMaxBul(pDst->GetWeaponMaxBul());
					pSrc->SetWeaponCurBul(pDst->GetWeaponMaxBul());*/
					p_Info[j].MaxBulletNum = iMaxBulletNum;
					p_Info[j].CurBulletNum = iMaxBulletNum;
					p_Info[j].money -= iPrice;
					p1_isBuy = true;
				}
				if (bIsEpressed && (p_Info[j].money >= iPrice)&&!p2_isBuy && j == 1)
				{
					//cout << j << "buy Weapon" <<i << endl;
					//pSrc->SetWeaponID(pDst->GetWeaponID());
					p_Info[j].weaponID = iWeaponType;
					/*pSrc->SetWeaponMaxBul(pDst->GetWeaponMaxBul());
					pSrc->SetWeaponCurBul(pDst->GetWeaponMaxBul());*/
					p_Info[j].MaxBulletNum = iMaxBulletNum;
					p_Info[j].CurBulletNum = iMaxBulletNum;
					p_Info[j].money -= iPrice;
					p2_isBuy = true;

				}
				// 201123 �ִ� �Ѿ� set �Լ� �߰�
				//pDst->IsDead();
			}
		}
	}
}

void UpdateBullet(BulletInfo bullets[])
{
	for (int i = 0; i < MAX_BULLETS; ++i)
	{
		if (!bullets[i].IsDead)
		{
			bullets[i].PosX += bullets[i].DirX*bullets[i].speed;
			bullets[i].PosY += bullets[i].DirY*bullets[i].speed;
		}
	
		float fX = fabs(bullets[i].StartPosX - bullets[i].PosX);
		float fY = fabs(bullets[i].StartPosY - bullets[i].PosY);

		double dist = sqrt((fX * fX) + (fY * fY));

		if (dist >= bullets[i].range) 
		{
			bullets[i].IsDead = true;
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
    pInfo->money = 3000;
    pInfo->angle = 0.0f;
	pInfo->CurBulletNum = 0;
	pInfo->MaxBulletNum = 0;
	for (int i = 0; i < MAX_BULLETS; ++i)
	{
		pInfo->bullets[i].IsDead = true;
	}
}

void RotatePlayer(float mouseX, float mouseY, int playerID) 
{
    if (playerID == 1) 
    {
        p_Info[0].angle = -180.f * atan2(mouseY - vDir[0].y - p_Info[0].PosY, mouseX - vDir[0].x - p_Info[0].PosX) / PIE;
    }
    else if (playerID == 2) 
    {
        p_Info[1].angle = -180.f * atan2(mouseY - vDir[1].y - p_Info[1].PosY, mouseX - vDir[1].x - p_Info[1].PosX) / PIE;
    }

}


void MovePlayer(Key keycode) 
{
	float fSpeed = 2.0f;
    if (keycode.key_W_Press)
    {
        if (keycode.playerID == 1)
        {
            p_Info[0].PosY -= fSpeed ;
        }
        else if (keycode.playerID == 2)
        {
            p_Info[1].PosY -= fSpeed ;
        }
       
    }
        
    if (keycode.key_A_Press)
    {
        if (keycode.playerID == 1) 
        {
            p_Info[0].PosX -= fSpeed ;
        }
        else if (keycode.playerID == 2) 
        {
            p_Info[1].PosX -= fSpeed;
        }
    }

    if (keycode.key_S_Press) 
    {
        if (keycode.playerID == 1)
            p_Info[0].PosY += fSpeed;
        else if (keycode.playerID == 2)
            p_Info[1].PosY += fSpeed;
    }

       
    if (keycode.key_D_Press) {

        if (keycode.playerID == 1)
        {
            p_Info[0].PosX += fSpeed;
         
        }

        else if (keycode.playerID == 2)
        {
            p_Info[1].PosX += fSpeed;
        }
    }
   
}

void PlayerShoot(Key keycode)
{
	bool IsBulCreate = false;
	if (keycode.Mouse_L_Press)
	{
		if (keycode.playerID == 1)
		{
			if (p_Info[0].CurBulletNum > 0)
			{
				p_Info[0].CurBulletNum -= 1;

				for (int i = 0; i < MAX_BULLETS; ++i)
				{
					if (p_Info[0].bullets[i].IsDead)
					{
						p_Info[0].bullets[i].IsDead = false;
						p_Info[0].bullets[i].StartPosX = p_Info[0].PosX;
						p_Info[0].bullets[i].StartPosY = p_Info[0].PosY;
						p_Info[0].bullets[i].PosX = p_Info[0].PosX;
						p_Info[0].bullets[i].PosY = p_Info[0].PosY;
						p_Info[0].bullets[i].DirX = p_Info[0].DirX;
						p_Info[0].bullets[i].DirY = p_Info[0].DirY;

						if (p_Info[0].weaponID == 1) // RIFLE
						{
							p_Info[0].bullets[i].speed = 2.f;
							p_Info[0].bullets[i].range = 230.f;
							p_Info[0].bullets[i].damage = 10;
						}
						else if (p_Info[0].weaponID == 2) // SMG
						{
							p_Info[0].bullets[i].speed = 3.f;
							p_Info[0].bullets[i].range = 150.f;
							p_Info[0].bullets[i].damage = 3;
						}
						else if (p_Info[0].weaponID == 3) // SHOTGUN
						{
							p_Info[0].bullets[i].speed = 4.f;
							p_Info[0].bullets[i].range = 100.f;
							p_Info[0].bullets[i].damage = 15;
						}
						IsBulCreate = true;
					}
					if (IsBulCreate)
						break;
					//��ġ�� ������ range�� ���
				}
			}
	
		}
		else if (keycode.playerID == 2)
		{
			if (p_Info[1].CurBulletNum > 0)
			{
				p_Info[1].CurBulletNum -= 1;

				for (int i = 0; i < MAX_BULLETS; ++i)
				{
					if (p_Info[1].bullets[i].IsDead)
					{
						p_Info[1].bullets[i].IsDead = false;
						p_Info[1].bullets[i].StartPosX = p_Info[1].PosX;
						p_Info[1].bullets[i].StartPosY = p_Info[1].PosY;
						p_Info[1].bullets[i].PosX = p_Info[1].PosX;
						p_Info[1].bullets[i].PosY = p_Info[1].PosY;
						p_Info[1].bullets[i].DirX = p_Info[1].DirX;
						p_Info[1].bullets[i].DirY = p_Info[1].DirY;

						if (p_Info[1].weaponID == 1) // RIFLE
						{
							p_Info[1].bullets[i].speed = 2.f;
							p_Info[1].bullets[i].range = 230.f;
							p_Info[1].bullets[i].damage = 10;
						}
						else if (p_Info[1].weaponID == 2) // SMG
						{
							p_Info[1].bullets[i].speed = 3.f;
							p_Info[1].bullets[i].range = 150.f;
							p_Info[1].bullets[i].damage = 3;
						}
						else if (p_Info[1].weaponID == 3) // SHOTGUN
						{
							p_Info[1].bullets[i].speed = 4.f;
							p_Info[1].bullets[i].range = 100.f;
							p_Info[1].bullets[i].damage = 15;
						}
						IsBulCreate = true;
					}
					if (IsBulCreate)
						break;
					//��ġ�� ������ range�� ���
				}
			}
		}
	}

	if (keycode.key_R_Press)
	{
		if (keycode.playerID == 1)
		{
			p_Info[0].CurBulletNum = p_Info[0].MaxBulletNum;

		}

		else if (keycode.playerID == 2)
		{
			p_Info[1].CurBulletNum = p_Info[1].MaxBulletNum;
		}
	}
}

int recvn(SOCKET s, char* buf, int len, int flags);


// ����� ���� ������ ���� �Լ�

int main(int argc, char* argv[])
{

	
	ZeroMemory(&objarr, sizeof(objarr));
	ZeroMemory(&weaponarr, sizeof(weaponarr));
	ZeroMemory(&bulletarr_1, sizeof(bulletarr_1));
	ZeroMemory(&bulletarr_2, sizeof(bulletarr_2));


    InitializeCriticalSection(&cs); // �Ӱ迵�� �ʱ�ȭ
    
	// objarr �޾ƿ�
	FILE* f;
	f = fopen("objdata.txt", "r");
	for (int i = 0; i < OBJ_NUM; ++i)
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
	
   
	unsigned long startTime = GetTickCount64();

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

            EnterCriticalSection(&cs);
            tempKey = key;
            LeaveCriticalSection(&cs);

			
			




				//std::cout << key.ckey << std::endl;
				
				PlayerShoot(tempKey);//�Ҹ� �ʱ�ȭ�� ���⼭ ����
			if (GetTickCount64() >= startTime + 4)
			{
				MovePlayer(tempKey);
				RotatePlayer(tempKey.mouseX, tempKey.mouseY, tempKey.playerID);
				startTime = GetTickCount64();
				if (currentThreadId == threadId[0])
				{
					UpdateBullet(p_Info[0].bullets);
					UpdatePlayerRect(p_cols[0], p_Info[0]);
					UpdateBulletRect(bulletarr_1, p_Info[0].bullets);
					CalcDirVec(0, p_Info[0].PosX, p_Info[0].PosY, tempKey.mouseX, tempKey.mouseY);

				}
				else if (currentThreadId == threadId[1])
				{
					UpdateBullet(p_Info[1].bullets);
					UpdatePlayerRect(p_cols[1], p_Info[1]);
					UpdateBulletRect(bulletarr_2, p_Info[1].bullets);
					CalcDirVec(1, p_Info[1].PosX, p_Info[1].PosY, tempKey.mouseX, tempKey.mouseY);

				}
			}
				CollisionRectEx(objarr, p_cols);
				CollisionRectWeapon(weaponarr, p_cols, tempKey.key_E_Press);
				CollisionRect(objarr, bulletarr_1, p_Info[0].bullets);
				CollisionRect(objarr, bulletarr_2, p_Info[1].bullets);
				CollisionRectPlayerBul(p_cols[0], bulletarr_2, p_Info[1].bullets, &p_Info[0]);
				CollisionRectPlayerBul(p_cols[1], bulletarr_1, p_Info[0].bullets, &p_Info[1]);
			
			
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


