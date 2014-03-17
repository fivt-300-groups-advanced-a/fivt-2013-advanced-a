#ifndef SEGMENT_TREE

#define SEGMENT_TREE


#include <vector>
#include <cassert>
#include <iostream>
#include <cmath>
using namespace std;


template <class Type, class updType>
class Stree
{
	public:
		Stree() {
			size = 0;
			#ifdef TESTER
			get_cnt = get_it = update_cnt = update_it = 0;
			#endif
		}
		
		explicit Stree(int n) {
			construct(std::vector<Type>(n));
			#ifdef TESTER
			get_cnt = get_it = update_cnt = update_it = 0;
			#endif
		}
		
		explicit Stree(const std::vector<Type> &v) {
			construct(v);
			#ifdef TESTER
			get_cnt = get_it = update_cnt = update_it = 0;
			#endif
		}

		void resize(int n)
		{
			construct(std::vector<Type>(n));
		}
		
		void clear() {
			tree.clear();
			updator.clear();
			change.clear();
			size = 0;
		}
		
		Type get(int l, int r)
		{
			#ifdef TESTER
			get_cnt++;
			#endif
			assert((1 <= l) && (r <= size)); 
			return get(1, 1, (1 << (deg - 1)), l, r);
		}
		
		void update(int l, int r, const updType &upd)
		{
			#ifdef TESTER
			update_cnt++;
			#endif
			assert((1 <= l) && (r <= size)); 
			update(1, 1, (1 << (deg - 1)), l, r, upd);
		}
		
		#ifdef TESTER
		bool check()
		{
			return (get_it <= 3*get_cnt*deg&& update_it <= 3*update_cnt*deg);
		}
		
		bool is_empty()
		{
			return (size == 0);
		}
		#endif
		
	private:
		
		unsigned int size, deg;
		std::vector<Type> tree;
		std::vector<updType> updator;
		std::vector<bool> change; 
		#ifdef TESTER
		int get_cnt, update_cnt, get_it, update_it;
		#endif
		
		void construct(const std::vector<Type> &v)
		{
			clear();
			size = v.size();
			if (size > 0)
			{
				for(deg = 0; (1 << deg) < size; deg++);
				deg++;
				tree.resize(1 << deg, Type());
				updator.resize(1 << deg, updType());
				change.resize(1 << deg, false);
				build(1, 1, 1 << (deg - 1), v);
			}
		}
		
		void build(int v, int l, int r, const std::vector<Type> &vect)
		{
			if (l > size) 
				return;
			if (l == r) {
				tree[v] = vect[l - 1];
				return;
			}
			build(v << 1, l, (l + r) >> 1, vect);
			build((v << 1) + 1, ((l + r) >> 1) + 1, r, vect);
			Type res = tree[v << 1];
			res.merge(tree[(v << 1) + 1]);
			tree[v] = res;
		}
	
		void push(int v, int l, int r)
		{
			if (change[v]) {
				if (l < r) {
					updator[v << 1].merge(updator[v]);
					change[v << 1] = true;
					updator[(v << 1) + 1].merge(updator[v]);
					change[(v << 1) + 1] = true;
				}
				tree[v].update(updator[v], l, r);
				updator[v] = updType();
				change[v] = false;
			}
		}
		
		void update(int v, int l, int r, int range_l, int range_r, const updType &upd)
		{
			push(v, l, r);
			if ((range_r < l) || (range_l > r)) 
				return;
			#ifdef TESTER
			update_it++;
			#endif
				
			if ((l >= range_l) && (r <= range_r)) {
				updator[v] = upd;
				change[v] = true;
				push(v, l, r);
				return;
			}
			update(v << 1, l, (l + r) >> 1, range_l, range_r, upd);
			update((v << 1) + 1, ((l + r) >> 1) + 1, r, range_l, range_r, upd);
			Type res = tree[v << 1];
			res.merge(tree[(v << 1) + 1]);
			tree[v] = res;
		}
		
		Type get(int v, int l, int r, int range_l, int range_r)
		{
			push(v, l, r);
			if ((range_r < l) || (range_l > r)) {
				return Type();
			}
			#ifdef TESTER
			get_it++;
			#endif
			if ((l >= range_l) && (r <= range_r)) {
				//cout << "Tree: " << l << " " << r << " " << tree[v].get_pref() << " " << tree[v].get_suff() << " " << tree[v].get_ans() << endl;
				//cout << "Tree: " << l << " " << r << " " << tree[v].get_min() << " " << tree[v].get_max() << " " << tree[v].get_sum() << endl;
				return tree[v];
			}
			Type res = get(v << 1, l, (l + r) >> 1, range_l, range_r);
			//cout << "LEFT: " << l << " " << r << " " << res.get_pref() << " " << res.get_suff() << " " << res.get_ans() << " " << res.get_sum() << endl;
			res.merge(get((v << 1) + 1, ((l + r) >> 1) + 1, r, range_l, range_r));
			//cout << "Tree: " << l << " " << r << " " << res.get_pref() << " " << res.get_suff() << " " << res.get_ans() << " " << res.get_sum() << endl;
			//cout << "Tree: " << l << " " << r << " " << res.get_l() << " " << res.get_r() << " " << res.get_ans() << endl;
			//cout << "Tree: " << l << " " << r << " " << res.get_min() << " " << res.get_max() << " " << res.get_sum() << endl;
			return res;
		}
	
};

#endif