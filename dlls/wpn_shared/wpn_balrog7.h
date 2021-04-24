#pragma once

#ifdef CLIENT_DLL
namespace cl {
#else
namespace sv {
#endif

//Balrog7
#define M249_MAX_SPEED            220
#define M249_DAMAGE            32
#define M249_RANGE_MODIFER        0.97
#define M249_RELOAD_TIME        4.7
#define BALROG7_MAX_CLIP            120
#define	BALROG7_DEFAULT_GIVE		120


class CBalrog7 : public CBasePlayerWeapon
{
	enum balrog7_e
    {
        BALROG7_IDLE1,
        BALROG7_SHOOT1,
        BALROG7_SHOOT2,
        BALROG7_SHOOT3,
        BALROG7_RELOAD,
        BALROG7_DRAW
    };

public:
	void Spawn() override;
	void Precache() override;
	int GetItemInfo(ItemInfo *p) override;
	void SecondaryAttack() override;
	void ItemPostFrame() override;
	BOOL Deploy() override;
	float GetMaxSpeed() override;
	int iItemSlot() override { return PRIMARY_WEAPON_SLOT; }
	void PrimaryAttack() override;
	void Reload() override;
	void WeaponIdle() override;
	bool HasSecondaryAttack() override { return true; }
	
	BOOL UseDecrement() override
	{
#ifdef CLIENT_WEAPONS
		return TRUE;
#else
		return FALSE;
#endif
	}
	KnockbackData GetKnockBackData() override { return {350.0f, 250.0f, 300.0f, 100.0f, 0.6f}; }
	const char *GetCSModelName() override { return "models/w_balrog7.mdl"; }

public:
	void Balrog7Fire(float flSpread, duration_t flCycleTime, BOOL fUseAutoAim);
	float GetDamage();
	float BalrogDamage();
	void RadiusDamage(Vector vecAiming, float flDamage);
	Vector Get_ShootPosition(CBaseEntity *pevAttacker, Vector vecSrc,Vector vecDir);
	int m_iShell;
	int iShellOn;
	int m_iModelExplo;
	int m_iBalrog7Explo;

private:
	unsigned short m_usFireBalrog7;
};

}
