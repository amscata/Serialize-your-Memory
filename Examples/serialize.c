/*
  Serialize your Memory
  Memory serialization program for C Language
  Core Algorithms {malloc_e, free_e, mmap_e, munmap_e}
  
  Copyright (C) 2013 Amith Chinthaka
  released under GPL 2.0. All following copyrights
  hold. This notice must be retained.

  See LICENSE for details
  See DOCUMENTATION for API
  Welcome to Develop  
*/

#include "serialize.h"

char memory[NUMCHARS];
void* BASE = (void*)&memory[0];

void* malloc_e(size_t size)
{
	Node *current = (Node*)BASE;
	int *ptr = NULL;
	int i = 0;
	
	//First fit algorithm to allocate memory chunk
	while(P(current->next) != NULL)
	{
		current = (Node*)(P(current->next));
		if(current->status == 0)
		{
			if(P(current->next) != NULL)
			{
				if((int)((char*)P(current->next) - (char*)P(current->ref)) >= (int)size)
				{
					if(((int)((char*)P(current->next) - (char*)P(current->ref)) - size) > sizeof(Node))
					{
						//create a new free node by dividing current node into two
						current->status = 1;
						ptr = (int*)P(current->ref);
						
						//make a free node
						Node *temp = (Node*)(P(current->next));
						current->next = V((char*)(ptr) + size);
						current = (Node*)(P(current->next));
						current->next = V(temp);
						current->ref = V((char*)current + sizeof(Node));
						current->status = 0;
						
					}
					else
					{
						//allocate whole memory chunk
						current->status = 1;
						ptr = P(current->ref);
						i++;
						break;
					}
				}
			}
			else
			{
				//There is no next node. we reached to last node
				ptr = P(current->ref);
				current->status = 1;
				
				//make a new free node
				current->next = (int)(current->ref + size);
				current = (Node*)(P(current->next));
				current->ref = V((char*)current + sizeof(Node));
				current->status = 0;
				i++;
				break;
			}
		}
		i++;
	}
	
	//List has only a header node
	if(i == 0)
	{
		current->next = V((char*)current + sizeof(Node));
		current = (Node*)(P(current->next));
		current->ref = V((char*)current + sizeof(Node));
		ptr = P(current->ref);
		current->status = 1;
		
		//make a free node
		current->next = (int)(current->ref + size);
		current = (Node*)(P(current->next));
		current->ref = V((char*)current + sizeof(Node));
		current->status = 0;
	}
	
	return ptr;
}

void free_e(void *ptr)
{
	Node *current = (Node*)BASE;
	
	while(P(current->next) != NULL)
	{
		current = (Node*)(P(current->next));;
		if(current->status == 1 && P(current->ref) == ptr)
		{
			current->status = 0;
			//printf("done free:%p\n", current->ref);
			break;
		}
	}
	
	defragmentFreeNodes();
}

void defragmentFreeNodes()
{
	Node *current = (Node*)BASE;
	Node *previous = current;
	
	while(P(current->next) != NULL)
	{
		current = (Node*)(P(current->next));
		if(P(current->next) != NULL)
		{
			if(current->status == 0 && ((Node*)(P(current->next)))->status == 0)
			{
				if(P(((Node*)(P(current->next)))->next) != NULL)
				{
					Node *temp = (Node*)P(((Node*)(P(current->next)))->next);
					current->next = V(temp);
					current = previous;
				}
				else
				{
					current->next = 0;
					break;
				}
			}
			else
			{
				previous = current;
			}
		}
	}
}

void printList()
{
	Node *current = (Node*)BASE;
	printf("_____________________________\n");
	printf("start address:%p\n", &memory[0]);
	printf("end address:%p\n", &memory[NUMCHARS - 1]);
	printf("_____________________________\n");
	printf("me:%p\n", current);
	printf("ref:%p\n", P(current->ref));
	printf("status:%d\n", current->status);
	printf("next:%p\n", P(current->next));
	printf("_____________________________\n");
	while(P(current->next) != NULL)
	{
		current = (Node*)(P(current->next));
		printf("me:%p\n", current);
		printf("ref:%p\n", P(current->ref));
		printf("status:%d\n", current->status);
		printf("next:%p\n", P(current->next));
		printf("_____________________________\n");
	}
}

/* 
 * This mmap sample code downloaded from
 * http://www.linuxquestions.org/questions/programming-9/mmap-tutorial-c-c-511265/
 * Original dumentation didn't change
 * Thanks for this sample code
 * Amith Chinthaka
 */
void mmap_e(char* filename)
{
	int i;
    int fd;
    int result;
    char *map;  /* mmapped array of char's */

    /* Open a file for writing.
     *  - Creating the file if it doesn't exist.
     *  - Truncating it to 0 size if it already exists. (not really needed)
     *
     * Note: "O_WRONLY" mode is not sufficient when mmaping.
     */
    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    if (fd == -1) {
		perror("Error opening file for writing");
		exit(EXIT_FAILURE);
    }

    /* Stretch the file size to the size of the (mmapped) array of ints
     */
    result = lseek(fd, FILESIZE-1, SEEK_SET);
    if (result == -1) {
		close(fd);
		perror("Error calling lseek() to 'stretch' the file");
		exit(EXIT_FAILURE);
    }
    
    /* Something needs to be written at the end of the file to
     * have the file actually have the new size.
     * Just writing an empty string at the current file position will do.
     *
     * Note:
     *  - The current position in the file is at the end of the stretched 
     *    file due to the call to lseek().
     *  - An empty string is actually a single '\0' character, so a zero-byte
     *    will be written at the last byte of the file.
     */
    result = write(fd, "", 1);
    if (result != 1) {
		close(fd);
		perror("Error writing last byte of the file");
		exit(EXIT_FAILURE);
    }

    /* Now the file is ready to be mmapped.
     */
    map = mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
		close(fd);
		perror("Error mmapping the file");
		exit(EXIT_FAILURE);
    }
    
    /* Now write int's to the file as if it were memory (an array of ints).
     */
    for (i = 0; i < NUMCHARS; i++) {
		map[i] = memory[i];
    }

    /* Don't forget to free the mmapped memory
     */
    if (munmap(map, FILESIZE) == -1) {
		perror("Error un-mmapping the file");
		/* Decide here whether to close(fd) and exit() or not. Depends... */
    }

    /* Un-mmaping doesn't close the file, so we still need to do that.
     */
    close(fd);
}

/* 
 * This munmap sample code downloaded from
 * http://www.linuxquestions.org/questions/programming-9/mmap-tutorial-c-c-511265/
 * Original dumentation didn't change
 * Thanks for this sample code
 * Amith Chinthaka
 */
void* munmap_e(char* filename)
{
	int i;
    int fd;
    char *map;  /* mmapped array of char's */

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
		perror("Error opening file for reading");
		exit(EXIT_FAILURE);
    }

    map = mmap(0, FILESIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
		close(fd);
		perror("Error mmapping the file");
		exit(EXIT_FAILURE);
    }
    
    for (i = 0; i < NUMCHARS; i++) {
		memory[i] = map[i];
    }
    
    if (munmap(map, FILESIZE) == -1) {
		perror("Error un-mmapping the file");
    }
    
    //Remapping the memory
    BASE = (void*)&memory[0];
    Node* temp = (Node*)BASE;
    temp = (Node*)(P(temp->next));
    
    close(fd);
	
    return P(temp->ref);
}


/* Same as P define. For client's usage */
void* PA(int virAddr)
{
	return P(virAddr);
}

/* Same as V define. For client's usage */
unsigned int VA(void* phyAddr)
{
	return V(phyAddr);
}

/* Assigning given text into a char* */
void putString(char* p, const char *text)
{
	int i;
	for (i = 0; text[i] != '\0'; i++)
	{
		p[i] = text[i];
	}
	p[i + 1] = '\0';
}



