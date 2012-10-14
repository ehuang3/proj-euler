#ifndef GRAPH_H
#define GRAPH_H

#include "types.h"
#include "list.h"

/*******************************************************************************
  Datatype definitions
*******************************************************************************/
typedef struct Vertex
{
	void* data;
	list adj;
} Vertex;

typedef struct Edge
{
	Vertex* start;
	Vertex* end;
} Edge;

typedef struct Graph
{
	list V;
	list E;
}	Graph;



#endif 
