#include <iostream>
#include <math.h>
using namespace std;
#define ElemTp char
typedef struct node
{
	ElemTp data;
	struct node *lchild, *rchild;
}BiTNode, *BiT;
BiT crtBT()
{
	char ch = getchar();
	if (ch == '\n')return NULL;
	if (ch == '#') return NULL;
	BiT bt = new BiTNode();
	bt->data = ch;
	bt->lchild = crtBT();
	bt->rchild = crtBT();
	return bt;
}
int height(BiT bt)
{
	if (!bt) return 0;
	int lh = height(bt->lchild);
	int rh = height(bt->rchild);
	return (lh > rh ? lh : rh) + 1;
}
void crtArr(BiT bt, ElemTp* a, int i = 0)
{
	if (!bt)return;
	a[i] = bt->data;
	crtArr(bt->lchild, a, 2 * i + 1);
	crtArr(bt->rchild, a, 2 * i + 2);
}
void print(BiT bt, ElemTp* a, int h, int n)
{
	int k = 0;//current layer
	int underline = pow(2, h - k - 1);
	int spSum = -2 + pow(2, h - k);
	int sp = spSum - underline;
	int last_underline = underline; // number of underlines in the last layer
	int v = 0;//offset
	ElemTp *l1 = new ElemTp[pow(2, h + 1)];
	ElemTp *l2 = new ElemTp[pow(2, h + 1)];
	ElemTp *l3 = new ElemTp[pow(2, h + 1)];
	for (int j = 0; j < pow(2, h + 1); j++)
	{
		l1[j] = ' '; l2[j] = ' '; l3[j] = ' ';
	}
	for (int i = 0; i < n; i++)
	{
		//rchild's offset to lchild is 2*last_underline right
		if (i && !(i % 2)) v += 2 *last_underline;
		if(a[i]!='#') l1[spSum + v] = a[i];
		//print the last layer of the tree
		if (k >= h - 1)
		{
			if (i == n-1)
			{
				for (int j = 0; j < pow(2, h + 1); j++)cout << l1[j];
				cout << endl;
			}
			v = floor((i + 1 - pow(2, k) + 1) / 2)  * (4 * last_underline);
			continue;
		}
		// lchild && rchild exist
		if (a[2 * i + 1] != '#' && a[2 * i + 2] != '#')
		{
			for (int j = 0; j < underline; j++) l2[v + sp + j] = '_';
			l2[v + sp + underline] = '|';
			for (int j = 0; j < underline; j++) l2[v + sp + underline + 1 + j] = '_';
			l3[v + sp] = '|';
			l3[v + sp + underline * 2] = '|';
		}
		//only lchild exists
		else if (a[2 * i + 1] != '#')
		{
			for (int j = 0; j < underline; j++)l2[v + sp + j] = '_';
			l2[v + sp + underline] = '|';
			l3[v + sp] = '|';
		}
		//only rchild exists
		else if (a[2 * i + 2] != '#')
		{
			l2[v + sp + underline] = '|';
			for (int j = 0; j < underline; j++) l2[v + sp + underline + 1 + j] = '_';
			l3[v + sp + underline * 2] = '|';
		}
		// if the very last element in a layer, output the whole layer, layer++
		if (i == pow(2, k+1) - 2)
		{
			last_underline = underline;
			for (int j = 0; j < pow(2, h + 1); j++) cout << l1[j];
			cout << endl;
			for (int j = 0; j < pow(2, h + 1); j++) cout << l2[j];
			cout << endl;
			for (int j = 0; j < pow(2, h + 1); j++) cout << l3[j];
			cout << endl;
			k++;
			underline = pow(2, h - k - 1);
			spSum = -2 + pow(2, h - k);
			sp = spSum - underline;
			for (int j = 0; j < pow(2, h + 1); j++)
			{
				l1[j] = ' '; l2[j] = ' '; l3[j] = ' ';
			}
		}
		v = floor((i+1 - pow(2, k) + 1) / 2)  * (4 * last_underline);
	}
}
int main()
{
	BiT bt = crtBT();
	int h = height(bt);
	int n = pow(2, h) - 1;
	ElemTp *a = new ElemTp[n];
	for (int i = 0; i < n; i++) a[i] = '#';
	crtArr(bt, a);
	print(bt, a, h, n);
	system("pause");
}
