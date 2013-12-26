 #include <cstdio>
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

struct rsq
{
	int l, r, sum, add;
	rsq *left, *right;
};

rsq* build(int l, int r)
{
	rsq* root = (rsq*) malloc(sizeof(rsq));
	root->l = l;
	root->r = r;
	root->add = 0;
	root->sum = 0;
	if (l < r)
	{
		root->left = build(l, (l + r) / 2);
		root->right = build(((l + r) / 2) + 1, r);
	}
	else
	{
		root->left = NULL;
		root->right = NULL;
	}
	return root;
}

int sum(rsq *root, int l, int r, int d)
{
	d += root->add;
	if ((l == (root->l)) && (r == (root->r)))
		return (root->sum + d * (r - l + 1));
	int ans = 0;
	if (l < (((root->l + root->r) / 2) + 1))
		ans += sum(root->left, l, min((root->l + root->r) / 2, r), d);
	if (r > ((root->l + root->r) / 2))
		ans += sum(root->right, max(((root->l + root->r) / 2) + 1, l), r, d);
	d -= root->add;
	return ans;
}
void modify(int x, rsq *root, int l, int r)
{
	if ((l == (root->l)) && (r == (root->r)))
	{
		root->add += x;
		return;
	}
	if (l < (((root->l + root->r) / 2) + 1))
		modify(x, root->left, l, min(r, (root->l + root->r) / 2));
	if (r > ((root->l + root->r) / 2))
		modify(x, root->right, max(l, ((root->l + root->r) / 2) + 1), r);	    
	
	root->sum = sum(root->left, root->left->l, root->left->r, 0) + sum(root->right, root->right->l, root->right->r, 0);
}
int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int N;
	cin >> N;
	rsq* lep = build(0, N);
	int M;
	cin >> M;
	for (int i = 0; i < M; ++i)
	{
		int cmd, l, r, x;
		cin >> cmd;
		if (cmd == 1)
		{
			cin >> l >> r >> x;
			modify(x, lep, l, r - 1);
		}
		if (cmd == 2)
		{
			cin >> l >> r;
			cout << sum(lep, l, r - 1, 0) << endl;
		}
	}
	return 0;
}