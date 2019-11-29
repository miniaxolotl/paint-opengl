/**
 * object.h
 * Interface for an base object
 * Author:		Elias Mawa
 * Created on: 	10-16-2019
 * Last Edit:	10-16-2019
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "../global.h"

/**
 * Generic template for all objects in program.
 */
class Object
{
public:
	/**
	 * Sets object ID & increments total object counter.
	 */
    Object();

	/**
	 * Decrease object counter.
	 */
    ~Object();

	/**
	 * Redraw the object.
	 */
    virtual void draw() = 0;

	/**
	 * Update the object logic.
	 */
    virtual void update() = 0;
private:
	/** Object ID code. */
	unsigned long ID;
};

#endif // OBJECT_H
