#pragma once

#ifdef CLIENT_DLL
namespace cl {
#else
namespace sv {
#endif

//tmp
#define USP_MAX_SPEED		250
#define USP_DAMAGE		34
#define USP_DAMAGE_SIL		30
#define USP_RANGE_MODIFER	0.79
#define USP_RELOAD_TIME		2.7s
#define USP_INSPECT_TIME		5.89s

class CUSP : public CBasePlayerWeapon
{
	enum usp_e;
public:
	void Spawn() override;
	void Precache() override;
	int GetItemInfo(ItemInfo *p) override;
	BOOL Deploy() override;
	float GetMaxSpeed() override { return m_fMaxSpeed; }
	int iItemSlot() override { return PISTOL_SLOT; }
	void PrimaryAttack() override;
	void SecondaryAttack() override;
	void Inspect() override;
	duration_t GetInspectTime() override { return USP_INSPECT_TIME; }
	void Reload() override;
	void WeaponIdle() override;
	BOOL UseDecrement() override {
#ifdef CLIENT_WEAPONS
		return TRUE;
#else
		return FALSE;
#endif
	}
	BOOL IsPistol() override { return TRUE; }
	KnockbackData GetKnockBackData() override { return { 85.0f, 100.0f, 100.0f, 80.0f, 0.8f }; }

public:
	void USPFire(float flSpread, duration_t flCycleTime, BOOL fUseSemi);
	NOXREF void MakeBeam();
	time_point_t m_NextInspect;
	NOXREF void BeamUpdate();

	int m_iShell;

private:
	unsigned short m_usFireUSP;
};

}
