/*
 * avr_config.h
 * Contains values that you can change to customize the way that the library behaves
 *
 *  Created on: 1 Jan 2011
 *      Author: Andy Brown
 */

#ifndef __C4BCBBDE_67BC_4bb4_A5E1_7745E49AF0B6
#define __C4BCBBDE_67BC_4bb4_A5E1_7745E49AF0B6

#include <stdlib.h>


namespace avrstl {

// default alloc-ahead for vectors. quoting from the SGI docs:
//
//   "It is crucial that the amount of growth is proportional to the current capacity(),
//    rather than a fixed constant: in the former case inserting a series of elements
//    into a vector is a linear time operation, and in the latter case it is quadratic."
//
// If this advice pertains to you, then uncomment the first line and comment out the second.
// The default here in avr-land is to assume that memory is scarce.

//	template<typename T> size_t AvrVectorAllocAhead(size_t oldSize_) { return 2*oldSize_; }
	template<typename T> size_t AvrVectorAllocAhead(size_t oldSize_) { return 20+oldSize_; }
//	template<> size_t AvrVectorAllocAhead<char>(size_t oldSize_) { return 20+oldSize_; }     // sample specialization for char

// minimum buffer size allocated ahead by a deque

	inline size_t AvrDequeBufferSize() { return 20; }

// alloc-ahead additional memory increment for strings. The default SGI implementation will add
// the old size, doubling memory each time. We don't have memory to burn, so add 20 types each time

	template<typename T> size_t AvrStringAllocAheadIncrement(size_t oldSize_) { return 20; }
}


#endif
