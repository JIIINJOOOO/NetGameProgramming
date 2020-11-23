#include "stdafx.h"
#include "AstarMgr.h"
#include "Terrain.h"

IMPLEMENT_SINGLETON(CAstarMgr)

CAstarMgr::CAstarMgr()
{
}


CAstarMgr::~CAstarMgr()
{
}

void CAstarMgr::StartAstar(const D3DXVECTOR3 & vStart, const D3DXVECTOR3 & vGoal)
{
	CObj* pTerrain = CObjMgr::GetInstance()->GetTerrain();
	vector<INFO*>& vecTile = dynamic_cast<CTerrain*>(pTerrain)->GetVecTile();

	for (auto& pTile : vecTile)
	{
		if (1 == pTile->byOption)
			continue;

		//pTile->byDrawID = 3;
	}

	// 새로이 길 찾을 때마다 기존의 리스트들을 비우고 시작한다.
	m_OpenLst.clear();
	m_CloseLst.clear();
	m_BestLst.clear();

	// vStart(시작점)의 인덱스를 구한다.
	int iStartIdx = GetTileIndex(vStart);

	// vGoal(도착점)의 인덱스를 구한다.
	int iGoalIdx = GetTileIndex(vGoal);

	if (0 > iStartIdx || 0 > iGoalIdx)
		return;

	if (iStartIdx == iGoalIdx)
		return;	

	if (1 == vecTile[iGoalIdx]->byOption)
		return;

	// true를 반환하면 경로 탐색을 완료함.
	if (true == MakeRoute(iStartIdx, iGoalIdx))
		MakeBestLst(iStartIdx, iGoalIdx); // 경로를 만든다.

}

bool CAstarMgr::MakeRoute(int iStartIdx, int iGoalIdx)
{
	CObj* pTerrain = CObjMgr::GetInstance()->GetTerrain();
	vector<INFO*>& vecTile = dynamic_cast<CTerrain*>(pTerrain)->GetVecTile();
	vector<list<INFO*>>& vecAdj = dynamic_cast<CTerrain*>(pTerrain)->GetVecAdjacency();

	if (!m_OpenLst.empty())
		m_OpenLst.pop_front();

	// 현재 거쳐온 경로는 close에 보관.
	m_CloseLst.push_back(iStartIdx);

	// 휴리스틱(비용)을 비교하여 오름차순.
	list<int>	SortLst;

	for (auto& pTile : vecAdj[iStartIdx])
	{
		// 인접한 타일이 골지점이면 
		if (iGoalIdx == pTile->iIndex)
		{
			pTile->iParentIdx = iStartIdx;
			return true; // 재귀는 종료 된다.
		}

		// 인접한 타일 중에 open, close에 있는지 조사.
		if (false == CheckOpenLst(pTile->iIndex) && false == CheckCloseLst(pTile->iIndex))
		{
			pTile->iParentIdx = iStartIdx;
			SortLst.push_back(pTile->iIndex);
		}
	}

	// 인접한 타일들의 거리들을 비교한다. (휴리스틱 조사)
	SortLst.sort([&vecTile, &iStartIdx, &iGoalIdx](int a, int b)
	{
		D3DXVECTOR3 vDir1 = vecTile[iGoalIdx]->vPos - vecTile[a]->vPos;
		D3DXVECTOR3 vDir2 = vecTile[iStartIdx]->vPos - vecTile[a]->vPos;

		D3DXVECTOR3 vDir3 = vecTile[iGoalIdx]->vPos - vecTile[b]->vPos;
		D3DXVECTOR3 vDir4 = vecTile[iStartIdx]->vPos - vecTile[b]->vPos;

		float fDistA = D3DXVec3Length(&vDir1) + D3DXVec3Length(&vDir2);
		float fDistB = D3DXVec3Length(&vDir3) + D3DXVec3Length(&vDir4);

		return fDistA < fDistB;
	});

	// 정렬 순으로 OpenLst에 Push한다.
	for (int& iOpenIndex : SortLst)
		m_OpenLst.push_back(iOpenIndex);

	// 아무리 탐색해봐도 경로가 없을 때
	if (m_OpenLst.empty())
		return false; // 탐색 종료.

	return MakeRoute(m_OpenLst.front(), iGoalIdx);	
}

void CAstarMgr::MakeBestLst(int iStartIdx, int iGoalIdx)
{
	CObj* pTerrain = CObjMgr::GetInstance()->GetTerrain();
	vector<INFO*>& vecTile = dynamic_cast<CTerrain*>(pTerrain)->GetVecTile();

	m_BestLst.push_front(vecTile[iGoalIdx]);
	
	vecTile[iGoalIdx]->byDrawID = 14;
	int iRouteIdx = vecTile[iGoalIdx]->iParentIdx;	

	while (true)
	{
		if (iRouteIdx == iStartIdx)
			break;

		vecTile[iRouteIdx]->byDrawID = 14;
		m_BestLst.push_front(vecTile[iRouteIdx]);
		iRouteIdx = vecTile[iRouteIdx]->iParentIdx;
	}
}

int CAstarMgr::GetTileIndex(const D3DXVECTOR3 & vPos)
{
	CObj* pTerrain = CObjMgr::GetInstance()->GetTerrain();
	vector<INFO*>& vecTile = dynamic_cast<CTerrain*>(pTerrain)->GetVecTile();

	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		if (Picking(vPos, i))
			return i;
	}

	return -1;	// 인덱스 찾기 실패!
}

bool CAstarMgr::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	CObj* pTerrain = CObjMgr::GetInstance()->GetTerrain();
	vector<INFO*>& vecTile = dynamic_cast<CTerrain*>(pTerrain)->GetVecTile();

	// 내적을 이용한 픽킹

	// 1. 12시를 기준으로 시계 방향의 마름모 꼭지점을 구한다.
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y + (TILECY * 0.5f), 0.f),
		D3DXVECTOR3(vecTile[iIndex]->vPos.x + (TILECX * 0.5f), vecTile[iIndex]->vPos.y, 0.f),
		D3DXVECTOR3(vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y - (TILECY * 0.5f), 0.f),
		D3DXVECTOR3(vecTile[iIndex]->vPos.x - (TILECX * 0.5f), vecTile[iIndex]->vPos.y, 0.f)
	};

	// 2. 시계 방향으로 방향 벡터를 구한다.
	D3DXVECTOR3 vDir[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3],
	};

	// 3. 각 법선 벡터를 구한다.
	D3DXVECTOR3 vNormal[4] =
	{
		D3DXVECTOR3(-vDir[0].y, vDir[0].x, 0.f),
		D3DXVECTOR3(-vDir[1].y, vDir[1].x, 0.f),
		D3DXVECTOR3(-vDir[2].y, vDir[2].x, 0.f),
		D3DXVECTOR3(-vDir[3].y, vDir[3].x, 0.f),
	};

	// 4. 구한 법선 벡터들을 단위벡터로 만들어 준다.
	for (int i = 0; i < 4; ++i)
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

	// 5. 각 꼭지점과 마우스 간의 방향벡터를 구한다.
	D3DXVECTOR3 vMouseDir[4] =
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);

	// 4면에 대해서 vMouseDir과 vNormal을 내적하여 나온 결과가 둔각(음수)이면 true!
	for (int i = 0; i < 4; ++i)
	{
		// 하나라도 양수가 나오면 false
		if (0.f < D3DXVec3Dot(&vMouseDir[i], &vNormal[i]))
			return false;
	}

	return true;
}

bool CAstarMgr::CheckOpenLst(int iIndex)
{
	for (int& iOpenIndex : m_OpenLst)
	{
		if (iIndex == iOpenIndex)
			return true;	// open에 존재함.
	}

	return false;
}

bool CAstarMgr::CheckCloseLst(int iIndex)
{
	for (int& iCloseIndex : m_CloseLst)
	{
		if (iIndex == iCloseIndex)
			return true;	// Close에 존재함.
	}

	return false;
}
