#pragma once

#ifdef CLIENT_DLL
namespace cl {
#else
namespace sv {
#endif

//xm1014
#define XM1014_MAX_SPEED	220
#define XM1014_CONE_VECTOR	Vector(0.0725, 0.0725, 0.0)	// special shotgun spreads
#define	GATLING_DEFAULT_GIVE 40
#define	GATLING_MAX_CLIP 40

class CGatling : public CBasePlayerWeapon
{
	enum gatling_e
    {
        GATLING_IDLE,
        GATLING_FIRE1,
        GATLING_FIRE2,
        GATLING_RELOAD,
        GATLING_DRAW
    };
public:
	void Spawn() override;
	void Precache() override;
	int GetItemInfo(ItemInfo *p) override;
	BOOL Deploy() override;
	float GetMaxSpeed() override { return XM1014_MAX_SPEED; }
	int iItemSlot() override { return PRIMARY_WEAPON_SLOT; }
	void PrimaryAttack() override;
	int GetPrimaryAttackDamage() const;
	void Reload() override;
	void WeaponIdle() override;
	BOOL UseDecrement() override {
#ifdef CLIENT_WEAPONS
		return TRUE;
#else
		return FALSE;
#endif
	}
	KnockbackData GetKnockBackData() override { return { 750.0f, 550.0f, 650.0f, 500.0f, 0.4f }; }
	const char *GetCSModelName() override { return "models/w_gatling.mdl"; }
#ifndef CLIENT_DLL
	WeaponBuyAmmoConfig GetBuyAmmoConfig() override { return { "ammo_12gauge" , 65 }; }
#endif

public:
	int m_iShell;

private:
	unsigned short m_usFireGatling;
};

}
