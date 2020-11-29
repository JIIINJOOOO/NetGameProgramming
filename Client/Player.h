#pragma once
#include "Obj.h"
class CPlayer :
	public CObj
{
public:
	enum STANCE
	{
		IDLE,WALK,THROW,ATTACK,DEAD,STANCE_END
	};
public:
	CPlayer();
	virtual ~CPlayer();

public:
	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	// m_wstrObjKey�� �ٲ㼭 ���� �ٲ� �ִϸ��̼� ��� �ǰ�
	const TCHAR* ChangeWeapon();


private:
	void KeyCheck();
private:
	wstring	m_wstrStateKey;

	// Texture �ִϸ��̼� Key ��ȯ ����.
	STANCE	m_eCurState;
	STANCE	m_ePreState;


	//CObj*	m_pMouse; // ���콺 ��ü -> �Ƹ� �� �ʿ� ������?
};

