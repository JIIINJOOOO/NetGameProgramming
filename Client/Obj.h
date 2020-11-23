#pragma once
class CObj
{
public:
	CObj();
	CObj(const INFO & tInfo);
	virtual ~CObj();

public:
 	const INFO& GetInfo() const { return m_tInfo; }
	const RECT& GetCollRect() const { return m_tCollRect; } // �߰�
	const wstring& GetObjKey() { return m_wstrObjKey; }
	const WEAPONID& GetWeaponID() { return m_eWeapon; } // �߰�
	//const vector<TEXINFO*>& GetVecTexture() const { return m_vecTexture; } // �߰� -> �浹ó���� �Ẹ��

public:
	void SetPos(D3DXVECTOR3 vPos) { m_tInfo.vPos = vPos; } // �߰�
	void SetDir(D3DXVECTOR3 vDir) { m_tInfo.vDir = vDir; } // �߰�
	void SetWeaponID(WEAPONID eWeapon) { m_eWeapon = eWeapon; } // �߰�

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
	// �̹��� ũ�⿡ �°� �浹�ڽ� ���� �Լ�
	void UpdateCollRect(const TEXINFO* pTexInfo); // �߰�
	void MoveFrame();

	
protected:
	INFO	m_tInfo;
	FRAME	m_tFrame;

	RECT	m_tCollRect; // �浹 �ڽ�

	// ������ �ִ� ����
	WEAPONID	m_eWeapon;

	float	m_fSpeed;
	float	m_fAngle;

	// 201117 ��/źȯ ����
	// �� ��Ÿ�
	float	m_fRange;
	int		m_iDmg;
	// 201117 �÷��̾� hp
	int		m_iHP;

	bool	m_bIsInit;
	bool	m_bIsDead;

	wstring	m_wstrObjKey;

	OBJLIST*	m_pBulletLst;	/*�Ѿ� ����Ʈ.*/

	// ������ ����ȭ
	vector<TEXINFO*> m_vecTexture;


};

