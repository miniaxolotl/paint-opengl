/**
 * main.cxx
 * @brief Main entry point for program & implementation of files in global header
 * @author Elias Mawa
 * @date 10-16-2019
 * Author:		Elias Mawa
 * Created on: 	10-16-2019
 * Last Edit:	12-03-2019
 */
#include "global.h"
#include "model/canvas/canvas.h"

/******************************************************
 * VARIABLES
 ******************************************************/

unsigned int objects = 0;
Canvas* c = new Canvas();

int window_width = 800;
int window_height = 800;

// KEY FLAGS
bool mouse_active = false, mouse_left = false, mouse_right = false, x_key = false;
float mouse_x = 0, mouse_y = 0;

// color values
int rgb[3] = { 255, 255, 255 };
float alpha = 1;
const float COLORS = 255.0f;
unsigned int hsv_hue = 180;

/******************************************************
 * 	MAIN
 ******************************************************/

int main(int argc, char** argv)
{
	init(argc,argv);

    glutMainLoop();

    return 0;
}

/******************************************************
 * 	FUNCTIONS
 ******************************************************/

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

	if(mouse_active)
	{
		place(mouse_right);
	}
} // void update()

void place(bool clear_flag)
{
	if(!clear_flag & !x_key)
	{
		c->paint(mouse_x, mouse_y, rgb[0], rgb[1], rgb[2], alpha);
	}
	else if(clear_flag)
	{
		c->clear(mouse_x, mouse_y);
	}
	
} // void place()

void mouse(int button, int state, int x, int y)
{
	/* KEY PRESSES */
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouse_active = true;
		mouse_right = false;
		mouse_x = (2*(float)x/window_width-1);
		mouse_y = (2*(float)y/window_height-1)*-1;
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		mouse_active = true;
		mouse_right = true;
		mouse_x = (2*(float)x/window_width-1);
		mouse_y = (2*(float)y/window_height-1)*-1;
	}

	/* KEY RELEASES */
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		mouse_active = false;
		mouse_left = false;
		mouse_x=0;
		mouse_y=0;
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		mouse_active = false;
		mouse_right = false;
		mouse_x=0;
		mouse_y=0;
	}
	
	/* MOUSE SCROLLING */
	// hue delta
	float hsv_mod = (360/128);
	if(button == 3) // scroll up
	{
		if(hsv_hue + hsv_mod <= 360)
		{
			hsv_hue += hsv_mod;
			HSVtoRGB(hsv_hue, rgb);
		}
	}
	else if(button == 4) // scroll down
	{		
		if(hsv_hue - hsv_mod >= 0)
		{
			hsv_hue -= hsv_mod;
			HSVtoRGB(hsv_hue, rgb);
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
	mouse_x = (2*(float)x/window_width-1);
	mouse_y = (2*(float)y/window_height-1)*-1;
	
	// if user is scrolling colors
	if(x_key)
	{
		HSVtoRGB((mouse_x+1)*(360.0f/2), rgb);
	}
} // void mouse_m_f(int x, int y)

void mouse_p(int x, int y)
{
	mouse_x=2*(float)x/window_width-1;
	mouse_y=(2*(float)y/window_height-1)*-1;

	if(x_key)
	{
		HSVtoRGB((mouse_x+1)*(360.0f/2), rgb);
	}
} // void mouse_m_f(int x, int y)

void kb_s(int key, int x, int y)
{
	switch (key)
	{
	case 114:
		// ctrl
		break;
	
	case 115:
		// alt
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
			x_key = true;
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
			x_key = false;
			break;
	}
}

void terminal_thread_func()
{
	std::string input;

	while(true)
	{
		printf("Input rgba values [0-255] [0-255] [0-255] [0-1]: ");
		scanf ("%d",&rgb[0]);
		scanf ("%d",&rgb[1]);
		scanf ("%d",&rgb[2]);
		scanf ("%f",&alpha);
		
    	getchar(); // clear input buffer on error 
	}
} // void terminal_thread_func()

void HSVtoRGB(int H, int output[3],double S, double V) {
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

	output[0] = (Rs + m) * 255;
	output[1] = (Gs + m) * 255;
	output[2] = (Bs + m) * 255;
}