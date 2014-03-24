/*
  Serialize your Memory
  Memory serialization program for C Language  
  Core Defines {NUMCHARS, FILESIZE, V, P, Node structure}
  
  Copyright (C) 2013 Amith Chinthaka
  released under GPL 2.0. All following copyrights
  hold. This notice must be retained.

  See LICENSE for details
  See DOCUMENTATION for API
  Welcome to Develop  
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define NUMCHARS (50000)
#define FILESIZE (NUMCHARS * sizeof(char))
#define V(phyAddr) ((void*)phyAddr - BASE)/sizeof(char)
#define P(virAddr) ((virAddr == 0)?((void*)0):((void*)(BASE + virAddr*sizeof(char))))

typedef struct node
{
	unsigned int ref;
	int status;
	unsigned int next;
}Node;

void* malloc_e(size_t size);
void free_e(void* ptr);
void defragmentFreeNodes();
void printList();
void mmap_e(char* filename);
void* munmap_e(char* filename);
void putString(char* p, const char* text);
void* PA(int virAddr);
unsigned int VA(void* phyAddr);
