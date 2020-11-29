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

	// 201129 �÷��̾� �� CreateObj 
	static HRESULT CreateObj(CObj*& pObject,int iPlayerID)
	{
		pObject = new T;
		pObject->SetPlayerID(iPlayerID);

		if (FAILED(pObject->Initialize()))
		{
			SafeDelete(pObject);
			return E_FAIL;
		}

		return S_OK;
	}

	// �Ѿ� ������ �� WEAPONID�� ���� ���ǵ� �ٸ��� -> ���� �ӵ��� �÷��̾�� ����
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
