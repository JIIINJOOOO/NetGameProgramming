#include "stdafx.h"
#include "Wall.h"
#include "Mouse.h"


CWall::CWall()
{
}

CWall::CWall(const INFO & tInfo)
	:CObj(tInfo)
{
}


CWall::~CWall()
{
	Release();
}

HRESULT CWall::Initialize()
{

	m_tInfo.vSize = { 5.f, 20.f, 0.f };

	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Wall/Wall_0%d.png",
		L"Wall", TEX_MULTI, L"Wall", 6)))
	{
		ERR_MSG(L"Wall Texture Load Failed!");
		return E_FAIL;
	}

	// ������ ����ȭ
	m_vecTexture = CTextureMgr::GetInstance()->GetTexVector(L"Wall", L"Wall");

	return S_OK;
}

void CWall::LateInit()
{
}

int CWall::Update()
{
	CObj::LateInit();
	// �Ʒ��� �ͷ��� �Ŵ������� �ϴ°� �ƴѰ�?
	//float fTime = CTimeMgr::GetInstance()->GetTime();

	//if (0 > CMouse::GetMouse().x)
	//	CScrollMgr::SetScroll(500.f * fTime, 0.f);
	//if (WINCX < CMouse::GetMouse().x)
	//	CScrollMgr::SetScroll(-500.f * fTime, 0.f);
	//if (0 > CMouse::GetMouse().y)
	//	CScrollMgr::SetScroll(0.f, 500.f * fTime);
	//if (WINCY < CMouse::GetMouse().y)
	//	CScrollMgr::SetScroll(0, -500.f * fTime);

	return NO_EVENT;
}

void CWall::LateUpdate()
{
}

void CWall::Render()
{
	//CObj::UpdateCollRect(m_vecTexture[m_tInfo.byDrawID]); // �浹�ڽ� �߰�

	D3DXMATRIX matTrans;

	D3DXVECTOR3 vScroll = m_tInfo.vPos + CScrollMgr::GetScroll();

	D3DXMatrixIdentity(&m_tInfo.matWorld);
	D3DXMatrixTranslation(&matTrans, vScroll.x, vScroll.y, 0.f);

	m_tInfo.matWorld = matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	/*const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		L"Wall", L"Wall", m_tInfo.byDrawID);*/
	
	// ������ ����ȭ
	const TEXINFO* pTexInfo = m_vecTexture[m_tInfo.byDrawID];

	CObj::UpdateCollRect(pTexInfo); // �浹�ڽ� �߰�


	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CWall::Release()
{
}
