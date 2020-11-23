#include "stdafx.h"
#include "Weapon.h"


CWeapon::CWeapon()
{
	srand((unsigned)time(NULL));
}

CWeapon::CWeapon(const INFO & tInfo)
	:CObj(tInfo)
{
}


CWeapon::~CWeapon()
{
	Release();
}

HRESULT CWeapon::Initialize()
{
	m_fAngle = rand() % 180;

	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Weapon/Weapon_0%d.png",
		L"Weapon", TEX_MULTI, L"Weapon", 6)))
	{
		ERR_MSG(L"Weapon Texture Load Failed!");
		return E_FAIL;
	}


	// 프레임 최적화
	m_vecTexture = CTextureMgr::GetInstance()->GetTexVector(L"Weapon", L"Weapon");
	SetWeaponID();
	// 201123 무기별 최대 총알 설정
	SetWeaponMaxBul();
	return S_OK;
}

void CWeapon::LateInit()
{
}

int CWeapon::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;
	return NO_EVENT;
}

void CWeapon::LateUpdate()
{
}

void CWeapon::Render()
{
	D3DXMATRIX matRotZ, matTrans;

	D3DXVECTOR3 vScroll = m_tInfo.vPos + CScrollMgr::GetScroll();

	D3DXMatrixIdentity(&m_tInfo.matWorld);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));
	D3DXMatrixTranslation(&matTrans, vScroll.x, vScroll.y, 0.f);

	// 스 자 이 공 부
	m_tInfo.matWorld = matRotZ * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	/*const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		L"Weapon", L"Weapon", m_tInfo.byDrawID);*/
	
	// 프레임 최적화
	const TEXINFO* pTexInfo = m_vecTexture[m_tInfo.byDrawID];

	CObj::UpdateCollRect(pTexInfo); // 충돌박스 추가


	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CWeapon::Release()
{
}

void CWeapon::SetWeaponID()
{
	switch (m_tInfo.byDrawID)
	{
	case 0:
		m_eWeapon = WEAPONID::RIFLE;
		break;
	case 1:
		m_eWeapon = WEAPONID::SMG;
		break;
	case 2:
		m_eWeapon = WEAPONID::SHOTGUN;
		break;
	case 3:
		m_eWeapon = WEAPONID::CLUB;
		break;
	case 4:
		m_eWeapon = WEAPONID::BAT;
		break;
	case 5:
		m_eWeapon = WEAPONID::KNIFE;
		break;
	default:
		break;
	}
}

// 201123 무기별 최대총알 설정 함수
void CWeapon::SetWeaponMaxBul()
{
	switch (m_eWeapon)
	{
	case RIFLE:
		m_iMaxBulletNum = 5;
		break;
	case SMG:
		m_iMaxBulletNum = 3;
		break;
	case SHOTGUN:
		m_iMaxBulletNum = 2;
		break;
	default:
		break;
	}
}
