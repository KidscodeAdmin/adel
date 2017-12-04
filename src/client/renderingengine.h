/*
Minetest
Copyright (C) 2010-2013 celeron55, Perttu Ahola <celeron55@gmail.com>
Copyright (C) 2017 nerzhul, Loic Blot <loic.blot@unix-experience.fr>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#pragma once

#include <vector>
#include <string>
#include "irrlichttypes_extrabloated.h"
#include "debug.h"

class ITextureSource;
class Camera;
class Client;
class LocalPlayer;
class Hud;
class Minimap;
class Sky;
class Clouds;

class RenderingEngine
{
public:
	RenderingEngine(IEventReceiver *eventReceiver);
	~RenderingEngine();

	v2u32 getWindowSize() const;
	void setResizable(bool resize);

	video::IVideoDriver *getVideoDriver();

	static const char *getVideoDriverName(irr::video::E_DRIVER_TYPE type);
	static const char *getVideoDriverFriendlyName(irr::video::E_DRIVER_TYPE type);
	static float getDisplayDensity();
	static v2u32 getDisplaySize();

	static void setXorgClassHint(const video::SExposedVideoData &video_data,
			const std::string &name);
	bool setWindowIcon();
	bool setXorgWindowIconFromPath(const std::string &icon_file);
	static bool print_video_modes();

	static RenderingEngine *get_instance() { return s_singleton; }

	static io::IFileSystem *get_filesystem()
	{
		sanity_check(s_singleton && s_singleton->m_device);
		return s_singleton->m_device->getFileSystem();
	}

	static video::IVideoDriver *get_video_driver()
	{
		sanity_check(s_singleton && s_singleton->m_device);
		return s_singleton->m_device->getVideoDriver();
	}

	static scene::IMeshCache *get_mesh_cache()
	{
		sanity_check(s_singleton && s_singleton->m_device);
		return s_singleton->m_device->getSceneManager()->getMeshCache();
	}

	static scene::ISceneManager *get_scene_manager()
	{
		sanity_check(s_singleton && s_singleton->m_device);
		return s_singleton->m_device->getSceneManager();
	}

	static irr::IrrlichtDevice *get_raw_device()
	{
		sanity_check(s_singleton && s_singleton->m_device);
		return s_singleton->m_device;
	}

	static u32 get_timer_time()
	{
		sanity_check(s_singleton && s_singleton->m_device &&
				s_singleton->m_device->getTimer());
		return s_singleton->m_device->getTimer()->getTime();
	}

	static gui::IGUIEnvironment *get_gui_env()
	{
		sanity_check(s_singleton && s_singleton->m_device);
		return s_singleton->m_device->getGUIEnvironment();
	}

	inline static void draw_load_screen(const std::wstring &text,
			gui::IGUIEnvironment *guienv, ITextureSource *tsrc,
			float dtime = 0, int percent = 0, bool clouds = true)
	{
		s_singleton->_draw_load_screen(
				text, guienv, tsrc, dtime, percent, clouds);
	}

	inline static void draw_scene(Camera *camera, Client *client, LocalPlayer *player,
			Hud *hud, Minimap *mapper, gui::IGUIEnvironment *guienv,
			const v2u32 &screensize, Sky &sky, Clouds &clouds,//const video::SColor &skycolor,
			bool show_hud, bool show_minimap)
	{
		s_singleton->_draw_scene(camera, client, player, hud, mapper, guienv,
				screensize, sky, clouds,//skycolor, 
				show_hud, show_minimap);
	}

	static bool run()
	{
		sanity_check(s_singleton && s_singleton->m_device);
		return s_singleton->m_device->run();
	}

	static std::vector<core::vector3d<u32>> getSupportedVideoModes();
	static std::vector<irr::video::E_DRIVER_TYPE> getSupportedVideoDrivers();

private:
	enum parallax_sign
	{
		LEFT = -1,
		RIGHT = 1,
		EYECOUNT = 2
	};

	void _draw_load_screen(const std::wstring &text, gui::IGUIEnvironment *guienv,
			ITextureSource *tsrc, float dtime = 0, int percent = 0,
			bool clouds = true);

	void _draw_scene(Camera *camera, Client *client, LocalPlayer *player, Hud *hud,
			Minimap *mapper, gui::IGUIEnvironment *guienv,
			const v2u32 &screensize, Sky &sky, Clouds &clouds,//const video::SColor &skycolor,
			bool show_hud, bool show_minimap);

	void draw_anaglyph_3d_mode(Camera *camera, bool show_hud, Hud *hud,
			bool draw_wield_tool, Client *client,
			gui::IGUIEnvironment *guienv);

	void draw_interlaced_3d_mode(Camera *camera, bool show_hud, Hud *hud,
			const v2u32 &screensize, bool draw_wield_tool, Client *client,
			gui::IGUIEnvironment *guienv, const video::SColor &skycolor);

	void draw_sidebyside_3d_mode(Camera *camera, bool show_hud, Hud *hud,
			const v2u32 &screensize, bool draw_wield_tool, Client *client,
			gui::IGUIEnvironment *guienv, const video::SColor &skycolor);

	void draw_top_bottom_3d_mode(Camera *camera, bool show_hud, Hud *hud,
			const v2u32 &screensize, bool draw_wield_tool, Client *client,
			gui::IGUIEnvironment *guienv, const video::SColor &skycolor);

	void draw_pageflip_3d_mode(Camera *camera, bool show_hud, Hud *hud,
			const v2u32 &screensize, bool draw_wield_tool, Client *client,
			gui::IGUIEnvironment *guienv, const video::SColor &skycolor);

	void draw_plain(Camera *camera, bool show_hud, Hud *hud, const v2u32 &screensize,
			bool draw_wield_tool, Client *client,
			gui::IGUIEnvironment *guienv, Sky &sky, Clouds &clouds);//const video::SColor &skycolor);

	void init_texture(const v2u32 &screensize, video::ITexture **texture,
			const char *name);

	video::ITexture *draw_image(const v2u32 &screensize, parallax_sign psign,
			const irr::core::matrix4 &startMatrix,
			const irr::core::vector3df &focusPoint, bool show_hud,
			Camera *camera, Hud *hud, bool draw_wield_tool, Client *client,
			gui::IGUIEnvironment *guienv, const video::SColor &skycolor);

	video::ITexture *draw_hud(const v2u32 &screensize, bool show_hud, Hud *hud,
			Client *client, bool draw_crosshair,
			const video::SColor &skycolor, gui::IGUIEnvironment *guienv,
			Camera *camera);

	irr::IrrlichtDevice *m_device = nullptr;
	static RenderingEngine *s_singleton;
};

// MALEK
// ---
#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_OPENGL_

#if defined(_IRR_OPENGL_USE_EXTPOINTER_)
#define GL_GLEXT_LEGACY 1
#else
#define GL_GLEXT_PROTOTYPES 1
#endif
#ifdef _IRR_WINDOWS_API_
// include windows headers for HWND
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#define GL_GLEXT_LEGACY 1
#include <GL/gl.h>
#include "glext.h"
#ifdef _MSC_VER
#pragma comment(lib, "OpenGL32.lib")
#endif
#elif defined(_IRR_COMPILE_WITH_X11_DEVICE_)
#define GL_GLEXT_LEGACY 1
#define GLX_GLEXT_LEGACY 1
#include <GL/gl.h>
#include "glext.h"
#include <GL/glx.h>
#elif defined(_IRR_OSX_PLATFORM_)
#include <OpenGL/gl.h>
#elif defined(_IRR_COMPILE_WITH_SDL_DEVICE_)
#define NO_SDL_GLEXT
#include <SDL/SDL_video.h>
#include <SDL/SDL_opengl.h>
#else
#if defined(_IRR_OSX_PLATFORM_)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#endif

#if !defined(_IRR_OSX_PLATFORM_)
#ifdef _IRR_OPENGL_USE_EXTPOINTER_
#ifdef _IRR_WINDOWS_API_
#define IRR_OGL_LOAD_EXTENSION(x) wglGetProcAddress(reinterpret_cast<const char*>(x))
#elif defined(_IRR_COMPILE_WITH_SDL_DEVICE_) && !defined(_IRR_COMPILE_WITH_X11_DEVICE_)
#define IRR_OGL_LOAD_EXTENSION(x) SDL_GL_GetProcAddress(reinterpret_cast<const char*>(x))
#else
// Accessing the correct function is quite complex
// All libraries should support the ARB version, however
// since GLX 1.4 the non-ARB version is the official one
// So we have to check the runtime environment and
// choose the proper symbol
// In case you still have problems please enable the
// next line by uncommenting it
#define _IRR_GETPROCADDRESS_WORKAROUND_

#ifndef _IRR_GETPROCADDRESS_WORKAROUND_
__GLXextFuncPtr(*IRR_OGL_LOAD_EXTENSION_FUNCP)(const GLubyte*) = 0;
#ifdef GLX_VERSION_1_4
int major = 0, minor = 0;
if (glXGetCurrentDisplay())
glXQueryVersion(glXGetCurrentDisplay(), &major, &minor);
if ((major>1) || (minor>3))
IRR_OGL_LOAD_EXTENSION_FUNCP = glXGetProcAddress;
else
#endif
IRR_OGL_LOAD_EXTENSION_FUNCP = glXGetProcAddressARB;
#define IRR_OGL_LOAD_EXTENSION(X) IRR_OGL_LOAD_EXTENSION_FUNCP(reinterpret_cast<const GLubyte*>(X))
#else
#define IRR_OGL_LOAD_EXTENSION(X) glXGetProcAddressARB(reinterpret_cast<const GLubyte*>(X))
#endif // workaround
#endif // Windows, SDL, or Linux
#endif
#endif

//! OpenGL FBO Render Target.
class COpenGLRenderSurface : public irr::video::ITexture
{
public:

	//! FrameBufferObject constructor
	COpenGLRenderSurface(const core::dimension2d<u32>& size, const io::path& name, const bool isDepth = false,
		irr::video::ECOLOR_FORMAT format = irr::video::ECF_UNKNOWN);

	//! destructor
	virtual ~COpenGLRenderSurface();

	//! lock function
	virtual void* lock(irr::video::E_TEXTURE_LOCK_MODE mode = irr::video::ETLM_READ_WRITE, u32 mipmapLevel = 0) { return NULL; }

	//! unlock function
	virtual void unlock() {}

	//! Returns original size of the texture (image).
	virtual const core::dimension2d<u32>& getOriginalSize() const { return ImageSize; }

	//! Returns size of the texture.
	virtual const core::dimension2d<u32>& getSize() const { return TextureSize; }

	//! returns driver type of texture (=the driver, that created it)
	virtual irr::video::E_DRIVER_TYPE getDriverType() const { return irr::video::EDT_OPENGL; }

	//! returns color format of texture
	virtual irr::video::ECOLOR_FORMAT getColorFormat() const { return ColorFormat; }

	//! returns pitch of texture (in bytes)
	virtual u32 getPitch() const { return 0; }

	//! return open gl texture name
	GLuint getOpenGLTextureName() const { return TextureName; }

	//! return whether this texture has mipmaps
	virtual bool hasMipMaps() const { return false; }

	//! Regenerates the mip map levels of the texture.
	/** Useful after locking and modifying the texture
	\param mipmapData Pointer to raw mipmap data, including all necessary mip levels, in the same format as the main texture image. If not set the mipmaps are derived from the main image. */
	virtual void regenerateMipMapLevels(void* mipmapData = 0) {}

	//! Is it a render target?
	virtual bool isRenderTarget() const { return true; }

	//! Is it a FrameBufferObject?
	virtual bool isFrameBufferObject() const { return true; }

	// warning: this structure MUST be binary compatible with COpenGLTexture!
	core::dimension2d<u32> ImageSize;
	core::dimension2d<u32> TextureSize;
	irr::video::ECOLOR_FORMAT ColorFormat;
	irr::video::IVideoDriver* Driver;
	irr::video::IImage* Image;
	irr::video::IImage* MipImage;

	GLuint TextureName;
	GLint InternalFormat;
	GLenum PixelFormat;
	GLenum PixelType;

	u8 MipLevelStored;
	bool HasMipMaps;
	bool MipmapLegacyMode;
	bool IsRenderTarget;
	bool AutomaticMipmapUpdate;
	bool ReadOnlyLock;
	bool KeepImage;

protected:
	//! protected constructor with basic setup, no GL texture name created, for derived classes
	COpenGLRenderSurface(const io::path& name) : ITexture(name),
		ColorFormat(irr::video::ECF_A8R8G8B8),
		TextureName(0), InternalFormat(GL_RGBA8), PixelFormat(GL_BGRA_EXT),
		PixelType(GL_UNSIGNED_BYTE) {
	}
};
class COpenGLRenderTarget
{
public:
	//! FrameBufferObject constructor
	COpenGLRenderTarget(const core::dimension2d<u32>& size, const io::path& name, const bool withDepth = false,
		irr::video::IVideoDriver* driver = 0, irr::video::ECOLOR_FORMAT format = irr::video::ECF_UNKNOWN);

	//! destructor
	virtual ~COpenGLRenderTarget();

	void bindRTT(bool clearColor = false, bool clearDepth = false, video::SColor = 0);
	void unbindRTT(const v2u32 &screensize);

	COpenGLRenderSurface* ColorTexture;
	COpenGLRenderSurface* DepthTexture;

protected:
	irr::video::IVideoDriver* Driver;
	GLuint ColorFrameBuffer;
};
#endif 
// END MALEK
