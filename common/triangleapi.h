/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#if !defined( TRIANGLEAPI_H )
#define TRIANGLEAPI_H
#ifdef _WIN32
#ifndef __MINGW32__
#pragma once
#endif /* not __MINGW32__ */
#endif

typedef enum 
{
	TRI_FRONT = 0,
	TRI_NONE = 1,
} TRICULLSTYLE;

#define TRI_API_VERSION		1

#define TRI_TRIANGLES		0
#define TRI_TRIANGLE_FAN	1
#define TRI_QUADS			2
#define TRI_POLYGON			3
#define TRI_LINES			4	
#define TRI_TRIANGLE_STRIP	5
#define TRI_QUAD_STRIP		6
#define TRI_POINTS			7 // Xash3D added

typedef struct triangleapi_s
{
	int			version;

	void		( *RenderMode )( int mode );
	void		( *Begin )( int primitiveCode );
	void		( *End ) ( void );

	void		( *Color4f ) ( float r, float g, float b, float a );
	void		( *Color4ub ) ( unsigned char r, unsigned char g, unsigned char b, unsigned char a );
	void		( *TexCoord2f ) ( float u, float v );
	void		( *Vertex3fv ) ( const vec3_t worldPnt );
	void		( *Vertex3f ) ( float x, float y, float z );
	void		( *Brightness ) ( float brightness );
	void		( *CullFace ) ( TRICULLSTYLE style );
	int			( *SpriteTexture ) ( struct model_s *pSpriteModel, int frame );
	qboolean	( *WorldToScreen ) ( const vec3_t world, vec3_t_ref screen );  // Returns 1 if it's z clipped
	void		( *Fog ) ( const vec3_t flFogColor, float flStart, float flEnd, int bOn ); //Works just like GL_FOG, flFogColor is r/g/b.
	void		( *ScreenToWorld ) ( const vec3_t screen, vec3_t_ref world  );
	void	(*GetMatrix)( const int pname, float *matrix );
	int		(*BoxInPVS)( const vec3_t mins, const vec3_t maxs );
	void	(*LightAtPoint)( const vec3_t pos, float *value );
	void	(*Color4fRendermode)( float r, float g, float b, float a, int rendermode );
	void	(*FogParams)( float flDensity, int iFogSkybox );


} triangleapi_t;

#endif // TRIANGLEAPI_H
