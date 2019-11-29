/**
 * canvas.h
 * Wrapper for drawing and modifying a canvas.
 * Author:		Elias Mawa
 * Created on: 	10-16-2019
 * Last Edit:	11-29-2019
 */

#ifndef CANVAS_H
#define CANVAS_H

#include "../../global.h"

#include "canvas_node.h"
#include "../../type/brush_type.h"
#include "../../type/direction_type.h"

/**
 * Wrapper for a canvas graph
 */
class Canvas : private Object
{
public:
	/* Constructors & Deconstructors */

	/** Construct a default canvas 32x32 */
    Canvas();

	/** Contrtuct a canvas of a specified size
	 * @param width Width of canvas
	 * @param height Height of canvas
	 */
    Canvas(int width, int hight);

    ~Canvas();

	/* Functions */

	/** Draw the object */
    void draw();
	/** Update the object */
    void update();
	
	/** Draw the canvas background */
	void background();
	/** Paint a pixel onto the canvas */
	void paint(float x, float y, unsigned short r, unsigned short g, unsigned short b);

	/* Variables */
	
	/** Pixel width */
    static const int width = 32;
	/** Pixel height */
    static const int height = width;

	///////////////////////////
	//	Getters
	///////////////////////////
	
	/**
	 * Get the graph representation of the canvas
	 * @return Graph of the canvas
	 */
	CanvasNode*** getGraph();

	///////////////////////////
	//	Setters
	///////////////////////////

private:
	/** Graph representation of a canvas */
	CanvasNode*** graph;
};

#endif // CANVAS_H
