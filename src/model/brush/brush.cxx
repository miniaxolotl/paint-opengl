/**
 * brush.css
 * Implementation of brush painting algorithims.
 * Author:		Elias Mawa
 * Created on: 	10-16-2019
 * Last Edit:	12-04-2019
 */

#include "brush.h"
#include <thread>
#include <stdlib.h>

Brush::Brush()
{

}

Brush::~Brush()
{

}

/* Functions */

void Brush::draw()
{

} // Brush::draw()

void Brush::update()
{

} // Brush::update()

void Brush::flood_fill(CanvasNode* node, float r, float g, float b)
{

	if(node == NULL)
	{
		return;
	}
	else
	{
		float old_r = node->getR(), old_g = node->getG(), old_b = node->getB();

		flood_fill(node, old_r, old_g, old_b, r, g, b);
	}
	return;
} // Brush::flood_fill(CanvasNode* node)

void Brush::flood_fill(CanvasNode* node, float old_r, float old_g, float old_b, float new_r, float new_g, float new_b)
{
	std::chrono::milliseconds dura(25);

	// return if node does not exist
	if(node == NULL || kill)
	{
		return;
	}
	else
	{
		int r = node->getR(), g = node->getG(), b = node->getB();

		// if replacement color is same as old color, return
		if(r == new_r && g == new_g && b == new_b)
		{
			return;
		}

		// for demo
		if(slowmode) 
		{ 
			std::this_thread::sleep_for(dura);
		}
		
		// if old node color is equal to new node color replace it
		if((r == old_r && g == old_g && b == old_b) || !node->isVisible())
		{
			node->setVisible();

			node->setColor(new_r,new_g,new_b);

			flood_fill(node->getLink(DIRECTION::EAST), old_r, old_g, old_b, new_r, new_g, new_b);
			flood_fill(node->getLink(DIRECTION::NORTH), old_r, old_g, old_b, new_r, new_g, new_b);
			flood_fill(node->getLink(DIRECTION::SOUTH), old_r, old_g, old_b, new_r, new_g, new_b);
			flood_fill(node->getLink(DIRECTION::WEST), old_r, old_g, old_b, new_r, new_g, new_b);
		}	
	}

	return;
} // Brush::flood_fill(CanvasNode* node)

void Brush::pixel(CanvasNode* node, float r, float g, float b)
{
	pixel(node, r, g, b, pixel_size);
} // Brush::pixel(CanvasNode* node)

void Brush::pixel(CanvasNode* node, float r, float g, float b, int size)
{
	if(node == NULL || size < 1)
	{
		return;
	}
	else
	{
		node->setColor(r,g,b);
		node->setVisible();

		pixel(node->getLink(DIRECTION::NORTH), r, g, b, size-1);
		pixel(node->getLink(DIRECTION::EAST), r, g, b, size-1);
		pixel(node->getLink(DIRECTION::SOUTH), r, g, b, size-1);
		pixel(node->getLink(DIRECTION::WEST), r, g, b, size-1);

	}
} // Brush::pixel(CanvasNode* node)

///////////////////////////
//	Getters
///////////////////////////

///////////////////////////
//	Setters
///////////////////////////