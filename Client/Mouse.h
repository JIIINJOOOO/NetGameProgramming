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

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	wstring	m_wstrStateKey;

};

