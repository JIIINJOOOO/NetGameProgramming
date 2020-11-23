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
	// �� �Ʒ��� ��ġ�� �ҷ�����
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
	// Ÿ�� �������� �׷��� ������ִ� �Լ�
	void ReadyAdjacency();



private:
	//vector<INFO*>	m_vecTile;
	vector<CObj*>	m_vecWall;
	vector<CObj*>	m_vecDoor;
	vector<CObj*>	m_vecDeco;
	vector<CObj*>	m_vecWeapon;

	// �� Ÿ���� ���� ������ ������ �׷���.
	vector<list<INFO*>> m_vecAdjacency;
};

