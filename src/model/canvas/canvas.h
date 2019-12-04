/**
 * canvas.h
 * Wrapper for drawing and modifying a canvas.
 * Author:		Elias Mawa
 * Created on: 	10-16-2019
 * Last Edit:	12-04-2019
 */

#ifndef CANVAS_H
#define CANVAS_H

#include "../../global.h"

#include "canvas_node.h"
#include "../../type/brush_type.h"
#include "../brush/brush.h"
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
	/** Draws rainbow spectrum strip */
	void color_bar();
	/** draw the brush */
	void brush_indicator();
	/** Paint a pixel onto the canvas */
	void paint(float x, float y, int red, int green, int blue, float alpha);
	/** Clear the node at the specified coordiantes */
	void clear(float x, float y);
	/** Clear the whole canvas */
	void clear();

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

	/** Set the brush type */
	void set_tool(BRUSH_TYPE type);
	
private:
	/** Graph representation of a canvas */
	CanvasNode*** graph;
	Brush brush;
	BRUSH_TYPE brush_type;
};

#endif // CANVAS_H
