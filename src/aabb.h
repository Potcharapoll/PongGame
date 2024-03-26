#ifndef AABB_H
#define AABB_H
#include <cglm/struct.h>

typedef struct {
    vec2s min;
    vec2s max;
} box_t;

static inline bool aabb_collision(box_t a, box_t b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x || a.min.y > b.max.y || a.max.y < b.min.y) {
        return false;
    }
    return true;
}
#endif
