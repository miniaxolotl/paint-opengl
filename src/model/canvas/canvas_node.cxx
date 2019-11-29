#include "canvas_node.h"
#include "canvas.h"

/*************************
 * Constructors & Deconstructors
 *************************/

CanvasNode::CanvasNode(float x_val, float y_val) : Object::Object()
{
	float q = (2.0f/Canvas::width);
	float r = q/2;
	
	x = x_val+r;
	y = y_val+r;
} // CanvasNode::CanvasNode(float x_val, float y_val) : Object::Object()

CanvasNode::~CanvasNode()
{

} // CanvasNode::~CanvasNode()

/* Functions */

void CanvasNode::draw()
{
	if(visible)
	{
		float q = (2.0f/Canvas::width);
		float r = q/2;

		glColor3f(r/255,g/255,b/255);
		// glPointSize(2.5f);
		glBegin(GL_POLYGON);

		glBegin(GL_POLYGON);

		glVertex2f(x-r, y-r);
		glVertex2f(x+r, y-r);
		glVertex2f(x+r, y+r);
		glVertex2f(x-r, y+r);

		glEnd();
	}
} // CanvasNode::draw()

void CanvasNode::update()
{

} // CanvasNode::update()

void CanvasNode::paint(unsigned short r, unsigned short g, unsigned short b)
{

} // CanvasNode::paint(unsigned short r, unsigned short g, unsigned short b)

void CanvasNode::getLink(DIRECTION direction)
{

} // CanvasNode::getLink(DIRECTION direction)

///////////////////////////
//	Getters
///////////////////////////

///////////////////////////
//	Setters
///////////////////////////

void CanvasNode::setColor(float red, float green, float blue, float alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
} // CanvasNode::setColor(float red, float green, float blue, float alpha)

void CanvasNode::setNeighbor(CanvasNode* node, DIRECTION direction)
{
	switch (direction)
	{
		case DIRECTION::NORTH:
			n = node;
			break;
		case DIRECTION::SOUTH:
			s = node;
			break;
		case DIRECTION::EAST:
			e = node;
			break;
		case DIRECTION::WEST:
			w = node;
			break;
		case DIRECTION::SIZE:
			// do nothing
			break;
	}
} // CanvasNode::setNeighbor(CanvasNode* node, DIRECTION direction)