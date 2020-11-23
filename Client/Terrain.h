#pragma once

#include "Obj.h"

class CTerrain : public CObj
{
public:
	CTerrain();
	virtual ~CTerrain();

public:
	vector<INFO*>& GetVecTile() { return m_vecTile; }
	vector<list<INFO*>>& GetVecAdjacency() { return m_vecAdjacency; }

public:
	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void LateInit() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	HRESULT LoadTile();
	void ReadyAdjacency();

private:
	vector<INFO*>	m_vecTile;

	// 각 타일의 인접 정보를 보관할 그래프.
	vector<list<INFO*>> m_vecAdjacency;

	
};
