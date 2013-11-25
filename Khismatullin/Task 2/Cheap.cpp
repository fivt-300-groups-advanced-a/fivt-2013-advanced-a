#ifndef BINOMIAL_HEAP

#define BINOMIAL_HEAP

template<class T>
class Ctree
{
	friend class Cheap;
	public:
		Ctree(T _value)
		{
			value = _value;
		}
		int getRank()
		{
			return rank;
		}
		Ctree* getNext()
		{
			
		}
		

	private:
		T value;
		Ctree* parent, son, brother;
		int rank;
};



#endif