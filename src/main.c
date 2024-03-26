#include "vao.h"
#include "window.h"
#include "aabb.h"
#include "rectangle.h"
#include "renderer.h"
#include "sound.h"
#include "text.h"
#include "state.h"
#include "util.h"

struct State state;
static text_t text;
static struct Rectangle r_paddle, l_paddle, ball;
static sound_t *pock, *plant;
static sound_context_t sc;
static u32 r_score = 0;
static u32 l_score = 0;

static inline box_t rect_to_box(struct Rectangle *self) {
    return (box_t) {
        .min = {self->position.x, self->position.y}, 
            .max = {self->position.x+self->size.u, self->position.y+self->size.v}
    };
}

static inline void border_render(void) {
    float width = 12.5f;
    for (int i = 0; i < window.size.v/48; i++) {
        float stride = 50.0*i;
        vertex_t vertices[4] = {
            { .position = {window.size.u/2.0f-width/2.0f,       00.0f+stride      , 0.0f }, .color = {0.4, 0.4, 0.4, 1.0}, .tex_coord = {0.0, 0.0} },
            { .position = {window.size.u/2.0f,                  00.0f+stride      , 0.0f }, .color = {0.4, 0.4, 0.4, 1.0}, .tex_coord = {0.0, 0.0} },
            { .position = {window.size.u/2.0f,                  00.0f+stride+40.0f, 0.0f }, .color = {0.4, 0.4, 0.4, 1.0}, .tex_coord = {0.0, 0.0} },
            { .position = {window.size.u/2.0f-width/2.0f,       00.0f+stride+40.0f, 0.0f }, .color = {0.4, 0.4, 0.4, 1.0}, .tex_coord = {0.0, 0.0} }
        };
        vbo_subdata(state.renderer.vbo, sizeof(vertices), vertices);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
    }
}

static inline void ball_reset_position(struct Rectangle *self) {
    self->position = (vec3s){ window.size.u/2.0f-15.0/2.0f , window.size.v/2.0f-15.0f/2.0f, 0.0f };
    rectangle_update_vertices(self);
}

static inline void score_render(void) {
    char buffer[108];
    int len;

    len = snprintf(buffer, 108, "%d", l_score);
    float padding = 4.0f;
    text_render(text, buffer, 10.0f, padding, (vec2s){window.size.u/2.0f - (50.0f+(50.0f*(padding/10))) - ((8.0*10.0f*len)-(8.0*padding*len)), window.size.v - 100.0f}, (vec3s){1.0f, 1.0f, 1.0f});

    len = snprintf(buffer, 108, "%d", r_score);
    text_render(text, buffer, 10.0f, padding, (vec2s){window.size.u/2.0f + 50.0f, window.size.v - 100.0f}, (vec3s){1.0f, 1.0f, 1.0f});
}

static inline void process_input(void) {

    // reset input state and velocity
    memset(&window.keyboard, 0, sizeof(window.keyboard));
    r_paddle.velocity = glms_vec2_zero();
    l_paddle.velocity = glms_vec2_zero();

    // left paddle input
    if (glfwGetKey(window.window, GLFW_KEY_W) == GLFW_PRESS) {
        window.keyboard.keys[GLFW_KEY_W].pressed = true;
    }  
    if (glfwGetKey(window.window, GLFW_KEY_S) == GLFW_PRESS) {
        window.keyboard.keys[GLFW_KEY_S].pressed = true;
    }  

    // right paddle input
    if (glfwGetKey(window.window, GLFW_KEY_UP) == GLFW_PRESS) {
        window.keyboard.keys[GLFW_KEY_UP].pressed = true;
    }  
    if (glfwGetKey(window.window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        window.keyboard.keys[GLFW_KEY_DOWN].pressed = true;
    }  

    if (window.keyboard.keys[GLFW_KEY_W].pressed) {
        l_paddle.velocity.y = 300.0f;
    }
    if (window.keyboard.keys[GLFW_KEY_S].pressed) {
        l_paddle.velocity.y = -(300.0f);
    }

    if (window.keyboard.keys[GLFW_KEY_UP].pressed) {
        r_paddle.velocity.y = 300.0f;
    }
    if (window.keyboard.keys[GLFW_KEY_DOWN].pressed) {
        r_paddle.velocity.y = -(300.0f);
    }

    // debug
    if (glfwGetKey(window.window, GLFW_KEY_0) == GLFW_PRESS) {
        window.keyboard.keys[GLFW_KEY_0].pressed = true;
    }  
    if (glfwGetKey(window.window, GLFW_KEY_9) == GLFW_PRESS) {
        window.keyboard.keys[GLFW_KEY_9].pressed = true;
    }  
    if (window.keyboard.keys[GLFW_KEY_0].pressed) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (window.keyboard.keys[GLFW_KEY_9].pressed) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

static inline void resolution_collision(struct Rectangle *self) {
    if (self->position.y+self->size.v > window.size.v) {
        self->position.y = window.size.v - self->size.v;
    }

    if (self->position.y < 0) {
        self->position.y = 0;
    }
}

static inline void ball_collision(void) {
    if (ball.position.y+ball.size.v > window.size.v || ball.position.y < 0) {
        sound_play(pock);
        ball.velocity.y *= -1; 
    }

    // It kinda buggy when collide with paddle in projectile motion because we only invert our x 
    // I think we could fix this by check collide by each axis?>
    if (aabb_collision(rect_to_box(&ball), rect_to_box(&l_paddle)) || aabb_collision(rect_to_box(&ball), rect_to_box(&r_paddle))) {
        sound_play(pock);
        ball.velocity.x *= -1;
    }

    if (ball.position.x+ball.size.u > window.size.u || ball.position.x < 0) {
        if (ball.position.x < 0) r_score++;
        else l_score++;
        sound_play(plant);
        ball_reset_position(&ball);
    }
}

static inline void rectangle_render(struct Rectangle *self) {
    rectangle_update_vertices(self);
    vbo_subdata(state.renderer.vbo, sizeof(self->vertices), self->vertices);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}


void init(void) {
    state.window = &window;
    renderer_init(&state.renderer);

    rectangle_init(&l_paddle, (vec3s){50.0f,                           window.size.v/2.0f-20.0f/2.0f,   0.0f}, (ivec2s){20, 100}, (vec4s){1.0f, 1.0f, 1.0f, 1.0f});
    rectangle_init(&r_paddle, (vec3s){window.size.u-75.0f,             window.size.v/2.0f-20.0f/2.0f,   0.0f}, (ivec2s){20, 100}, (vec4s){1.0f, 1.0f, 1.0f, 1.0f});
    rectangle_init(&ball,     (vec3s){window.size.u/2.0f - 15.0f/2.0f, window.size.v/2.0f - 15.0f/2.0f, 0.0f}, (ivec2s){15, 15},  (vec4s){1.0f, 1.0f, 1.0f, 1.0f});
    ball.velocity = (vec2s){300,300};

    sound_context_init(&sc);
    sound_context_use(&sc);

    pock  = sound_init("res/audio/pong.wav");
    plant = sound_init("res/audio/plant.wav");
    text  = text_init(8, (ivec2s){64, 40});
}

static inline void render(void) {
    renderer_clear();
    vao_bind(state.renderer.vao);
    vbo_bind(state.renderer.vbo);

    renderer_shader_use(&state.renderer, SHADER_BASIC);
    border_render();
    rectangle_render(&ball);
    rectangle_render(&l_paddle);
    rectangle_render(&r_paddle);

    score_render();
}

void update(void) {
    process_input();

    l_paddle.position.x += l_paddle.velocity.x*window.delta_time;
    l_paddle.position.y += l_paddle.velocity.y*window.delta_time;

    r_paddle.position.x += r_paddle.velocity.x*window.delta_time;
    r_paddle.position.y += r_paddle.velocity.y*window.delta_time;

    ball.position.x += ball.velocity.x*window.delta_time;
    ball.position.y += ball.velocity.y*window.delta_time;

    resolution_collision(&l_paddle);
    resolution_collision(&r_paddle);
    ball_collision();

    render();

    glfwPollEvents();
    glfwSwapBuffers(window.window);
}

void cleanup(void) {
    renderer_destroy(&state.renderer);

    sound_free(pock);
    sound_free(plant);
    sound_context_destroy(&sc);
}

int main(void) {
    window_create(init, update, cleanup);
    window_loop();
    return 0;
}
