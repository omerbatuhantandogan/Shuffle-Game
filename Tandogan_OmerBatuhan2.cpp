/*********
   CTIS164 - Template Source Program
----------
STUDENT : Omer Batuhan Tandogan
SECTION : 1
HOMEWORK: 2
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:

            S key SHUFFLES THE LINE SEGMENTS


*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  1000 // Period for the timer.
#define TIMER_ON         0 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {
    int r, g, b;       //Struct of color
}color_t;


typedef struct{
    float x1=-9876, y1= -9876, x2= -9876, y2= -9876;  //Struct of line segment
}segment_t;

segment_t all[10]; segment_t first;   //F1-F2 segment and others
int lscount = 0;  //Count of segment
int m = -123;   //F1 F2 F3

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

//Griding, name, surname, id number
void staticPlane() {
    glColor3f(0.2, 0.2, 0.2);
    glRectf(-500, 400, 500, -400);

    glLineWidth(2);
    glBegin(GL_LINES);
    for (int i = 500; i >= -500; i -= 25) {
        glColor3ub(41, 99, 99);
        glVertex2f(i, 400);
        glVertex2f(i, -400);
        glColor3ub(43, 99, 41);
        glVertex2f(500, i);
        glVertex2f(-500, i);
    }
    glEnd();
    glLineWidth(1);

    glColor3f(0.1, 0.1, 0.1);
    glRectf(-500, 400, 500, 350);
    glRectf(-500, -400, 500, -350);

    glColor3f(1, 1, 1);
    vprint(-500, 370, GLUT_BITMAP_TIMES_ROMAN_24, "Omer Batuhan TANDOGAN - 21803754");
    vprint(200, 370, GLUT_BITMAP_9_BY_15, "HW #2 | 2020/2021 Spring");
    vprint(-250, -380, GLUT_BITMAP_TIMES_ROMAN_24, "F1 sets start, F2 sets end point, F3 adds line segments, S shuffles lines");
}

//Start and end points of both main segment and others.
void buttons() {
    glColor3f(0, 0, 1);
    for (int i = 0; i < 10; i++) {
        if (all[i].x1 != -9876) {
            circle(all[i].x1, all[i].y1, 5);
        }
        if (all[i].x2 != -9876) {
            circle(all[i].x2, all[i].y2, 5);
        }
    }
    if (first.x1 != -9876) {
        glColor3f(0, 0, 1);
        circle(first.x1, first.y1, 15);
        glColor3f(1, 1, 0);
        circle(first.x1, first.y1, 10);
        glColor3f(0, 0, 0);
        vprint(first.x1 - 5, first.y1 - 5, GLUT_BITMAP_HELVETICA_18, "S");
    }

    if (first.x2 != -9876) {
        glColor3f(0, 0, 1);
        circle(first.x2, first.y2, 15);
        glColor3f(1, 1, 0);
        circle(first.x2, first.y2, 10);
        glColor3f(0, 0, 0);
        vprint(first.x2 - 5, first.y2 - 5, GLUT_BITMAP_HELVETICA_18, "E");
    }

    glColor4ub(255, 255, 0, 150);
    switch (m) {
    case 1:    
        circle(-237, -370, 13);
        break;
    case 2:
        circle(-103, -370, 13);
        break;
    case 3:
        circle(76, -370, 13);
        break;
    }

}

//Segments according to specificied color
void lineSegment(int x1, int y1, int x2, int y2,color_t color) {
    glColor3ub(color.r, color.g, color.b);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    glLineWidth(1);
}

//Plain segment
void lineSegmentMain(int x1, int y1, int x2, int y2) {
    glColor3ub(0,0,0);
    glLineWidth(4);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    glLineWidth(1);
}


//Color of segments according the existence of intersection
color_t cmpIntersection(segment_t segment1, segment_t segment2) {
    float t1 = (((segment1.x2 - segment1.x1) * (segment1.y1 - segment2.y1)) - ((segment1.y2 - segment1.y1) * (segment1.x1 - segment2.x1))) / (((segment1.x2 - segment1.x1) * (segment2.y2 - segment2.y1)) - ((segment2.x2 - segment2.x1) * (segment1.y2 - segment1.y1)));

    float t2 = (((segment2.x2 - segment2.x1) * (segment1.y1 - segment2.y1)) - ((segment1.x1 - segment2.x1) * (segment2.y2 - segment2.y1))) / (((segment1.x2 - segment1.x1) * (segment2.y2 - segment2.y1)) - ((segment2.x2 - segment2.x1) * (segment1.y2 - segment1.y1)));

    if (t1 <= 1 && t2 <= 1 && t1 >= 0 && t2 >= 0)
        return { 255,0,0 };
    else
        return { 0,255,0 };
}


//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    staticPlane();
    buttons();
    for (int i = 0; i < lscount; i++)
        lineSegment(all[i].x1, all[i].y1, all[i].x2, all[i].y2, cmpIntersection(all[i], first));
    if (first.x1 != -9876 && first.x2 != -9876)
        lineSegmentMain(first.x1, first.y1, first.x2, first.y2);
    
    //Display functions above

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == 's' || key == 'S') {   ///Shuffle line segments

        for (int i = 0; i < lscount; i++) {
            bool fromStart = rand() % 2 == 0 ? true : false;
            if (fromStart) {
                all[i].x1 = -500 + (rand() % 1001);
                all[i].y1 = -400 + (rand() % 801);
            }
            else {
                all[i].x2 = -500 + (rand() % 1001);
                all[i].y2 = -400 + (rand() % 801);
            }
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    case GLUT_KEY_F1: m = 1; break;
    case GLUT_KEY_F2: m = 2; break;  //F1 F2 F3
    case GLUT_KEY_F3: m = 3; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    x = x - winWidth / 2;
    y = winHeight / 2 - y;

    //Conversion above, clicking things below

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
        printf("%d %d\n", x, y);
        switch (m) {
            case 1:
                first.x1 = x;
                first.y1 = y;
                break;
            case 2:
                first.x2 = x;
                first.y2 = y;
                break;
            case 3:
                if (lscount < 10) {
                    if (all[lscount].x1 == -9876) {
                        all[lscount].x1 = x;
                        all[lscount].y1 = y;
                    }
                    else {
                        all[lscount].x2 = x;
                        all[lscount].y2 = y;
                        lscount++;
                    }
                }
                break;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Hw2 - Line segment intersections on 2D plane by Omer Batuhan Tandogan");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}
