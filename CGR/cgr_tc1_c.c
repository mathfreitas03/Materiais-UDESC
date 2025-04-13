// gcc -Iinclude -g cgr_tc1_a.c -o cgr_tc1_a.exe -Llib-mingw-w64 -lglfw3 -lgdi32 -lopengl32 -lglu32

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
static double screen_zoom = -5.0f;

// Alterações na altura relativa dos objetps
static double screen_height = -1.0f;

// Movimentações no eixo x
static double screen_horizontal = 0.0f;

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
    gluPerspective(35.0f, fAspect, 1.0, 40.0);  
  
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
    glClearColor(0.25f, 0.25f, 0.50f, 1.0f);  

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

	// white
	glColor3f(1.f, 1.f, 1.f);  

	// Main Body  
//	gluSphere();  // Bottom

	// Cabeça
	glPushMatrix(); // save transform matrix state
		glTranslatef(0.0f, 1.5f, 0.0f);
		gluSphere(pObj, 0.24f, 26, 13);
	glPopMatrix(); // restore transform matrix state

    // Visor
    glColor3f(.6f, .0f, .0f);
    glPushMatrix();
        glTranslatef(.0, 1.6f, .05f);
        glRotatef(90, 1.0f, 0.0f, 0.0f);
        gluCylinder(pObj, .2f, .2f, .06f, 20, 1);  
    glPopMatrix();

    // Corpo no meio
    glPushMatrix(); // save transform matrix state
        glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(0.0f, 1.1f, 0.0f);
		gluSphere(pObj, 0.3f, 26, 13);
        glTranslatef(0.0f, -1.1f, 0.0f);
        glPushMatrix();glRotatef(-90, 1.0, 0.0, 0.0);
        
        glTranslatef(0.0f, .0f, .5f);
        gluCylinder(pObj, 0.2f, 0.3f, .6, 26, 13);
        glPopMatrix();
	glPopMatrix(); // restore transform matrix state

    // Braço Esquerdo
    glPushMatrix();
        glColor3f(1.f, 1.f, 1.f);
        glTranslatef(-.3f, 1.1f, 0.0f);
        gluSphere(pObj, .18f, 15, 15);
        glRotatef(60, 1.0f, 0.0f, 0.0f);
        glRotatef(-10, 0.0f, 1.f, .0f);
        gluCylinder(pObj, .14f, .10f, .4, 10, 10);
        glTranslatef(.0f, .0f, .4f);
        gluSphere(pObj, .11f, 15, 15);
        glRotatef(30, 0.0f, 1.f, .0f);
        glRotatef(-30, 1.0f, 0.f, .0f);
        gluCylinder(pObj, .10f, .08f, .35, 10, 10);
        glTranslatef(.0f, .0f, .35f);
        gluSphere(pObj, .1f, 15, 15);
    glPopMatrix();

    // Braço Direito
    glPushMatrix();
        glColor3f(1.f, 1.f, 1.f);
        glTranslatef(.3f, 1.1f, 0.0f);
        gluSphere(pObj, .18f, 15, 15);
        glRotatef(60, 1.0f, 0.0f, 0.0f);
        glRotatef(10, 0.0f, 1.f, .0f);
        gluCylinder(pObj, .14f, .10f, .4, 10, 10);
        glTranslatef(.0f, .0f, .4f);
        gluSphere(pObj, .11f, 15, 15);
        glRotatef(-30, 0.0f, 1.f, .0f);
        glRotatef(-30, 1.0f, 0.f, .0f);
        gluCylinder(pObj, .10f, .08f, .35, 10, 10);
        glTranslatef(.0f, .0f, .35f);
        gluSphere(pObj, .1f, 15, 15);
    glPopMatrix();


    double white[] = {1.f, 1.f, 1.f};
    // Perna esquerda
    glPushMatrix();
        glColor3f(1.f, 1.f, 1.f);
        glTranslatef(-.17f, .4f, 0.0f);
        gluSphere(pObj, .18f, 15, 15);
        glTranslatef(.0f, -.5f, .0f);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glPushMatrix();
            glRotatef(-5, 1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, -0.05f, .0f);
            gluCylinder(pObj, .13f, .18f, .5f, 10, 10);
        glPopMatrix();
        glTranslatef(.0f, .0f, -.4f);
        glPushMatrix();
            glRotatef(5, 1.0f, 0.0f, 0.0f);
            gluCylinder(pObj, .13f, .13f, .5f, 10, 10);
        glPopMatrix();
        drawParalelepipedo(.0f, -.12f, .0f, 0.3f, 0.5f, 0.15f, white);
    glPopMatrix();

    // Perna direita
    glPushMatrix();
        glColor3f(1.f, 1.f, 1.f);
        glTranslatef(.17f, .4f, 0.0f);
        gluSphere(pObj, .18f, 15, 15);
        glTranslatef(.0f, -.5f, .0f);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glPushMatrix();
            glRotatef(-5, 1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, -0.05f, .0f);
            gluCylinder(pObj, .13f, .18f, .5f, 10, 10);
        glPopMatrix();
        glTranslatef(.0f, .0f, -.4f);
        glPushMatrix();
            glRotatef(5, 1.0f, 0.0f, 0.0f);
            gluCylinder(pObj, .13f, .13f, .5f, 10, 10);
        glPopMatrix();
        drawParalelepipedo(.0f, -.12f, .0f, 0.3f, 0.5f, 0.15f, white);
    glPopMatrix();

    // Braço Esquerdo
    glPushMatrix();

    glPopMatrix();


// glColor, glPushMatrix,...

	// Hat

	// Hat brim
          
    // Restore the matrix state  
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
