#pragma once
#include "Obj.h"

class CMouse :
	public CObj
{
public:
	CMouse();
	virtual ~CMouse();

public:
	static D3DXVECTOR3 GetMousePos();

	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	wstring	m_wstrStateKey;

};

