#ifndef PRESENT_CLASS
#define PRESENT_CLASS

class Present
{
private:
	int _id;
	int _weight;
public:
	Present(int id, int weihght);
	int Id() { return this->_id; };
	int Weight() { return this->_weight; };
	void SetWeight(int w) { _weight = w; };
};

#endif