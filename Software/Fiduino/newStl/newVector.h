#ifndef NEWVECTOR_H
#define NEWVECTOR_H

/// adapted from Data Structures and Algorithm Analysis in C++ by Mark Allan Weiss

namespace std {
	template <typename Object>
	class Vector {  
	  public:
	  	// Construct a vector of a certain storage capaity
	    explicit Vector(int initSize = 0):theSize(initSize),theCapacity(initSize + SPARE_CAPACITY) {
	        objects = new Object[ theCapacity ];
	    }
	    
	    // Construct a vector from another vector
	    Vector(const Vector & rhs ): objects(0) { operator=(rhs); }

	    // Destruct a vector
	    ~Vector() { delete [] objects; }

	    // Overridden = operator
	    const Vector & operator=(const Vector & rhs) {
	        if(this != &rhs) {
	            delete[] objects;
	            theSize = rhs.size();
	            theCapacity = rhs.theCapacity;
	            
	            objects = new Object[capacity()];
	            for(int k = 0; k < size(); k++)
	            	objects[k] = rhs.objects[k];
	        } return *this;
	    }

	    // Change vector size
	    void resize(int newSize) {
	        if(newSize > theCapacity) reserve(newSize * 2 + 1);
	        theSize = newSize;
	    }

	    // Request a change in vector capacity
	    void reserve(int newCapacity) {
	        if(newCapacity < theSize) return;
	        Object *oldArray = objects;
	        objects = new Object[newCapacity];
	        for(int k = 0; k < theSize; k++) 
	        	objects[k] = oldArray[k];
	        theCapacity = newCapacity;
	        delete [] oldArray;
	    }

	    // Overridden [] operator
	    Object & operator[](int index) { return objects[index]; }
	    const Object& operator[](int index) const { return objects[index]; }

	    // Test whether vector is empty
	    bool empty() const { return size() == 0; }
	    
	    // Return vector size
	    int size() const { return theSize; }
	    
	    // Return size of allocated storage capacity
	    int capacity() const { return theCapacity; }

	    // Add an element at the end of the vector
	    void push_back(const Object& x) {
	        if(theSize == theCapacity)
	            reserve(2 * theCapacity + 1);
	        objects[theSize++] = x;
	    }

	    // Delete the last element of the vector
	    void pop_back() { theSize--; }

	    // Access last element of vector
	    const Object& back() const { return objects[theSize - 1]; }

	    typedef Object* iterator;
	    typedef const Object* const_iterator;

	    // Return iterator to the beginning
	    iterator begin() { return &objects[0]; }
	    const_iterator begin() const { return &objects[0]; }

	    // Return iterator to the end
	    iterator end() { return &objects[size() - 1]; }
	    const_iterator end() const { return &objects[size() - 1]; }

	    enum { SPARE_CAPACITY = 16 };  
	  private:
	    int theSize,theCapacity;
	    Object* objects;
	};
};

#endif