#pragma once

#ifdef CLIENT_DLL
namespace cl {
#else
namespace sv {
#endif

//Deagle
#define DEAGLE_MAX_SPEED	250
#define DEAGLE_DAMAGE		54
#define DEAGLE_RANGE_MODIFER	0.81
#define DEAGLE_RELOAD_TIME	2.2s
#define	DEAGLE_INSPECT_TIME 5.7s

class CDEAGLE : public CBasePlayerWeapon
{
	enum deagle_e
    {
        DEAGLE_IDLE1,
        DEAGLE_SHOOT1,
        DEAGLE_SHOOT2,
        DEAGLE_SHOOT_EMPTY,
        DEAGLE_RELOAD,
        DEAGLE_DRAW
    };
public:
	void Spawn() override;
	void Precache() override;
	int GetItemInfo(ItemInfo *p) override;
	BOOL Deploy() override;
	float GetMaxSpeed() override { return m_fMaxSpeed; }
	int iItemSlot() override { return PISTOL_SLOT; }
	void PrimaryAttack() override;
	void SecondaryAttack() override;
	void Reload() override;
	void Inspect() override;
	duration_t GetInspectTime() override { return DEAGLE_INSPECT_TIME; }
	void WeaponIdle() override;
	BOOL UseDecrement() override {
#ifdef CLIENT_WEAPONS
		return TRUE;
#else
		return FALSE;
#endif
	}
	BOOL IsPistol() override { return TRUE; }
	KnockbackData GetKnockBackData() override { return { 350.0f, 250.0f, 350.0f, 100.0f, 0.6f }; }

public:
	void DEAGLEFire(float flSpread, duration_t flCycleTime, BOOL fUseSemi);
	float GetDamage() const;
	int m_iShell;
	time_point_t m_NextInspect;

private:
	unsigned short m_usFireDeagle;
};

}
