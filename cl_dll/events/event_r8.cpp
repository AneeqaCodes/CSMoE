/*
*
*    This program is free software; you can redistribute it and/or modify it
*    under the terms of the GNU General Public License as published by the
*    Free Software Foundation; either version 2 of the License, or (at
*    your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but
*    WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    In addition, as a special exception, the author gives permission to
*    link the code of this program with the Half-Life Game Engine ("HL
*    Engine") and Modified Game Libraries ("MODs") developed by Valve,
*    L.L.C ("Valve").  You must obey the GNU General Public License in all
*    respects for all of the code used other than the HL Engine and MODs
*    from Valve.  If you modify this file, you may extend this exception
*    to your version of the file, but you are not obligated to do so.  If
*    you do not wish to do so, delete this exception statement from your
*    version.
*
*/
#include "events.h"

namespace cl::event::r8 {

enum r8_e
{
	IDLE,
	DRAW,
	DRAW_ALT,
	PREPARE,
	UNPREPARE,
	FIRE,
	DRYFIRE,
	ALT1,
	ALT2,
	ALT3,
	LOOKAT01,
	RELOAD,
	LOOKAT01_STICKER,
	LOOKAT01_STICKER_LOOP
};

void EV_FireR8( event_args_t *args )
{
	vec3_t ShellVelocity;
	vec3_t ShellOrigin;
	vec3_t vecSrc;

	int idx = args->entindex;
	Vector origin( args->origin );
	Vector velocity( args->velocity );
	Vector angles(
		args->iparam1 / 100.0f + args->angles[0],
		args->iparam2 / 100.0f + args->angles[1],
		args->angles[2]
	);
	Vector forward, right, up;
	AngleVectors( angles, forward, right, up );

	if ( EV_IsLocal( idx ) )
	{
		++g_iShotsFired;
		EV_MuzzleFlash();
		if(args->bparam2)
			gEngfuncs.pEventAPI->EV_WeaponAnimation(FIRE, 2 );
		else
			gEngfuncs.pEventAPI->EV_WeaponAnimation(Com_RandomLong(ALT1, ALT3), 2);
		if( !gHUD.cl_righthand->value )
		{
			EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 35.0, -11.0, -16.0, 0);
		}
		else
		{
			EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 35.0, -11.0, 16.0, 0);
		}
	}
	else
	{
		EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 20.0, -12.0, 4.0, 0);
	}

	EV_EjectBrass(ShellOrigin, ShellVelocity, angles[ YAW ], g_iPShell, TE_BOUNCE_SHELL);

	PLAY_EVENT_SOUND("weapons/csgo_ports/r8/r8-1.wav");

	EV_GetGunPosition( args, vecSrc, origin );
	Vector vSpread( args->fparam1, args->fparam2, 0.0f );
	
	EV_HLDM_FireBullets( idx,
		forward, right,	up,
		1, vecSrc, forward,
		vSpread, 8192.0, BULLET_PLAYER_50AE,
		2 );
}

}
