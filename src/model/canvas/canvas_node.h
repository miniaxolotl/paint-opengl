/**
 * canvas_node.h
 * Node (pixel) in a canvas
 * Author:		Elias Mawa
 * Created on: 	Oct 16, 2019
 * Last Edit:	Oct 16, 2019
 */

#ifndef CANVAS_NODE_H
#define CANVAS_NODE_H

#include "../../global.h"
#include "../object.h"

#include "../../type/direction_type.h"

/** 
 * Representation of a node (pixel) on a canvas
 */
class CanvasNode : private Object
{
public:
	/* Constructors & Deconstructors */

	/** 
	 * Create a node with a specified x & y location
	 * @param x The x location of node
	 * @param y The y location of node
	 */
    CanvasNode(float x, float y);

    ~CanvasNode();

	/* Functions */

	/** Draw the node */
    void draw();
	/** Update the node */
    void update();

	/** Set the rgb value of the node  */
	void paint(unsigned short r, unsigned short g, unsigned short b);

	/** Get the neighbor of the node in the specified direction */
	void getLink(DIRECTION direction);

	///////////////////////////
	//	Getters
	///////////////////////////

	///////////////////////////
	//	Setters
	///////////////////////////

	/** Set the color of the node */
	void setColor(float r, float g, float b, float a = 0);

	/** Set the node as visible */
	void setVisible() { visible = true; }
	/** Set the node as invisible */
	void setInvisible() { visible = false; }

	/** Set the neightbor of the node in the specified direction */
	void setNeighbor(CanvasNode* node, DIRECTION direction);
private:
	/** width of the node */
	int width = 2/Canvas::width;
	/** height of the node */
	int height = width;

	/** North link */
	CanvasNode* n;
	/** South link */
	CanvasNode* s;
	/** East link */
	CanvasNode* e;
	/** West link */
	CanvasNode* w;

	/** Visiblity flag for node */
	bool visible = false;

	/** Red level */
	float r;
	/** Green level */
	float g;
	/** Blue level */
	float b;
	/** Alpha level */
	float a;

	/** x location of node */
	float x;
	/** y location of node */
	float y;
};

#endif // CANVAS_H
