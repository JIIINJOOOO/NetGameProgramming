#pragma once

class CObj;
class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)

private:
	CObjMgr();
	~CObjMgr();

public:
	CObj* GetTerrain() { return m_ObjLst[OBJ_TERRAIN].front(); }
	CObj* GetMouse() { return m_ObjLst[OBJ_MOUSE].front(); }
	OBJLIST* GetPlayerBulletLst() { return &m_ObjLst[OBJ_PLAYERBULLET]; }

	// 원본 GetPlayer()
	CObj* GetPlayer() { return m_ObjLst[OBJ_PLAYER].front(); }
	// 201129 player 1개 더 추가하면서 만든 GetPlayerLst
	OBJLIST* GetPlayerLst() { return &m_ObjLst[OBJ_PLAYER]; }
	// 201201 장애물 리스트 가져오기
	OBJLIST* GetObstacleLst() { return &m_ObjLst[OBJ_OBSTACLE]; }
	// 201201 무기 리스트 가져오기
	OBJLIST* GetWeaponLst() { return &m_ObjLst[OBJ_WEAPON]; }

public:
	void AddObject(CObj* pObject, OBJID eID);
	void Update();
	void LateUpdate();
	void Render();
	void Release();

private:
	OBJLIST	m_ObjLst[OBJ_END];
};

