/**
 * brush.css
 * Implementation of brush painting algorithims.
 * Author:		Elias Mawa
 * Created on: 	10-16-2019
 * Last Edit:	12-04-2019
 */

#include "brush.h"
#include <queue>
#include <set>
#include <chrono>
#include <thread>

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

void Brush::flood_fill(CanvasNode* node, float r, float g, float b, float a)
{
	if(node == NULL)
	{
		return;
	}
	else
	{
		float old_r = node->getR(), old_g = node->getG(), old_b = node->getB();
		float old_a = node->getA();

		flood_fill(node, old_r, old_g, old_b, old_a, r, g, b, a);
	}
	return;
} // Brush::flood_fill(CanvasNode* node)

void Brush::flood_fill(CanvasNode* node, float old_r, float old_g, float old_b, float old_a, float new_r, float new_g, float new_b, float new_a)
{
	std::chrono::milliseconds dura(25);

	if(node == NULL || kill)
	{
		return;
	}

	// If replacement color is same as old color, nothing to do
	if(node->isVisible() && node->getR() == new_r && node->getG() == new_g && node->getB() == new_b && node->getA() == new_a)
	{
		return;
	}

	// If starting node doesn't match old color, nothing to do
	if(!(node->getR() == old_r && node->getG() == old_g && node->getB() == old_b && node->getA() == old_a))
	{
		return;
	}

	std::queue<CanvasNode*> q;
	std::set<CanvasNode*> visited;

	q.push(node);
	visited.insert(node);

	while(!q.empty() && !kill)
	{
		CanvasNode* current = q.front();
		q.pop();

		int r = current->getR(), g = current->getG(), b = current->getB();
		float a = current->getA();

		// Skip if already filled or color doesn't match
		if(current->isVisible() && r == new_r && g == new_g && b == new_b && a == new_a)
		{
			continue;
		}

		if(r == old_r && g == old_g && b == old_b && a == old_a)
		{
			if(slowmode) { std::this_thread::sleep_for(dura); }

			current->setVisible();
			current->setColor(new_r, new_g, new_b, new_a);

			CanvasNode* neighbors[4] = {
				current->getLink(DIRECTION::NORTH),
				current->getLink(DIRECTION::EAST),
				current->getLink(DIRECTION::SOUTH),
				current->getLink(DIRECTION::WEST)
			};

			for(int i = 0; i < 4; i++)
			{
				CanvasNode* neighbor = neighbors[i];
				if(neighbor != NULL && visited.find(neighbor) == visited.end())
				{
					visited.insert(neighbor);
					q.push(neighbor);
				}
			}
		}
	}
	return;
} // Brush::flood_fill(CanvasNode* node)

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