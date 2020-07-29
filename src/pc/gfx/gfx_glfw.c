#if 0
#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

Bool glXGetSyncValuesOML(Display* dpy,
                         Drawable drawable,
                         int64_t* ust,
                         int64_t* msc,
                         int64_t* sbc);

Bool glXGetMscRateOML(Display* dpy,
                      Drawable drawable,
                      int32_t* numerator,
                      int32_t* denominator);

int64_t glXSwapBuffersMscOML(Display* dpy,
                             Drawable drawable,
                             int64_t target_msc,
                             int64_t divisor,
                             int64_t remainder);

Bool glXWaitForMscOML(Display* dpy,
                      Drawable drawable,
                      int64_t target_msc,
                      int64_t divisor,
                      int64_t remainder,
                      int64_t* ust,
                      int64_t* msc,
                      int64_t* sbc);

Bool glXWaitForSbcOML(Display* dpy,
                      Drawable drawable,
                      int64_t target_sbc,
                      int64_t* ust,
                      int64_t* msc,
                      int64_t* sbc);

static struct {
    GLFWwindow *window;
} glfw;

static void gfx_glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "GLFW error: %s\n", description);
}

static void gfx_glfw_monitor_callback(GLFWmonitor *monitor, int event) {
    printf("Monitor %p changed %d\n", monitor, event);
    if (event != GLFW_DISCONNECTED) {
        int xpos, ypos;
        glfwGetMonitorPos(monitor, &xpos, &ypos);
        printf("x: %d, y: %d\n", xpos, ypos);
    }
}

void gfx_glfw_init(void) {
    glfwSetErrorCallback(gfx_glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "GLFW init failed\n");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfw.window = glfwCreateWindow(640, 480, "Super Mario 64 GLFW", NULL, NULL);
    if (glfw.window == NULL) {
        fprintf(stderr, "glfwCreateWindow failed\n");
        exit(1);
    }
    glfwMakeContextCurrent(glfw.window);
    glfwSetMonitorCallback(gfx_glfw_monitor_callback);
}

void gfx_glfw_handle_events(void) {
    glfwPollEvents();
    if (glfwWindowShouldClose(glfw.window)) {
        exit(0);
    }
}

void gfx_glfw_swap_buffers(void) {
    static uint64_t cnt = 0;
    if (0) {
        int mc;
        GLFWmonitor **monitors = glfwGetMonitors(&mc);
        for (int i = 0; i < mc; i++) {
            GLFWmonitor *monitor = monitors[i];
            int xpos, ypos;
            glfwGetMonitorPos(monitor, &xpos, &ypos);
            if (++cnt % 60 == 0) printf("x: %d, y: %d\n", xpos, ypos);
        }
    }
    if (1) {
        XWindowAttributes xwa;
        XGetWindowAttributes(glfwGetX11Display(), glfwGetX11Window(glfw.window), &xwa);
        int rootx, rooty;
        Window child;
        XTranslateCoordinates(glfwGetX11Display(), glfwGetX11Window(glfw.window), xwa.root, xwa.x, xwa.y, &rootx, &rooty, &child);
        printf("%d %d\n", rootx, rooty);
        
        int32_t numerator = 0, denominator = 0;
        Bool ok = glXGetMscRateOML(glfwGetX11Display(), glfwGetX11Window(glfw.window), &numerator, &denominator);
        printf("%d %d %d %f\n", ok, numerator, denominator, (double)numerator / denominator);
        
        int mc;
        GLFWmonitor **monitors = glfwGetMonitors(&mc);
        XRRScreenResources *sr = XRRGetScreenResourcesCurrent(glfwGetX11Display(), glfwGetX11Window(glfw.window));
        for (int i = 0; i < mc; i++) {
            GLFWvidmode *vidmode = glfwGetVideoMode(monitors[i]);
            printf("glfw Hz: %d\n", vidmode->refreshRate);
            RRCrtc crtc = glfwGetX11Adapter(monitors[i]);
            XRRCrtcInfo *ci = XRRGetCrtcInfo(glfwGetX11Display(), sr, crtc);
            for (int j = 0; j < sr->nmode; j++) {
                if (sr->modes[j].id == ci->mode) {
                    unsigned long numerator = sr->modes[j].dotClock;
                    unsigned long denominator = (unsigned long)sr->modes[j].vTotal * (unsigned long)sr->modes[j].hTotal;
                    if (sr->modes[j].modeFlags & RR_DoubleScan) {
                        denominator *= 2;
                    }
                    if (sr->modes[j].modeFlags & RR_Interlace) {
                        // We want fields per second and not full frames per second
                        numerator *= 2;
                    }
                    printf("%d %d %f\n", i, j, (double)numerator / (double)denominator);
                    break;
                }
            }
            XRRFreeCrtcInfo(ci);
        }
        XRRFreeScreenResources(sr);
    }
    glfwSwapBuffers(glfw.window);
}

void gfx_window_system_init(void) {
    gfx_glfw_init();
}

void gfx_window_system_handle_events(void) {
    gfx_glfw_handle_events();
}

void gfx_window_system_swap_buffers(void) {
    gfx_glfw_swap_buffers();
}
#endif
