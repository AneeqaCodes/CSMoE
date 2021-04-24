#pragma once

#ifdef CLIENT_DLL
namespace cl {
#else
namespace sv {
#endif

//Famas
#define FAMAS_MAX_SPEED		240
#define FAMAS_RELOAD_TIME	3.3s
#define FAMAS_DAMAGE		30
#define FAMAS_DAMAGE_BURST	34
#define FAMAS_INSPECT_TIME		3.52s
#define FAMAS_RANGE_MODIFER	0.96

class CFamas : public CBasePlayerWeapon
{
	enum famas_e
    {
        FAMAS_IDLE1,
        FAMAS_RELOAD,
        FAMAS_DRAW,
        FAMAS_SHOOT1,
        FAMAS_SHOOT2,
        FAMAS_SHOOT3
    };
public:
	void Spawn() override;
	void Precache() override;
	int GetItemInfo(ItemInfo *p) override;
	BOOL Deploy() override;
	float GetMaxSpeed() override { return FAMAS_MAX_SPEED; }
	int iItemSlot() override { return PRIMARY_WEAPON_SLOT; }
	void PrimaryAttack() override;
	void SecondaryAttack() override;
	void Reload() override;
	void Inspect() override;
	duration_t GetInspectTime() override { return FAMAS_INSPECT_TIME; }
	void WeaponIdle() override;
	BOOL UseDecrement() override {
#ifdef CLIENT_WEAPONS
		return TRUE;
#else
		return FALSE;
#endif
	}
	KnockbackData GetKnockBackData() override { return { 350.0f, 250.0f, 300.0f, 100.0f, 0.6f }; }

public:
	void FamasFire(float flSpread, duration_t flCycleTime, BOOL fUseAutoAim, BOOL bFireBurst);
	float GetDamage() const;
public:
	int m_iShell;
	time_point_t m_NextInspect;
	int iShellOn;
};

}
