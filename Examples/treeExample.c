/*
  Serialize your Memory
  Memory serialization program for C Language  
  Example Program: Tree Example
  
  Copyright (C) 2013 Amith Chinthaka
  released under GPL 2.0. All following copyrights
  hold. This notice must be retained.

  See LICENSE for details
  See DOCUMENTATION for API
  Welcome to Develop
*/

#include "serialize.h"
#define TP(v) ((Tree*)(PA(v)))

typedef struct tree {
	int value;
	unsigned int left;
	unsigned int right;
}Tree;

Tree* head;
Tree* current;

/* 0 - left; else - right */
Tree* add(Tree* t, int value, int side)
{
	if (side == 0)
	{
		t->left = VA(malloc_e(sizeof(Tree)));
		TP(t->left)->value = value;
		return TP(t->left);
	}
	else
	{
		t->right = VA(malloc_e(sizeof(Tree)));
		TP(t->right)->value = value;
		return TP(t->right);
	}
}

void print(Tree* t)
{
	if (t != NULL)
	{
		printf("%p:%d\n", t, t->value);
		if (PA(t->left) != NULL)
		{
			print(PA(t->left));
		}
		if (PA(t->right) != NULL)
		{
			print(PA(t->right));
		}
	}
}

int main()
{
	head = malloc_e(sizeof(Tree));
	current = head;
	
	add(current, 31, 0);
	current = add(current, 24, 1);
	add(current, 45, 0);
	add(current, 3, 1);
	print(head);
		
	mmap_e("treeSerialized.bin");
	
	printf("\n1 Retrieving...\n");
	head = (Tree*)munmap_e("treeSerialized.bin");
	current = TP(head->left);
	add(current, 12, 0);
	add(current, 78, 1);
	print(head);
	
	mmap_e("treeSerialized.bin");
	
	printf("\n2 Retrieving...\n");
	head = (Tree*)munmap_e("treeSerialized.bin");
	print(head);
	
	//printList();
	
	return 0;
}
