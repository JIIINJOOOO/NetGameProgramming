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

	CObj* GetPlayer() { return m_ObjLst[OBJ_PLAYER].front(); }


public:
	void AddObject(CObj* pObject, OBJID eID);
	void Update();
	void LateUpdate();
	void Render();
	void Release();

private:
	OBJLIST	m_ObjLst[OBJ_END];
};

