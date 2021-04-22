#pragma once

#ifdef CLIENT_DLL
namespace cl {
#else
namespace sv {
#endif

//M14EBR
#define M14EBR_MAX_SPEED			215
#define M14EBR_DAMAGE			46
#define M14EBR_RANGE_MODIFER		0.98
#define M14EBR_RELOAD_TIME		3.2

class CM14EBR : public CBasePlayerWeapon
{
	enum m14ebr_e;
public:
	void Spawn() override;
	void Precache() override;
	int GetItemInfo(ItemInfo *p) override;
	BOOL Deploy() override;
	float GetMaxSpeed() override { return M14EBR_MAX_SPEED; }
	int iItemSlot() override { return PRIMARY_WEAPON_SLOT; }
	void PrimaryAttack() override;
	void SecondaryAttack() override {}
	void Reload() override;
	void WeaponIdle() override;
	BOOL UseDecrement() override {
#ifdef CLIENT_WEAPONS
		return TRUE;
#else
		return FALSE;
#endif
	}

	KnockbackData GetKnockBackData() override { return { 450.0f, 400.0f, 400.0f, 200.0f, 0.5f }; }
	const char *GetCSModelName() override { return "models/w_m14ebr.mdl"; }

public:
	void M14EBRFire(float flSpread, duration_t flCycleTime, BOOL fUseAutoAim);

	int m_iShell;
	int iShellOn;

private:
	unsigned short m_usFireM14EBR;
};

}
