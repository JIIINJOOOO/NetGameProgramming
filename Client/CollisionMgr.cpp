#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include <iostream>


CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::CollisionRect(OBJLIST & dstLst, OBJLIST & srcLst)
{
	for (CObj* pDst : dstLst)
	{
		for (CObj* pSrc : srcLst)
		{
			RECT rc = {};

			const RECT dstRect = pDst->GetCollRect();
			const RECT srcRect = pSrc->GetCollRect();

			if (IntersectRect(&rc, &dstRect, &srcRect))
			{
				pDst->IsDead();
				pSrc->IsDead();
			}
		}
	}
}

void CCollisionMgr::CollisionRectPlayerBul(OBJLIST& dstLst, OBJLIST& srcLst)
{
	for (CObj* pDst : dstLst)
	{
		for (CObj* pSrc : srcLst)
		{
			RECT rc = {};

			const RECT dstRect = pDst->GetCollRect();
			const RECT srcRect = pSrc->GetCollRect();

			if (IntersectRect(&rc, &dstRect, &srcRect))
			{
				cout << "플레이어 ID:" << pDst->GetPlayerID() << "총알 ID:" << pSrc->GetPlayerID() << endl;
				//pDst->IsDead();
				if (pDst->GetPlayerID() == 1 && pSrc->GetPlayerID() == 2)
				{
					pSrc->IsDead();
				}
				else if (pDst->GetPlayerID() == 2 && pSrc->GetPlayerID() == 1)
				{
					pSrc->IsDead();
				}
			}
		}
	}
}


// 벽 충돌 알고리즘
void CCollisionMgr::CollisionRectEx(OBJLIST & dstLst, OBJLIST & srcLst)
{
	// 밀려날 거리
	float fMoveX = 0.f, fMoveY = 0.f;

	for (CObj*& pDst : dstLst)
	{
		for (CObj*& pSrc : srcLst)
		{
			if (CheckRect(pDst, pSrc, &fMoveX, &fMoveY))
			{
				// 파고든 깊이가 짧은 쪽으로 pSrc를 밀어내자!
				if (fMoveX > fMoveY) // y축으로 밀어냄
				{
					float fX = pSrc->GetInfo().vPos.x;
					float fY = pSrc->GetInfo().vPos.y;

					if (pDst->GetInfo().vPos.y > fY)
						fMoveY *= -1.f;

					pSrc->SetPos(D3DXVECTOR3(fX, fY + fMoveY,0.f));
				}
				else // x축으로 밀어냄
				{
					float fX = pSrc->GetInfo().vPos.x;
					float fY = pSrc->GetInfo().vPos.y;

					if (pDst->GetInfo().vPos.x > fX)
						fMoveX *= -1.f;

					pSrc->SetPos(D3DXVECTOR3(fX + fMoveX, fY,0.f));
				}
			}
		}
	}


	//for (CObj* pDst : dstLst)
	//{
	//	for (CObj* pSrc : srcLst)
	//	{
	//		RECT rc = {};

	//		const RECT dstRect = pDst->GetInfo().rcColRect;
	//		const RECT srcRect = pSrc->GetInfo().rcColRect;

	//		if (MyIntersectRect(&rc, &dstRect, &srcRect))
	//		{
	//			float w = static_cast<float>(rc.right - rc.left);
	//			float h = static_cast<float>(rc.bottom - rc.top);

	//			if (w > h)	//위, 아래에서 충돌.
	//			{
	//				if (rc.top == dstRect.top)	//위에서 충돌 했다면
	//				{
	//					(pSrc)->AddSetYpos(-h);
	//					(pSrc)->AddSetColBoxPosY(-h);

	//				}
	//				else if (rc.bottom == dstRect.bottom)	//아래에서 충돌했다면
	//				{
	//					(pSrc)->AddSetYpos(h);
	//					(pSrc)->AddSetColBoxPosY(h);
	//				}

	//			}
	//			else        //좌, 우에서 충돌.
	//			{
	//				if (rc.left == dstRect.left)			//좌에서 충돌 했다면
	//				{
	//					(pSrc)->AddSetXpos(-w);
	//					(pSrc)->AddSetColBoxPosX(-w);

	//				}
	//				else if (rc.right == dstRect.right)	//우에서 충돌 했다면
	//				{
	//					(pSrc)->AddSetXpos(w);
	//					(pSrc)->AddSetColBoxPosX(w);
	//				}
	//			}
	//		}
	//	}
	//}
}


void CCollisionMgr::CollisionSphere(OBJLIST & dstLst, OBJLIST & srcLst)
{
	for (CObj*& pDst : dstLst)
	{
		for (CObj*& pSrc : srcLst)
		{
			if (CheckSphere(pDst, pSrc))
			{
				
			}
		}
	}
}

// 무기- 플레이어 충돌처리
void CCollisionMgr::CollisionRectWeapon(OBJLIST & dstLst, OBJLIST & srcLst)
{
	for (CObj* pDst : dstLst)
	{
		for (CObj* pSrc : srcLst)
		{
			RECT rc = {};

			const RECT dstRect = pDst->GetCollRect();
			const RECT srcRect = pSrc->GetCollRect();

			if (IntersectRect(&rc, &dstRect, &srcRect))
			{
				//pSrc->SetWeaponID(pDst->GetWeaponID());
				// 플레이어가 무기와 충돌중이다
				pSrc->SetIsOverlap(true);
				if (pSrc->GetIsPressedE() && (pSrc->GetMoney() >= pDst->GetMoney()))
				{
					pSrc->SetWeaponID(pDst->GetWeaponID());
					pSrc->SetWeaponMaxBul(pDst->GetWeaponMaxBul());
					pSrc->SetWeaponCurBul(pDst->GetWeaponMaxBul());
					pSrc->SetMoney(pSrc->GetMoney() - pDst->GetMoney());
				}
				// 201123 최대 총알 set 함수 추가
				//pDst->IsDead();
			}
		}
	}
}

bool CCollisionMgr::CheckSphere(CObj * pDst, CObj * pSrc)
{
	float fSumRad = ((pDst->GetInfo().vSize.x)*0.5 + (pSrc->GetInfo().vSize.x)*0.5);
	const float fDist = D3DXVec3Length(&(pDst->GetInfo().vPos - pSrc->GetInfo().vPos));


	return fSumRad >= fDist;
}

bool CCollisionMgr::CheckRect(CObj * pDst, CObj * pSrc, float * pMoveX, float * pMoveY)
{
	// 두 사각형의 가로, 세로 축 반지름의 합을 구한다.
	float fSumRadX = ((pDst->GetCollRect().right - pDst->GetCollRect().left) + (pSrc->GetCollRect().right - pSrc->GetCollRect().left)) * 0.5f;
	float fSumRadY = ((pDst->GetCollRect().bottom - pDst->GetCollRect().top) + (pSrc->GetCollRect().bottom - pSrc->GetCollRect().top)) * 0.5f;

	// 두 사각형의 가로, 세로 중점의 거리를 구한다.
	// fabs(X): X의 절대 값을 구하는 함수. <cmath>에서 제공. 
	float fDistX = fabs(pDst->GetInfo().vPos.x - pSrc->GetInfo().vPos.x);
	float fDistY = fabs(pDst->GetInfo().vPos.y - pSrc->GetInfo().vPos.y);
	//const float fDist = D3DXVec3Length(&(pDst->GetInfo().vPos - pSrc->GetInfo().vPos));


	// 가로 축 반지름 합이 가로 축 거리보다 클 때와
	// 세로 축 반지름 합이 세로 축 거리보다 클 때
	if (fSumRadX > fDistX && fSumRadY > fDistY)
	{
		// 두 사각형이 겹쳤을 때 파고든 길이도 구한다.
		*pMoveX = fSumRadX - fDistX;
		*pMoveY = fSumRadY - fDistY;

		return true;
	}

	return false;
}
