#pragma once

/*
	template<class T>
	concept SecondaryAttackZoom_c = requires
	{
		T::ZoomFOV;
	};
*/

template<class CFinal, class CBase = CBaseTemplateWeapon>
class TSecondaryAttackSniperZoom1 : public CBase
{
public:

	static constexpr auto Rec_SecondaryAttack_HasZoom = true;
	int Ref_GetMinZoomFOV() { return df::ZoomFOV::Get(static_cast<CFinal &>(*this).WeaponTemplateDataSource()); }

public:
	void SecondaryAttack(void) override
	{
		CFinal &wpn = static_cast<CFinal &>(*this);
		auto &&data = wpn.WeaponTemplateDataSource();
		switch (CBase::m_pPlayer->m_iFOV)
		{
			case 90: CBase::m_pPlayer->m_iFOV = CBase::m_pPlayer->pev->fov = df::ZoomFOV::Get(data); break;
		default: CBase::m_pPlayer->m_iFOV = CBase::m_pPlayer->pev->fov = 90; break;
		}

		CBase::m_pPlayer->ResetMaxSpeed();
		EMIT_SOUND(ENT(CBase::pev), CHAN_ITEM, "weapons/zoom.wav", 0.2, 2.4);
		CBase::m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.3s;

		return CBase::SecondaryAttack(); // pass over
	}

	bool HasSecondaryAttack() override { return true || CBase::HasSecondaryAttack(); }
};
