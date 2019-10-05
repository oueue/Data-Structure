#include <iostream>
using namespace std;
typedef struct
{
	int i;
	int k;

}ElemTp;
void perm(int n, int m)
{
	int *a = new int[m];
	int *flag = new int[n];
	for (int i = 0; i < n; i++) flag[i] = 0;
	ElemTp *s = new ElemTp[m + 1];
	int top = -1;
	int k = 0, i = 0;
	//push(s, { i, k});
	s[++top] = { 0, k };
	cout << "push {" << i << ", " << k << "}" << endl;//test line
	while (i != n)
	{
		if (flag[i] == 0)
		{
			a[k] = i + 1;
			flag[i] = 1;
			//push(s, { i, ++k});
			s[++top] = { i, ++k };
			cout << "push {" << i << ", " << k << "}" << endl;//test line
		}
		if (k == m)
		{
			for (int i = 0; i < m; i++) printf("%d ", a[i]);
			printf("\n");
			//pop(s);
			cout << "pop" << endl;//test
			top--;
			flag[i] = 0;
			k--;
		}
		i++;
		if (i == n && top != -1)
		{
			//e = pop(s);
			ElemTp e = s[top--];
			i = 0;
			k = e.k-1;
			cout << "pop2 : i=" <<i << endl;//test
		}
	}
}

int main()
{
	perm(3, 2);
	system("pause");
}
