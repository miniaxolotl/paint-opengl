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
		float s = q/2;

		glColor4f(r/255.0f,g/255.0f,b/255.0f,1.0f);
		// glPointSize(2.5f);
		glBegin(GL_POLYGON);

		glVertex2f(x-s, y-s);
		glVertex2f(x+s, y-s);
		glVertex2f(x+s, y+s);
		glVertex2f(x-s, y+s);

		glEnd();
	}
} // CanvasNode::draw()

void CanvasNode::update()
{

} // CanvasNode::update()

void CanvasNode::paint(unsigned short r, unsigned short g, unsigned short b)
{

} // CanvasNode::paint(unsigned short r, unsigned short g, unsigned short b)

CanvasNode* CanvasNode::getLink(DIRECTION direction)
{
	switch(direction)
	{
		case DIRECTION::NORTH :
			return n;
			break;
		
		case DIRECTION::EAST :
			return e;
			break;
		
		case DIRECTION::SOUTH :
			return s;
			break;
		
		case DIRECTION::WEST :
			return w;
			break;
		
		case DIRECTION::SIZE :
			return NULL;
			break;
	}
	return NULL;
} // CanvasNode::getLink(DIRECTION direction)

bool CanvasNode::isVisible()
{
	return visible;
}

///////////////////////////
//	Getters
///////////////////////////

///////////////////////////
//	Setters
///////////////////////////

void CanvasNode::setColor(int red, int green, int blue, int alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
} // CanvasNode::setColor(float red, float green, float blue, float alpha)

void CanvasNode::setLink(CanvasNode* node, DIRECTION direction)
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