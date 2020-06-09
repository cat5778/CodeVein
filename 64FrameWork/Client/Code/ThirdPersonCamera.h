#ifndef ThirdPerson_h__
#define ThirdPerson_h__

#include "Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END


class CThirdPersonCamera : public Engine::CCamera
{
private:
	explicit CThirdPersonCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CThirdPersonCamera(void);

public:
	void	SetUp_Target(Engine::CTransform* pTargetInfo);

public:
	HRESULT Ready_GameObject(const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFovY,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

private:
	void	Key_Input(const _float& fTimeDelta);
	void	Target_Renewal(const _float& fTimeDelta);
	void	Mouse_Move(const _float& fTimeDelta);
	void	Lerp_Cam(_float fTimeDelta);
private:
	Engine::CTransform*		m_pTargetInfo = nullptr;
	_float					m_fDistance = 1.5f;
	_float					m_fVerticalAngle = 0.f;
	_float					m_fHorizonAngle = 0.f;
	_float					m_fOldVerticalAngle = 0.f;
	_float					m_fOldHorizonAngle = 0.f;
	_float					m_fLerpVerticalAngle = 0.f;
	_float					m_fLerpHorizonAngle = 0.f;

	_float					m_fTime[2] = { 0.f,0.f };
	_vec3					m_vOldEye;



	const	_matrix*		m_pParentBoneMatrix = nullptr;
	const	_matrix*		m_pParentWorldMatrix = nullptr;
	_matrix					m_matWorld;

public:
	static CThirdPersonCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		Engine::CTransform* pTargetInfo,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFovY,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

private:
	virtual void Free(void) override;

};


#endif // ThirdPerson_h__
