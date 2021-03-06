#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "CollisionMgr.h"

//네트워크통신을 위한 전역변수
extern int retval;
extern WSADATA wsa;
extern SOCKET sock;
extern SOCKADDR_IN serveraddr;
extern void err_display(char* msg);
extern void err_quit(char* msg);

extern PlayerNumCheck playercheck;


PlayerInfo p1;
PlayerInfo p2;

IMPLEMENT_SINGLETON(CObjMgr)

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::AddObject(CObj * pObject, OBJID eID)
{
	m_ObjLst[eID].push_back(pObject);
}

void CObjMgr::Update()
{
	if (playercheck.enterPlayerNum == 3) 
	{
		retval = recv(sock, (char*)&p1, sizeof(PlayerInfo), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
		}
		retval = recv(sock, (char*)&p2, sizeof(PlayerInfo), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
		}
	}

	for (OBJLIST& objLst : m_ObjLst)
	{
		OBJITER iter_begin = objLst.begin();
		OBJITER iter_end = objLst.end();

		for (; iter_begin != iter_end;)
		{
			
			if (objLst == m_ObjLst[OBJ_PLAYER]&& playercheck.enterPlayerNum == 3)
			{
				(*iter_begin)->SetPlayerInfo(p1);
				(*iter_begin)->SetPlayerInfo(p2);

			}

			int iEvent = (*iter_begin)->Update();

			if (objLst.empty())
				break;

			if (DEAD_OBJ == iEvent)
			{
				SafeDelete(*iter_begin);
				iter_begin = objLst.erase(iter_begin);
			}
			else
				++iter_begin;
		}
	}
}

void CObjMgr::LateUpdate()
{
	for (OBJLIST& objLst : m_ObjLst)
	{
		for (CObj*& pObj : objLst)
			pObj->LateUpdate();
	}

	// 프레임 저하 원인
	//CCollisionMgr::CollisionRectEx(m_ObjLst[OBJID::OBJ_OBSTACLE], m_ObjLst[OBJID::OBJ_PLAYER]);
	//CCollisionMgr::CollisionRectWeapon(m_ObjLst[OBJID::OBJ_WEAPON], m_ObjLst[OBJID::OBJ_PLAYER]);
	// 적이랑 플레이어 총알 충돌처리
	CCollisionMgr::CollisionRectPlayerBul(m_ObjLst[OBJID::OBJ_PLAYER], m_ObjLst[OBJID::OBJ_PLAYERBULLET]);
	// 장애물이랑 총알 충돌처리
	CCollisionMgr::CollisionRect(m_ObjLst[OBJID::OBJ_OBSTACLE], m_ObjLst[OBJID::OBJ_PLAYERBULLET]);
	// 적이랑 근접무기 들고있는 플레이어 충돌처리
	//CCollisionMgr::CollisionRect(m_ObjLst[OBJID::OBJ_ENEMY], m_ObjLst[OBJID::OBJ_PLAYER]);

	//CCollisionMgr::CollisionRectEx(m_ObjLst[OBJID::OBJ_ENEMY], m_ObjLst[OBJID::OBJ_PLAYER]);

}

void CObjMgr::Render()
{
	for (OBJLIST& objLst : m_ObjLst)
	{
		for (CObj*& pObj : objLst)
			pObj->Render();
	}
}

void CObjMgr::Release()
{
	for (OBJLIST& objLst : m_ObjLst)
	{
		for_each(objLst.begin(), objLst.end(), SafeDelete<CObj*>);
		objLst.clear();
	}
}
