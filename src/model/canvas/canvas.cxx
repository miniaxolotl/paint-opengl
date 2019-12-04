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
} // Canvas::update()
 
void Canvas::color_bar()
{
	/* Viewport bounds */
	float x0 = -1, x1 = 1, y0 = -1, y1 = 1;

	/* RGB Values */
	int rgb_val[3] = { 0, 0 ,0 };
	// int q = width * 2;

	for(int i=0;i<360;i++)
	{
		// HSVtoRGB(i*(360/q),1,1,rgb);
		HSVtoRGB(i*6,rgb_val);
		
		double x0 = ((float)i/width)-1;
		double x1 = (((float)i/width)-1)+((float)1/width);
		double y0 = ((float)1/height)-1;
		double y1 = -1;

		// Draw rainbow sliver
		glColor3f(rgb_val[0]/COLORS,rgb_val[1]/COLORS,rgb_val[2]/COLORS);
		glBegin(GL_POLYGON);
		glVertex2f(x0,y0);
		glVertex2f(x1,y0);
		glVertex2f(x1,y1);
		glVertex2f(x0,y1);
		glEnd();    
	}
}

void Canvas::brush_indicator()
{		 
	/** radius of small indicator */
	float radius = ( .03125 );
	/** radius of large indicator */
	float radius_contrast = radius+0.03125;
	float x = -1+radius_contrast;
	float y = -1+radius_contrast+((float)1/height);
    float theta;

	/* Background object */
	glColor4f(rgb[0]/COLORS, rgb[1]/COLORS, rgb[2]/COLORS,0.3f);

	glBegin(GL_POLYGON);

	for(int i=0; i<16; i++)
	{
        theta = i*M_PI/8;
        glVertex2f(radius_contrast*cos(theta)+x, radius_contrast*sin(theta)+y);
    }
	glEnd();

	/* Foreground object */
	glColor4f(rgb[0]/COLORS,rgb[1]/COLORS,rgb[2]/COLORS,7.0f);

	glBegin(GL_POLYGON);
	for(int i=0; i<16; i++)
	{
        theta = i*M_PI/8;
        glVertex2f(radius*cos(theta)+x, radius*sin(theta)+y);
    }
	glEnd();

} // Canvas::update()

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
	for(int i=0;i<32;i++)
	{
		for(int j=0;j<32;j++)
		{
			GLfloat k=0.0f+i, l=0.0f+j;

			if(s)
			{
				glColor3f(192/COLORS,192/COLORS,192/COLORS);
				glBegin(GL_POLYGON);

				glVertex2f(((k/16)-1),((l/16)-1));
				glVertex2f(((k/16)-1)+q,((l/16)-1));
				glVertex2f(((k/16)-1)+q,((l/16)-1)+q);
				glVertex2f(((k/16)-1),((l/16)-1)+q);

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
	// Calculate node index in array.
	int x_s = ((((x+1)/2)*width));
	int y_s = ((((y+1)/2)*height));

	if((x_s>=0 && x_s<width) && (y_s>=0 && y_s<height))
	{
		// Pointer to the specified node.
		CanvasNode* node = graph[x_s][y_s];

		switch (brush_type)
		{
			case BRUSH_TYPE::PIXEL:
				brush.pixel(node,r,g,b);
				break;

			case BRUSH_TYPE::FLOOD:
				brush.flood_fill(node,r,g,b);
				break;

			default:
				break;
		}
	}
} // Canvas::paint(float x, float y, unsigned short r, unsigned short g, unsigned short b)

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
CanvasNode*** Canvas::getGraph() {return graph; } // Canvas::getGraph()

///////////////////////////
//	Setters
///////////////////////////

void Canvas::set_tool(BRUSH_TYPE type)
{
	brush_type = type;
}