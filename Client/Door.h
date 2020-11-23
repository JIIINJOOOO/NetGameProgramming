#pragma once
#include "Obj.h"
class CDoor :
	public CObj
{
public:
	CDoor();
	CDoor(const INFO & tInfo);
	virtual ~CDoor();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;
};

