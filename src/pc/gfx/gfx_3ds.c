#ifdef TARGET_N3DS

#include <3ds.h>
#include <citro3d.h>
#include "gfx_3ds.h"

#ifdef N3DS_USE_ANTIALIASING

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_XY))

#else

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NONE))

#endif

static C3D_RenderTarget* sTarget;

static bool checkN3DS()
{
    bool isNew3DS = false;

	if (R_SUCCEEDED(APT_CheckNew3DS(&isNew3DS)))
		return isNew3DS;

	return false;
}

static void gfx_3ds_init(void) 
{
    if (checkN3DS())
		osSetSpeedupEnable(true);

    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
#ifdef N3DS_USE_ANTIALIASING
    sTarget = C3D_RenderTargetCreate(240*2, 400*2, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
#else
    sTarget = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
#endif
	C3D_RenderTargetSetOutput(sTarget, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
}

static void gfx_3ds_main_loop(void (*run_one_game_iter)(void)) 
{
    while (aptMainLoop())
        run_one_game_iter();

    ndspExit();
    C3D_Fini();
	gfxExit();
}

static void gfx_3ds_get_dimensions(uint32_t *width, uint32_t *height) 
{
    *width = 400;
    *height = 240;
}

static void gfx_3ds_handle_events(void) 
{
    
}

static bool gfx_3ds_start_frame(void)
{
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C3D_RenderTargetClear(sTarget, C3D_CLEAR_ALL, 0x000000FF, 0xFFFFFFFF);
	C3D_FrameDrawOn(sTarget);
    return true;
}

static void gfx_3ds_swap_buffers_begin(void) 
{
    C3D_FrameEnd(0);
    if(C3D_GetProcessingTime() < 1000.0f / 60.f)
        gspWaitForVBlank();
}

static void gfx_3ds_swap_buffers_end(void) 
{
}

static double gfx_3ds_get_time(void) 
{
    return 0.0;
}

struct GfxWindowManagerAPI gfx_3ds =
{
    gfx_3ds_init,
    gfx_3ds_main_loop,
    gfx_3ds_get_dimensions,
    gfx_3ds_handle_events,
    gfx_3ds_start_frame,
    gfx_3ds_swap_buffers_begin,
    gfx_3ds_swap_buffers_end,
    gfx_3ds_get_time
};

#endif