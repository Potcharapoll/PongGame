#ifndef STATE_H
#define STATE_H
#include "window.h"
#include "renderer.h"

struct State {
    mat4s proj;
    struct Window *window;
    struct Renderer renderer;  
};
extern struct State state;
#endif
