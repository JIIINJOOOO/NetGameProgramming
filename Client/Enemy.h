#pragma once
#include "Obj.h"
class CEnemy :
	public CObj
{
public:
	enum STANCE
	{
		IDLE,WALK,ATTACK,DEAD,STANCE_END
	};
public:
	CEnemy();
	virtual ~CEnemy();

public:
	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;
public:
	const TCHAR* GetObjKey();
private:
	wstring	m_wstrStateKey;

	// Texture �ִϸ��̼� Key ��ȯ ����.
	STANCE	m_eCurState;
	STANCE	m_ePreState;

	int iDist;

};

