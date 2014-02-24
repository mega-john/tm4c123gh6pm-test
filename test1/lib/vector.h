/*
 * vector.h
 *
 *  Created on: 21.02.2014
 *      Author: estarcev
 */

#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct vector_
{
	void** data;
	int size;
	int count;
} vector;

extern void vector_init(vector*);
extern int vector_count(vector*);
extern void vector_add(vector*, void*);
extern void vector_set(vector*, int, void*);
extern void *vector_get(vector*, int);
extern void vector_delete(vector*, int);
extern void vector_free(vector*);

#endif /* VECTOR_H_ */
