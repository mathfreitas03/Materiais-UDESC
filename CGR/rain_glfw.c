// gcc -Iinclude -g rain_glfw.c -Llib-mingw-w64 -lglfw3 -lgdi32 -lopengl32 -lglu32 -lm -o rain_glfw && ./rain_glfw

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
#define MAX_PARTICLES 800
#define GRAVITY 0.0003

typedef struct {
    float x, y;
    float veloc_y, veloc_x;
    unsigned lifetime;
} Particle;

Particle particles[MAX_PARTICLES];

void resizeWindow(GLFWwindow* window, int width, int height ) {
    GLfloat ratio;
    if (height == 0) height = 1;
    ratio = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -6.0f);
}

void handleKeyPress(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
}

void InitParticle(int pause) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        float x_pos = ((float)(rand() % 2400) / 100.0f) - 12.0f; 
        float y_pos = ((float)(rand() % 1000) / 100.0f);         
        particles[i].x = x_pos;
        particles[i].y = 6.0f + y_pos;  // Começa acima da tela
        particles[i].veloc_y = 0.15f + ((float)(rand() % 10) / 1000.0f); // Pequena variação na queda
        particles[i].veloc_x = particles[i].veloc_y;
        particles[i].lifetime = 400 + rand() % 100;
    }
}

void drawGLScene(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_LINES);
    int active_particles = 0;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].lifetime > 0) {
            active_particles++;

            particles[i].y -= particles[i].veloc_y;
            particles[i].y -= 1.2 * GRAVITY;
            particles[i].x -= 0.5 * particles[i].veloc_x;
            particles[i].lifetime--;

            if (particles[i].y < -6.0f) {
                float x_pos = ((float)(rand() % 2400) / 100.0f) - 12.0f;
                float y_pos = ((float)(rand() % 1000) / 100.0f);
                particles[i].x = x_pos;
                particles[i].y = 6.0f + y_pos;
                particles[i].veloc_y = 0.1f + ((float)(rand() % 10) / 1000.0f);
                particles[i].lifetime = 400 + rand() % 100;
            }
            glColor3f(.5f, .5f, 1.0f);
            glVertex2f(particles[i].x, particles[i].y);
            glVertex2f(particles[i].x + 0.1f, particles[i].y + 0.2f);
        }
    }
    glEnd();

    glfwSwapBuffers(window);
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

    InitParticle(0);

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
