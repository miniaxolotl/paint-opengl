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

const int WINDOW_SIZE = 800;
int main_window = 0;
int window_width = WINDOW_SIZE;
int window_height = WINDOW_SIZE;

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

// Color picker drag flags
bool color_picking_hue = false;
bool color_picking_sv = false;

// Color picker bounds in normalized coords
static const float HUE_BAR_Y = -1.0f;
static const float HUE_BAR_HEIGHT = 0.06f;
static const float SV_SIZE = 0.25f;
static const float SV_X = 0.75f;
static const float SV_Y = HUE_BAR_Y + HUE_BAR_HEIGHT + 0.02f;

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
    glutReshapeWindow(WINDOW_SIZE, WINDOW_SIZE);

	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    glutDisplayFunc(draw);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_m);
	glutPassiveMotionFunc(mouse_p);
	glutSpecialFunc(kb_s);
	glutKeyboardFunc(kb);
	glutKeyboardUpFunc(kb_up);
	glutReshapeFunc(reshape);
	glutIdleFunc(main_loop);

} // void init(int argc, char** argv)

void reshape(int w, int h)
{
	// Enforce 1:1 aspect ratio
	int size = (w < h) ? w : h;
	glutReshapeWindow(size, size);
	window_width = size;
	window_height = size;

	glViewport(0, 0, size, size);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
}

void draw()
{
	main_loop();
	// set background colour
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

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

	if(mouse_active && !color_picking_hue && !color_picking_sv)
	{
		place();
	}
} // void update()

void place()
{
	if(!mouse_right && !x_key && mouse_active)
	{
		c->paint(mouse_x, mouse_y, rgb[0], rgb[1], rgb[2], alpha);
	}
	else if(mouse_right)
	{
		c->clear(mouse_x, mouse_y);
	}
} // void place()

// Convert pixel coordinates to normalized -1..1
static float toNormX(int px) { return (2.0f * (float)px / (float)window_width) - 1.0f; }
static float toNormY(int py) { return 1.0f - (2.0f * (float)py / (float)window_height); }

void mouse(int button, int state, int x, int y)
{
	float nx = toNormX(x);
	float ny = toNormY(y);

	/* KEY PRESSES */
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// Check if clicking on hue bar
		if(ny >= HUE_BAR_Y && ny <= HUE_BAR_Y + HUE_BAR_HEIGHT && nx >= -1.0f && nx <= 1.0f)
		{
			color_picking_hue = true;
			hsv_hue = (unsigned int)(((nx + 1.0f) / 2.0f) * 360.0f);
			HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
			return;
		}

		// Check if clicking on SV square
		if(nx >= SV_X && nx <= SV_X + SV_SIZE && ny >= SV_Y && ny <= SV_Y + SV_SIZE)
		{
			color_picking_sv = true;
			hsv_saturation = (nx - SV_X) / SV_SIZE;
			hsv_value = (ny - SV_Y) / SV_SIZE;
			HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
			return;
		}

		mouse_active = true;
		mouse_right = false;
		mouse_x = nx;
		mouse_y = ny;
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		mouse_active = true;
		mouse_right = true;
		mouse_x = nx;
		mouse_y = ny;
	}

	/* KEY RELEASES */
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		mouse_active = false;
		mouse_left = false;
		color_picking_hue = false;
		color_picking_sv = false;
		mouse_x = 0;
		mouse_y = 0;
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		mouse_active = false;
		mouse_right = false;
		mouse_x = 0;
		mouse_y = 0;
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
	float nx = toNormX(x);
	float ny = toNormY(y);
	mouse_x = nx;
	mouse_y = ny;

	if(color_picking_hue)
	{
		hsv_hue = (unsigned int)(((nx + 1.0f) / 2.0f) * 360.0f);
		HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
	}
	else if(color_picking_sv)
	{
		hsv_saturation = (nx - SV_X) / SV_SIZE;
		hsv_value = (ny - SV_Y) / SV_SIZE;
		if(hsv_saturation < 0.0f) hsv_saturation = 0.0f;
		if(hsv_saturation > 1.0f) hsv_saturation = 1.0f;
		if(hsv_value < 0.0f) hsv_value = 0.0f;
		if(hsv_value > 1.0f) hsv_value = 1.0f;
		HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
	}
	else if(x_key)
	{
		hsv_hue = (unsigned int)(((nx + 1.0f) / 2.0f) * 360.0f);
		HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
	}
}

void mouse_p(int x, int y)
{
	float nx = toNormX(x);
	float ny = toNormY(y);
	mouse_x = nx;
	mouse_y = ny;

	if(color_picking_hue)
	{
		hsv_hue = (unsigned int)(((nx + 1.0f) / 2.0f) * 360.0f);
		HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
	}
	else if(color_picking_sv)
	{
		hsv_saturation = (nx - SV_X) / SV_SIZE;
		hsv_value = (ny - SV_Y) / SV_SIZE;
		if(hsv_saturation < 0.0f) hsv_saturation = 0.0f;
		if(hsv_saturation > 1.0f) hsv_saturation = 1.0f;
		if(hsv_value < 0.0f) hsv_value = 0.0f;
		if(hsv_value > 1.0f) hsv_value = 1.0f;
		HSVtoRGB(hsv_hue, rgb, hsv_saturation, hsv_value);
	}
	else if(x_key)
	{
		hsv_hue = (unsigned int)(((nx + 1.0f) / 2.0f) * 360.0f);
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

    float pad = 10;

    // ---- Top-left info panel ----
    float panelW = 200;
    float panelH = 130;
    // Clamp panel within window bounds
    float panelX = pad;
    float panelY = window_height - panelH - pad;
    if(panelX + panelW > window_width - pad) panelX = window_width - panelW - pad;
    if(panelY < pad) panelY = pad;

    drawRect(panelX, panelY, panelW, panelH, 0.1f, 0.1f, 0.1f, 0.75f);

    // Draw border
    glColor4f(0.5f, 0.5f, 0.5f, 0.9f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(panelX, panelY);
    glVertex2f(panelX + panelW, panelY);
    glVertex2f(panelX + panelW, panelY + panelH);
    glVertex2f(panelX, panelY + panelH);
    glEnd();

    float lineH = 18;
    float tx = panelX + 10;
    float ty = panelY + panelH - 20;
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

    if(slowmode)
    {
        drawText("Slow Mode: ON", tx, ty, font, 1.0f, 0.7f, 0.3f);
        ty -= lineH;
    }

    drawText("Press H for help", tx, ty, font, 0.7f, 0.7f, 0.7f);

    // ---- Current color preview (top-right) ----
    float prevSize = 40;
    float px = window_width - prevSize - pad;
    float py = window_height - prevSize - pad;
    // Clamp within bounds
    if(px < pad) px = pad;
    if(py < pad) py = pad;

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
        float hh = 340;
        float hx = (window_width - hw) / 2;
        float hy = (window_height - hh) / 2;
        // Clamp within bounds
        if(hx < pad) hx = pad;
        if(hy < pad) hy = pad;
        if(hx + hw > window_width - pad) hx = window_width - hw - pad;
        if(hy + hh > window_height - pad) hy = window_height - hh - pad;

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
        drawText("Click Hue Bar       - Pick hue", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
        drawText("Click SV Square     - Pick saturation/value", htx, hty, hfont, 1.0f, 1.0f, 1.0f); hty -= 20;
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
