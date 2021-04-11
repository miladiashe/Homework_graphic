/* simple painting program with text, lines, triangles,
rectangles, and points */

#define NULL 0
#define LINE 1
#define RECTANGLE 2
#define TRIANGLE 3
#define PPOINTSS 4
#define TEXTT 5

/*
추가한부분 
*/

#define CIRCLE 6
#define OPPOINT 7

#include <GL/freeglut.h>
#include <stdlib.h>

void mymouse(int, int, int, int);
void key(unsigned char, int, int);

void drawSquare(int, int);
void myReshape(GLsizei, GLsizei);
void mydisplay(void);
void myinit();

/*
* 내가추가한함수
*/

int HWpick(int, int);
void HWmydisplay(void);

void screen_box(int, int, int);
void right_menu(int);
void middle_menu(int);
void color_menu(int);
void pixel_menu(int);
void fill_menu(int);
int pick(int, int);


/* globals */

GLsizei wh = 500, ww = 500; /* initial window size */
GLfloat size = 3.0;   /* half side length of square */
int draw_mode = 0; /* drawing mode */
int rx, ry; /*raster position*/

GLfloat r = 1.0, g = 1.0, b = 1.0; /* drawing color */
int fill = 0; /* fill flag */


/*점 찍는  함수였다...*/
void drawSquare(int x, int y)
{
	y = wh - y;
	glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);
	glBegin(GL_POLYGON);
	glVertex2f(x + size, y + size);
	glVertex2f(x - size, y + size);
	glVertex2f(x - size, y - size);
	glVertex2f(x + size, y - size);
	glEnd();
}

/* reshaping routine called whenever window is resized or moved */

void myReshape(GLsizei w, GLsizei h)
{

	/* adjust clipping box */

	glMatrixMode(GL_PROJECTION); /*투영*/
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);/*직교 관측을 하겠다, 세팅은 정해진 범주 없음. 이 값 내에 들어있는 부분만 보인다.*/
	/*glOrtho: 가시범위 설정*/
	glMatrixMode(GL_MODELVIEW);
	/*모델 뷰 모드에서 그림을 그리는 등의 액션을 취한다.*/
	glLoadIdentity();

	/* adjust viewport and clear */

	glViewport(0, 0, w, h);
	/*그림 그릴 창의 위치를 조정*/
	glClearColor((GLclampf)0.8, (GLclampf)0.8, (GLclampf)0.8, (GLclampf)1.0);
	/*정해진 색으로 배경을 채운다.*/
	glClear(GL_COLOR_BUFFER_BIT);
	/*버퍼 깨끗하게*/

	/* set global size for use by drawing routine */

	ww = w;
	wh = h;
}

void myinit()/*맨 처음에 시작하는것. reshape와 비슷하지만 처음에하는것*/
{

	glViewport(0, 0, ww, wh);
	/*초기값 세팅값으로 불러온다.*/


	/* Pick 2D clipping window to match size of X window. This choice
	avoids having to scale object coordinates each time window is
	resized. */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)ww, 0.0, (GLdouble)wh, -1.0, 1.0);

	/* set clear color to black and clear window */

	glClearColor((GLclampf)0.8, (GLclampf)0.8, (GLclampf)0.8, (GLclampf)1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void mymouse(int btn, int state, int x, int y)/*마우스갖고 하는건 다 여기들어감. 어디를 클릭하느냐에 따라 액션이 달라진다!*/
{
	static int count;
	int where;/*pick에서 들어온 값*/
	static int xp[2], yp[2];
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)/*마우스 왼쪽을 눌러야한다*/
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		where = HWpick(x, y);/*어디를 눌렀는가*/
		glColor3f(r, g, b);
		if (where != 0)/*0이 아니면 메뉴를 선택한 것이다.*/
		{
			count = 0;/*카운트를 0으로 만들어줌*/
			draw_mode = where;
		}
		else switch (draw_mode)/*캔버스를 누른 것. 선택한 그림모드에 따라 동작이 다르다.*/
		{
		case(LINE):/*카운트 0이고 mode는 line이다.*/
			if (count == 0)/*카운트가 0일 때*/
			{
				count++;
				xp[0] = x;
				yp[0] = y;
			}
			else
			{
				glBegin(GL_LINES);
				glVertex2i(x, wh - y);
				glVertex2i(xp[0], wh - yp[0]);
				glEnd();
				draw_mode = 0;/*안그리는 모드로 되돌리기*/
				count = 0;
			}
			break;


		case(RECTANGLE):
			if (count == 0)
			{
				count++;
				xp[0] = x;
				yp[0] = y;
			}
			else
			{
				if (fill) glBegin(GL_POLYGON);/*색칠여부*/
				else glBegin(GL_LINE_LOOP);
				glVertex2i(x, wh - y);
				glVertex2i(x, wh - yp[0]);
				glVertex2i(xp[0], wh - yp[0]);
				glVertex2i(xp[0], wh - y);
				glEnd();
				draw_mode = 0;
				count = 0;
			}
			break;
		case (TRIANGLE):
			switch (count)
			{
			case(0):
				count++;
				xp[0] = x;
				yp[0] = y;
				break;
			case(1):
				count++;
				xp[1] = x;
				yp[1] = y;
				break;
			case(2):
				if (fill) glBegin(GL_POLYGON);
				else glBegin(GL_LINE_LOOP);
				glVertex2i(xp[0], wh - yp[0]);
				glVertex2i(xp[1], wh - yp[1]);
				glVertex2i(x, wh - y);
				glEnd();
				draw_mode = 0;
				count = 0;
			}
			break;
		case(PPOINTSS):
		{
			drawSquare(x, y);
			count++;
		}
		break;
		case(TEXTT):
		{
			rx = x;
			ry = wh - y;
			glRasterPos2i(rx, ry);
			count = 0;
		}
		/*내가추가함*/
		case(CIRCLE):
			if (count == 0)
			{
				count++;
				xp[0] = x;
				yp[0] = y;
			}
			else
			{
				if (fill) glBegin(GL_POLYGON);
				else glBegin(GL_LINE_LOOP);
				glVertex2i(x, wh - y);
				glVertex2i(x, wh - yp[0]);
				glVertex2i(xp[0], wh - yp[0]);
				glVertex2i(xp[0], wh - y);
				glEnd();
				draw_mode = 0;
				count = 0;
			}
			break;
		}   /* endelse*/
	}/*endif*/
	glPopAttrib();
	glFlush();
}

int pick(int x, int y)/*메뉴 위치 지정*/
{
	y = wh - y;
	if (y < wh - ww / 10) return 0;
	else if (x < ww / 10) return LINE;/*10분의 1*/
	else if (x < ww / 5) return RECTANGLE;/*10분의 2*/
	else if (x < 3 * ww / 10) return TRIANGLE;/*10분의 3*/
	else if (x < 2 * ww / 5) return PPOINTSS;/*10분의 4*/
	else if (x < ww / 2) return TEXTT;/*10분의 5*/
	else return 0;
}

int HWpick(int x, int y)/*세로 메뉴로 변경*/
{
	/*y = wh - y;*/
	if (x < ww - wh / 10) return 0;
	else if (y < wh / 10) return LINE;/*10분의 1*/
	else if (y < wh / 5) return RECTANGLE;/*10분의 2*/
	else if (y < 3 * wh / 10) return TRIANGLE;/*10분의 3*/
	else if (y < 2 * wh / 5) return PPOINTSS;/*10분의 4*/
	else if (y < wh / 2) return TEXTT;/*10분의 5*/
	else return 0;
}

void HWmydisplay(void)
{
	int shift = 0;

	glColor3f(1.0, 1.0, 1.0);/*직선*/
	screen_box(ww - wh / 10, wh- (wh/10), wh / 10);
	glColor3f(1.0, 0.0, 0.0);/*네모*/
	screen_box(ww - wh / 10, wh - (wh / 5), wh / 10);
	glColor3f(0.0, 1.0, 0.0);
	screen_box(ww - wh / 10, wh - (3*wh / 10), wh / 10);
	glColor3f(0.0, 0.0, 1.0);
	screen_box(ww - wh / 10, wh - (2*wh / 5), wh / 10);
	glColor3f(1.0, 1.0, 0.0);
	screen_box(ww - wh / 10, wh - (wh/2), wh / 10);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(wh / 40, wh - ww / 20);
	glVertex2i(wh / 40 + ww / 20, wh - ww / 20);
	glEnd();

	screen_box(ww / 10 + ww / 40, wh - ww / 10 + ww / 40, ww / 20);

	glBegin(GL_TRIANGLES);
	glVertex2i(ww / 5 + ww / 40, wh - ww / 10 + ww / 40);
	glVertex2i(ww / 5 + ww / 20, wh - ww / 40);
	glVertex2i(ww / 5 + 3 * ww / 40, wh - ww / 10 + ww / 40);
	glEnd();
	glPointSize(3.0);
	glBegin(GL_POINTS);
	glVertex2i(3 * ww / 10 + ww / 20, wh - ww / 20);
	glEnd();
	glRasterPos2i(2 * ww / 5, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
	shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
	glRasterPos2i(2 * ww / 5 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'B');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'B');
	glRasterPos2i(2 * ww / 5 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'C');
	glFlush();

}

void screen_box(int x, int y, int s)
{
	glBegin(GL_QUADS);
	glVertex2i(x, y);
	glVertex2i(x + s, y);
	glVertex2i(x + s, y + s);
	glVertex2i(x, y + s);
	glEnd();
}

void right_menu(int id)
{
	if (id == 1) exit(0);
	else
	{
		glClearColor((GLclampf)0.8, (GLclampf)0.8, (GLclampf)0.8, (GLclampf)1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		mydisplay();
	}
}

void middle_menu(int id)
{

}

void color_menu(int id)
{
	if (id == 1) { r = 1.0; g = 0.0; b = 0.0; }
	else if (id == 2) { r = 0.0; g = 1.0; b = 0.0; }
	else if (id == 3) { r = 0.0; g = 0.0; b = 1.0; }
	else if (id == 4) { r = 0.0; g = 1.0; b = 1.0; }
	else if (id == 5) { r = 1.0; g = 0.0; b = 1.0; }
	else if (id == 6) { r = 1.0; g = 1.0; b = 0.0; }
	else if (id == 7) { r = 1.0; g = 1.0; b = 1.0; }
	else if (id == 8) { r = 0.0; g = 0.0; b = 0.0; }
}

void pixel_menu(int id)
{
	if (id == 1) size = 2 * size;
	else if (size > 1) size = size / 2;
}

void fill_menu(int id)
{
	if (id == 1) fill = 1;
	else fill = 0;
}

void key(unsigned char k, int xx, int yy)
{
	if (draw_mode != TEXTT) return;
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2i(rx, ry);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, k);
	/*glutStrokeCharacter(GLUT_STROKE_ROMAN,i); */
	rx += glutBitmapWidth(GLUT_BITMAP_9_BY_15, k);

}

void mydisplay(void)
{
	int shift = 0;

	glColor3f(1.0, 1.0, 1.0);
	screen_box(0, wh - ww / 10, ww / 1);
	glColor3f(1.0, 0.0, 0.0);
	screen_box(ww / 10, wh - ww / 10, ww / 10);
	glColor3f(0.0, 1.0, 0.0);
	screen_box(ww / 5, wh - ww / 10, ww / 10);
	glColor3f(0.0, 0.0, 1.0);
	screen_box(3 * ww / 10, wh - ww / 10, ww / 10);
	glColor3f(1.0, 1.0, 0.0);
	screen_box(2 * ww / 5, wh - ww / 10, ww / 10);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(wh / 40, wh - ww / 20);
	glVertex2i(wh / 40 + ww / 20, wh - ww / 20);
	glEnd();

	screen_box(ww / 10 + ww / 40, wh - ww / 10 + ww / 40, ww / 20);

	glBegin(GL_TRIANGLES);
	glVertex2i(ww / 5 + ww / 40, wh - ww / 10 + ww / 40);
	glVertex2i(ww / 5 + ww / 20, wh - ww / 40);
	glVertex2i(ww / 5 + 3 * ww / 40, wh - ww / 10 + ww / 40);
	glEnd();
	glPointSize(3.0);
	glBegin(GL_POINTS);
	glVertex2i(3 * ww / 10 + ww / 20, wh - ww / 20);
	glEnd();
	glRasterPos2i(2 * ww / 5, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
	shift = glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'A');
	glRasterPos2i(2 * ww / 5 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'B');
	shift += glutBitmapWidth(GLUT_BITMAP_9_BY_15, 'B');
	glRasterPos2i(2 * ww / 5 + shift, wh - ww / 20);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'C');
	glFlush();

}


int main(int argc, char** argv)
{
	int c_menu, p_menu, f_menu;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("square");
	c_menu = glutCreateMenu(color_menu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Cyan", 4);
	glutAddMenuEntry("Magenta", 5);
	glutAddMenuEntry("Yellow", 6);
	glutAddMenuEntry("White", 7);
	glutAddMenuEntry("Black", 8);
	p_menu = glutCreateMenu(pixel_menu);
	glutAddMenuEntry("increase pixel size", 1);
	glutAddMenuEntry("decrease pixel size", 2);
	f_menu = glutCreateMenu(fill_menu);
	glutAddMenuEntry("fill on", 1);
	glutAddMenuEntry("fill off", 2);
	glutCreateMenu(right_menu);
	glutAddMenuEntry("quit", 1);
	glutAddMenuEntry("clear", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutCreateMenu(middle_menu);
	glutAddSubMenu("Colors", c_menu);
	glutAddSubMenu("Pixel Size", p_menu);
	glutAddSubMenu("Fill", f_menu);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	myinit();
	/*시작할 때 한 번 부름*/
	glutReshapeFunc(myReshape);
	/*윈도우 사이즈가 바뀔 때 myReshape 함수를 부른다*/
	glutKeyboardFunc(key);/*마찬가지겠지*/
	glutMouseFunc(mymouse);
	/*마우스가 뭔가 발생하면 mymouse 함수를 부른다.*/
	glutDisplayFunc(HWmydisplay);
	/*디스플레이에 관한 이벤트? 윈도우가 하나라도 열려지면 최소한 한 번은 불러진다. 
	첫 화면을 그리는 함수.*/
	glutMainLoop();
	return 0;
}
