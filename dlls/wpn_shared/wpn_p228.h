#pragma once

#ifdef CLIENT_DLL
namespace cl {
#else
namespace sv {
#endif

//p228
#define P228_MAX_SPEED		250
#define P228_DAMAGE		32
#define P228_RANGE_MODIFER	0.8
#define P228_RELOAD_TIME	2.7s
#define P228_INSPECT_TIME		5.17s

class CP228 : public CBasePlayerWeapon
{
	enum p228_e
    {
        P228_IDLE,
        P228_SHOOT1,
        P228_SHOOT2,
        P228_SHOOT3,
        P228_SHOOT_EMPTY,
        P228_RELOAD,
        P228_DRAW
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
	void Inspect() override;
	duration_t GetInspectTime() override { return P228_INSPECT_TIME; }
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
	void P228Fire(float flSpread, duration_t flCycleTime, BOOL fUseSemi);
	NOXREF void MakeBeam();
	NOXREF void BeamUpdate();

public:
	int m_iShell;
	time_point_t m_NextInspect;

private:
	unsigned short m_usFireP228;
};

}
