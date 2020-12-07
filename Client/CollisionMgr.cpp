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
				cout << "�÷��̾� ID:" << pDst->GetPlayerID() << "�Ѿ� ID:" << pSrc->GetPlayerID() << endl;
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


// �� �浹 �˰���
void CCollisionMgr::CollisionRectEx(OBJLIST & dstLst, OBJLIST & srcLst)
{
	// �з��� �Ÿ�
	float fMoveX = 0.f, fMoveY = 0.f;

	for (CObj*& pDst : dstLst)
	{
		for (CObj*& pSrc : srcLst)
		{
			if (CheckRect(pDst, pSrc, &fMoveX, &fMoveY))
			{
				// �İ�� ���̰� ª�� ������ pSrc�� �о��!
				if (fMoveX > fMoveY) // y������ �о
				{
					float fX = pSrc->GetInfo().vPos.x;
					float fY = pSrc->GetInfo().vPos.y;

					if (pDst->GetInfo().vPos.y > fY)
						fMoveY *= -1.f;

					pSrc->SetPos(D3DXVECTOR3(fX, fY + fMoveY,0.f));
				}
				else // x������ �о
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

	//			if (w > h)	//��, �Ʒ����� �浹.
	//			{
	//				if (rc.top == dstRect.top)	//������ �浹 �ߴٸ�
	//				{
	//					(pSrc)->AddSetYpos(-h);
	//					(pSrc)->AddSetColBoxPosY(-h);

	//				}
	//				else if (rc.bottom == dstRect.bottom)	//�Ʒ����� �浹�ߴٸ�
	//				{
	//					(pSrc)->AddSetYpos(h);
	//					(pSrc)->AddSetColBoxPosY(h);
	//				}

	//			}
	//			else        //��, �쿡�� �浹.
	//			{
	//				if (rc.left == dstRect.left)			//�¿��� �浹 �ߴٸ�
	//				{
	//					(pSrc)->AddSetXpos(-w);
	//					(pSrc)->AddSetColBoxPosX(-w);

	//				}
	//				else if (rc.right == dstRect.right)	//�쿡�� �浹 �ߴٸ�
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

// ����- �÷��̾� �浹ó��
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
				// �÷��̾ ����� �浹���̴�
				pSrc->SetIsOverlap(true);
				if (pSrc->GetIsPressedE() && (pSrc->GetMoney() >= pDst->GetMoney()))
				{
					pSrc->SetWeaponID(pDst->GetWeaponID());
					pSrc->SetWeaponMaxBul(pDst->GetWeaponMaxBul());
					pSrc->SetWeaponCurBul(pDst->GetWeaponMaxBul());
					pSrc->SetMoney(pSrc->GetMoney() - pDst->GetMoney());
				}
				// 201123 �ִ� �Ѿ� set �Լ� �߰�
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
	// �� �簢���� ����, ���� �� �������� ���� ���Ѵ�.
	float fSumRadX = ((pDst->GetCollRect().right - pDst->GetCollRect().left) + (pSrc->GetCollRect().right - pSrc->GetCollRect().left)) * 0.5f;
	float fSumRadY = ((pDst->GetCollRect().bottom - pDst->GetCollRect().top) + (pSrc->GetCollRect().bottom - pSrc->GetCollRect().top)) * 0.5f;

	// �� �簢���� ����, ���� ������ �Ÿ��� ���Ѵ�.
	// fabs(X): X�� ���� ���� ���ϴ� �Լ�. <cmath>���� ����. 
	float fDistX = fabs(pDst->GetInfo().vPos.x - pSrc->GetInfo().vPos.x);
	float fDistY = fabs(pDst->GetInfo().vPos.y - pSrc->GetInfo().vPos.y);
	//const float fDist = D3DXVec3Length(&(pDst->GetInfo().vPos - pSrc->GetInfo().vPos));


	// ���� �� ������ ���� ���� �� �Ÿ����� Ŭ ����
	// ���� �� ������ ���� ���� �� �Ÿ����� Ŭ ��
	if (fSumRadX > fDistX && fSumRadY > fDistY)
	{
		// �� �簢���� ������ �� �İ�� ���̵� ���Ѵ�.
		*pMoveX = fSumRadX - fDistX;
		*pMoveY = fSumRadY - fDistY;

		return true;
	}

	return false;
}
