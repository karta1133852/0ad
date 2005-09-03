#ifndef __SIMPLEGROUP_H__
#define __SIMPLEGROUP_H__

#include "objectgroupplacer.h"
#include "tileclass.h"

class SimpleGroup : public ObjectGroupPlacer
{
public:
	class Element {
	public:
		std::string type;
		int count;
		float distance;
		
		Element::Element();
		Element::Element(const std::string& type, int count, float distance);
		Element::~Element();

		bool place(int cx, int cy, class Map* m, Constraint* constr, std::vector<Object*>& ret);
	};

	std::vector<Element*> elements;
	int x, y;
	TileClass* tileClass;

	virtual bool place(class Map* m, Constraint* constr);

	SimpleGroup(std::vector<Element*>& elements, TileClass* tileClass, int x, int y);
	virtual ~SimpleGroup(void);
};

#endif