#ifndef META_ASSIGN_DECL
#define META_ASSIGN_DECL

struct MetaAssign
{
	bool assigned;
	int assignedValue;
	MetaAssign()
	{
		assigned = false;
		assignedValue = 0;
	}
	MetaAssign(bool _assigned, int _assignedValue) : assigned(_assigned), assignedValue(_assignedValue) {}
	void clear()
	{
		assigned = false;
		assignedValue = 0;
	}
};

#endif