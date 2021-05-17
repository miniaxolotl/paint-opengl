/**
 * brush.cxx
 * Implementation of brush painting algorithims.
 * Author:		Elias Mawa
 * Created on: 	10-16-2019
 * Last Edit:	12-04-2019
 */

#include "brush.h"
#include <chrono>
#include <thread>

Brush::Brush() : ff_active(false)
{

}

Brush::~Brush()
{
	cancel_fill();
}

/* Functions */

void Brush::draw()
{

} // Brush::draw()

void Brush::update()
{

} // Brush::update()

void Brush::flood_fill_start(CanvasNode* node, float r, float g, float b, float a)
{
	cancel_fill();

	if(node == NULL)
	{
		return;
	}

	ff_old_r = node->getR();
	ff_old_g = node->getG();
	ff_old_b = node->getB();
	ff_old_a = node->getA();

	ff_new_r = r;
	ff_new_g = g;
	ff_new_b = b;
	ff_new_a = a;

	// If replacement color is same as old color, nothing to do
	if(node->isVisible() && ff_old_r == ff_new_r && ff_old_g == ff_new_g && ff_old_b == ff_new_b && ff_old_a == ff_new_a)
	{
		return;
	}

	// If starting node doesn't match old color, nothing to do
	if(!(node->getR() == ff_old_r && node->getG() == ff_old_g && node->getB() == ff_old_b && node->getA() == ff_old_a))
	{
		return;
	}

	ff_queue.push(node);
	ff_visited.insert(node);
	ff_active = true;
}

bool Brush::flood_fill_step(int batch_size)
{
	if(!ff_active || kill)
	{
		ff_active = false;
		return false;
	}

	std::chrono::milliseconds dura(10);
	int processed = 0;

	while(!ff_queue.empty() && !kill && processed < batch_size)
	{
		CanvasNode* current = ff_queue.front();
		ff_queue.pop();

		int r = current->getR(), g = current->getG(), b = current->getB();
		float a = current->getA();

		// Skip if already filled or color doesn't match
		if(current->isVisible() && r == ff_new_r && g == ff_new_g && b == ff_new_b && a == ff_new_a)
		{
			continue;
		}

		if(r == ff_old_r && g == ff_old_g && b == ff_old_b && a == ff_old_a)
		{
			if(slowmode) { std::this_thread::sleep_for(dura); }

			current->setVisible();
			current->setColor(ff_new_r, ff_new_g, ff_new_b, ff_new_a);

			CanvasNode* neighbors[4] = {
				current->getLink(DIRECTION::NORTH),
				current->getLink(DIRECTION::EAST),
				current->getLink(DIRECTION::SOUTH),
				current->getLink(DIRECTION::WEST)
			};

			for(int i = 0; i < 4; i++)
			{
				CanvasNode* neighbor = neighbors[i];
				if(neighbor != NULL && ff_visited.find(neighbor) == ff_visited.end())
				{
					ff_visited.insert(neighbor);
					ff_queue.push(neighbor);
				}
			}
		}
		processed++;
	}

	if(ff_queue.empty() || kill)
	{
		ff_active = false;
		return false;
	}

	return true;
}

bool Brush::is_filling() const
{
	return ff_active;
}

void Brush::cancel_fill()
{
	ff_active = false;
	while(!ff_queue.empty()) ff_queue.pop();
	ff_visited.clear();
}

void Brush::pixel(CanvasNode* node, float r, float g, float b, float a)
{
	pixel(node, r, g, b, a, pixel_size);
} // Brush::pixel(CanvasNode* node)

void Brush::pixel(CanvasNode* node, float r, float g, float b, float a, int size)
{
	if(node == NULL || size < 1)
	{
		return;
	}
	else
	{
		node->setColor(r,g,b,a);
		node->setVisible();

		pixel(node->getLink(DIRECTION::NORTH), r, g, b, a, size-1);
		pixel(node->getLink(DIRECTION::EAST), r, g, b, a, size-1);
		pixel(node->getLink(DIRECTION::SOUTH), r, g, b, a, size-1);
		pixel(node->getLink(DIRECTION::WEST), r, g, b, a, size-1);

	}
} // Brush::pixel(CanvasNode* node)

///////////////////////////
//	Getters
///////////////////////////

///////////////////////////
//	Setters
///////////////////////////
