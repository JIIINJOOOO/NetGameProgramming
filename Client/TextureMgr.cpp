#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO* CTextureMgr::GetTexture(const TCHAR* pObjKey, const TCHAR* pStateKey, 
	const int & iIndex)
{
	// 우선 ObjKey로 CTexture를 탐색
	auto iter_find = find_if(m_MapTexture.begin(), m_MapTexture.end(),
		[&pObjKey](auto& MyPair)
	{
		return pObjKey == MyPair.first;
	});

	if (m_MapTexture.end() == iter_find)
		return nullptr;

	return iter_find->second->GetTexture(pStateKey, iIndex);
}

// 프레임 최적화
const vector<TEXINFO*>& CTextureMgr::GetTexVector(const TCHAR * pObjKey, const TCHAR * pStateKey)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return dynamic_cast<CMultiTexture*>(m_MapTexture[pObjKey])->GetTexVector(pStateKey);
}

HRESULT CTextureMgr::ReadImgPath(const wstring & wstrPath)
{
	wifstream fin;

	fin.open(wstrPath);

	if (fin.fail())
	{
		ERR_MSG(L"ImgPath Load Failed");
		return E_FAIL;
	}

	TCHAR szObjKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szCount[MIN_STR] = L"";
	TCHAR szPath[MAX_STR] = L"";	

	while (true)
	{
		// 구분자('|') 단위로 끊어서 읽어들임.
		fin.getline(szObjKey, MAX_STR, '|');
		fin.getline(szStateKey, MAX_STR, '|');
		fin.getline(szCount, MIN_STR, '|');
		fin.getline(szPath, MAX_STR);

		// eof: 현재 지시자가 eof자리를 넘어서면 true 반환.
		if (fin.eof())
			break;				

		int iCount = _ttoi(szCount);

		if (FAILED(InsertTexture(szPath, szObjKey, TEX_MULTI, szStateKey, iCount)))
		{
			fin.close();

			ERR_MSG(L"Multi Texture Load Failed");
			return E_FAIL;
		}
	}

	fin.close();

	return S_OK;
}

HRESULT CTextureMgr::InsertTexture(const TCHAR * pFilePath, const TCHAR * pObjKey, TEXTYPE eType,
	const TCHAR * pStateKey, const int & iCount)
{
	// 우선 ObjKey로 CTexture를 탐색
	auto iter_find = find_if(m_MapTexture.begin(), m_MapTexture.end(),
		[&pObjKey](auto& MyPair)
	{
		return pObjKey == MyPair.first;
	});

	// 해당 ObjKey를 찾지 못하면 새로 삽입해주어야 한다.
	if (m_MapTexture.end() == iter_find)
	{
		CTexture* pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;
		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->InsertTexture(pFilePath, pStateKey, iCount)))
		{
			ERR_MSG(pFilePath);
			SafeDelete(pTexture);
			return E_FAIL;
		}

		m_MapTexture.insert({ pObjKey, pTexture });
	}	
	else if (TEX_MULTI == eType)
		iter_find->second->InsertTexture(pFilePath, pStateKey, iCount);

	return S_OK;
}

void CTextureMgr::Release()
{
	for_each(m_MapTexture.begin(), m_MapTexture.end(),
		[](auto& MyPair)
	{
		SafeDelete(MyPair.second);
	});

	m_MapTexture.clear();
}
