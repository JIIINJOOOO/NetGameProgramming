#pragma once
#include "Obj.h"
class CWeapon :
	public CObj
{
public:
	CWeapon();
	CWeapon(const INFO & tInfo);
	virtual ~CWeapon();

public:
	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;
public:
	void SetWeaponID();
	// 201123 최대 총알 추가 함수
	void SetWeaponMaxBul();
};

