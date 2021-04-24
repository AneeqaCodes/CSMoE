/*
wpn_cannon.cpp - Black Dragon Cannon from CSO
Copyright (C) 2019 Moemod Hyakuya

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "z4b_cannonplasma.h"
#include "weapons/KnifeAttack.h"

#ifndef CLIENT_DLL
#include "soundent.h"
#include "gamemode/mods.h"
#endif

#ifdef CLIENT_DLL
namespace cl {
#else
namespace sv {
#endif

LINK_ENTITY_TO_CLASS(z4b_cannonplasma, CCannonPlasma)

void CCannonPlasma::Spawn(void)
{
	Base::Spawn();
	m_iClip = WEAPON_NOCLIP;
	m_iDefaultAmmo = ItemInfoData_t::iMaxAmmo1;
}

void CCannonPlasma::Precache(void)
{
	Base::Precache();
	PRECACHE_MODEL("sprites/plasmaroll.spr");

	PRECACHE_SOUND("weapons/cannon-1.wav");

	m_usFire = PRECACHE_EVENT(1, "events/cannonplasma.sc");
}

int CCannonPlasma::ExtractAmmo(CBasePlayerWeapon *pWeapon)
{
	if (m_iDefaultAmmo)
	{
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] = m_iDefaultAmmo;
		m_iClip = WEAPON_NOCLIP;
		m_iDefaultAmmo = 0;
		return TRUE;
	}
	return CBasePlayerWeapon::ExtractAmmo(pWeapon);
}

void CCannonPlasma::PrimaryAttack(void)
{
	CannonPlasmaFire(0, 3.5s, FALSE);
}

void CCannonPlasma::CannonPlasmaFire(float flSpread, duration_t flCycleTime, BOOL fUseAutoAim)
{
	m_bDelayFire = true;

	if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
	{
		if (m_fFireOnEmpty)
		{
			PlayEmptySound();
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.2s;
		}

		return;
	}

	m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;

	m_pPlayer->pev->effects |= EF_MUZZLEFLASH;
#ifndef CLIENT_DLL
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);
#endif
	UTIL_MakeVectors(m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle);
	Vector vecSrc = m_pPlayer->GetGunPosition();

#ifndef CLIENT_DLL
	Vector vecDir = KnifeAttack2(vecSrc, gpGlobals->v_forward, GetDamage(), 405.0f, 45.0f, DMG_BULLET, this->pev, m_pPlayer->pev);
#else
	Vector vecDir = gpGlobals->v_forward;
#endif

	int flags;
#ifdef CLIENT_WEAPONS
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	PLAYBACK_EVENT_FULL(flags, m_pPlayer->edict(), m_usFire, 0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, (int)(m_pPlayer->pev->punchangle.x * 100), (int)(m_pPlayer->pev->punchangle.y * 100), FALSE, FALSE);

	// "sprites/black_smoke4.spr"

	m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = BRIGHT_GUN_FLASH;
#ifndef CLIENT_DLL
	m_pPlayer->m_iExtraSoundTypes = bits_SOUND_DANGER;
#endif
	m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + flCycleTime;

#ifndef CLIENT_DLL
	if (!m_iClip)
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
#endif
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 3.5s;

	if (m_pPlayer->pev->velocity.Length2D() > 0)
		KickBack(13.0, 2.25, 1.45, 0.7, 12.0, 10.0, 1);
	else if (!FBitSet(m_pPlayer->pev->flags, FL_ONGROUND))
		KickBack(13.0, 5.0, 1.85, 0.55, 15.0, 5.7, 2);
	else if (FBitSet(m_pPlayer->pev->flags, FL_DUCKING))
		KickBack(9.0, 2.1, 1.25, 0.5, 15.0, 5.5, 1);
	else
		KickBack(13.0, 3.2, 1.5, 0.5, 15.0, 10.0, 2);
}

float CCannonPlasma::GetDamage()
{
#ifndef CLIENT_DLL
	if (g_pModRunning->DamageTrack() == DT_ZB)
		return 1150;
	else if (g_pModRunning->DamageTrack() == DT_ZBS)
		return 2950;
	else if (g_pModRunning->DamageTrack() == DT_ZSE)
		return 650;
#endif
	return 410;
}

}
