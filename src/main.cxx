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

bool kill = false;
bool slowmode = false;

Canvas* c = new Canvas();

int main_window = 0;
int window_width = 800;
int window_height = 800;

// KEY FLAGS
bool mouse_active = false, mouse_left = false, mouse_right = false, x_key = false;
float mouse_x = 0, mouse_y = 0;

// color values
int rgb[3] = { 0, 0, 0 };
float alpha = 1;
int pixel_size = 1;
const float COLORS = 255.0f;
unsigned int hsv_hue = 180;
float hsv_saturation = 1.0f;
float hsv_value = 1.0f;
bool show_help = false;

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
    main_window = glutCreateWindow("Paint32");
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
	drawUI();

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
		place();
	}
} // void update()

void place()
{
	// while(true)
	// {
		if(!mouse_right && !x_key && mouse_active)
		{
			// printf("%d %d %d\n", rgb[0], rgb[1], rgb[2]);
			// printf("%f %f\n", mouse_x, mouse_y);
			c->paint(mouse_x, mouse_y, rgb[0], rgb[1], rgb[2], alpha);
		}
		else if(mouse_right)
		{
			c->clear(mouse_x, mouse_y);
		}
	// }
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
	float hsv_mod = (360/128);
	int mod = glutGetModifiers();
	
	if(button == 3)
	{
		if(mod & GLUT_ACTIVE_SHIFT)
		{
			if(hsv_saturation + 0.05f <= 1.0f)
			{
				hsv_saturation += 0.05f;
				HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
			}
		}
		else if(mod & GLUT_ACTIVE_CTRL)
		{
			if(hsv_value + 0.05f <= 1.0f)
			{
				hsv_value += 0.05f;
				HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
			}
		}
		else
		{
			if(hsv_hue + hsv_mod <= 360)
			{
				hsv_hue += hsv_mod;
				HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
			}
		}
	}
	else if(button == 4)
	{		
		if(mod & GLUT_ACTIVE_SHIFT)
		{
			if(hsv_saturation - 0.05f >= 0.0f)
			{
				hsv_saturation -= 0.05f;
				HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
			}
		}
		else if(mod & GLUT_ACTIVE_CTRL)
		{
			if(hsv_value - 0.05f >= 0.0f)
			{
				hsv_value -= 0.05f;
				HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
			}
		}
		else
		{
			if(hsv_hue - hsv_mod >= 0)
			{
				hsv_hue -= hsv_mod;
				HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
			}
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

	if(x_key)
	{
		hsv_hue = (mouse_x+1)*(360.0f/2);
		HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
	}
}

void mouse_p(int x, int y)
{
	mouse_x=2*(float)x/window_width-1;
	mouse_y=(2*(float)y/window_height-1)*-1;

	if(x_key)
	{
		hsv_hue = (mouse_x+1)*(360.0f/2);
		HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
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
			kill = true;
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

		case 's':
			slowmode = !slowmode;
			break;

		case '-':
			if(pixel_size-1 > 0)
			{
				pixel_size--;
			}
			break;

		case '+':
			if(pixel_size+1 < 5)
			{
				pixel_size++;
			}
			break;

		case '=':
			pixel_size = 1;
			break;

		case 'h':
			show_help = !show_help;
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

		case 'c':
			kill = false;
			c->clear();
			break;
	}
}

void drawText(const char* text, float x, float y, void* font, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void drawRect(float x, float y, float w, float h, float r, float g, float b, float a)
{
    glColor4f(r, g, b, a);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void setPixelProjection()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, window_width, 0, window_height);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void restoreProjection()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void drawUI()
{
    setPixelProjection();

    // ---- Top-left info panel ----
    float panelW = 200;
    float panelH = 130;
    float pad = 10;
    drawRect(pad, window_height - panelH - pad, panelW, panelH, 0.1f, 0.1f, 0.1f, 0.75f);

    // Draw border
    glColor4f(0.5f, 0.5f, 0.5f, 0.9f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(pad, window_height - panelH - pad);
    glVertex2f(pad + panelW, window_height - panelH - pad);
    glVertex2f(pad + panelW, window_height - pad);
    glVertex2f(pad, window_height - pad);
    glEnd();

    float lineH = 18;
    float tx = pad + 10;
    float ty = window_height - pad - 20;
    void* font = GLUT_BITMAP_9_BY_15;

    char buf[128];
    const char* toolName = (c->get_tool() == BRUSH_TYPE::PIXEL) ? "Pixel" : "Bucket";
    snprintf(buf, sizeof(buf), "Tool: %s", toolName);
    drawText(buf, tx, ty, font, 1.0f, 1.0f, 1.0f);
    ty -= lineH;

    snprintf(buf, sizeof(buf), "Brush Size: %d", pixel_size);
    drawText(buf, tx, ty, font, 1.0f, 1.0f, 1.0f);
    ty -= lineH;

    snprintf(buf, sizeof(buf), "RGB: %d, %d, %d", rgb[0], rgb[1], rgb[2]);
    drawText(buf, tx, ty, font, rgb[0]/COLORS, rgb[1]/COLORS, rgb[2]/COLORS);
    ty -= lineH;

    snprintf(buf, sizeof(buf), "HSV: %d, %d%%, %d%%", hsv_hue, (int)(hsv_saturation*100), (int)(hsv_value*100));
    drawText(buf, tx, ty, font, 1.0f, 1.0f, 1.0f);
    ty -= lineH;

    drawText("Press H for help", tx, ty, font, 0.7f, 0.7f, 0.7f);

    // ---- Current color preview (top-right) ----
    float prevSize = 40;
    float px = window_width - prevSize - pad;
    float py = window_height - prevSize - pad;
    drawRect(px, py, prevSize, prevSize, 0.1f, 0.1f, 0.1f, 0.75f);
    drawRect(px + 2, py + 2, prevSize - 4, prevSize - 4, rgb[0]/COLORS, rgb[1]/COLORS, rgb[2]/COLORS, alpha);
    glColor4f(0.5f, 0.5f, 0.5f, 0.9f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(px, py);
    glVertex2f(px + prevSize, py);
    glVertex2f(px + prevSize, py + prevSize);
    glVertex2f(px, py + prevSize);
    glEnd();

    // ---- Help overlay ----
    if (show_help)
    {
        float hw = 420;
        float hh = 320;
        float hx = (window_width - hw) / 2;
        float hy = (window_height - hh) / 2;
        drawRect(hx, hy, hw, hh, 0.05f, 0.05f, 0.05f, 0.9f);
        glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(hx, hy);
        glVertex2f(hx + hw, hy);
        glVertex2f(hx + hw, hy + hh);
        glVertex2f(hx, hy + hh);
        glEnd();

        float htx = hx + 20;
        float hty = hy + hh - 30;
        void* hfont = GLUT_BITMAP_HELVETICA_18;
        drawText("Controls", htx, hty, hfont, 1.0f, 0.8f, 0.3f);
        hty -= 28;

        hfont = GLUT_BITMAP_9_BY_15;
        drawText("Left Click          - Paint", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
        drawText("Right Click         - Erase", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
        drawText("Scroll Wheel        - Change Hue", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
        drawText("Shift + Scroll      - Change Saturation", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
        drawText("Ctrl + Scroll       - Change Value", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
        drawText("X + Move Mouse      - Pick color by position", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
        drawText("C                   - Clear canvas", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
        drawText("B                   - Bucket fill tool", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
        drawText("V                   - Pixel brush tool", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
        drawText("+ / -               - Increase / Decrease brush", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
        drawText("=                   - Reset brush size", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
        drawText("S                   - Toggle slow fill mode", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
        drawText("H                   - Toggle this help", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
    }

    restoreProjection();
}

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