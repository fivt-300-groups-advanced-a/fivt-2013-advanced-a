#include <fstream>
#include <vector>
#include <queue>
//#include <memory.h>

using namespace std;

template <class Type, class AnyReader, class AnyWriter, class AnyComparator>
class ExternalSorter
{
	typedef pair<Type, int> myPair;

	//template <typename Comparator>
	struct CompClass
	{
		private:
			AnyComparator comp;
		public:
			explicit CompClass(AnyComparator & _comp)
			{
				comp = _comp;
			}
			CompClass(){}
			bool operator () (const myPair & A, const myPair & B)
			{
				return(comp(B.first, A.first));
			}
	};
private:
	//const static int max_K = 1000;
	const static int elementsInPart = 10000;
	AnyReader * reader;
	AnyWriter * resultWriter;
	//in destructor we don't delete this 2 objects, because they were created in main program and will be destroyed there
	
	vector <AnyReader *> readersVector;
	char buf[10];
	int K;
	long long n;
	Type a[elementsInPart];

	CompClass pairComparator;
	AnyComparator typeComparator;
	//AnyReader new_reader;

	//priority_queue <myPair> order;
	


public:
	ExternalSorter(AnyReader & _reader, AnyWriter & _writer, AnyComparator & _cmp)// : reader(_reader), resultWriter(_writer), cmp(cmp1) {}
	{
		reader = &_reader;
		resultWriter = &_writer;
		typeComparator = _cmp;
		pairComparator = CompClass(typeComparator);
	}

	void addPart(int & elemCnt)
	{
		sort(a, a + elemCnt, typeComparator);
		//memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d", K);
		AnyWriter w(buf);
		for (int j = 0; j < elemCnt; j++)
			w(a[j]);
		elemCnt = 0;
		K++;
		w.close();
	}

	void externalSort()
	{
		K = 0;
		int elemCnt = 0;
		while((*reader)(a[elemCnt]))
		{
			elemCnt++;
			n++;
			if (elemCnt == elementsInPart)
			{
				addPart(elemCnt);
			}
		}
		if (elemCnt != 0)
			addPart(elemCnt);
		priority_queue <myPair, vector<myPair>, CompClass> order(pairComparator);
		
		
		for (int part = 0; part < K; part++)
		{
			sprintf(buf, "%d", part);
			AnyReader * ptr = new AnyReader(buf);
			readersVector.push_back(ptr);
			Type x;
			(*readersVector.back())(x);
			order.push(make_pair(x, part));
		}
		//Type tmp;
		//readersVector[0](tmp);
		//readersVector[0](tmp);
		while(!order.empty())
		{
			pair <Type, int> minElem = order.top();
			order.pop();
			(*resultWriter)(minElem.first);
			Type newElem;
			bool canRead = (*readersVector[minElem.second])(newElem);
			if (canRead)
				order.push(make_pair(newElem, minElem.second));
		}
		resultWriter->close();
	}

	~ExternalSorter()
	{
		for (int j = 0; j < readersVector.size(); j++)
			delete readersVector[j];
	}
};