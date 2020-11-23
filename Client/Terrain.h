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
	// CObj��(��) ���� ��ӵ�
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

	// �� Ÿ���� ���� ������ ������ �׷���.
	vector<list<INFO*>> m_vecAdjacency;

	
};
