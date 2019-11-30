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
	// // Generate all nodes
	// graph = new CanvasNode**[w];
	// for(int i=0;i<w;i++)
	// {
	// 	graph[i] = new CanvasNode*[h];
	// }

	// // Instantiate all nodes
	// for(int i=0;i<w;i++)
	// {
	// 	for(int j=0;j<h;j++)
	// 	{
	// 		graph[i][j] = new CanvasNode();
	// 	}
	// }

	// // Set all the node pointers
	// for(int i=0;i<w;i++)
	// {
	// 	for(int j=0;j<h;j++)
	// 	{
	// 		// North node
	// 		if(j-1>0) { graph[i][j]->setNeighbor(graph[i][j-1], DIRECTION::NORTH); }
	// 		else { graph[i][j-1]->setNeighbor(NULL, DIRECTION::NORTH); }

	// 		// South node
	// 		if(j+1>0) { graph[i][j]->setNeighbor(graph[i][j+1], DIRECTION::SOUTH); }
	// 		else { graph[i][j+1]->setNeighbor(NULL, DIRECTION::SOUTH); }

	// 		// East node
	// 		if(i+1>0) { graph[i][j]->setNeighbor(graph[i+1][j], DIRECTION::EAST); }
	// 		else { graph[i+1][j]->setNeighbor(NULL, DIRECTION::EAST); }

	// 		// West node
	// 		if(i-1>0) { graph[i][j]->setNeighbor(graph[i-1][j], DIRECTION::WEST); }
	// 		else { graph[i-1][j]->setNeighbor(NULL, DIRECTION::WEST); }
	// 	}
	// }
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
} // Canvas::draw()

void Canvas::update()
{
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			graph[i][j]->update();
		}
	}
} // Canvas::update()

void Canvas::brush_indicator()
{		 

	float radius = (.0625);
	float radius_contrast = radius+0.0625;
	float x = 1-radius_contrast;
    float theta;

	// int u_color = 0xbd38fb + 0xe;
	// unsigned rIntValue = (u_color / 256 / 256) % 256;
	// unsigned gIntValue = (u_color / 256      ) % 256;
	// unsigned bIntValue = (u_color            ) % 256;
	// printf("%x %u %u %u\n", u_color, rIntValue, gIntValue, bIntValue);

	glColor4f(((255.0f-r)/255.0f),((255.0f-g)/255.0f),((255.0f-b)/255.0f),0.6f);
	glBegin(GL_POLYGON);
	// background
	for(int i=0; i<16; i++)
	{
        theta = i*M_PI/8;
        glVertex2f(radius_contrast*cos(theta)+x, radius_contrast*sin(theta)+x);
    }
	glEnd();

	glColor4f(r/255.0f,g/255.0f,b/255.0f,0.6f);
	glBegin(GL_POLYGON);
	// forground
	for(int i=0; i<16; i++)
	{
        theta = i*M_PI/8;
        glVertex2f(radius*cos(theta)+x, radius*sin(theta)+x);
    }
	glEnd();

} // Canvas::update()

void Canvas::background()
{
	float q = (2.0f/Canvas::width);
	float r = q/2.0f;
	bool s = true;

	// whole background
	glColor3f(220/255.0f,220/255.0f,220/255.0f);
	glBegin(GL_POLYGON);

	glVertex2f(-1,-1);
	glVertex2f(+1,-1);
	glVertex2f(+1,+1);
	glVertex2f(-1,+1);

	glEnd();

	for(int i=0;i<32;i++)
	{
		for(int j=0;j<32;j++)
		{
			GLfloat k=0.0f+i, l=0.0f+j;

			if(s)
			{
				glColor3f(192/255.0f,192/255.0f,192/255.0f);
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

void Canvas::paint(float x, float y, int r, int g, int b, int a)
{
	int x_s = ((((x+1)/2)*width));
	int y_s = ((((y+1)/2)*height));

	CanvasNode* node = NULL;

	if((x_s>=0 && x_s<width) && (y_s>=0 && y_s<height))
	{
		node = graph[x_s][y_s];

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
	// printf("%f,%f\n",x,y);
	int x_s = ((((x+1)/2)*width));
	int y_s = ((((y+1)/2)*height));
	// printf("%d,%d\n",x_s,y_s);

	// printf("x:%d y:%d - scaled\n",x_s,y_s);
	if((x_s>=0 && x_s<width) && (y_s>=0 && y_s<height))
	{
		graph[x_s][y_s]->setInvisible();
		graph[x_s][y_s]->setColor(-1,-1,-1);
	}
} // Canvas::paint(float x, float y)

void Canvas::clear()
{
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			graph[i][j]->setInvisible();
			graph[i][j]->setColor(-1,-1,-1);
		}
	}
} // Canvas::paint()

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