#include <iostream>

using namespace std;

class Station
{
public:
	Station(string n,int l);
	~Station();
	void appendNeight(Station s);
	string getName();
	int getNeiNum();
	Station getNei(int i);

private:
	string name;
	int lineNum[3];
	static Station neight[4];
	int neiNum;
};

Station::Station(string n,int l)
{
	name = n;
	lineNum[0] = l;
	neiNum = 0;
}

Station::~Station()
{
}

string Station::getName() {
	return name;
}

int Station::getNeiNum() {
	return neiNum;
}

void Station::appendNeight(Station s)
{
	neight[neiNum++] = s;
}

Station Station::getNei(int i) {
	return neight[i];
}