/**
 * brush.h
 * Collection of algorithims for drawing brush strokes
 * Author:		Elias Mawa
 * Created on: 	11-29-2019
 * Last Edit:	11-29-2019
 */

#ifndef BRUSH_H
#define BRUSH_H

#include "../../global.h"
#include "../object.h"

#include "../canvas/canvas_node.h"

/** 
 * Representation of a node (pixel) on a canvas
 */
class Brush : private Object
{
public:
	/* Constructors & Deconstructors */
	Brush();
    ~Brush();

	/* Functions */

	/** Draw the object */
    void draw();
	/** Update the object */
    void update();

	/** Flood fill algorithim brush */
	void flood_fill(CanvasNode* node, float r, float g, float b);
	/** Flood fill algorithim brush */
	void flood_fill(CanvasNode* node, float r, float g, float b, float rn, float gn, float bn);
	/** Single pixel brush */
	void pixel(CanvasNode* node, float r, float g, float b);

	///////////////////////////
	//	Getters
	///////////////////////////

	///////////////////////////
	//	Setters
	///////////////////////////

private:

};

#endif // BRUSH_H
