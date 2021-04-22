#pragma once

#ifdef CLIENT_DLL
namespace cl {
#else
namespace sv {
#endif

//sg550
#define SG550_MAX_SPEED			210
#define SG550_MAX_SPEED_ZOOM		150
#define SG550_RANGE_MODIFER		0.98
#define SG550_RELOAD_TIME		3.35s
#define SG550_INSPECT_TIME		4.20s

class CSG550 : public CBasePlayerWeapon
{
	enum sg550_e;
public:
	void Spawn() override;
	void Precache() override;
	int GetItemInfo(ItemInfo *p) override;
	BOOL Deploy() override;
	float GetMaxSpeed() override;
	int iItemSlot() override { return PRIMARY_WEAPON_SLOT; }
	void PrimaryAttack() override;
	void SecondaryAttack() override;
	void Reload() override;
	void WeaponIdle() override;
	void Inspect() override;
	duration_t GetInspectTime() override { return SG550_INSPECT_TIME; }
	BOOL UseDecrement() override {
#ifdef CLIENT_WEAPONS
		return TRUE;
#else
		return FALSE;
#endif
	}
	KnockbackData GetKnockBackData() override { return { 450.0f, 400.0f, 400.0f, 200.0f, 0.5f }; }

public:
	void SG550Fire(float flSpread, duration_t flCycleTime, BOOL fUseAutoAim);
	float GetDamage() const;
public:
	int m_iShell;
	time_point_t m_NextInspect;

private:
	unsigned short m_usFireSG550;
};

}
