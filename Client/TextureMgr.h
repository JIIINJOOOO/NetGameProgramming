#pragma once

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO* GetTexture(const TCHAR* pObjKey, const TCHAR* pStateKey = L"" ,
		const int& iIndex = 0 );
	// 프레임 최적화
	const vector<TEXINFO*>& GetTexVector(const TCHAR* pObjKey, const TCHAR* pStateKey);

public:
	HRESULT ReadImgPath(const wstring& wstrPath);
	HRESULT InsertTexture(const TCHAR* pFilePath , const TCHAR* pObjKey, TEXTYPE eType, 
		const TCHAR* pStateKey = L"" ,	const int& iCount = 0 );
	void Release();

private:
	map<wstring, CTexture*>		m_MapTexture;
};

