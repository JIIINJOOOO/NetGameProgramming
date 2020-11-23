#pragma once
#include "Obj.h"
class CDoor :
	public CObj
{
public:
	CDoor();
	CDoor(const INFO & tInfo);
	virtual ~CDoor();

	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;
};

