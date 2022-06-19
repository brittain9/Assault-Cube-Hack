#include "gldraw.h"

void GL::SetupOrtho()
{
	// Orthographic projection matrix: need to draw in 2d in 3d game
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	// 0, 0 for corners. Viewport 2 is width, viewport 3 is height.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	// Create Orthographic projection matrix for 2d. 0 is left, viewport[2] is far left. Frustrum near and far need to be guessed.
	glMatrixMode(GL_MODELVIEW);
	// Be in modelview mode to draw stuff
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	// Disable depth test so what we draw on screen is on top layer.
}

void GL::RestoreGL()
{
	glPopMatrix();
	glPopAttrib();
}

void GL::DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3])
{
	glColor3ub(color[0], color[1], color[2]);
	// Whenever you see glBegin and glEnd, its immediate mode
	glBegin(GL_QUADS);
	// rectangle
	glVertex2f(x, y);
	// top left
	glVertex2f(x + width, y);
	// top right
	glVertex2f(x + width, y + height);
	// bottom right
	glVertex2f(x, y + height);
	//bottom left
	glEnd();

	// Draw shapes starting from top left clockwise.
}

void GL::DrawOutline(float x, float y, float width, float height, float lineWidth, const GLubyte color[3])
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	glColor3ub(color[0], color[1], color[2]);
	// ub = unsigned byte
	glVertex2f(x - 0.5f, y - 0.5f);
	// 2f indicates function takes 2 floats.
	glVertex2f(x + width + 0.5f, y - 0.5f);
	glVertex2f(x + width + 0.5f, y + height + 0.5f);
	glVertex2f(x - 0.5f, y + height + 0.5f);
	glVertex2f(x - 0.5f, y - 0.5f);
	glEnd();
}