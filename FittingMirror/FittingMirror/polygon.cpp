#include "polygon.h"
#include <gl\GL.h>

extern CvPoint points[NUI_SKELETON_POSITION_COUNT];
int rot = 1.0f;
void polygon::createPloygon(float x, float y, int screenW, int screenH)
{
	float gx = (float)((x - ((float)screenW / 2.0)*(float)(1.0 / (float)((screenW / 2.0)))));
	GLfloat glx = -((x - (screenW / 2))*(1.0 / ((screenW - 100) / 2)));
	float gy = -(float)((y - ((float)screenH / 2.0)*(float)(1.0 / (float)(screenH / 2.0))));
	GLfloat gly = -((y - (screenH / 2))*(1.0 / (screenH / 2)));

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(glx + 0.0f, gly + 0.1f, 0.267f); // { Front }
	glVertex3f(glx - 0.1f, gly - 0.1f, 0.267f); // { Front }
	glVertex3f(glx + 0.1f, gly - 0.1f, 0.267f); // { Front }

	glColor3f(0.0f, 0.5f, 0.0f);
	glVertex3f(glx + 0.0f, gly + 0.1f, 0.267f); // { Right }
	glVertex3f(glx + 0.1f, gly - 0.1f, 0.267f); // { Right }
	glVertex3f(glx + 0.1f, gly - 0.1f, -0.267f); // { Right }

	glColor3f(0.0f, 0.0f, 0.5f);
	glVertex3f(glx + 0.0f, gly + 0.1f, 0.267f); // { Back }
	glVertex3f(glx + 0.1f, gly - 0.1f, -0.267f); // { Back }
	glVertex3f(glx - 0.1f, gly - 0.1f, -0.267f); // { Back }

	glColor3f(1.0f, 0.0f, 0.5f);
	glVertex3f(glx + 0.0f, gly + 0.1f, 0.267f); // { Left }
	glVertex3f(glx - 0.1f, gly - 0.1f, -0.267f); // { Left }
	glVertex3f(glx - 0.1f, gly - 0.1f, 0.267f); // { Left }
	glEnd();
}

void polygon::quads(float x, float y, int screenW, int screenH){
	float gx = (float)((x - ((float)screenW / 2.0)*(float)(1.0 / (float)((screenW / 2.0)))));
	GLfloat glx = -((x - (screenW / 2))*(1.0 / ((screenW - 100) / 2)));
	float gy = -(float)((y - ((float)screenH / 2.0)*(float)(1.0 / (float)(screenH / 2.0))));
	GLfloat gly = -((y - (screenH / 2))*(1.0 / (screenH / 2)));
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(glx - 0.3f, gly + 0.3f, 0.267f); // { Front }
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(glx - 0.3f, gly - 0.3f, 0.267f); // { Front }
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(glx + 0.3f, gly - 0.3f, 0.267f); // { Front }
	glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(glx + 0.3f, gly + 0.3f, 0.267f); // { Front }

	glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(glx + 0.3f, gly + 0.3f, 0.267f); // { Right }
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(glx + 0.3f, gly - 0.3f, 0.267f); // { Right }
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(glx + 0.3f, gly - 0.3f, -0.267f); // { Right }
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(glx + 0.3f, gly + 0.3f, -0.267f); // { Right }

	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(glx + 0.3f, gly + 0.3f, -0.267f); // { Back }
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(glx + 0.3f, gly - 0.3f, -0.267f); // { Back }
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(glx - 0.3f, gly - 0.3f, -0.267f); // { Back }
	glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(glx - 0.3f, gly + 0.3f, -0.267f); // { Back }

	glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(glx - 0.3f, gly + 0.3f, -0.267f); // { Left }
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(glx - 0.3f, gly - 0.3f, -0.267f); // { Left }
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(glx - 0.3f, gly - 0.3f, 0.267f); // { Left }
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(glx - 0.3f, gly + 0.3f, 0.267f); // { Left }

	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(glx - 0.3f, gly + 0.3f, 0.267f); // { Top }
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(glx + 0.3f, gly + 0.3f, 0.267f); // { Top }
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(glx + 0.3f, gly + 0.3f, -0.267f); // { Top }
	glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(glx - 0.3f, gly + 0.3f, -0.267f); // { Top }

	glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(glx + 0.3f, gly - 0.3f, 0.267f); // { Bottom }
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(glx - 0.3f, gly - 0.3f, 0.267f); // { Bottom }
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(glx - 0.3f, gly - 0.3f, -0.267f); // { Bottom }
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(glx + 0.3f, gly - 0.3f, -0.267f); // { Bottom }
	glEnd();
	rot++;
}