//#pragma once

#ifndef _TEMPLATE_QUEUE_
#define _TEMPLATE_QUEUE_

#include <iostream>

template <class T>
class tempQueue {
public:

	/*T x = 0;
	T y = 0;
	T z = 0;*/

	//how many items are in the queue
	int arrayCounter;

	//size of the queue
	int arraySize;
	T* myArray;
	/*
	void Push(T newItem);
	void Pop();
	void Print(); 
	int GetSize();
	bool IsEmpty();
	*/

	/*tempQueue()
	{
		T x = y = z = 0;
	}*/

	tempQueue()
	{
		//make an array of size one with no members in the queue (yet)
		myArray = new T[1];

		arrayCounter = 0;
		arraySize = 1;

		
	}

	//copy constructor
	tempQueue(const tempQueue& thing)
	{
		arrayCounter = thing.arrayCounter;
		arraySize = thing.arrayCounter;
		
		//make sure to prevent memory leaks
		if (myArray != nullptr) {
			delete [] myArray;
		}

		myArray = new T[arraySize];
		
		for (int arrayIterator = 0; arrayIterator < thing.arraySize; arrayIterator ++) {
			myArray[arrayIterator] = thing.myArray[arrayIterator];
		}
	}

	//copy assignment operator
	tempQueue& operator = (const tempQueue& thing) 
	{
		arrayCounter = thing.arrayCounter;
		arraySize = thing.arrayCounter;
		myArray = thing.myArray;
		return *this;
	}

	//destructor
	~tempQueue() noexcept
	{
		
		delete [] myArray;
	}
	
	void Push(T newItem) 
	{
		//there's a new item in the queue so increase the int that represents the number of items
		arrayCounter++;

		//if the new item would be more than the array size, make a new array twice as big.  Size doubling uses more memory, but is faster as it means I don't need to make a new array each time I push
		if (arrayCounter > arraySize) {
			arraySize = arraySize * 2;
			T* newArray = new T[arraySize];
			
			// copy data from myArray into newArray
			//arraysize is divided by 2 to become its old size to both prevent myArray from going out of bounds and also because there's no point assigning data in the new array that wasn't in the old one
			for (int arrayIterator = 0; arrayIterator < (arraySize / 2); arrayIterator++)
			{
				newArray[arrayIterator] = myArray[arrayIterator];
			}
			
			//old array isn't needed, let's prevent memory leaks
			delete [] myArray;

			//myArray becomes the newArray.  Well, actually, it's more like NewArray becomes myArray.....
			myArray = newArray;
			std::cout << "added " << newItem << " to queue\n";
			
			//now that we made an array big enough to hold the new item, add the new item.  ArrayCounter was increased earlier, so if you don't subtract one you'll be skipping a slot
			myArray[arrayCounter - 1] = newItem;


		}
		//array's big enough to take new items
		else
		{
			std::cout << "added " << newItem << " to queue\n";
			myArray[arrayCounter - 1] = newItem;
		}
		
		
	}

	//deleting myArray was causing heap corruption so this was to make sure that's what was going on
	/*void TestDelete()
	{
		delete myArray;

	}*/

	void Pop()
	{
		//don't pop if there's nothing in the queue.  If there's something in it, pop it
		if (IsEmpty() == false)
		{
			//knowing what's popped will probably help with testing
			std::cout << "popped " << myArray[0] << "\n";
			
			//we don't care about the first item in myArray, as we're overwriting it, so start the iterator at one
			for (int arrayIterator = 1; arrayIterator <= arraySize; arrayIterator++) {
				myArray[arrayIterator - 1] = myArray[arrayIterator];
			}
			//the queue has one less item so let's reflect that
			arrayCounter--;
		}
		
		//If there wasn't something in the queue, then the queue is empty, better let the person know that nothing happened
		else 
		{
			std::cout << "queue is empty \n";
		}
	}

	void Print()
	{
		//bool tempSize = arraySize; //DEBUG APPARENTLY bools can be any unsigned int?  Just keeping this here to remind myself of that in the future
		//bool tempCount = arrayCounter;
		for (int arrayIterator = 0; arrayIterator < arrayCounter; arrayIterator++) {
			std::cout << myArray[arrayIterator] << " ";
		}
		std::cout << "\n current size is " << arraySize << " and current number of items in queue is " << arrayCounter << "\n";
	}

	//I'm currently not using it except in one place I forgot it existed and have just been using arrayCounter itself.  Pretty sure I'd be using this if it were private though.
	int GetSize()
	{
		return arrayCounter;
	}

	bool IsEmpty()
	{
		if (GetSize() == 0)
		{
			return true;
		}
		//error message
		else if (arrayCounter < 0)
		{
			std::cout << "I'm not sure how but you messed something up \n";
			return false;
		}
		else
		{
			return false;
		}
	}
};
#endif //_TEMPLATE_QUEUE_