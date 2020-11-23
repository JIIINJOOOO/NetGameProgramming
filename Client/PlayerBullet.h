#pragma once
#include "Obj.h"
class CPlayerBullet :
	public CObj
{
public:
	CPlayerBullet();
	virtual ~CPlayerBullet();

	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;
//private:
//	WEAPONID m_eID; // ���⿡ ���� �ٸ� źȯ(���ǵ�,����, ������ ����)
private:
	wstring	m_wstrStateKey;
	
	// 201117
	float m_fDistance; // �߻��� �÷��̾� ��ġ�� �Ѿ� ���� �Ÿ�
	D3DXVECTOR3 m_fPlayerPos; // �÷��̾ �߻��� ��ġ
	
};

