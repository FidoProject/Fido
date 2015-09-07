/*
 * pnew.cpp
 *
 *  Created on: 24 Dec 2011
 *      Author: Andy Brown
 */

/**
 * Global placement operator new
 */

#include <cstddef>

void* operator new(std::size_t size_,void *ptr_)
{
	return ptr_;
}
