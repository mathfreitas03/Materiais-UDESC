// gcc -Iinclude -g fireworks_glfw.c -Llib-mingw-w64 -lglfw3 -lgdi32 -lopengl32 -lglu32 -lm -o fireworks_glfw && ./fireworks_glfw

#define GLFW_INCLUDE_NONE
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <unistd.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define GRAVITY 0.0003
#define ASCENT_GRAVITY 0.0008
#define MAX_FIREWORKS 4
#define PARTICLES_PER_FIREWORK 600

typedef struct {
    float x, y, veloc_x, veloc_y, horizontal_vel, veloc_subida;
    unsigned lifetime;
} Particle;

typedef struct {
    Particle particles[PARTICLES_PER_FIREWORK];
    int exploded;
    int before_explosion_time;
    int active;
    float x, y;
    int color_index;
} Firework;

Firework fireworks[MAX_FIREWORKS];
int firework_timer = 0;

float cores[3][3] = {
    {1.0f, 0.0f, 0.0f}, 
    {0.0f, 0.0f, 1.0f},  
    {1.0f, 1.0f, 1.0f}   
};

void resizeWindow(GLFWwindow* window, int width, int height ) {
    GLfloat ratio;
    if ( height == 0 ) height = 1;
    ratio = ( GLfloat )width / ( GLfloat )height;
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 45.0f, ratio, 0.1f, 100.0f );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    glTranslatef(0.0f,0.0f,-6.0f);
}

void handleKeyPress(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
}

void InitFirework(Firework* fw, float x, float y) {
    fw->exploded = 0;
    fw->before_explosion_time = 150;
    fw->active = 1;
    fw->x = x;
    fw->y = y;
    fw->color_index = rand() % 3;

    float direction = (rand() % 2 == 0) ? 1.0f : -1.0f;

    for (int i = 0; i < PARTICLES_PER_FIREWORK; i++) {
        float velocity = ((float) rand() / RAND_MAX) * 0.02f;
        int angle = rand() % 360;

        fw->particles[i].veloc_x = cos(M_PI * angle / 180.0) * velocity;
        fw->particles[i].veloc_y = sin(M_PI * angle / 180.0) * velocity;
        fw->particles[i].x = x;
        fw->particles[i].y = y;
        fw->particles[i].lifetime = rand() % 100;
        fw->particles[i].horizontal_vel = direction * 0.005f;
        fw->particles[i].veloc_subida = 0.02f;
    }
}

void drawGLScene(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_POINTS);

    for (int f = 0; f < MAX_FIREWORKS; f++) {
        if (!fireworks[f].active) continue;
        glColor3f(cores[fireworks[f].color_index][0], cores[fireworks[f].color_index][1], cores[fireworks[f].color_index][2]);

        int active_particles = 0;
        for (int i = 0; i < PARTICLES_PER_FIREWORK; i++) {
            Particle* p = &fireworks[f].particles[i];

            if (!fireworks[f].exploded && p->lifetime) {
                active_particles++;
                p->x += 2 * p->horizontal_vel;
                p->y += 1.05 * p->veloc_subida;
                p->y -= ASCENT_GRAVITY;
                glVertex2f(p->x, p->y);
            } else if (fireworks[f].exploded && p->lifetime) {
                active_particles++;
                p->veloc_y -= GRAVITY;
                p->x += p->veloc_x;
                p->y += p->veloc_y;
                p->lifetime--;
                glVertex3f(p->x, p->y, 0.0f);
            }
        }

        if (!fireworks[f].exploded) {
            fireworks[f].before_explosion_time--;
            if (fireworks[f].before_explosion_time <= 0) {
                fireworks[f].exploded = 1;
            }
        }

        if (active_particles == 0) {
            fireworks[f].active = 0;
        }
    }

    glEnd();

    firework_timer--;
    if (firework_timer <= 0) {
        for (int i = 0; i < MAX_FIREWORKS; i++) {
            if (!fireworks[i].active) {
                float dir = rand() % 2 ? 1.0f : -1.0f;
                float x = dir * ((float) rand() / RAND_MAX) * 2.5f;
                InitFirework(&fireworks[i], x, -3.0f);
                break;
            }
        }
        firework_timer = 50 + rand() % 100;
    }
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fireworks", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resizeWindow);
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    resizeWindow(window, SCREEN_WIDTH, SCREEN_HEIGHT);

    while (!glfwWindowShouldClose(window)) {
        handleKeyPress(window);
        drawGLScene(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
