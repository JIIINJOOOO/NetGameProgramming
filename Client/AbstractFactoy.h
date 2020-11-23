#pragma once

class CObj;

template <typename T>
class CAbstractFactory
{
public:
	static HRESULT CreateObj(CObj*& pObject)
	{
		pObject = new T;

		if (FAILED(pObject->Initialize()))
		{
			SafeDelete(pObject);
			return E_FAIL;
		}

		return S_OK;
	}

	// 총알 생성할 때 WEAPONID에 따라 스피드 다르게 -> 생성 속도는 플레이어에서 제어
	static CObj* CreateObj(D3DXVECTOR3 vPos, D3DXVECTOR3 vDir, WEAPONID eID)
	{
		CObj* pObj = new T;
		pObj->Initialize();

		pObj->SetPos(vPos);
		pObj->SetDir(vDir);
		pObj->SetWeaponID(eID);


		return pObj;
	}
	
};
