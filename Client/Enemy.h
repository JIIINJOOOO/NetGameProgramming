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
	// CObj을(를) 통해 상속됨
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

	// Texture 애니메이션 Key 변환 변수.
	STANCE	m_eCurState;
	STANCE	m_ePreState;

	int iDist;

};

