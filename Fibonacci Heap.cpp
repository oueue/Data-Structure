#include <stdio.h>
#include <stdlib.h>
// implementation of Fibonacci Heap which is efficent in insert/decrease key
typedef struct node
{
	int key;
	int degree;
	struct node *parent;
	struct node *child;
	struct node *left;
	struct node *right;
	char mark;
	char child_cut;
}
typedef struct fib_heap
{
	int n;
	node *min;
}fib_heap;

fib_heap* make_fib_heap();
node* make_node(int);
void fib_heap_insert(fib_heap*, node*);
node* fib_heap_min(fib_heap*);
void fib_heap_union(fib_heap*, fib_heap*);
node* fib_heap_extract_min(fib_heap*);
void consolidate(fib_heap*);
void fib_heap_link(fib_heap*, node*, node*);
void cut(fib_heap*, node*, node*);
void cascading_cut(fib_heap*, node*);

fib_heap* make_fib_heap()
{
	fib_heap *heap = (fib_heap*) malloc(sizeof(fib_heap));
	heap->n = 0;
	heap->min = NULL;
	return heap;
}

node* make_node(int key)
{
	node *x = (node*) malloc(sizeof(node));
	x->key = key;
	x->degree = 0;
	x->parent = NULL;
	x->child = NULL;
	x->left = x;
	x->right = x;
	x->mark = 0;
	x->child_cut = 0;
	return x;
}
//double-linked list makes insert more efficient since fibonacci heap
//does not require trees sorted by their degree, therefore getting 
//both successor and prior immediately is important
void fib_heap_insert(fib_heap *heap, node *x)
{
	if(heap->min == NULL)
	{
		heap->min = x;
		x->left = x;
		x->right = x;
	}
	else
	{
		heap->min->left->right = x;
		x->left = heap->min->left;
		heap->min->left = x;
		x->right = heap->min;
		if(x->key < heap->min->key) heap->min = x;
	}
	heap->n++;
}
//maintain a pointer makes it more efficent
node* fib_heap_min(fib_heap *heap)
{
	return heap->min;
}
void fib_heap_union(fib_heap *heap1, fib_heap *heap2)
{
	if(heap1 == NULL || heap1->min == NULL)
	{
		heap1->min = heap2->min;
		return;
	}
	else if(heap3 == NULL || heap2->min == NULL) return;
	node *temp1 = heap1->min->right;
	node *temp2 = heap2->min->left;
	heap1->min->right = heap2->min;
	heap2->min->left = heap1->min;
	temp1->left = temp2;
	temp2->right = temp1;
	if(heap2->min->key < heap1->min->key) heap1->min = heap2->min;
	free(heap2);
}
//consolidate ensures degrees of trees in a heap are unique
void consolidate(fib_heap *heap)
{
	// allocate an array to point to trees with different degrees
	int size_of_table = 45;
	node **table = (node**)malloc(size_of_table *sizeof(node*));
	for(int i = 0; i<size_of_table; i++) table[i] = NULL;
	int num_of_roots = 0;
	node *x = heap->min;
	if(x != NULL)
	{
		num_of_roots++;
		x = x->right;
		while(x!=heap->min)
		{
			num_of_roots++;
			x = x->right;
		}
	}
	while(num_of_roots--)
	{
		int d = x->degree;
		node *next = x->right;
		while(table[d]!=NULL)
		{
			node *y = table[d];
			if(x->key > y->key)
			{
				node *temp = x; x = y; y = temp;
			}
			fib_heap_link(heap, y, x); //link y to x meaning it's min heap
			table[d] = NULL;
			d++;
		}
		table[d] = x;
		x = next;
	}
	heap->min = NULL;
	//copy trees in the table to current heap
	for(int i = 0; i<size_of_table; i++)
	{
		if(table[i] != NULL)
		{
			if(heap->min == NULL)
			{
				heap->min = table[i];
				heap->min->left = heap->min;
				heap->min->right = heap->min;
			}
			else
			{
				(heap->min->left)->right = table[i];
				table[i]->left = heap->min->left;
				table[i]->right = heap->min;
				heap->min->left = table[i];
				if(table[i]->key < heap->min->key) heap->min = table[i];
			}
		}
	}
}
//extract_min: unlink min and make all its children roots, then consolidate
node* fib_heap_extract_min(fib_heap *heap)
{
	node *z = heap->min;
	if(z != NULL)
	{
		node *x = z->child;
		int num_of_children = z->degree;
		// add all children of min to the root list
		while(num_of_children--)
		{
			node *nextchild = x->right;
			(heap->min->left)->right = x;
			x->right = heap->min;
			x->left = heap->min->left;
			heap->min->left = x;
			if(x->key < heap->min->key) heap->min = x;
			x->parent = NULL;
			x = nextchild;
		}
		//remove the min from the root list
		(z->left)->right = z->right;
		(z->right)->left = z->left;
		//check if the minimum node has children originally and update min pointer
		heap->min = z->right;
		if(z == z->right && z->child == NULL); // there is only one node originally
		else if(z == z->right) heap->min = z->child; // z has no siblings originally
		else consolidate(heap);
		heap->n--;
	}
	return z;
}
//link y to x
void fib_heap_link(fib_heap *heap, node *y, node *x)
{
	// isolate y
	y->left->right = y->right;
	y->right->left = y->left;
	//make y child of x
	y->parent = x;
	x->child->left->right = y;
	y->left = x->child->left;
	y->right = x->child;
	x->child->left = y;
	x->degree++;
	y->mark = 0;
}
// cut y's child x from y
void cut(fib_heap *heap, node *x, node *y)
{
	if(x == x->right) y->child = NULL;
	else
	{
		x->right->left = x->left;
		x->left->right = x->right;
		if(y->child == x) y->child = x->right;
	}
	y->degree--;
	x->parent = NULL;
	//add x to root list
	x->left = heap->min;
	x->right = heap->min->right;
	heap->min->right = x;
	x->right->left = x;
	x->mark = 0;
}
// if y's mark reach 2, x's parent y also needs to be cut from its parent z
void cascading_cut(fib_heap *heap, node *y)
{
	node *z = y->parent;
	if(z!=NULL)
	{
		y->mark++;
		if(y->mark == 2)
		{
			cut(heap, y, z);
			cascading_cut(heap, z);
		}
	}
}
