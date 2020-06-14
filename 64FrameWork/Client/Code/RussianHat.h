#pragma once


#include "Defines.h"
#include "GameObject.h"
#include "DynamicObject.h"

class CRussianHat : public CDynamicObject
{
public:
	explicit				CRussianHat(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx);
	explicit				CRussianHat(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo);
	virtual					~CRussianHat(void);

public:
	HRESULT					Ready_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	


private:
	virtual HRESULT			Add_Component(void);
	void					Set_TransformData();



public:
	static CRussianHat*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx = 0);
	static CRussianHat*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx ,  TRANSFORM_INFO tInfo);

private:
	virtual void			Free(void) override;
};

