#pragma once
class CObj
{
public:
	CObj();
	CObj(const INFO & tInfo);
	virtual ~CObj();

public:
 	const INFO& GetInfo() const { return m_tInfo; }
	const RECT& GetCollRect() const { return m_tCollRect; } // 추가
	const wstring& GetObjKey() { return m_wstrObjKey; }
	const WEAPONID& GetWeaponID() { return m_eWeapon; } // 추가
	//const vector<TEXINFO*>& GetVecTexture() const { return m_vecTexture; } // 추가 -> 충돌처리에 써보자
	// 201123 최대/현재 총알 get
	const int& GetWeaponMaxBul() { return m_iMaxBulletNum; } // 추가
	const int& GetWeaponCurBul() { return m_iCurBulletNum; } // 추가

public:
	void SetPos(D3DXVECTOR3 vPos) { m_tInfo.vPos = vPos; } // 추가
	void SetDir(D3DXVECTOR3 vDir) { m_tInfo.vDir = vDir; } // 추가
	void SetWeaponID(WEAPONID eWeapon) { m_eWeapon = eWeapon; } // 추가
	// 201123 최대/현재 총알 set -> 콜리젼매니저에서 호출
	void SetWeaponMaxBul(int iMaxBulNum) { m_iMaxBulletNum = iMaxBulNum; }
	void SetWeaponCurBul(int iMaxBulNum) { m_iCurBulletNum = iMaxBulNum; }
public:
	virtual HRESULT Initialize() PURE;
	virtual void LateInit();
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;
public:
	void IsDead() { m_bIsDead = true; }

protected:
	// 이미지 크기에 맞게 충돌박스 생성 함수
	void UpdateCollRect(const TEXINFO* pTexInfo); // 추가
	void MoveFrame();

	
protected:
	INFO	m_tInfo;
	FRAME	m_tFrame;

	RECT	m_tCollRect; // 충돌 박스

	// 가지고 있는 무기
	WEAPONID	m_eWeapon;

	float	m_fSpeed;
	float	m_fAngle;

	// 201117 총/탄환 관련
	// 총 사거리
	float	m_fRange;
	int		m_iDmg;
	// 총 최대 총알 / 현재 총알
	int		m_iMaxBulletNum;
	int		m_iCurBulletNum;

	// 201117 플레이어 hp
	int		m_iHP;

	bool	m_bIsInit;
	bool	m_bIsDead;

	wstring	m_wstrObjKey;

	OBJLIST*	m_pBulletLst;	/*총알 리스트.*/

	// 프레임 최적화
	vector<TEXINFO*> m_vecTexture;


};

