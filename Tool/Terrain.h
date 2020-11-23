#pragma once

class CToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	vector<INFO*>& GetVecTile() { return m_vecTile; }
	vector<INFO*>& GetVecWall() { return m_vecWall; }
	vector<INFO*>& GetVecDoor() { return m_vecDoor; }
	vector<INFO*>& GetVecDeco() { return m_vecDeco; }
	vector<INFO*>& GetVecWeapon() { return m_vecWeapon; }
	vector<UNIT_DATA*>& GetVecEnemy() { return m_vecEnemy; }


public:
	void SetMainView(CToolView* pView) { m_pMainView = pView; }

public:
	void Initialize();
	void Update();
	void Render();
	void MiniRender();
	void Release();
public:
	void ReleaseTile();
	void ReleaseWall();
	void ReleaseDoor();
	void ReleaseDeco();
	void ReleaseWeapon();


public:
	void TileChange(const D3DXVECTOR3& vPos, const int& iDrawID, int iOption);
public: // 추가~
	void AddWall(D3DXVECTOR3& vPos, const int& iDrawID,
		const float& fScaleX, const float& fScaleY, int iOption /*추가*/); // -> 상하좌우 반전해서 타일 4방향 끝에 맞추자
	void AddDoor(D3DXVECTOR3& vPos, const int& iDrawID,
		const float& fScaleX, const float& fScaleY, int iOption /*추가*/);  // -> 상하좌우 반전해서 타일 4방향 끝에 맞추자
	void AddDeco(const D3DXVECTOR3& vPos, const int& iDrawID,
		const float& fScaleX, const float& fScaleY, int iOption /*추가*/); // -> 하나의 스프라이트로 반전해서 배치할 수 있게
	void AddWeapon(const D3DXVECTOR3& vPos, const int& iDrawID,
		const float& fScaleX, const float& fScaleY, int iOption /*추가*/); // -> 나중에 회전값 넣어보자
	void AddEnemy(const D3DXVECTOR3& vPos, const UNIT_DATA& tData, const WEAPONID& eID); // 추가
public:
	int GetTileIndex(const D3DXVECTOR3& vPos);
	const D3DXVECTOR3& GetTilePos(int iIndex);
	bool Picking(const D3DXVECTOR3& vPos, const int& iIndex);

private:
	vector<INFO*>	m_vecTile;
	//추가~
	vector<INFO*>	m_vecWall;
	vector<INFO*>	m_vecDoor;
	vector<INFO*>	m_vecDeco;
	vector<INFO*>	m_vecWeapon;
	vector<UNIT_DATA*>	m_vecEnemy; // 추가
	//~추가
	CToolView*		m_pMainView;
};

