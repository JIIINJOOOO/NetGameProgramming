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
	// 201123 �ִ�/���� �Ѿ� get
	const int& GetWeaponMaxBul() { return m_iMaxBulletNum; } // �߰�
	const int& GetWeaponCurBul() { return m_iCurBulletNum; } // �߰�
	// �÷��̾ e�� �������� get
	const bool& GetIsPressedE() { return m_bIsPressedE; } // �߰�
	// ���� �ݾ� get
	const int& GetMoney() { return m_iMoney; } // �߰�
	// 201129 �÷��̾��� id get
	const int& GetPlayerID() { return m_iPlayerID; } // �߰�

public:
	void SetPos(D3DXVECTOR3 vPos) { m_tInfo.vPos = vPos; } // �߰�
	void SetDir(D3DXVECTOR3 vDir) { m_tInfo.vDir = vDir; } // �߰�
	void SetWeaponID(WEAPONID eWeapon) { m_eWeapon = eWeapon; } // �߰�
	// 201123 �ִ�/���� �Ѿ� set -> �ݸ����Ŵ������� ȣ��
	void SetWeaponMaxBul(int iMaxBulNum) { m_iMaxBulletNum = iMaxBulNum; }
	void SetWeaponCurBul(int iMaxBulNum) { m_iCurBulletNum = iMaxBulNum; }
	// �÷��̾ ����� �浹������ set
	void SetIsOverlap(bool bIsOverlap) { m_bIsOverlap = bIsOverlap; }
	// �÷��̾��� ������ set
	void SetMoney(int iMoney) { m_iMoney = iMoney; }
	// 201129 �÷��̾��� ID set
	void SetPlayerID(int iID) { m_iPlayerID = iID; }



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
	// �� �ִ� �Ѿ� / ���� �Ѿ�
	int		m_iMaxBulletNum;
	int		m_iCurBulletNum;

	// 201117 �÷��̾� hp
	int		m_iHP;
	// 201124 �÷��̾� ������ / �� ����
	int		m_iMoney;
	// ����� �÷��̾ �浹�ϰ� ���� �� e�� ������ ����
	bool	m_bIsOverlap; // �浹���϶�
	bool	m_bIsPressedE; // e�� ���ȴ���
	// �÷��̾� ID
	int m_iPlayerID;


	bool	m_bIsInit;
	bool	m_bIsDead;

	wstring	m_wstrObjKey;

	OBJLIST*	m_pBulletLst;	/*�Ѿ� ����Ʈ.*/

	// ������ ����ȭ
	vector<TEXINFO*> m_vecTexture;
};

