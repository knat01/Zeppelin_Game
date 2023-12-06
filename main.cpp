/*******************************************************************
		   Hierarchical Multi-Part Model Example
********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gl/glut.h>
#include <utility>
#include <vector>
#include "VECTOR3D.h"
#include "QuadMesh.h"
#include <SOIL.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>


float enemyZeppelinX = 0.0f;
float enemyZeppelinMoveSpeed = 0.5f;
int enemyDirection = 1;
const float enemyZeppelinXMin = -35.0f;
const float enemyZeppelinXMax = 35.0f;
const float PI = 3.14159265358979323846;
float enemyZeppelinAngle = 0.0f;
int turnCounter = 0;
const int turnDuration = 10;
const float enemyZeppelinZMin = -35.0f;
const float enemyZeppelinZMax = 35.0f;
int enemyZeppelinHits = 0;
float crashDescentRate = 0.005f;
float crashRotationSpeed = 2.0f;
int crashStartTime = 0;
float enemyCrashRotationY = 0.0f;
void drawBlastParticles();
void updateBlastParticles();
void initBlastParticles(float x, float y, float z);
static bool particlesInitialized = false;
void topDownDisplay(void);
void topDownReshape(int w, int h);
void topDownDisplay(void);
void renderScene();
void initOpenGLTopDown(int w, int h);
void enemyShootTimer(int value);
bool isUserZeppelinActive = true;

bool isUserZeppelinCrashing = false;
int userCrashStartTime;
float userCrashRotationY = 0.0f;



GLuint texture;
GLuint enemyTexture;

const int vWidth  = 650;
const int vHeight = 500;

// Bullet properties
struct Bullet {
    GLfloat x, y, z;
    GLfloat speed;
    GLfloat direction;
};
std::vector<Bullet> bullets;
std::vector<Bullet> enemyBullets;

float robotBodyWidth = 8.0;
float robotBodyLength = 10.0;
float robotBodyDepth = 6.0;
float headWidth = 0.4*robotBodyWidth;
float headLength = headWidth;
float headDepth = headWidth;
float upperArmLength = robotBodyLength;
float upperArmWidth = 0.125*robotBodyWidth;
float gunLength = upperArmLength / 4.0;
float gunWidth = upperArmWidth;
float gunDepth = upperArmWidth;
float stanchionLength = robotBodyLength;
float stanchionRadius = 0.1*robotBodyDepth;
float baseWidth = 2 * robotBodyWidth;
float baseLength = 0.25*stanchionLength;
bool firstPersonView = false;
bool checkBulletCollision(Bullet bullet);
float enemyCrashRotationX = 0.0f;
float enemyCrashRotationZ = 0.0f;
float enmyZepplinAngleX = 0.0;
float enmyZeppelinAngle = 0.0;
float enmyZeppelinAltitude = 10.0;
float enmyZepplinAngleZ = 0.0;
int enemyZeppelinState = 0;
float robotAngle = 0.0;
float shoulderAngle = -40.0;
float gunAngle = -25.0;

GLfloat enemyMoveSpeed = 0.5f;
GLfloat enemyRotateSpeed = 5.0f;

GLfloat gun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat gun_mat_diffuse[] = { 0.01f,0.0f,0.01f,0.01f };
GLfloat gun_mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat gun_mat_shininess[] = { 100.0F };

GLfloat robotLowerBody_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat robotLowerBody_mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat robotLowerBody_mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
GLfloat robotLowerBody_mat_shininess[] = { 76.8F };

GLfloat enemyZeppelinWorldX = 0.0f;
GLfloat enemyZeppelinWorldY = 0.0f;
GLfloat enemyZeppelinWorldZ = 0.0f;

float enemyZeppelinRotationAngle = 0.0f;

GLfloat light_position0[] = { -4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_position1[] = { 4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };


int currentButton;

QuadMesh *groundMesh = NULL;

typedef struct BoundingBox {
    VECTOR3D min;
	VECTOR3D max;
} BBox;

int meshSize = 16;

void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void animationHandler(int param);
void loadTexture();
void loadEnemyTexture();
void drawZeppelinOval();
void drawZeppelin();
void drawZeppelinGondola();
void drawZeppelinTailFins();
void drawSingleFin(GLfloat height);
void drawDriveShaft();
void drawSingleBlade();
void drawPropeller();
void timer(int value);
void drawGun();
void handleKeypress(unsigned char key, int x, int y);
void drawEnemyZeppelin();
void updateEnemyZeppelinPosition();
void updateEnemyZeppelinPosition(int param);
void turnEnemyZeppelin(int param);
void animateEnemyCrash(int value);

GLfloat zeppelinX = 0.0f;
GLfloat zeppelinZ = 0.0f;
GLfloat zeppelinY = 0.0f;
GLfloat moveSpeed = 0.5f;
float zeppelinRotationAngle = 0.0f;
int mainWindow, topDownWindow;

void idleFunction() {
    glutSetWindow(mainWindow);
    glutPostRedisplay();

    glutSetWindow(topDownWindow);
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(vWidth, vHeight);
    glutInitWindowPosition(200, 30);
    mainWindow = glutCreateWindow("3D Hierarchical Example");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeypress);

    // Initialize OpenGL settings for the main window
    initOpenGL(vWidth, vHeight);

    // Load textures for the main window
    loadTexture();
    loadEnemyTexture();

    // Create the top-down view window
    glutInitWindowSize(vWidth, vHeight);
    glutInitWindowPosition(200, 550);
    topDownWindow = glutCreateWindow("Top-Down View Window");

    glutDisplayFunc(topDownDisplay);
    glutReshapeFunc(topDownReshape);
    initOpenGLTopDown(vWidth, vHeight);
    loadTexture();
    loadEnemyTexture();
    glutIdleFunc(idleFunction);
    std::srand(static_cast<unsigned int>(std::time(NULL)));


    glutTimerFunc(0, timer, 0);
    glutTimerFunc(0, updateEnemyZeppelinPosition, 0);
    glutTimerFunc(2000, enemyShootTimer, 0);

    glutMainLoop();

    return 0;
}


GLfloat zeppelinOval_mat_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat zeppelinOval_mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat zeppelinOval_mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat zeppelinOval_mat_shininess[] = { 10.0 };

GLfloat zeppelinOvalWidth = 12.0f;
GLfloat zeppelinOvalLength = 4.0f;
GLfloat zeppelinOvalDepth = 4.0f;

void initOpenGL(int w, int h)
{
	// Set up and enable lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.4F, 0.4F, 0.4F, 0.0F);
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	VECTOR3D origin = VECTOR3D(-16.0f, 0.0f, 16.0f);
	VECTOR3D dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
	VECTOR3D dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
	groundMesh = new QuadMesh(meshSize, 32.0);
	groundMesh->InitMesh(meshSize, origin, 32.0, 32.0, dir1v, dir2v);

	VECTOR3D ambient = VECTOR3D(0.0f, 0.05f, 0.0f);
	VECTOR3D diffuse = VECTOR3D(0.4f, 0.8f, 0.4f);
	VECTOR3D specular = VECTOR3D(0.04f, 0.04f, 0.04f);
	float shininess = 0.2;
	groundMesh->SetMaterial(ambient, diffuse, specular, shininess);

}

void initOpenGLTopDown(int w, int h)
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.4F, 0.4F, 0.4F, 0.0F);
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set up ground quad mesh
	VECTOR3D origin = VECTOR3D(-16.0f, 0.0f, 16.0f);
	VECTOR3D dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
	VECTOR3D dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
	groundMesh = new QuadMesh(meshSize, 32.0);
	groundMesh->InitMesh(meshSize, origin, 32.0, 32.0, dir1v, dir2v);

	VECTOR3D ambient = VECTOR3D(0.0f, 0.05f, 0.0f);
	VECTOR3D diffuse = VECTOR3D(0.4f, 0.8f, 0.4f);
	VECTOR3D specular = VECTOR3D(0.04f, 0.04f, 0.04f);
	float shininess = 0.2;
	groundMesh->SetMaterial(ambient, diffuse, specular, shininess);

}

bool checkEnemyBulletCollision(Bullet bullet) {
    // Define the bounding box for the player's zeppelin
    float playerLeft = zeppelinX - zeppelinOvalWidth / 2;
    float playerRight = zeppelinX + zeppelinOvalWidth / 2;
    float playerBottom = zeppelinY - zeppelinOvalLength / 2;
    float playerTop = zeppelinY + zeppelinOvalLength / 2;
    float playerFront = zeppelinZ - zeppelinOvalDepth / 2;
    float playerBack = zeppelinZ + zeppelinOvalDepth / 2;

    // Check if bullet is within the bounding box of the player's zeppelin
    return (bullet.x >= playerLeft && bullet.x <= playerRight) &&
           (bullet.y >= playerBottom && bullet.y <= playerTop) &&
           (bullet.z >= playerFront && bullet.z <= playerBack);
}


void fireEnemyBullet() {
    Bullet newBullet;
    newBullet.x = enemyZeppelinWorldX;
    newBullet.y = enemyZeppelinWorldY;
    newBullet.z = enemyZeppelinWorldZ;
    newBullet.speed = 0.2f;
    newBullet.direction = enemyZeppelinRotationAngle;

    enemyBullets.push_back(newBullet);
}

void enemyShootTimer(int value) {
    fireEnemyBullet();
    glutTimerFunc(3000, enemyShootTimer, 0);
}


void loadTexture() {
    texture = SOIL_load_OGL_texture
    (   //Add your path to the provided texture jpg here
        "C:\\Users\\shaun\\Downloads\\blip_texture.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if(texture == 0) {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void loadEnemyTexture() {
    enemyTexture = SOIL_load_OGL_texture
    (
        //Add your path to the provided texture jpg here
        "C:\\Users\\shaun\\Downloads\\enemy_blip_texture.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if(enemyTexture == 0) {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
    }


    glBindTexture(GL_TEXTURE_2D, enemyTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void renderScene() {
    glPushMatrix();
    glTranslatef(0.0, -20.0, 0.0);
    groundMesh->DrawMesh(meshSize);
    glPopMatrix();

    // Draw the main Zeppelin
    if (!isUserZeppelinCrashing) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glPushMatrix();
        glTranslatef(zeppelinX, zeppelinY, zeppelinZ);
        glRotatef(zeppelinRotationAngle, 0.0f, 1.0f, 0.0f);
        drawZeppelin();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }


    // Draw the enemy Zeppelin
    if (enemyZeppelinState != 2) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, enemyTexture);
        glPushMatrix();
        glTranslatef(enemyZeppelinWorldX, enemyZeppelinWorldY, enemyZeppelinWorldZ);
        drawEnemyZeppelin();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    drawBlastParticles();
    updateBlastParticles();
    glDisable(GL_TEXTURE_2D);
    animateEnemyCrash(0);

    for (const Bullet& bullet : enemyBullets) {
    glPushMatrix();
    glTranslatef(bullet.x, bullet.y, bullet.z);
    glColor3f(1.0, 0.0, 0.0);
    glutSolidSphere(0.2, 10, 10);
    glPopMatrix();
}
    for (auto it = enemyBullets.begin(); it != enemyBullets.end(); ) {
    if (checkEnemyBulletCollision(*it)) {
        isUserZeppelinCrashing = true;
        userCrashStartTime = glutGet(GLUT_ELAPSED_TIME);
        it = enemyBullets.erase(it);
    } else {
        ++it;
    }
}
    std::vector<Bullet>::iterator it = bullets.begin();
    while (it != bullets.end()) {
        Bullet &bullet = *it;

        glPushMatrix();
        glTranslatef(bullet.x, bullet.y, bullet.z);
        glColor3f(1.0, 0.0, 0.0);
        glutSolidSphere(0.2, 10, 10);
        glPopMatrix();

        GLfloat radianAngle = bullet.direction * M_PI / 180.0;
        bullet.x += bullet.speed * sin(radianAngle);
        bullet.z += bullet.speed * cos(radianAngle);



        if (checkBulletCollision(bullet)) {
            printf("Enemy Zeppelin Hit!\n");
            enemyZeppelinHits++;
            if (enemyZeppelinHits >= 20 && enemyZeppelinState != 1) {
                enemyZeppelinState = 1;
                crashStartTime = glutGet(GLUT_ELAPSED_TIME);
            }

            it = bullets.erase(it);
        } else {
            ++it;
        }
    }


    glBindTexture(GL_TEXTURE_2D, 0);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    if (firstPersonView) {
        // Camera position adjustments for first-person view
        float cameraOffsetForward = 11.0; // Distance in front of the zeppelin
        float cameraHeight = 1.5; // Height offset from zeppelin center
        float radianAngle = -zeppelinRotationAngle * M_PI / 180.0;
        float cameraX = zeppelinX + cos(radianAngle) * cameraOffsetForward;
        float cameraZ = zeppelinZ + sin(radianAngle) * cameraOffsetForward;
        float lookAtX = zeppelinX + cos(radianAngle) * 100.0;
        float lookAtZ = zeppelinZ + sin(radianAngle) * 100.0;

        gluLookAt(cameraX, zeppelinY + cameraHeight, cameraZ, lookAtX, zeppelinY + cameraHeight, lookAtZ, 0.0, 1.0, 0.0);
    } else {
        // World View Camera settings
        gluLookAt(0.0, 6.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    }

    // Render the scene
    renderScene();

    glutSwapBuffers();

}


void animateUserCrash(int value) {
    const int totalCrashDuration = 40000; // Duration in milliseconds
    const float totalDescent = 10.0f; // Total descent distance
    const float yRotationSpeed = 30.0f; // Degrees per second for Y-axis rotation

    if (isUserZeppelinCrashing) {
        int elapsedTime = glutGet(GLUT_ELAPSED_TIME) - userCrashStartTime;
        if (elapsedTime < totalCrashDuration) {
            float progress = static_cast<float>(elapsedTime) / totalCrashDuration;

            zeppelinY -= progress * totalDescent / totalCrashDuration * 50;
            userCrashRotationY += yRotationSpeed * 50 / 1000;

            glutPostRedisplay();
            glutTimerFunc(50, animateUserCrash, 0);
        } else {
            isUserZeppelinCrashing = false;
        }
    }
}


void topDownDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 50.0, 0.0,
              0.0, 0.0, 0.0,
              0.0, 0.0, -1.0);

    renderScene();

    glutSwapBuffers();
}

void topDownReshape(int w, int h) {
    if (h == 0) h = 1;

    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



float smoothStep(float edge0, float edge1, float x) {
    x = std::max(0.0f, std::min(1.0f, (x - edge0) / (edge1 - edge0)));
    return x * x * (3 - 2 * x);
}



void animateEnemyCrash(int value) {
    const int totalCrashDuration = 40000; // Duration in milliseconds
    const float totalDescent = 10.0f; // Total descent distance
    const float yRotationSpeed = 30.0f; // Degrees per second for Y-axis rotation

    if (enemyZeppelinState == 1) {
        if (enemyZeppelinHits >= 20 && !particlesInitialized) {
            initBlastParticles(enemyZeppelinWorldX, enemyZeppelinWorldY, enemyZeppelinWorldZ);
            particlesInitialized = true;
        }

        int elapsedTime = glutGet(GLUT_ELAPSED_TIME) - crashStartTime;
        if (elapsedTime < totalCrashDuration) {
            float progress = static_cast<float>(elapsedTime) / totalCrashDuration;

            enemyZeppelinWorldY -= progress * totalDescent / totalCrashDuration * 50;
            enemyCrashRotationY += yRotationSpeed * 50 / 1000;

            glutPostRedisplay();
            glutTimerFunc(50, animateEnemyCrash, 0);
        } else {
            enemyZeppelinState = 2;
        }
    }
}


float targetAngle = 0.0;
bool isRotating = false;


bool checkBulletCollision(Bullet bullet) {
    float enemyLeft = enemyZeppelinWorldX - zeppelinOvalWidth / 2;
    float enemyRight = enemyZeppelinWorldX + zeppelinOvalWidth / 2;
    float enemyBottom = enemyZeppelinWorldY - zeppelinOvalLength / 2;
    float enemyTop = enemyZeppelinWorldY + zeppelinOvalLength / 2;
    float enemyFront = enemyZeppelinWorldZ - zeppelinOvalDepth / 2;
    float enemyBack = enemyZeppelinWorldZ + zeppelinOvalDepth / 2;

    return (bullet.x >= enemyLeft && bullet.x <= enemyRight) &&
           (bullet.y >= enemyBottom && bullet.y <= enemyTop) &&
           (bullet.z >= enemyFront && bullet.z <= enemyBack);
}

void updateEnemyZeppelinPosition(int param) {
    if (isRotating) {
        float angleDiff = fmod(targetAngle - enemyZeppelinRotationAngle + 360, 360);
        if (angleDiff > 180) {
            angleDiff -= 360;
        }

        if (fabs(angleDiff) < 1.0) {
            isRotating = false;
            enemyZeppelinRotationAngle = targetAngle;
        } else {
            enemyZeppelinRotationAngle += (angleDiff > 0 ? 1 : -1) * std::min(fabs(angleDiff), enemyRotateSpeed);
        }
    } else {
        float angleRadians = enemyZeppelinRotationAngle * (PI / 180.0);
        enemyZeppelinWorldX += enemyMoveSpeed * cos(angleRadians);
        enemyZeppelinWorldZ += enemyMoveSpeed * sin(angleRadians);

        int randomTurn = rand() % 50;
        if (enemyZeppelinWorldX <= enemyZeppelinXMin || enemyZeppelinWorldX >= enemyZeppelinXMax ||
            enemyZeppelinWorldZ <= enemyZeppelinZMin || enemyZeppelinWorldZ >= enemyZeppelinZMax || randomTurn == 0) {
            targetAngle = static_cast<float>(rand() % 360);
            isRotating = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(20, updateEnemyZeppelinPosition, 0);
}




void drawGun() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, gun_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gun_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, gun_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, gun_mat_shininess);

    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);

    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, -2.0);
    gluCylinder(quadric, 0.4, 0.4, 4.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, -1.8, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    gluCylinder(quadric, 0.4, 0.4, 4.0, 20, 20);
    glPopMatrix();

    gluDeleteQuadric(quadric);
}



void drawZeppelinOval() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, zeppelinOval_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, zeppelinOval_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, zeppelinOval_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, zeppelinOval_mat_shininess);

    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);

    glPushMatrix();
    glScalef(zeppelinOvalWidth, zeppelinOvalLength, zeppelinOvalDepth);
    gluSphere(quadric, 1.0, 20, 20);
    glPopMatrix();
    gluDeleteQuadric(quadric);
}


GLfloat fin_mat_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
GLfloat fin_mat_specular[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat fin_mat_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat fin_mat_shininess[] = { 10.0 };

GLfloat finWidth = 0.2f;
GLfloat finLength = 3.0f;
GLfloat finHeight = 2.5f;

void drawSingleFin(GLfloat height) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, fin_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, fin_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, fin_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, fin_mat_shininess);

    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);

    glPushMatrix();
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(finWidth, height, finLength);
    gluCylinder(quadric, 0.5, 0.0, 1.0, 10, 10);
    glPopMatrix();
    gluDeleteQuadric(quadric);
}

void drawZeppelinTailFins() {
    // Top FIn
    glPushMatrix();
    glTranslatef(0.75, zeppelinOvalLength/2 , 0);
    glRotatef(-180.0f, 0.0f, 0.0f, 1.0f);
    drawSingleFin(finHeight);
    glPopMatrix();

    // Bottom Fin
    glPushMatrix();
    glTranslatef(0.75, zeppelinOvalLength/2 - 4 , 0);
    glRotatef(-180.0f, 0.0f, 0.0f, 1.0f);
    drawSingleFin(finHeight);
    glPopMatrix();
}


GLfloat gondola_mat_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat gondola_mat_specular[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat gondola_mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat gondola_mat_shininess[] = { 10.0 };

GLfloat gondolaWidth = 6.0f;
GLfloat gondolaLength = 2.5f;


void drawZeppelinGondola() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, gondola_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, gondola_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, gondola_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, gondola_mat_shininess);

    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(gondolaWidth, gondolaLength, gondolaWidth);
    glutSolidCube(1.0);

    glPopMatrix();
}


GLfloat propeller_mat_ambient[] = { 0.1, 0.6, 0.1, 1.0 };
GLfloat propeller_mat_specular[] = { 0.3, 0.8, 0.3, 1.0 };
GLfloat propeller_mat_diffuse[] = { 0.2, 0.7, 0.2, 1.0 };
GLfloat propeller_mat_shininess[] = { 10.0 };

GLfloat bladeLength = 2.0f;
GLfloat bladeWidth = 0.3f;
GLfloat cylinderHeight = 1.0f;



//Propeller Timer
void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}


void drawZeppelinPropeller() {
    glMaterialfv(GL_FRONT, GL_AMBIENT, propeller_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, propeller_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, propeller_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, propeller_mat_shininess);

    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(glutGet(GLUT_ELAPSED_TIME) * 0.1f, 0.0f, 0.0f, 1.0f);

    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -cylinderHeight / 2);
    gluCylinder(quadric, 0.1, 0.1, cylinderHeight, 20, 20);
    glPopMatrix();

    //Blades
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glRotatef(i * 90.0f, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, bladeLength / 2, 0.0f);
        glScalef(bladeWidth, bladeLength, 0.05f);
        glutSolidCube(1.0);
        glPopMatrix();
    }
    gluDeleteQuadric(quadric);
    glPopMatrix();
}

void drawEnemyZeppelin() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, enemyTexture);

    glPushMatrix();
    glTranslatef(enemyZeppelinWorldX, enemyZeppelinWorldY, enemyZeppelinWorldZ);
    glRotatef(enemyZeppelinRotationAngle, 0.0f, 1.0f, 0.0f);
    glRotatef(enemyCrashRotationY, 0.0f, 1.0f, 0.0f);
    // Main Base body of the Enemy Zeppelin
    drawZeppelinOval();

    // Enemy Zeppelin Propeller
    glPushMatrix();
    glTranslatef(-zeppelinOvalWidth, 0.0, 0.0);
    drawZeppelinPropeller();
    glPopMatrix();

    // Tail Fins of the Enemy Zeppelin
    glPushMatrix();
    glTranslatef(-zeppelinOvalWidth, 0.0, 0.0);
    drawZeppelinTailFins();
    glPopMatrix();

    // Gondola of the Enemy Zeppelin
    glPushMatrix();
    glTranslatef(0.0f, -(zeppelinOvalLength / 2 + gondolaLength / 3), 0.0f);
    drawZeppelinGondola();
    glPopMatrix();

    // Gondola Propeller of the Enemy Zeppelin
    glPushMatrix();
    glTranslatef(-(zeppelinOvalLength / 2 + gondolaLength) + 0.8 , -(zeppelinOvalLength / 2 + gondolaLength / 3) - 1.5, 0.0);
    drawZeppelinPropeller();
    glPopMatrix();

    // Gun of the Enemy Zeppelin
    glPushMatrix();
    glTranslatef(0.0, -zeppelinOvalLength, 0.0); // Position at the bottom of the zeppelin
    drawGun();
    glPopMatrix();

    glPopMatrix(); // End of enemy zeppelin transformations
    glDisable(GL_TEXTURE_2D);
}

struct Particle {
    float x, y, z;
    float vx, vy, vz;
    float life;
};
std::vector<Particle> blastParticles;


void initBlastParticles(float x, float y, float z) {
    const int numParticles = 100;
    blastParticles.clear();

    for (int i = 0; i < numParticles; ++i) {
        Particle p;
        p.x = x;
        p.y = y;
        p.z = z;
        float speed = 0.05f * static_cast<float>(rand()) / RAND_MAX;
        float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * PI;
        float elevation = static_cast<float>(rand()) / RAND_MAX * PI - PI / 2.0f;
        p.vx = speed * cos(angle) * cos(elevation);
        p.vy = speed * sin(elevation);
        p.vz = speed * sin(angle) * cos(elevation);
        p.life = 2.0f;
        blastParticles.push_back(p);
    }
}

void updateBlastParticles() {
    for (auto& p : blastParticles) {
        p.x += p.vx;
        p.y += p.vy;
        p.z += p.vz;
        p.life -= 0.01f;
    }
    blastParticles.erase(std::remove_if(blastParticles.begin(), blastParticles.end(),
                                        [](const Particle& p) { return p.life <= 0; }),
                         blastParticles.end());

    for (Bullet& bullet : enemyBullets) {
    GLfloat radianAngle = bullet.direction * M_PI / 180.0;
    bullet.x += bullet.speed * sin(radianAngle);
    bullet.z += bullet.speed * cos(radianAngle);
}

}


void drawBlastParticles() {
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (const auto& p : blastParticles) {
        glColor3f(1.0f, 0.5f, 0.0f);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
}

void drawZeppelin() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Main Base body of the Zeppelin
    drawZeppelinOval();

    //Zeppelin Propeller
    glPushMatrix();
    glTranslatef(-zeppelinOvalWidth, 0.0, 0.0);
    drawZeppelinPropeller();
    glPopMatrix();

    // Tail Fins
    glPushMatrix();
    glTranslatef(-zeppelinOvalWidth, 0.0, 0.0);
    drawZeppelinTailFins();
    glPopMatrix();

    // Gondola
    glPushMatrix();
    glTranslatef(0.0f, -(zeppelinOvalLength / 2 + gondolaLength / 3), 0.0f);
    drawZeppelinGondola();
    glPopMatrix();

    // Gondola Propeller
    glPushMatrix();
    glTranslatef(-(zeppelinOvalLength / 2 + gondolaLength) + 0.8 , -(zeppelinOvalLength / 2 + gondolaLength / 3)- 1.5 , 0.0);
    drawZeppelinPropeller();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.0, -zeppelinOvalLength, 0.0);
    drawGun();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}



void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 'd':  // Rotate Left
            zeppelinRotationAngle -= 5.0f;
            break;
        case 'a':  // Rotate Right
            zeppelinRotationAngle += 5.0f;
            break;
        case 'q':  // Move Away From screen
            zeppelinZ -= moveSpeed;
            break;
        case 'e':  // Move towards screen
            zeppelinZ += moveSpeed;
            break;
        case 's':  // Move backward
            zeppelinX -= moveSpeed;
            break;
        case 'w':  // Move forward
            zeppelinX += moveSpeed;
            break;
        case 'r':  // Move Up
            zeppelinY += moveSpeed;
            break;
        case 'f':  // Move Down
            zeppelinY -= moveSpeed;
            break;
        case 'v':  // Toggle between FPV and world view
            firstPersonView = !firstPersonView;
            break;
        case 'p': {
            GLfloat gunOffsetLength = 0.0;
            GLfloat gunOffsetHeight = 6.0;
            GLfloat radianAngle = zeppelinRotationAngle * M_PI / 180.0;

            GLfloat bulletDirection = zeppelinRotationAngle + 90.0f;
            GLfloat bulletRadianAngle = bulletDirection * M_PI / 180.0;

            Bullet newBullet;
            newBullet.x = zeppelinX + (gunOffsetLength * cos(bulletRadianAngle));
            newBullet.y = zeppelinY - gunOffsetHeight;
            newBullet.z = zeppelinZ + (gunOffsetLength * sin(bulletRadianAngle));
            newBullet.speed = 0.2f;
            newBullet.direction = bulletDirection;

            bullets.push_back(newBullet);
            break;
        }
        default:
            break;
    }
    glutPostRedisplay();
}


void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / h, 0.2, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void functionKeys(int key, int x, int y)
{
	// Help key
	if (key == GLUT_KEY_F1)
	{

	}
	// Do transformations with arrow keys
	//else if (...)   // GLUT_KEY_DOWN, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_LEFT
	//{
	//}

	glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse button callback - use only if you want to
void mouse(int button, int state, int x, int y)
{
	currentButton = button;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			;

		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			;
		}
		break;
	default:
		break;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse motion callback - use only if you want to
void mouseMotionHandler(int xMouse, int yMouse)
{
	if (currentButton == GLUT_LEFT_BUTTON)
	{
		;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}

