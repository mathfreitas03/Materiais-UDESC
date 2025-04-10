// gcc -Iinclude -g cgr_tc1_b.c -o cgr_tc1_b.exe -Llib-mingw-w64 -lglfw3 -lgdi32 -lopengl32 -lglu32

#define GLFW_INCLUDE_NONE

#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>  // Header File For The GLFW Library
#include <GL/gl.h>       // Header File For The OpenGL32 Library
#include <GL/glu.h>      // Header File For The GLu32 Library
#include <math.h>

void processInput(GLFWwindow *window);

// settings
#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080

// Rotation
static GLfloat yRot = 0.0f;
static GLfloat xRot = 0.0f;

// Meu zoom
static double screen_zoom = -10.0f;

// Alterações na altura relativa dos objetps
static double screen_height = -1.0f;

// Movimentações no eixo x
static double screen_horizontal = 0.0f;

// Definição das cores a serem utilizadas repetidas vezes

double castle_color[] = {255.0f/255.0f, 232.0f/255.0f, 204.0f/255.0f};
double castle_door_color[3] = {0.4235f, 0.2355f, 0.0471f};
double bridge_color[3] = {99.0f/255.0f, 102.0f/255.0f, 106.f/255.f};
GLfloat modelMatrix[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

void rotateAroundAxis(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat rad = angle * M_PI / 180.0f;
    GLfloat c = cosf(rad);
    GLfloat s = sinf(rad);
    GLfloat oneMinusC = 1.0f - c;

    GLfloat r[16] = {
        x * x * oneMinusC + c,     x * y * oneMinusC - z * s, x * z * oneMinusC + y * s, 0.0f,
        y * x * oneMinusC + z * s, y * y * oneMinusC + c,     y * z * oneMinusC - x * s, 0.0f,
        z * x * oneMinusC - y * s, z * y * oneMinusC + x * s, z * z * oneMinusC + c,     0.0f,
        0.0f,                      0.0f,                      0.0f,                      1.0f
    };

    // Multiplica a nova rotação pela matriz acumulada
    glPushMatrix();
    glLoadMatrixf(modelMatrix);
    glMultMatrixf(r);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
    glPopMatrix();
}

/* function to reset our viewport after a window resize */
void resizeWindow(GLFWwindow* window, int w, int h )
{
    GLfloat fAspect;  
  
    // Prevent a divide by zero  
    if(h == 0)  
        h = 1;  
  
    // Set Viewport to window dimensions  
    glViewport(0, 0, w, h);  
  
    fAspect = (GLfloat)w/(GLfloat)h;  
  
    // Reset coordinate system  
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();  
  
    // Produce the perspective projection  
    gluPerspective(100.0f, fAspect, 1.0, 40.0);  
  
    glMatrixMode(GL_MODELVIEW);  
    glLoadIdentity();  
}

// This function does any needed initialization on the rendering context.  Here it sets up and initializes the lighting for the scene.  
void setupRC(){  

    // Light values and coordinates  
    GLfloat  whiteLight[] = { 0.05f, 0.05f, 0.05f, 1.0f };  
    GLfloat  sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };  
    GLfloat  lightPos[] = { -10.f, 5.0f, 5.0f, 1.0f };  
  
    glEnable(GL_DEPTH_TEST);    // Hidden surface removal  
    glFrontFace(GL_CCW);        // Counter clock-wise polygons face out  
    glEnable(GL_CULL_FACE);     // Do not calculate inside  
  
    // Enable lighting  
    glEnable(GL_LIGHTING);  
  
    // Setup and enable light 0  
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,whiteLight);  
    glLightfv(GL_LIGHT0,GL_AMBIENT,sourceLight);  
    glLightfv(GL_LIGHT0,GL_DIFFUSE,sourceLight);  
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);  
    glEnable(GL_LIGHT0);  
  
    // Enable color tracking  
    glEnable(GL_COLOR_MATERIAL);  
      
    // Set Material properties to follow glColor values  
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  
  
    // Black blue background  
    glClearColor(0.529f, 0.807f, 0.980f, 1.0f);  

}  

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset > 0)
        screen_zoom += 0.4f;
    else if (yoffset < 0)
        screen_zoom -= 0.4f;
}

/* function to handle key press events */
void handleKeyPress(GLFWwindow *window)
{
    glfwSetScrollCallback(window, scroll_callback);

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) 
        rotateAroundAxis(5.0f, 0.0f, 1.0f, 0.0f);
  
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)  
        rotateAroundAxis(-5.0f, 0.0f, 1.0f, 0.0f);
                  
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
        rotateAroundAxis(5.0f, 1.0f, 0.0f, 0.0f);
  
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)  
        rotateAroundAxis(-5.0f, 1.0f, 0.0f, 0.0f);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        screen_height -= 0.2f; 

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    screen_height += 0.2f; 

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    screen_horizontal += 0.2f; 

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    screen_horizontal -= 0.2f; 

    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        screen_height = -1.0f;
        // Resetar matriz acumulada para identidade
        for (int i = 0; i < 16; ++i)
            modelMatrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
}

// void handleKeyPress(GLFWwindow *window)
// {
//     if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//         glfwTerminate();
//         exit(EXIT_SUCCESS);
//     }
//     if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) 
//         yRot -= 5.0f;  
//     if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)  
//         yRot += 5.0f;      
//     if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
//         xRot -= 5.0f;  
//     if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)  
//         xRot += 5.0f;
//     if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
//         xRot = 0.0f;
//         yRot = 0.0f;  
//     }  
//     xRot = (GLfloat)((const int)xRot % 360);  
//     yRot = (GLfloat)((const int)yRot % 360);  
// }

void drawTower(GLUquadricObj* pObj, double x, double y, double z){
    
    glPushMatrix();

        // Posiciona a torre no centro da coordenada fornecida
        glTranslatef(x, y, z);

        // Centraliza verticalmente
        glTranslatef(0.0f, 0.0f, -7.0f); // 10 (base) + 4 (telhado) / 2

        // BASE DA TORRE
        glColor3f(castle_color[0], castle_color[1], castle_color[2]);
        glRotatef(-90, 1.0f, 0.0f, 0.0f); // torre cresce agora no eixo Z
        gluCylinder(pObj, 3.0f, 3.0f, 12.0f, 50, 50);

        // TELHADO
        glColor3f(255.0f/255.0f, 142.0f/255.0f, 120.0f/255.0f);
        glTranslatef(.0f, .0f, 12.f);
        gluCylinder(pObj, 3.0f, 0.5f, 4.f, 50, 50);

        // JANELAS
        const float windowZ = -2.0f; //

        float positions[][2] = {
            { 2.8f,  0.0f},
            {-2.8f,  0.0f},
            { 0.0f,  2.8f},
            { 0.0f, -2.8f},
            { 2.0f,  2.0f},
            { 2.0f, -2.0f},
            {-2.0f,  2.0f},
            {-2.0f, -2.0f}
        };

        glColor3f(0.0f, 0.0f, 0.0f);
        for(int i = 0; i < 8; ++i) {
            glPushMatrix();
                glTranslatef(positions[i][0], positions[i][1], windowZ);
                gluCylinder(pObj, 0.3, 0.3, 0.8, 5, 5);
            glPopMatrix();
        }

    glPopMatrix();
}

void drawTerrain(){
    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE);
    glPushMatrix();
        glTranslatef(-100.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLES);
            glTranslatef(0.0f, 0.0f, -2.f);
            glColor3f(0.5f, 0.8f, 0.2f); // cor do plano (verde claro)
            glVertex3f(-200.0f, 0.0f, -200.0f); // canto inferior esquerdo
            glVertex3f( 200.0f, 0.0f, -200.0f); // canto inferior direito
            glVertex3f( 200.0f, 0.0f,  200.0f); // canto superior direito
            glVertex3f(-200.0f, 0.0f,  200.0f); // canto superior esquerdo
        glEnd();
    glPopMatrix();
}

void drawParalelepipedo(double x, double y, double z, double size_w, double size_h, double depth, double rgb[3]){

    double dx = size_w / 2.0;
    double dy = size_h / 2.0;
    double dz = depth / 2.0;

    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE);
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(rgb[0], rgb[1], rgb[2]);

    glBegin(GL_QUADS);

    // Frente
    glVertex3f(-dx, -dy, dz);
    glVertex3f( dx, -dy, dz);
    glVertex3f( dx,  dy, dz);
    glVertex3f(-dx,  dy, dz);

    // Trás
    glVertex3f( dx, -dy, -dz);
    glVertex3f(-dx, -dy, -dz);
    glVertex3f(-dx,  dy, -dz);
    glVertex3f( dx,  dy, -dz);

    // Direita
    glVertex3f( dx, -dy, dz);
    glVertex3f( dx, -dy, -dz);
    glVertex3f( dx,  dy, -dz);
    glVertex3f( dx,  dy, dz);

    // Esquerda
    glVertex3f(-dx, -dy, -dz);
    glVertex3f(-dx, -dy, dz);
    glVertex3f(-dx,  dy, dz);
    glVertex3f(-dx,  dy, -dz);

    // Topo
    glVertex3f(-dx,  dy, dz);
    glVertex3f( dx,  dy, dz);
    glVertex3f( dx,  dy, -dz);
    glVertex3f(-dx,  dy, -dz);

    // Base
    glVertex3f(-dx, -dy, -dz);
    glVertex3f( dx, -dy, -dz);
    glVertex3f( dx, -dy, dz);
    glVertex3f(-dx, -dy, dz);

    glEnd();
    glPopMatrix();
}

void drawWall(double x, double y, double z, double size_w, double size_h, double depth){
    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE); // Para desenhar todos os lados visíveis
    glPushMatrix();
    glTranslatef(x, y + 3.0f, z);
    drawParalelepipedo(0.0f, 0.0f, 0.0f, size_w, size_h, depth, castle_color); // Comprimento 15, altura 7 e profundidade 2
    int ameias = (int)(size_w / 2.0f);
    double trilha_ameias = -size_w / 2.0 + 1.0f;

    for(int i = 0; i < ameias; i++) {
        drawParalelepipedo(trilha_ameias, size_h - 3.0f, 0.0f, 1.0f, 1.0f, 1.0f, castle_color);
        trilha_ameias += 2.0f;
    }
    glPopMatrix();
}

void drawCircle(float cx, float cy, float r, int segments) {
    glLineWidth(50.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.1f, 0.0f);
    glColor3f(48.0f/255.0f,92.0f/255.0f,222.0f/255.0f);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * M_PI * (float)i / (float)segments;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
    glLineWidth(1.0f); 
    glPopMatrix();
}

/* Here goes our drawing code */
void drawGLScene(GLFWwindow* window)
{
    GLUquadricObj *pObj;    // Quadric Object  
      
    // Clear the window with current clearing color  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
  
    // Save the matrix state and do the rotations  
    glPushMatrix();

	// Move object back and do in place rotation  
	glTranslatef(screen_horizontal, screen_height, screen_zoom);  
    glMultMatrixf(modelMatrix); 

	// glRotatef(yRot, 0.0f, 1.0f, 0.0f);  
    // glRotatef(xRot, 1.0f, 0.0f, 0.0f);  

	// Draw something  
	pObj = gluNewQuadric();  
	gluQuadricNormals(pObj, GLU_SMOOTH);  

	// glColor3f(8.f, 1.f, 8.f);  

    //glTranslatef(0.0f, .0f, -2.0f);
    // glBegin(GL_QUADS); // ou GL_TRIANGLES para dois triângulos
    // glColor3f(0.5f, 0.8f, 0.2f); // cor do plano (verde claro)

    // glVertex3f(-255.0f, 0.0f, -255.0f); // canto inferior esquerdo
    // glVertex3f( 255.0f, 0.0f, -255.0f); // canto inferior direito
    // glVertex3f( 255.0f, 0.0f,  255.0f); // canto superior direito
    // glVertex3f(-255.0f, 0.0f,  255.0f); // canto superior esquerdo
    // glEnd();

    drawTerrain();

    glTranslatef(-7.5f, 0.0f, -15.0f);

    drawTower(pObj, 0.0f, 0.0f, 0.0f);
    drawTower(pObj, 0.0f, 0.0f, 20.0f);
    drawTower(pObj, 20.0f, 0.0f, 0.0f);
    drawTower(pObj, 20.0f, 0.0f, 20.0f);

    drawWall(10.f, .0f, 13.f, 15.0f, 7.0f, 2.0f);
    drawWall(10.f, .0f, -7.f, 15.0f, 7.0f, 2.0f);

    drawCircle(10.0f, -2.0f, 20.0f, 150);

        glPushMatrix();
            glRotatef(-90, 0.0, 1.0, 0.0);
            drawWall(3.f, 0.0f, -20.f, 15.0f, 7.0f, 2.0f);
            drawWall(3.f, 0.0f, 0.0f, 15.0f, 7.0f, 2.0f);
            drawParalelepipedo(3.0f, 2.0f, 1.0f, 3.0f, 4.0f, 1.0f, castle_door_color);
            drawParalelepipedo(3.0f, .0f, 10.0f, 5.f, 1.f, 5.0f, bridge_color);
        glPopMatrix();

    glPopMatrix();  
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snowman", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resizeWindow);
    glfwSwapInterval(1);
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    resizeWindow(window, SCREEN_WIDTH, SCREEN_HEIGHT);
    setupRC();

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        handleKeyPress(window);

        // render
        drawGLScene(window);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}
