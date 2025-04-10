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

	// Mid section
//	glPushMatrix();
//		glTranslatef(); 
//		gluSphere();
//	glPopMatrix();

	// Head
	glPushMatrix(); // save transform matrix state
		glTranslatef(0.0f, 1.5f, 0.0f);
		gluSphere(pObj, 0.24f, 26, 13);
	glPopMatrix(); // restore transform matrix state

    // Corpo no meio
    glPushMatrix(); // save transform matrix state
		glTranslatef(0.0f, 1.0f, 0.0f);
		gluSphere(pObj, 0.35f, 26, 13);
	glPopMatrix(); // restore transform matrix state

    // Base do corpo
    glPushMatrix(); // save transform matrix state
		glTranslatef(0.0f, 0.5f, 0.0f);
		gluSphere(pObj, 0.4f, 26, 13);
	glPopMatrix(); // restore transform matrix state

	// Nariz (orange)
	glColor3f(.949f, 0.624f, 0.02f);  
	glPushMatrix();
		glTranslatef(0.0f, 1.5f, 0.2f);
		gluCylinder(pObj, 0.04f, 0.0f, 0.3f, 26, 13);  
	glPopMatrix();  

	// Olho (esquerdo)
    glColor3f(.0f, .0f, .0f);
    glPushMatrix();
        glTranslatef(.08f, 1.6f, 0.2f);
        gluSphere(pObj, 0.03f, 10, 10);  
    glPopMatrix();

    // Olho (direito)
    glPushMatrix();
        glTranslatef(-.08f, 1.6f, 0.2f);
        gluSphere(pObj, 0.03f, 10, 10); 
    glPopMatrix();

    // Chapeu
    glPushMatrix();
        glTranslatef(0.0f, 1.68f, 0.0f);            
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);            
        gluCylinder(pObj, 0.27f, 0.27f, 0.05f, 30, 1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0f, 1.7f, 0.0f);      
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);   
        gluCylinder(pObj, 0.19, 0.22f, 0.26f, 30, 1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.0f, 1.68f, 0.0f);
        gluDisk(pObj, 12.0f, 12.0f, 100, 100);
    glPopMatrix();

    // Braços
    glPushMatrix();
        // Braço direito
        glTranslatef(-.25f, 1.2f, 0.0f);
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(-20.0f, 1.0f, .0f, 0.0f);
        gluCylinder(pObj, 0.03f, 0.02f, .45f, 10, 10);

        glTranslatef(.0f, .0f, 0.4f);
        gluCylinder(pObj, 0.02f, 0.001f, .2f, 10, 10);

        glRotatef(-20.0f, 1.0f, .0f, 0.0f);
        gluCylinder(pObj, 0.02f, 0.001f, .2f, 10, 10);

        glRotatef(40.0f, 1.0f, .0f, 0.0f);
        gluCylinder(pObj, 0.02f, 0.001f, .2f, 10, 10);

    glPopMatrix();

    glPushMatrix();
        // Braço esquerdo
        glTranslatef(.3f, 1.18f, 0.0f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(-20.0f, 1.0f, .0f, 0.0f);
        gluCylinder(pObj, 0.03f, 0.02f, .45f, 10, 10);

        glTranslatef(.0f, .0f, 0.45f);
        gluCylinder(pObj, 0.02f, 0.001f, .2f, 10, 10);

        glRotatef(-20.0f, 1.0f, .0f, 0.0f);
        gluCylinder(pObj, 0.02f, 0.001f, .2f, 10, 10);

        glRotatef(40.0f, 1.0f, .0f, 0.0f);
        gluCylinder(pObj, 0.02f, 0.001f, .2f, 10, 10);
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
