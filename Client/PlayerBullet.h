#pragma once
#include "Obj.h"
class CPlayerBullet :
	public CObj
{
public:
	CPlayerBullet();
	virtual ~CPlayerBullet();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;
//private:
//	WEAPONID m_eID; // 무기에 따라 다른 탄환(스피드,각도, 샷건은 개수)
private:
	wstring	m_wstrStateKey;
	
	// 201117
	float m_fDistance; // 발사한 플레이어 위치와 총알 사이 거리
	D3DXVECTOR3 m_fPlayerPos; // 플레이어가 발사한 위치
	
};

