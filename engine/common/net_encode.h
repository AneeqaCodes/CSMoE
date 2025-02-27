/*
net_encode.h - delta encode routines
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#ifndef NET_ENCODE_H
#define NET_ENCODE_H

#define DT_BYTE		BIT( 0 )	// A byte
#define DT_SHORT		BIT( 1 ) 	// 2 byte field
#define DT_FLOAT		BIT( 2 )	// A floating point field
#define DT_INTEGER		BIT( 3 )	// 4 byte integer
#define DT_ANGLE		BIT( 4 )	// A floating point angle ( will get masked correctly )
#define DT_TIMEWINDOW_8	BIT( 5 )	// A floating point timestamp, relative to sv.time
#define DT_TIMEWINDOW_BIG	BIT( 6 )	// and re-encoded on the client relative to the client's clock
#define DT_STRING		BIT( 7 )	// A null terminated string, sent as 8 byte chars
#define DT_SIGNED		BIT( 8 )	// sign modificator

#ifdef offsetof
#undef offsetof
#define offsetof( s, m )	(size_t)&(((s *)0)->m)
#endif

#define NUM_FIELDS( x )	((sizeof( x ) / sizeof( x[0] )) - 1)

// helper macroses
#define PP_STRINGIZE(x) #x
#define T_DEF( T, x )   #x, offsetof( T, x ), sizeof( ((T *)0)->x )
#define T_DEF_VEC( T, x, i )   PP_STRINGIZE(x) "[" PP_STRINGIZE(i) "]", offsetof( T, x ) + i * sizeof( ((T *)0)->x[i] ), sizeof( ((T *)0)->x[i] )
#define ENTS_DEF( x )	T_DEF( entity_state_t, x )
#define ENTS_DEF_VEC( x, i )	T_DEF_VEC( entity_state_t, x, i )
#define UCMD_DEF( x )	T_DEF( usercmd_t, x )
#define UCMD_DEF_VEC( x, i )	T_DEF_VEC( usercmd_t, x, i )
#define EVNT_DEF( x )	T_DEF( event_args_t, x )
#define EVNT_DEF_VEC( x, i )	T_DEF_VEC( event_args_t, x, i )
#define PHYS_DEF( x )	T_DEF( movevars_t, x )
#define PHYS_DEF_VEC( x, i )	T_DEF_VEC( movevars_t, x, i )
#define CLDT_DEF( x )	T_DEF( clientdata_t, x )
#define CLDT_DEF_VEC( x, i )	T_DEF_VEC( clientdata_t, x, i )
#define WPDT_DEF( x )	T_DEF( weapon_data_t, x )
#define WPDT_DEF_VEC( x, i )	T_DEF_VEC( weapon_data_t, x, i )

enum
{
	CUSTOM_NONE = 0,
	CUSTOM_SERVER_ENCODE,	// keyword "gamedll"
	CUSTOM_CLIENT_ENCODE,	// keyword "client"
};

// struct info (filled by engine)
typedef struct
{
	const char	*name;
	const size_t		offset;
	const size_t		size;
} delta_field_t;

// one field
typedef struct delta_s
{
	const char	*name;
	int		offset;		// in bytes
	int		size;		// used for bounds checking in DT_STRING
	int		flags;		// DT_INTEGER, DT_FLOAT etc
	float		multiplier;
	float		post_multiplier;	// for DEFINE_DELTA_POST
	int		bits;		// how many bits we send\receive
	qboolean		bInactive;	// unsetted by user request
} delta_t;

typedef void (*pfnDeltaEncode)( delta_t *pFields, const byte *from, const byte *to );

typedef struct
{
	const char	*pName;
	const delta_field_t	*pInfo;
	const int		maxFields;	// maximum number of fields in struct
	int		numFields;	// may be merged during initialization
	delta_t		*pFields;

	// added these for custom entity encode
	int		customEncode;
	char		funcName[32];
	pfnDeltaEncode	userCallback;
	qboolean		bInitialized;
} delta_info_t;

//
// net_encode.c
//
void Delta_Init( void );
void Delta_InitClient( void );
void Delta_Shutdown( void );
void Delta_InitFields( void );
int Delta_NumTables( void );
delta_info_t *Delta_FindStructByIndex( int index );
void Delta_AddEncoder( const char *name, pfnDeltaEncode encodeFunc );
int Delta_FindField( delta_t *pFields, const char *fieldname );
void Delta_SetField( delta_t *pFields, const char *fieldname );
void Delta_UnsetField( delta_t *pFields, const char *fieldname );
void Delta_SetFieldByIndex( struct delta_s *pFields, int fieldNumber );
void Delta_UnsetFieldByIndex( struct delta_s *pFields, int fieldNumber );

// send table over network
void Delta_WriteTableField( sizebuf_t *msg, int tableIndex, const delta_t *pField );
void Delta_ParseTableField( sizebuf_t *msg );


// encode routines
void MSG_WriteDeltaUsercmd( sizebuf_t *msg, struct usercmd_s *from, struct usercmd_s *to );
void MSG_ReadDeltaUsercmd( sizebuf_t *msg, struct usercmd_s *from, struct usercmd_s *to );
void MSG_WriteDeltaEvent( sizebuf_t *msg, struct event_args_s *from, struct event_args_s *to );
void MSG_ReadDeltaEvent( sizebuf_t *msg, struct event_args_s *from, struct event_args_s *to );
qboolean MSG_WriteDeltaMovevars( sizebuf_t *msg, struct movevars_s *from, struct movevars_s *to );
void MSG_ReadDeltaMovevars( sizebuf_t *msg, struct movevars_s *from, struct movevars_s *to );
void MSG_WriteClientData( sizebuf_t *msg, struct clientdata_s *from, struct clientdata_s *to, float timebase );
void MSG_ReadClientData( sizebuf_t *msg, struct clientdata_s *from, struct clientdata_s *to, float timebase );
void MSG_WriteWeaponData( sizebuf_t *msg, struct weapon_data_s *from, struct weapon_data_s *to, float timebase, int index );
void MSG_ReadWeaponData( sizebuf_t *msg, struct weapon_data_s *from, struct weapon_data_s *to, float timebase );
void MSG_WriteDeltaEntity( struct entity_state_s *from, struct entity_state_s *to, sizebuf_t *msg, qboolean force, qboolean player, float timebase );
qboolean MSG_ReadDeltaEntity( sizebuf_t *msg, struct entity_state_s *from, struct entity_state_s *to, int num, qboolean player, float timebase );

#endif//NET_ENCODE_H
