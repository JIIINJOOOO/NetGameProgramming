#pragma once
#include "Obj.h"
class CWall :
	public CObj
{
public:
	CWall();
	CWall(const INFO & tInfo);
	virtual ~CWall();

	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

};

