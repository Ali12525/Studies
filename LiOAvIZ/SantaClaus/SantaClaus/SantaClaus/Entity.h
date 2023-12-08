#ifndef ENTITY_CLASS
#define ENTITY_CLASS

#include <map>

class Entity
{
private:

public:
	Entity(int* keepingEnt, int keepingEntCount);

	int FindIn(int id);
};



#endif
