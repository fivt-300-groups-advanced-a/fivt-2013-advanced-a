
template <typename T>
class PermanentIntervalsSegmentTree
{
public:

	struct ReturnType{
		ReturnType(): neutral(true) 
		{}
		ReturnType(T element): neutral(false), number(1), left(element), right(element) 
		{}
		ReturnType(T left, T right, unsigned int number)
			: neutral(false), number(number), left(left), right(right) 
		{}

		long long number;
		bool neutral;
		T left, right;
	};

	long long get(long long left, long right)
	{
		return tree.get(left, right).number;
	}

	void add(long long left, long long right, const T& change_value)
	{
		tree.change(left, right, MetaInformation(1, change_value));
	}

	void assign(long long left, long long right, const T& change_value)
	{
		tree.change(left, right, MetaInformation(0, change_value));
	}	

private:
	struct MetaInformation
	{
		bool operation; // 0 = assign, 1 = add
		T changed;
	
		MetaInformation (bool operation, const T& change): operation(operation),changed(change) {}
    MetaInformation() {}
	};

	struct MergeMetaReturn 
	{
		void operator()(ReturnType& value_to_change, MetaInformation &changes, long long l, long long r)
		{
			if (changes->operation) // increase 
			{
				value_to_change.left += changes->changed;
				value_to_change.right += changes->changed;
			}
			else //assign
			{
				value_to_change.left = value_to_change.right = changes->changed;
				value_to_change.number = 1;
			}
		}
	};

	struct MergeMeta
	{
		void operator()(MetaInformation& old_changes, bool is_meta,  MetaInformation& new_changes, long long l, long long r)
		{
        if (!is_meta) {
          old_changes = new_changes;
          return;
        }
				if (old_changes->operation) // here is increasing
				{ 
					if (new_changes->operation) //merge with increasing
					{
						old_changes->changed += new_changes->changed; // got increasing again
					}
					else //merge with assign
					{
						old_changes->operation=0;
						old_changes->changed = new_changes -> changed; // got assign now
					}
				} 
				else // here is assigning
				{
					if (new_changes->operation) //merge with increasing
					{
						old_changes->changed += new_changes->changed; // got increased assigning now
					}
					else //merge with assigning
					{
						old_changes->changed = new_changes->changed; // got new assigning now
					}
				}
		}
	};

	struct MergeReturn
	{
		ReturnType operator()(const ReturnType& first, const ReturnType& second)
		{
			if (first.neutral) 
				return second;
			else if (second.neutral)
				return first;
			else if (first.right == second.left) 
				return ReturnType(first.left, second.right, first.number + second.number - 1);
			else 
				return ReturnType(first.left, second.right, first.number + second.number);
		}
	};


	GeneralSegmentTree< ReturnType, MetaInformation, MergeMeta, MergeReturn, MergeMetaReturn > tree;
};
