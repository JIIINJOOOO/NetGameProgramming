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
	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

public:
	// m_wstrObjKey를 바꿔서 무기 바뀐 애니메이션 재생 되게
	const TCHAR* ChangeWeapon();


private:
	void KeyCheck();
private:
	wstring	m_wstrStateKey;

	// Texture 애니메이션 Key 변환 변수.
	STANCE	m_eCurState;
	STANCE	m_ePreState;


	//CObj*	m_pMouse; // 마우스 객체 -> 아마 안 필요 할지두?
};

