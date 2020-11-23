#pragma once

class CObj;

class CMapMgr
{
	DECLARE_SINGLETON(CMapMgr)
private:
	CMapMgr();
	~CMapMgr();

public:
	//vector<INFO*>& GetVecTile() { return m_vecTile; }
	vector<list<INFO*>>& GetVecAdjacency() { return m_vecAdjacency; }

public:
	//HRESULT LoadTile(const TCHAR* szFilePath);
	// 이 아래는 위치만 불러오기
	HRESULT LoadWall(const TCHAR* szFilePath);
	HRESULT LoadDoor(const TCHAR* szFilePath);
	HRESULT LoadDeco(const TCHAR* szFilePath);
	HRESULT LoadWeapon(const TCHAR* szFilePath);

public:
	HRESULT Initialize();
	void Update();
	void LateUpdate();
	void Render();
	void Release();

private:
	// 타일 인접정보 그래프 만들어주는 함수
	void ReadyAdjacency();



private:
	//vector<INFO*>	m_vecTile;
	vector<CObj*>	m_vecWall;
	vector<CObj*>	m_vecDoor;
	vector<CObj*>	m_vecDeco;
	vector<CObj*>	m_vecWeapon;

	// 각 타일의 인접 정보를 보관할 그래프.
	vector<list<INFO*>> m_vecAdjacency;
};

