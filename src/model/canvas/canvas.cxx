#include "canvas.h"

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
			// North node
			if(j-1>0) { graph[i][j]->setNeighbor(graph[i][j-1], DIRECTION::NORTH); }
			else { graph[i][j]->setNeighbor(NULL, DIRECTION::NORTH); }

			// South node
			if(j+1<height) { graph[i][j]->setNeighbor(graph[i][j+1], DIRECTION::SOUTH); }
			else { graph[i][j]->setNeighbor(NULL, DIRECTION::SOUTH); }

			// East node
			if(i+1<width) { graph[i][j]->setNeighbor(graph[i+1][j], DIRECTION::EAST); }
			else { graph[i][j]->setNeighbor(NULL, DIRECTION::EAST); }

			// West node
			if(i-1>0) { graph[i][j]->setNeighbor(graph[i-1][j], DIRECTION::WEST); }
			else { graph[i][j]->setNeighbor(NULL, DIRECTION::WEST); }
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

void Canvas::paint(float x, float y, unsigned short r, unsigned short g, unsigned short b,  float a)
{
	// printf("%f,%f\n",x,y);
	int x_s = ((((x+1)/2)*width));
	int y_s = ((((y+1)/2)*height));
	// printf("%d,%d\n",x_s,y_s);

	// printf("x:%d y:%d - scaled\n",x_s,y_s);
	if((x_s>=0 && x_s<width) && (y_s>=0 && y_s<height))
	{
		graph[x_s][y_s]->setColor(r,g,b,a);
		graph[x_s][y_s]->setVisible();
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
	}
} // Canvas::paint(float x, float y)

///////////////////////////
//	Getters
///////////////////////////

/** Get the contained graph */
CanvasNode*** Canvas::getGraph() {return graph; } // Canvas::getGraph()

///////////////////////////
//	Setters
///////////////////////////