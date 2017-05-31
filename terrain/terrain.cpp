#include <stdio.h>
#include <GL/glut.h>
#include <vector>
#include <map>
#include <iostream>
#include <time.h>
#include <math.h>
#define ROW 10
#define COL 10
#define PI 3.1415926

using namespace std;

struct vec2f
{
	float x;
	float y;

};

struct vec3f
{
	float x;
	float y;
	float z;

};

vec3f Vertex[ROW][COL];
vec3f Normal[ROW][COL];
vec2f Texture[ROW][COL];
float lx = 1.8;
float ly = 1.8;

vec3f CalculateNormal(vec3f v1, vec3f v2, vec3f v3)
{
	vec3f e1,e2,n;
	e1.x = v2.x - v1.x; e1.y = v2.y - v1.y; e1.z = v2.z - v1.z;
	e2.x = v3.x - v1.x; e2.y = v3.y - v1.y; e2.z = v3.z - v1.z;

	n.x = e1.y*e2.z - e2.y*e1.z;
	n.y = e1.z*e2.x - e2.z*e1.x;
	n.z = e1.x*e2.y - e2.x*e1.y;

	/////////normalize

	float len = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
	if (len > 1.0e-8)
	{
		n.x = -n.x / len;
		n.y = -n.y / len;
		n.z = -n.z / len;
	}

	return n;
}

void terraininit(void)
{
	srand((unsigned)time(NULL));
	float sx = 0.0;
	float sy = 0.0;
	float sz = 0.0;

	float dx = lx / (ROW - 1);
	float dy = ly / (COL - 1);

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			Vertex[i][j].x = sx + dx * (i-(ROW-1.0)/2.0);
			Vertex[i][j].y = sy + dy * (j-(COL-1.0)/2.0);

			float h = sqrt(Vertex[i][j].x*Vertex[i][j].x + Vertex[i][j].y*Vertex[i][j].y);
			
			Vertex[i][j].z = rand() / (RAND_MAX + 1.0) * 0.25; /*0.125*sin((i*ROW+j)*PI/4);*/

			/*Texture[i][j].x = ROW*(float)i / (ROW - 1.0);
			Texture[i][j].y = COL*(float)j / (COL - 1.0);*/
		}
	}

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (i == 0)
			{
				if (j == 0)
				{
					vec3f nt = CalculateNormal(Vertex[i][j], Vertex[i + 1][j], Vertex[i][j + 1]);
					Normal[i][j].x = nt.x;
					Normal[i][j].y = nt.y;
					Normal[i][j].z = nt.z;
				}

				else if (j == COL - 1)
				{
					vec3f nt1 = CalculateNormal(Vertex[i][j-1], Vertex[i+1][j-1], Vertex[i][j]);
					vec3f nt2 = CalculateNormal(Vertex[i][j], Vertex[i+1][j-1], Vertex[i+1][j]);
					Normal[i][j].x = nt1.x + nt2.x;
					Normal[i][j].y = nt1.y + nt2.y;
					Normal[i][j].z = nt1.z + nt2.z;
				}
				else
				{
					vec3f nt1 = CalculateNormal(Vertex[i][j - 1], Vertex[i + 1][j - 1], Vertex[i][j]);
					vec3f nt2 = CalculateNormal(Vertex[i][j], Vertex[i + 1][j - 1], Vertex[i + 1][j]);
					vec3f nt3 = CalculateNormal(Vertex[i][j], Vertex[i + 1][j], Vertex[i][j+1]);
					Normal[i][j].x = nt1.x + nt2.x + nt3.x;
					Normal[i][j].y = nt1.y + nt2.y + nt3.y;
					Normal[i][j].z = nt1.z + nt2.z + nt3.z;
				}
			}
			else if (i = ROW -1)
			{
				if (j == 0)
				{
					vec3f nt1 = CalculateNormal(Vertex[i-1][j], Vertex[i][j], Vertex[i-1][j+1]);
					vec3f nt2 = CalculateNormal(Vertex[i-1][j+1], Vertex[i][j], Vertex[i][j+1]);
					Normal[i][j].x = nt1.x + nt2.x;
					Normal[i][j].y = nt1.y + nt2.y;
					Normal[i][j].z = nt1.z + nt2.z;
				}
				else if (j == COL-1)
				{
					vec3f nt = CalculateNormal(Vertex[i-1][j], Vertex[i][j-1], Vertex[i][j]);
					Normal[i][j].x = nt.x;
					Normal[i][j].y = nt.y;
					Normal[i][j].z = nt.z;
				}
				else
				{
					vec3f nt1 = CalculateNormal(Vertex[i - 1][j], Vertex[i][j - 1], Vertex[i][j]);
					vec3f nt2 = CalculateNormal(Vertex[i - 1][j], Vertex[i][j], Vertex[i - 1][j + 1]);
					vec3f nt3 = CalculateNormal(Vertex[i - 1][j + 1], Vertex[i][j], Vertex[i][j + 1]);
					Normal[i][j].x = nt1.x + nt2.x + nt3.x;
					Normal[i][j].y = nt1.y + nt2.y + nt3.y;
					Normal[i][j].z = nt1.z + nt2.z + nt3.z;
				}
			}
			else
			{
				if (j == 0)
				{
					vec3f nt1 = CalculateNormal(Vertex[i - 1][j], Vertex[i][j], Vertex[i - 1][j + 1]);
					vec3f nt2 = CalculateNormal(Vertex[i - 1][j + 1], Vertex[i][j], Vertex[i][j + 1]);
					vec3f nt3 = CalculateNormal(Vertex[i][j], Vertex[i + 1][j], Vertex[i][j + 1]);
					Normal[i][j].x = nt1.x + nt2.x + nt3.x;
					Normal[i][j].y = nt1.y + nt2.y + nt3.y;
					Normal[i][j].z = nt1.z + nt2.z + nt3.z;
				}
				else if (j == COL - 1)
				{

					vec3f nt1 = CalculateNormal(Vertex[i - 1][j], Vertex[i][j - 1], Vertex[i][j]);
					vec3f nt2 = CalculateNormal(Vertex[i][j], Vertex[i][j - 1], Vertex[i + 1][j - 1]);
					vec3f nt3 = CalculateNormal(Vertex[i][j], Vertex[i + 1][j - 1], Vertex[i + 1][j]);
					Normal[i][j].x = nt1.x + nt2.x + nt3.x;
					Normal[i][j].y = nt1.y + nt2.y + nt3.y;
					Normal[i][j].z = nt1.z + nt2.z + nt3.z;
				}
				else
				{

					vec3f nt1 = CalculateNormal(Vertex[i - 1][j], Vertex[i][j], Vertex[i - 1][j + 1]);
					vec3f nt2 = CalculateNormal(Vertex[i - 1][j + 1], Vertex[i][j], Vertex[i][j + 1]);
					vec3f nt3 = CalculateNormal(Vertex[i][j], Vertex[i + 1][j], Vertex[i][j + 1]);
					vec3f nt4 = CalculateNormal(Vertex[i - 1][j], Vertex[i][j - 1], Vertex[i][j]);
					vec3f nt5 = CalculateNormal(Vertex[i][j], Vertex[i][j - 1], Vertex[i + 1][j - 1]);
					vec3f nt6 = CalculateNormal(Vertex[i][j], Vertex[i + 1][j - 1], Vertex[i + 1][j]);
					Normal[i][j].x = nt1.x + nt2.x + nt3.x;
					Normal[i][j].y = nt1.y + nt2.y + nt3.y;
					Normal[i][j].z = nt1.z + nt2.z + nt3.z;
				}
			}
			float len = sqrt(Normal[i][j].x*Normal[i][j].x + Normal[i][j].y*Normal[i][j].y + Normal[i][j].z*Normal[i][j].z);
			if (len > 1.0e-8)
			{
				Normal[i][j].x /= len;
				Normal[i][j].y /= len;
				Normal[i][j].z /= len;
			}
		}
	}
	
}

void drawAxis(void)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);   ///x axis
	glVertex3f(50.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);   ///y axis
	glVertex3f(0.0, 50.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);   ///z axis
	glVertex3f(0.0, 0.0, 50.0);
	glEnd();
}


void init(void)
{
	glEnable(GL_DEPTH_TEST);						//…Ó∂»ºÏ≤‚

	GLfloat position[] = { 0.0, 0.0, 5.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat ambient[] = { 0.1, 0.9, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.25, 0.9, 0.5, 1.0 };
	GLfloat specular[] = { 0.0, 1.0, 0.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

	terraininit();	
}

void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	//glTranslatef(-lx / 2, 0, 0);
	glRotatef(45, 1.0, 0.0, 0.0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	drawAxis();
	glEnable(GL_LIGHTING);

	///////////draw the terrain 
	for (int i = 0; i < ROW-1; i ++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j < COL; j++)
		{
			glNormal3f(Normal[i][j].x, Normal[i][j].y, Normal[i][j].z);
			glVertex3f(Vertex[i][j].x, Vertex[i][j].y, Vertex[i][j].z);

			glNormal3f(Normal[i+1][j].x, Normal[i+1][j].y, Normal[i+1][j].z);
			glVertex3f(Vertex[i+1][j].x, Vertex[i+1][j].y, Vertex[i+1][j].z);
		}
		glEnd();
	}

	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0.0, 0.0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}