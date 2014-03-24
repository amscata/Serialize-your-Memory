/*
  Serialize your Memory
  Memory serialization program for C Language  
  Example Program: LinkedList Example
  
  Copyright (C) 2013 Amith Chinthaka
  released under GPL 2.0. All following copyrights
  hold. This notice must be retained.

  See LICENSE for details
  See DOCUMENTATION for API
  Welcome to Develop
*/

#include "serialize.h"
#define SP(v) (char*)(PA(v))
#define BP(v) ((Box*)(PA(v)))

typedef struct box {
	unsigned int name;
	int age;
	unsigned int next;
}Box;

Box* head;
Box* current;

void add(const char* name, int age)
{
	current = head;
	while (BP(current->next) != NULL)
	{
		current = BP(current->next);
	}
	current->next = VA(malloc_e(sizeof(Box)));
	current = BP(current->next);
	current->name = VA(malloc_e(sizeof(char)*10));
	putString(SP(current->name), name);
	current->age = age;	
}

void print()
{
	current = head;
	while (current != NULL)
	{
		printf("%s:%d\n", SP(current->name), current->age);
		current = BP(current->next);
	}
}

int main()
{
	head = malloc_e(sizeof(Box));
	current = head;
	
	add("Donna\0", 25);
	add("Harvey\0", 35);
	add("Mike\0", 23);
	add("Rechel\0", 24);		
	print();
		
	mmap_e("listSerialized.bin");
	
	printf("\n1 Retrieving...\n");
	head = (Box*)munmap_e("listSerialized.bin");
	add("Luvis\0", 32);
	add("Peterson\0", 32);
	print();
	
	mmap_e("listSerialized.bin");
	
	printf("\n2 Retrieving...\n");
	head = (Box*)munmap_e("listSerialized.bin");
	print();
	
	//printList();
	
	return 0;
}
