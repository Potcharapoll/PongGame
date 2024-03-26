#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/struct.h>
#include <cglm/cglm.h>

#include "types.h"

#define ERR(...) fprintf(stderr, __VA_ARGS__); abort();

typedef void (*Func)(void);

struct Keyboard {
    struct {
        bool pressed;
    } keys[GLFW_KEY_LAST];
};

struct Window {
    GLFWwindow *window;
    Func init, update, cleanup;
    ivec2s size;

    struct Keyboard keyboard;
    f32 delta_time;
};

void window_create(Func init, Func update, Func cleanup);
void window_loop();
extern struct Window window;
#endif
