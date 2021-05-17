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

#include <queue>
#include <set>

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

	/** Start a flood fill at the given node */
	void flood_fill_start(CanvasNode* node, float r, float g, float b, float a);
	/** Process one batch of the active flood fill. Returns true if still running. */
	bool flood_fill_step(int batch_size = 50);
	/** Check if a flood fill is currently in progress */
	bool is_filling() const;
	/** Cancel any active flood fill */
	void cancel_fill();

	/** Single pixel brush */
	void pixel(CanvasNode* node, float r, float g, float b, float a);
	/** Brush helper */
	void pixel(CanvasNode* node, float r, float g, float b, float a, int size);

	///////////////////////////
	//	Getters
	///////////////////////////

	///////////////////////////
	//	Setters
	///////////////////////////

private:
	// Incremental flood fill state
	std::queue<CanvasNode*> ff_queue;
	std::set<CanvasNode*> ff_visited;
	float ff_old_r, ff_old_g, ff_old_b, ff_old_a;
	float ff_new_r, ff_new_g, ff_new_b, ff_new_a;
	bool ff_active;
};

#endif // BRUSH_H
