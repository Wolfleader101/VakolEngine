#pragma once

#include <glm/vec3.hpp>

// https://github.com/id-Software/Quake-2/blob/master/qcommon/qfiles.h
// http://tfc.duke.free.fr/coding/md2-specs-en.html

/* Textures of MD2 Files */
struct pcx_t
{
	char			manufacturer;
	char			version;
	char			encoding;
	char			bits_per_pixel;
	unsigned short	xmin, ymin, xmax, ymax;
	unsigned short	hres, vres;
	unsigned char	pallete[48];
	char			reserved;
	char			color_planes;
	unsigned short	bytes_per_line;
	unsigned short	pallete_type;
	char			filler[58];
	unsigned char	data;
};

constexpr auto ID_ALIAS_HEADER = (('2' << 24) + ('P' << 16) + ('D' << 8) + 'I');
constexpr auto ALIAS_VERSION = 8;

constexpr auto MAX_TRIANGLES = 4096;
constexpr auto MAX_VERTICES  = 2048;
constexpr auto MAX_FRAMES	 = 512;
constexpr auto MAX_SKINS	 = 32;
constexpr auto MAX_SKIN_NAME = 64;

/* MD2 header */
struct md2_header_t
{
	int identity;               /* magic number: "IDP2" */
	int version;				/* version: must be 8 */

	int skin_width;             /* texture width */
	int skin_height;            /* texture height */

	int frame_size;             /* size in bytes of a frame */

	int num_skins;              /* number of skins */
	int num_vertices;           /* number of vertices per frame */
	int num_uv;                 /* number of texture coordinates */
	int num_tris;               /* number of triangles */
	int num_glcmds;             /* number of opengl commands */
	int num_frames;             /* number of frames */

	int offset_skins;           /* offset skin data */
	int offset_uv;              /* offset texture coordinate data */
	int offset_tris;            /* offset triangle data */
	int offset_frames;          /* offset frame data */
	int offset_glcmds;          /* offset OpenGL command data */
	int offset_end;             /* offset end of file */
};

/* Texture name */
struct md2_skin_t
{
	char name[MAX_SKIN_NAME];		/* texture file name */
};

/* Texture coords */
struct md2_texCoord_t
{
	short u;
	short v;
};

/* Triangle info */
struct md2_triangle_t
{
	unsigned short vertex[3];   /* vertex indices of the triangle */
	unsigned short uv[3];       /* tex. coord. indices */
};

/* Compressed vertex */
struct md2_vertex_t
{
	glm::bvec3 pos;			  /* position */
	glm::bvec3 normal_index;  /* normal vector index */
};

/* Model frame */
struct md2_frame_t
{
	glm::vec3 scale;               /* scale factor */
	glm::vec3 translate;           /* translation vector */
	char name[16];				   /* frame name */
	md2_vertex_t* verts;	   /* list of frame's vertices */
};

struct md2_model_t
{
	md2_header_t header;
};