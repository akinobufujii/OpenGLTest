#include <gl/glut.h>

static const char* WINDOW_TITLE = "00_Skelton";

void display()
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glutSolidTeapot(0.5);

	glutSwapBuffers();
}

// エントリーポイント
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow(WINDOW_TITLE);
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}
