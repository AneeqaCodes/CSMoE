#pragma once

#ifdef CLIENT_DLL
namespace cl {
#else
namespace sv {
#endif

//mp5navy
#define MP5N_MAX_SPEED			250
#define MP5N_DAMAGE			26
#define MP5N_RANGE_MODIFER		0.84
#define MP5N_RELOAD_TIME		2.63s
#define MP5N_INSPECT_TIME		5.37s

class CMP5N : public CBasePlayerWeapon
{
	enum mp5n_e;
public:
	void Spawn() override;
	void Precache() override;
	int GetItemInfo(ItemInfo *p) override;
	BOOL Deploy() override;
	float GetMaxSpeed() override { return MP5N_MAX_SPEED; }
	int iItemSlot() override { return PRIMARY_WEAPON_SLOT; }
	void PrimaryAttack() override;
	void Reload() override;
	void WeaponIdle() override;
	void Inspect() override;
	duration_t GetInspectTime() override { return MP5N_INSPECT_TIME; }
	BOOL UseDecrement() override {
#ifdef CLIENT_WEAPONS
		return TRUE;
#else
		return FALSE;
#endif
	}
	KnockbackData GetKnockBackData() override { return { 250.0f, 200.0f, 250.0f, 90.0f, 0.7f }; }

public:
	void MP5NFire(float flSpread, duration_t flCycleTime, BOOL fUseAutoAim);

	int m_iShell;
	time_point_t m_NextInspect;
	int iShellOn;

private:
	unsigned short m_usFireMP5N;
};

}
