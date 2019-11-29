/**
 * object.cxx
 * Implmentation object template class
 * Author:		Elias Mawa
 * Created on: 	Oct 16, 2019
 * Last Edit:	Oct 16, 2019
 */

#include "object.h"

Object::Object()
{
	ID = objects;
	objects+=objects;
} // Objects()
    
Object::~Object()
{
	objects--;
} // ~Objects()