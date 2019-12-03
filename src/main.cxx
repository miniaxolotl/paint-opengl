/**
 * main.cxx
 * @brief Main entry point for program & implementation of files in global header
 * @author Elias Mawa
 * @date 10-16-2019
 * Author:		Elias Mawa
 * Created on: 	10-16-2019
 * Last Edit:	11-29-2019
 */
#include "global.h"
#include "model/canvas/canvas.h"

int main(int argc, char** argv)
{
	init(argc,argv);

    glutMainLoop();

    return 0;
}

/** Variables **/
Canvas* c = new Canvas();

int window_width = 800;
int window_height = 800;

bool down = false;
bool clear = false;
float nx,ny;
bool ctrl = false;
float rgb[3] = { 54/255, 1, 1 };
unsigned int h = 180;
unsigned int rgb_hex = 0xffffff;

float a = .5;

unsigned int objects = 0;

std::thread terminal_thread(terminal_thread_func); 

/** Functions **/

void init(int argc, char** argv)
{
	// Initialize glut
    glutInit(&argc, argv); // pass in command line arguments
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);


    // create the window object and tell glut to use it
    int main_window = glutCreateWindow("Paint32");
    glutSetWindow(main_window);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // places the window and defines it's size
    glutPositionWindow(0,0);
    glutReshapeWindow(window_width, window_height);

	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);

	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	// glutTimerFunc(1000/60, timer, 0);

    glutDisplayFunc(draw);
	glutMouseFunc(mouse);
	// glutMouseWheelFunc(mouse_w);
	glutMotionFunc(mouse_m);
	glutPassiveMotionFunc(mouse_p);
	glutSpecialFunc(kb_s);
	glutKeyboardFunc(kb);
	glutKeyboardUpFunc(kb_up);
	glutIdleFunc(main_loop);
	glutMainLoop();

} // void init(int argc, char** argv)

void draw()
{
	main_loop();
	// set background colour
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glFrustum(-1,1,-1,1,2.0,20.0);
	glMatrixMode(GL_MODELVIEW);

	c->draw();

	glLoadIdentity();

	glutSwapBuffers();
} // void draw()

void main_loop()
{
	update();
    glutPostRedisplay();
} // void timer(int t)

void update()
{
	window_width = glutGet(GLUT_WINDOW_WIDTH);
	window_height = glutGet(GLUT_WINDOW_HEIGHT);

	if(down)
	{
		place(clear);
	}
} // void update()

void place(bool clear_flag)
{
	if(!clear_flag & !ctrl)
	{
		c->paint(nx, ny, rgb[0], rgb[1], rgb[2], a);
	}
	else if (clear_flag)
	{
		c->clear(nx,ny);
	}
	
} // void place()

void mouse(int b, int s,int x, int y)
{
	if(b == GLUT_LEFT_BUTTON && s == GLUT_DOWN)
	{
		down = true;
		clear = false;
		nx=2*(float)x/window_width-1;
		ny=(2*(float)y/window_height-1)*-1;
	}
	else if(b == GLUT_RIGHT_BUTTON && s == GLUT_DOWN)
	{
		down = true;
		clear = true;
		nx=2*(float)x/window_width-1;
		ny=(2*(float)y/window_height-1)*-1;
	}

	if(b == GLUT_LEFT_BUTTON && s == GLUT_UP)
	{
		down = false;
		clear = false;
		nx=0;
		ny=0;
	}
	else if(b == GLUT_RIGHT_BUTTON && s == GLUT_UP)
	{
		down = false;
		clear = false;
		nx=0;
		ny=0;
	}
	
	float z = (360/128); 
	if(b == 3) // up
	{
		if(h+z <= 360)
		{
			h += z;
			HSVtoRGB(h, rgb);
		}
	}
	else if(b == 4) // down
	{		
		if(h-z >= 0)
		{
			h -= z;
			HSVtoRGB(h, rgb);
		}
	}
} // void mouse_f(int b, int s,int x, int y)

void mouse_w(int button, int dir, int x, int y)
{
	if (dir > 0)
    {
        // Zoom in
    }
    else
    {
        // Zoom out
    }
} // mouse_w(int button, int dir, int x, int y)

void mouse_m(int x, int y)
{
	if(ctrl)
	{
		HSVtoRGB((nx+1)*(360.0f/2), rgb);
	}
} // void mouse_m_f(int x, int y)

void mouse_p(int x, int y)
{
	nx=2*(float)x/window_width-1;
	ny=(2*(float)y/window_height-1)*-1;

	if(ctrl)
	{
		HSVtoRGB((nx+1)*(360.0f/2), rgb);
	}
} // void mouse_m_f(int x, int y)

void kb_s(int key, int x, int y)
{
	// printf("KEYCODE: %d\n", key);
	switch (key)
	{
	case 114:
		
		break;
	
	case 115:
		/* code */
		break;
	
	default:
		break;
	}
} // void kb_s(int key, int x, int y)

void kb(unsigned char key, int x, int y)
{

	switch(key)
	{
		case 'x':
			ctrl = true;
			break;
		case 'c':
			c->clear();
			puts("CLEAR");
			break;

		case 'b':
			c->set_tool(BRUSH_TYPE::FLOOD);
			puts("TOOL: BUCKET");
			break;

		case 'v':
			c->set_tool(BRUSH_TYPE::PIXEL);
			puts("TOOL: PIXEL");
			break;

		default:
			break;
	}

} // void kb(unsigned char key, int x, int y)

void kb_up(unsigned char key, int x, int y)
{

	switch(key)
	{
		case 'x':
			ctrl = false;
			break;
	}
}

void terminal_thread_func()
{
	std::string input;

	while(true)
	{
		printf("Input rgb values [0-255] [0-255] [0-255]: ");
		scanf ("%f",&rgb[0]);
		scanf ("%f",&rgb[1]);
		scanf ("%f",&rgb[2]);
		
    	getchar(); // clear input buffer on error 
	}
} // void terminal_thread_func()

void HSVtoRGB(int H, float output[3],double S, double V) {
	double C = S * V;
	double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	double m = V - C;
	double Rs, Gs, Bs;

	if(H >= 0 && H < 60) {
		Rs = C;
		Gs = X;
		Bs = 0;	
	}
	else if(H >= 60 && H < 120) {	
		Rs = X;
		Gs = C;
		Bs = 0;	
	}
	else if(H >= 120 && H < 180) {
		Rs = 0;
		Gs = C;
		Bs = X;	
	}
	else if(H >= 180 && H < 240) {
		Rs = 0;
		Gs = X;
		Bs = C;	
	}
	else if(H >= 240 && H < 300) {
		Rs = X;
		Gs = 0;
		Bs = C;	
	}
	else {
		Rs = C;
		Gs = 0;
		Bs = X;	
	}
	
	// output[0] = (Rs + m) * 255;
	// output[1] = (Gs + m) * 255;
	// output[2] = (Bs + m) * 255;

	output[0] = (Rs + m);
	output[1] = (Gs + m);
	output[2] = (Bs + m);
}