#pragma once

// Copyright (C) 1999-2000 Id Software, Inc.
//
#ifndef __TR_TYPES_H
#define __TR_TYPES_H


#define	MAX_DLIGHTS		32			// can't be increased, because bit flags are used on surfaces
//#define	MAX_ENTITIES	1023		// can't be increased without changing drawsurf bit packing

#define	REFENTITYNUM_BITS	16		// can't be increased without changing drawsurf bit packing
#define	REFENTITYNUM_MASK	((1<<REFENTITYNUM_BITS) - 1)
// the last N-bit number (2^REFENTITYNUM_BITS - 1) is reserved for the special world refentity,
//  and this is reflected by the value of MAX_REFENTITIES (which therefore is not a power-of-2)
#define	MAX_REFENTITIES		((1<<REFENTITYNUM_BITS) - 1)
#define	REFENTITYNUM_WORLD	((1<<REFENTITYNUM_BITS) - 1)

#define	MAX_MINI_ENTITIES	1024		

// renderfx flags
#define	RF_MINLIGHT			0x00001	// allways have some light (viewmodel, some items)
#define	RF_THIRD_PERSON		0x00002	// don't draw through eyes, only mirrors (player bodies, chat sprites)
#define	RF_FIRST_PERSON		0x00004	// only draw through eyes (view weapon, damage blood blob)
#define	RF_DEPTHHACK		0x00008	// for view weapon Z crunching
#define RF_NODEPTH			0x00010	// No depth at all (seeing through walls)

#define RF_VOLUMETRIC		0x00020	// fake volumetric shading

#define	RF_NOSHADOW			0x00040	// don't add stencil shadows

#define RF_LIGHTING_ORIGIN	0x00080	// use refEntity->lightingOrigin instead of refEntity->origin
									// for lighting.  This allows entities to sink into the floor
									// with their origin going solid, and allows all parts of a
									// player to get the same lighting
#define	RF_SHADOW_PLANE		0x00100	// use refEntity->shadowPlane
#define	RF_WRAP_FRAMES		0x00200	// mod the model frames by the maxframes to allow continuous
										// animation without needing to know the frame count

#define RF_FORCE_ENT_ALPHA	0x00400 // override shader alpha settings
#define RF_RGB_TINT			0x00800 // override shader rgb settings

#define RF_FORKED			0x04000	// override lightning to have forks
#define RF_TAPERED			0x08000	// lightning tapers
#define RF_GROW				0x10000	// lightning grows from start to end during its life

#define RF_DISINTEGRATE1	0x20000	// does a procedural hole-ripping thing.
#define RF_DISINTEGRATE2	0x40000	// does a procedural hole-ripping thing with scaling at the ripping point

#define RF_SABERGLOWENDS	0x80000	// hack: draws the shader at top and bottom of saber, and bigger than usual


// refdef flags
#define RDF_NOWORLDMODEL	1		// used for player configuration screen
#define RDF_HYPERSPACE		4		// teleportation effect

typedef byte color4ub_t[4];

typedef struct {
	vec3_t		xyz;
	float		st[2];
	byte		modulate[4];
} polyVert_t;

typedef struct poly_s {
	qhandle_t			hShader;
	int					numVerts;
	polyVert_t			*verts;
} poly_t;

typedef enum {
	RT_MODEL,
	RT_POLY,
	RT_SPRITE,
	RT_ORIENTED_QUAD,
	RT_BEAM,
	RT_SABER_GLOW,
	RT_ELECTRICITY,
	RT_PORTALSURFACE,		// doesn't draw anything, just info for portals
	RT_LINE,
	RT_ORIENTEDLINE,
	RT_CYLINDER,
	RT_ENT_CHAIN,

	RT_MAX_REF_ENTITY_TYPE
} refEntityType_t;

typedef struct miniRefEntity_s 
{
	refEntityType_t		reType;
	int					renderfx;

	qhandle_t			hModel;				// opaque type outside refresh

	// most recent data
	vec3_t				axis[3];			// rotation vectors
	qboolean			nonNormalizedAxes;	// axis are not normalized, i.e. they have scale
	vec3_t				origin;				// also used as MODEL_BEAM's "from"

	// previous data for frame interpolation
	vec3_t				oldorigin;			// also used as MODEL_BEAM's "to"

	// texturing
	qhandle_t			customShader;		// use one image for the entire thing

	// misc
	byte				shaderRGBA[4];		// colors used by rgbgen entity shaders
	vec2_t				shaderTexCoord;		// texture coordinates used by tcMod entity modifiers

	// extra sprite information
	float				radius;
	float				rotation;			// size 2 for RT_CYLINDER or number of verts in RT_ELECTRICITY

	// misc
	double		shaderTime;			// subtracted from refdef time to control effect start times
	int			frame;				// also used as MODEL_BEAM's diameter

} miniRefEntity_t;

#pragma warning (disable : 4201 )
typedef struct {
	// this stucture must remain identical as the miniRefEntity_t
	//
	//
	refEntityType_t		reType;
	int					renderfx;

	qhandle_t			hModel;				// opaque type outside refresh

	// most recent data
	vec3_t				axis[3];			// rotation vectors
	qboolean			nonNormalizedAxes;	// axis are not normalized, i.e. they have scale
	vec3_t				origin;				// also used as MODEL_BEAM's "from"

	// previous data for frame interpolation
	vec3_t				oldorigin;			// also used as MODEL_BEAM's "to"

	// texturing
	qhandle_t			customShader;		// use one image for the entire thing

	// misc
	byte				shaderRGBA[4];		// colors used by rgbgen entity shaders
	vec2_t				shaderTexCoord;		// texture coordinates used by tcMod entity modifiers

	// extra sprite information
	float				radius;
	float				rotation;

	// misc
	float		shaderTime;			// subtracted from refdef time to control effect start times
	int			frame;				// also used as MODEL_BEAM's diameter
	//
	//
	// end miniRefEntity_t

	//
	//
	// specific full refEntity_t data
	//
	//

	// most recent data
	vec3_t		lightingOrigin;		// so multi-part models can be lit identically (RF_LIGHTING_ORIGIN)
	float		shadowPlane;		// projection shadows go here, stencils go slightly lower

	// previous data for frame interpolation
	int			oldframe;
	float		backlerp;			// 0.0 = current, 1.0 = old

	// texturing
	int			skinNum;			// inline skin index
	qhandle_t	customSkin;			// NULL for default skin

	// texturing
	union	
	{
//		int			skinNum;		// inline skin index
//		ivec3_t		terxelCoords;	// coords of patch for RT_TERXELS	
		struct
		{
			int		miniStart;
			int		miniCount;
		} uMini;
	} uRefEnt;

	// extra sprite information
	union {
		struct 
		{
			float rotation;
			float radius;
			byte  vertRGBA[4][4];
		} sprite;
		struct 
		{
			float width;
			float width2;
			float stscale;
		} line;
		struct	// that whole put-the-opening-brace-on-the-same-line-as-the-beginning-of-the-definition coding style is fecal
		{
			float	width;
			vec3_t	control1;
			vec3_t	control2;
		} bezier;
		struct
		{
			float width;
			float width2;
			float stscale;
			float height;
			float bias;
			qboolean wrap;
		} cylinder;
		struct 
		{
			float width;
			float deviation;
			float stscale;
			qboolean wrap;
			qboolean taper;
		} electricity;
	} data;

	float		endTime;
	float		saberLength;

/*
Ghoul2 Insert Start
*/
	vec3_t		angles;				// rotation angles - used for Ghoul2

	vec3_t		modelScale;			// axis scale for models
//	CGhoul2Info_v	*ghoul2;  		// has to be at the end of the ref-ent in order for it to be created properly
	void		*ghoul2;  		// has to be at the end of the ref-ent in order for it to be created properly
/*
Ghoul2 Insert End
*/
} refEntity_t;


#define	MAX_RENDER_STRINGS			8
#define	MAX_RENDER_STRING_LENGTH	32

typedef struct {
	int			x, y, width, height;
	float		fov_x, fov_y;
	vec3_t		vieworg;
	vec3_t		viewaxis[3];		// transformation matrix

	// time in milliseconds for shader effects and other time dependent rendering issues
	int			time;

	int			rdflags;			// RDF_NOWORLDMODEL, etc

	// 1 bits will prevent the associated area from rendering at all
	byte		areamask[MAX_MAP_AREA_BYTES];

	// text messages for deform text shaders
	char		text[MAX_RENDER_STRINGS][MAX_RENDER_STRING_LENGTH];
} refdef_t;


typedef enum {
	STEREO_CENTER,
	STEREO_LEFT,
	STEREO_RIGHT
} stereoFrame_t;


/*
** glconfig_t
**
** Contains variables specific to the OpenGL configuration
** being run right now.  These are constant once the OpenGL
** subsystem is initialized.
*/
typedef enum {
	TC_NONE,
	TC_S3TC,
	TC_S3TC_DXT
} textureCompression_t;

typedef struct {
	char					renderer_string[MAX_STRING_CHARS];
	char					vendor_string[MAX_STRING_CHARS];
	char					version_string[MAX_STRING_CHARS];
	char					extensions_string[BIG_INFO_STRING];

	int						maxTextureSize;			// queried from GL
	int						maxActiveTextures;		// multitexture ability

	int						colorBits, depthBits, stencilBits;

	qboolean				deviceSupportsGamma;
	textureCompression_t	textureCompression;
	qboolean				textureEnvAddAvailable;
	qboolean				textureFilterAnisotropicAvailable;
	qboolean				clampToEdgeAvailable;

	int						vidWidth, vidHeight;
	// aspect is the screen's physical width / height, which may be different
	// than scrWidth / scrHeight if the pixels are non-square
	// normal screens should be 4/3, but wide aspect monitors may be 16/9
	float					windowAspect;

	int						displayFrequency;

	// synonymous with "does rendering consume the entire screen?", therefore
	// a Voodoo or Voodoo2 will have this set to TRUE, as will a Win32 ICD that
	// used CDS.
	qboolean				isFullscreen;
	qboolean				stereoEnabled;
	qboolean				smpActive;		// dual processor
} glconfig_t;


#if !defined _WIN32

#ifdef __linux__
#define OPENGL_DRIVER_NAME	"libGL.so.1"
#else
#define OPENGL_DRIVER_NAME	"libGL.so"
#endif

#else

#define OPENGL_DRIVER_NAME	"opengl32"

#endif	// !defined _WIN32


//MME
typedef struct {
	int			maxAnisotropy;
	int			multiSamples;
	int			glWidth;
	int			glHeight;
	qboolean	framebufferObject;
	qboolean	framebufferMultiSample;
	qboolean	shaderSupport;
} glMMEConfig_t;

typedef enum {
	mmeShotFormatTGA,
	mmeShotFormatJPG,
	mmeShotFormatPNG,
	mmeShotFormatAVI,
} mmeShotFormat_t;

typedef enum {
	mmeShotTypeRGB,
	mmeShotTypeRGBA,
	mmeShotTypeGray,
} mmeShotType_t;


#endif	// __TR_TYPES_H
