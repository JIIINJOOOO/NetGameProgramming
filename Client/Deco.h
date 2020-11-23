#pragma once
#include "Obj.h"
class CDeco :
	public CObj
{
public:
	CDeco();
	CDeco(const INFO & tInfo);
	virtual ~CDeco();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;
};

