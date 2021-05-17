/**
 * canvas.cxx
 * Implementation of canvas class.
 * Author:		Elias Mawa
 * Created on: 	10-16-2019
 * Last Edit:	12-04-2019
 */

#include "canvas.h"
#include "../../global.h"

Canvas::Canvas() : Object::Object()
{
	brush_type = BRUSH_TYPE::PIXEL;

	// Generate all nodes
	graph = new CanvasNode**[width];
	for(int i=0;i<width;i++)
	{
		graph[i] = new CanvasNode*[height];
	}

	// Instantiate all nodes
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			graph[i][j] = new CanvasNode(((2*(float)i)/width)-1,((2*(float)j)/height)-1);
		}
	}

	// Set all the node pointers
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			// NOrth node
			if(j+1 < height)
			{
				if(graph[i][j+1] == NULL)
				{
					graph[i][j]->setLink(NULL, DIRECTION::NORTH);
				}
				else
				{
					graph[i][j]->setLink(graph[i][j+1], DIRECTION::NORTH);
				}
			}

			// South node
			if(j-1 >= 0)
			{
				if(graph[i][j-1] == NULL)
				{
					graph[i][j]->setLink(NULL, DIRECTION::SOUTH);
				}
				else
				{
					graph[i][j]->setLink(graph[i][j-1], DIRECTION::SOUTH);
				}
			}

			// East node
			if(i+1 < width)
			{
				if(graph[i+1][j] == NULL)
				{
					graph[i][j]->setLink(NULL, DIRECTION::EAST);
				}
				else
				{
					graph[i][j]->setLink(graph[i+1][j], DIRECTION::EAST);
				}
			}

			// West node
			if(i-1 >= 0)
			{
				if(graph[i-1][j] == NULL)
				{
					graph[i][j]->setLink(NULL, DIRECTION::WEST);
				}
				else
				{
					graph[i][j]->setLink(graph[i-1][j], DIRECTION::WEST);
				}
			}
		}
	}
} // Canvas::Canvas() : Object::Object()

Canvas::Canvas(int w, int h) : Object::Object()
{
	// TODO
} // Canvas::Canvas(int w, int h) : Object::Object()

Canvas::~Canvas()
{
	// Free all nodes
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			delete graph[i][j];
		}
		delete[] graph[i];
	}
	delete[] graph;
}

void Canvas::draw()
{
	background();

	// draw pixels
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			graph[i][j]->draw();
		}
	}

	brush_indicator();
	color_bar();
} // Canvas::draw()

void Canvas::update()
{
	// Update all nodes
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			graph[i][j]->update();
		}
	}

	// Process incremental flood fill step each frame
	if(brush.is_filling())
	{
		brush.flood_fill_step(slowmode ? 5 : 200);
	}
} // Canvas::update()
 
void Canvas::color_bar()
{
	int rgb_val[3] = { 0, 0 ,0 };
	float bar_height = 0.06f;
	float bar_y = -1.0f;

	// Draw hue bar
	for(int i=0;i<360;i++)
	{
		HSVtoRGB(i, rgb_val, 1.0f, 1.0f);
		
		double x0 = ((double)i/360.0)*2.0 - 1.0;
		double x1 = ((double)(i+1)/360.0)*2.0 - 1.0;

		glColor3f(rgb_val[0]/COLORS, rgb_val[1]/COLORS, rgb_val[2]/COLORS);
		glBegin(GL_POLYGON);
		glVertex2f(x0, bar_y);
		glVertex2f(x1, bar_y);
		glVertex2f(x1, bar_y + bar_height);
		glVertex2f(x0, bar_y + bar_height);
		glEnd();    
	}

	// Draw border around hue bar
	glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-1.0f, bar_y);
	glVertex2f(1.0f, bar_y);
	glVertex2f(1.0f, bar_y + bar_height);
	glVertex2f(-1.0f, bar_y + bar_height);
	glEnd();

	// Draw hue indicator triangle
	float hue_pos = (hsv_hue / 360.0f) * 2.0f - 1.0f;
	float indicator_size = 0.025f;
	glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
	glBegin(GL_TRIANGLES);
	glVertex2f(hue_pos, bar_y + bar_height + indicator_size);
	glVertex2f(hue_pos - indicator_size * 0.7f, bar_y + bar_height);
	glVertex2f(hue_pos + indicator_size * 0.7f, bar_y + bar_height);
	glEnd();

	// Draw saturation/value square above hue bar
	float sv_size = 0.25f;
	float sv_x = 0.75f;
	float sv_y = bar_y + bar_height + 0.02f;

	// Draw SV gradient
	for(int sy = 0; sy < (int)(sv_size * 200); sy++)
	{
		for(int sx = 0; sx < (int)(sv_size * 200); sx++)
		{
			float s = (float)sx / (sv_size * 200);
			float v = (float)sy / (sv_size * 200);
			HSVtoRGB(hsv_hue, rgb_val, s, v);
			
			double px0 = sv_x + (double)sx / (sv_size * 200) * sv_size;
			double px1 = sv_x + (double)(sx + 1) / (sv_size * 200) * sv_size;
			double py0 = sv_y + (double)sy / (sv_size * 200) * sv_size;
			double py1 = sv_y + (double)(sy + 1) / (sv_size * 200) * sv_size;

			glColor3f(rgb_val[0]/COLORS, rgb_val[1]/COLORS, rgb_val[2]/COLORS);
			glBegin(GL_POLYGON);
			glVertex2f(px0, py0);
			glVertex2f(px1, py0);
			glVertex2f(px1, py1);
			glVertex2f(px0, py1);
			glEnd();
		}
	}

	// Draw border around SV square
	glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(sv_x, sv_y);
	glVertex2f(sv_x + sv_size, sv_y);
	glVertex2f(sv_x + sv_size, sv_y + sv_size);
	glVertex2f(sv_x, sv_y + sv_size);
	glEnd();

	// Draw saturation/value indicator circle
	float sv_ind_x = sv_x + hsv_saturation * sv_size;
	float sv_ind_y = sv_y + hsv_value * sv_size;
	float ind_radius = 0.008f;
	glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
	glBegin(GL_POLYGON);
	for(int i = 0; i < 16; i++)
	{
		float theta = i * M_PI / 8;
		glVertex2f(ind_radius * cos(theta) + sv_ind_x, ind_radius * sin(theta) + sv_ind_y);
	}
	glEnd();
	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < 16; i++)
	{
		float theta = i * M_PI / 8;
		glVertex2f(ind_radius * cos(theta) + sv_ind_x, ind_radius * sin(theta) + sv_ind_y);
	}
	glEnd();
}

void Canvas::brush_indicator()
{		 
	if(!mouse_active)
	{
		return;
	}

	float radius = (0.03125f * pixel_size);
	float radius_contrast = radius + 0.02f;
	float theta;

	glColor4f(1.0f - rgb[0]/COLORS, 1.0f - rgb[1]/COLORS, 1.0f - rgb[2]/COLORS, 0.5f);
	glBegin(GL_POLYGON);
	for(int i=0; i<16; i++)
	{
		theta = i*M_PI/8;
		glVertex2f(radius_contrast*cos(theta)+mouse_x, radius_contrast*sin(theta)+mouse_y);
	}
	glEnd();

	glColor4f(rgb[0]/COLORS, rgb[1]/COLORS, rgb[2]/COLORS, 0.7f);
	glBegin(GL_POLYGON);
	for(int i=0; i<16; i++)
	{
		theta = i*M_PI/8;
		glVertex2f(radius*cos(theta)+mouse_x, radius*sin(theta)+mouse_y);
	}
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glLineWidth(1.5f);
	glBegin(GL_LINE_LOOP);
	for(int i=0; i<16; i++)
	{
		theta = i*M_PI/8;
		glVertex2f(radius_contrast*cos(theta)+mouse_x, radius_contrast*sin(theta)+mouse_y);
	}
	glEnd();
} // Canvas::brush_indicator()

void Canvas::background()
{
	float q = (2.0f/Canvas::width); // checker size
	bool s = true; // Flag for checkered style

	// Draw solid background
	glColor3f(220/COLORS,220/COLORS,220/COLORS);

	glBegin(GL_POLYGON);

	glVertex2f(-1,-1);
	glVertex2f(+1,-1);
	glVertex2f(+1,+1);
	glVertex2f(-1,+1);

	glEnd();

	// Draw a checkerd background ontop of solid background.
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			GLfloat k=0.0f+i, l=0.0f+j;

			if(s)
			{
				glColor3f(192/COLORS,192/COLORS,192/COLORS);
				glBegin(GL_POLYGON);

				glVertex2f(((k/(width/2))-1),((l/(height/2))-1));
				glVertex2f(((k/(width/2))-1)+q,((l/(height/2))-1));
				glVertex2f(((k/(width/2))-1)+q,((l/(height/2))-1)+q);
				glVertex2f(((k/(width/2))-1),((l/(height/2))-1)+q);

				glEnd();
				s=!s;
			}
			else
			{
				s=!s;
			}
		}
		s=!s;
	}
} // Canvas::background()

void Canvas::paint(float x, float y, int r, int g, int b, float a)
{
	int x_s = ((((x+1)/2)*width));
	int y_s = ((((y+1)/2)*height));

	if((x_s>=0 && x_s<width) && (y_s>=0 && y_s<height))
	{
		CanvasNode* node = graph[x_s][y_s];

		switch (brush_type)
		{
			case BRUSH_TYPE::PIXEL:
				brush.pixel(node, (float)r, (float)g, (float)b, a);
				break;

			case BRUSH_TYPE::FLOOD:
				brush.flood_fill_start(node, (float)r, (float)g, (float)b, a);
				break;

			default:
				break;
		}
	}
}

void Canvas::clear(float x, float y)
{
	// Calculate node index in array.
	int x_s = ((((x+1)/2)*width));
	int y_s = ((((y+1)/2)*height));

	// Clear the node at the speified x & y.
	if((x_s>=0 && x_s<width) && (y_s>=0 && y_s<height))
	{
		graph[x_s][y_s]->setInvisible();
		graph[x_s][y_s]->setColor(-1,-1,-1);
	}
} // Canvas::paint(float x, float y)

void Canvas::clear()
{
	// Clear all ndoes in graph.
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			graph[i][j]->setInvisible();
			graph[i][j]->setColor(-1,-1,-1);
		}
	}
} // Canvas::clear()

///////////////////////////
//	Getters
///////////////////////////

/** Get the contained graph */
CanvasNode*** Canvas::getGraph() {return graph; }

BRUSH_TYPE Canvas::get_tool() { return brush_type; }

///////////////////////////
//	Setters
///////////////////////////

void Canvas::set_tool(BRUSH_TYPE type)
{
	brush_type = type;
}
