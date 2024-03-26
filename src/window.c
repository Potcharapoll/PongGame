#include "window.h"
#include "GLFW/glfw3.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#pragma GCC diagnostic ignored "-Wmissing-braces"

struct Window window;


void window_create(Func init, Func update, Func cleanup) {
    if(!glfwInit()) {
        ERR("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.init    = init;
    window.update  = update;
    window.cleanup = cleanup;
    window.size    = (ivec2s){720, 480};
    window.window  = glfwCreateWindow(window.size.u, window.size.v, "PongGame", NULL, NULL);
    if (!window.window) {
        ERR("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window.window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ERR("Failed to initialize GLAD");
    }

}

static inline void _w_init(void) {
    window.init();
}

static inline void _w_update(void) {
    window.update();
}

static inline void _w_cleanup(void) {
    window.cleanup();
    glfwDestroyWindow(window.window);
    glfwTerminate();
}

void window_loop(void) {
    _w_init();

    f32 last_frame = glfwGetTime();
    f32 currnet_frame = 0;
    f32 accumulator = 0;
    char buffer[64];

    while (!glfwWindowShouldClose(window.window)) {

        {
            glfwGetWindowSize(window.window, &window.size.u, &window.size.v);
            glViewport(0, 0, window.size.u, window.size.v);
        }

        currnet_frame = glfwGetTime();
        window.delta_time = (currnet_frame - last_frame);
        last_frame = currnet_frame;
        accumulator += window.delta_time;

        if (accumulator >= 1.0f) {
            snprintf(buffer, 64, "PongGame (FPS: %.3f ms: %.3f)", 1.0 / window.delta_time, window.delta_time * 1000.0f);
            glfwSetWindowTitle(window.window, buffer);
            accumulator = 0.0f;
        }

        _w_update();
    }
    _w_cleanup();
}
