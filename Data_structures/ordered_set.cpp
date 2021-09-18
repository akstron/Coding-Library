 /* 
    1. Override '<=' and '<', while using user defined struct.

    2. To use it as ordered_multiset, maintain pairs
       such that all elements are distinct.

    3. For operations like lower_bound and upper_bound
       simply maintain a STL set along with ordered set

	4. Usage: 
		problem: https://atcoder.jp/contests/abc218/tasks/abc218_g
		submission: https://atcoder.jp/contests/abc218/submissions/25918848 
*/

#include <chrono>
#include <random>

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
 
int getRand(int l, int r)
{
	std::uniform_int_distribution<int> uid(l, r);
	return uid(rng);
}

template<class D>
class treap
{
    D neutral;

	struct data
	{
		int priority, cnt;
        D val;
		data *l, *r;
 
	 	data()
	 	{
	 		cnt = 0, l = NULL, r = NULL; val = NULL;
	 	}
 
		data (D val)
		{
		    cnt = 1;
            this->val = val;
			l = NULL, r = NULL;
			priority = getRand(1, 2e9);
		}
	};
 
	typedef struct data* node;
	 
	node head; 

	int cnt(node cur)
	{
		return cur ? cur->cnt : 0;
	}

    void updateCnt(node cur)
	{
		if(cur)
			cur->cnt = cnt(cur->l) + cnt(cur->r) + 1;
	}
 
	void less_equal(node cur, node &l, node &r, D k)
	{
		if(!cur)
			return void(l = r = 0);

        D idx = cur->val;

        // '<=' is overriden
        if(idx <= k)
            less_equal(cur->r, cur->r, r, k), l = cur;
        else
            less_equal(cur->l, l, cur->l, k), r = cur;
		
        updateCnt(cur);
	}

    
    void less(node cur, node &l, node &r, D val)
    {
        if(!cur)
			return void(l = r = 0);
			
        D idx = cur->val, k = val;

		// '<' should be overriden
        if(idx < k)
            less(cur->r, cur->r, r, k), l = cur;
        else
            less(cur->l, l, cur->l, k), r = cur;
		
        updateCnt(cur);
    }
	 
	void merge(node &cur, node l, node r)
	{
		// max heap type merging

		if(!l || !r)
			cur = l ? l : r;
		else if(l->priority > r->priority)
			merge(l->r, l->r, r), cur = l;
		else
			merge(r->l, l, r->l), cur = r;
		updateCnt(cur);
	}

    void clear(node cur)
	{
		if(!cur)
			return;
		clear(cur->l), clear(cur->r);
		delete cur;
	}

    
	void split_pos(node cur, node &l, node &r, int k, int add = 0)
	{
		if(!cur)
			return void(l = r = 0);

		int idx = add + cnt(cur->l);
		if(idx <= k)
			split_pos(cur->r, cur->r, r, k, idx + 1), l = cur;
		else
			split_pos(cur->l, l, cur->l, k, add), r = cur;

		updateCnt(cur);
	}
 

    public:

    
	treap(D neutral): head(0), neutral(neutral)  {}
	 
    int size()
    {
        return cnt(head);
    }

	void insert(D val)
	{
		if(!head)
		{
			head = new data(val);
			return;
		}

		node l, r, mid, mid2, rr;
		mid = new data(val);

		less(head, l, r, mid->val);
		merge(l, l, mid);

        /* Removing equal element */
		less_equal(r, mid2, rr, mid->val);
		merge(head, l, rr);
	}
	 
	void erase(D val)
	{
		node l, r, mid;
		less(head, l, r, val);
		less_equal(r, mid, r, val);
		merge(head, l, r);
	}
 
	void clear()
	{
		clear(head);
	}
 
    // 1 - indexed
	D find_by_order(int x)
	{
		if(!x)
			return neutral;
		x--;
		node l, r, mid;
		split_pos(head, l, r, x - 1);
		split_pos(r, mid, r, 0);
		D ans = neutral;
		if(cnt(mid) == 1)
			ans = mid->val;
		merge(r, mid, r);
		merge(head, l, r);
		return ans;
	}
 
    //1 indexed
	int order_of_key(D val) 
	{
		node l, r, mid;
		less(head, l, r, val);
		less_equal(r, mid, r, val);
		int ans = -1;
		if(cnt(mid) == 1)
			ans = 1 + cnt(l);
		merge(r, mid, r);
		merge(head, l, r);
		return ans;
	}

    /* Change this in accordance to the datatype used, else override '<<' */

	void inorder(node cur)
	{
		if(!cur) return;
		inorder(cur->l);
		cout<<cur->val<<" ";
		inorder(cur->r);
	}

	void inorder()
	{
		inorder(head);
	}
};