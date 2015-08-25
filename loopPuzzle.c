// Validate solutions for LoopPuzzle.com puzzles
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// A structure to represent an adjacency list node
struct AdjListNode
{
    int dest;
    struct AdjListNode* next;
};
 
// A structure to represent an adjacency liat
struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};
 
// A structure to represent a graph.
// Size of array will be V (number of vertices in graph)
struct Graph
{
    int V;
    struct AdjList* array;
};
 
// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
 
     // Initialize each adjacency list as empty by making head as NULL
    int i;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}
 
// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
 
    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}
 
// A utility function to print the adjacenncy list representation of graph
void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

// Check for loose ends and intersections
bool validate_square_vertices(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        int degree = 0;
        struct AdjListNode* pCrawl = graph->array[v].head;
        //printf("\n Adjacency list of vertex %d\n head ", v);
        
        while (pCrawl)
        {
            degree++;
	        //printf("-> %d", pCrawl->dest);
	        pCrawl = pCrawl->next;
        }

        if (degree == 1)
        {
            printf("Loose end.\n");
            return false;
        }

        else if (degree > 2)
        {
            printf("Intersection.\n");
            return false;
        }

        else if (degree == 0 || degree == 2)
        {
        	return true;
        }

        printf("\n");
    }
    return true;
}

// Detect loop in solution 
// Simple cycle detection in Graph using Depth First Search
bool detect_loop(struct Graph* graph, int v, bool visited[])
{
    visited[v] = true;
    
    struct AdjListNode* pCrawl = graph->array[v].head;
    while (pCrawl)
    {
    	if (pCrawl -> dest == v)
    		return true;
    	
    	if (visited[pCrawl -> dest])
    		pCrawl = pCrawl -> next;
    	else
    	{
    		visited[pCrawl -> dest] = true;
    		pCrawl = graph->array[pCrawl -> dest].head;
    	}
    }

    return false;
}

// Count number of simple loops in the solution (Should be 1)
bool validate_loopcount(struct Graph* graph)
{
    int i, loopcount = 0;
    int V = graph->V;
    bool visited[V];

    for(i = 0; i < V; i++)
        visited[i] = false;

    for(i = 0; i < V; i++)
        if(visited[i] == false)
        {
        	if (detect_loop(graph, i, visited))
        		loopcount++;
        }

    if (loopcount == 1)
    	return true;
    else
    	return false;
}

// Count number of edges around small unit square
int count_sq_edges(struct Graph* graph, int *square_vertices)
{
	int i, j, count = 0, num_unique_edges;
	for (i = 0; i < 4; i++)
	{
		int node_index = square_vertices[i];
		// printf("Node index = %d\n", node_index);
		struct AdjListNode* pCrawl = graph->array[node_index].head;
	    
	    while (pCrawl)
	    {
	        //printf("-> %d", pCrawl->dest);
	        for (j = 0; j < 4; j++)
	        {
	        	if (pCrawl -> dest == square_vertices[j] && j!=i)
	        		count++;
	        }

	        pCrawl = pCrawl->next;
	    }
	}

	num_unique_edges = count / 2;
    return num_unique_edges;
}

// Number in cell should be equal to number of edges surrounding it.  
bool validate_edgecount(int** problem_matrix, int size, struct Graph* graph)
{
	int i, j, k, jump = size + 1, filled_sq_count = 0;
	int *square_vertices, valid_sq_count = 0;
	square_vertices = (int*) malloc(sizeof(int) * 4);

	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			int edge_count = 0;
			if (problem_matrix[i][j] > -1)
			{
				filled_sq_count++;
				square_vertices[0] = jump * i + j;
				square_vertices[1] = square_vertices[0] + 1;
				square_vertices[2] = square_vertices[1] + size;
				square_vertices[3] = square_vertices[2] + 1;

				edge_count = count_sq_edges(graph, square_vertices);
				//printf("Edge count (i = %d, j = %d) = %d\n", i, j, edge_count);

				if (edge_count == problem_matrix[i][j])
					valid_sq_count++;
			}
		}
	}

	// printf("Valid edge count = %d\n", valid_sq_count);
	if (valid_sq_count == filled_sq_count)
		return true;
	else
		return false;
}

// Take solution as input and insert into graph
void populateGraph(struct Graph* graph, int num_edges)
{
	int edges[num_edges][2];
	int v1, v2;
	while (num_edges--)
	{
		scanf("%d %d", &v1, &v2);
		addEdge(graph, v1, v2);
	}
}

// Matrix puzzle as input
void input_problem(int** problem_matrix, int size)
{
	int row, col;
	for (row = 0; row < size; ++row)
		for (col = 0; col < size; ++col)
			scanf("%d", &problem_matrix[row][col]);
}

// Driver program to test above functions
int main()
{
	int i, j;

	printf("\nInstructions for verifying your LoopPuzzle solution:\n");
	printf("1. The problem matrix should have -1 value for empty cells.\n");
	printf("2. The solution should have space-separated edge-pairs.\n\n");

    // Input problem
    int size;
    printf("Insert size of matrix:\n");
    scanf("%d", &size);

    printf("\nInput problem matrix:\n");
    int *problem_matrix[size];
    for (i = 0; i < size; i++)
         problem_matrix[i] = (int *) malloc(size * sizeof(int));

    input_problem(problem_matrix, size);

    // Input solution
    // V => Number of vertices of graph
    int V = (size + 1) * (size + 1);
    
    int num_edges;
    printf("\nInsert number of edges:\n");
    scanf("%d", &num_edges);

    struct Graph* graph = createGraph(V);
    populateGraph(graph, num_edges);
 
    // print the adjacency list representation of the above graph
    // printGraph(graph);
 	
 	// Validate vertex degrees (Valid: 0 or 2)
 	bool v_vertex = validate_square_vertices(graph);
 	if (v_vertex)
 		printf("\n+ Valid vertex degrees.\n");
 	else
 		printf("\n- Invalid vertex degrees.\n");

	// Validate loop count (Should be 1)
	bool v_loop = validate_loopcount(graph);
	if (v_loop)
		printf("\n+ Valid loop count.\n"); 	
	else
		printf("\n- Invalid loop count.\n");

    // Validate edges (Number in cell = Number of edges surrounding cell)
    bool v_edge = validate_edgecount(problem_matrix, size, graph);
	if (v_edge)
		printf("\n+ Valid edge count.\n"); 	
	else
		printf("\n- Invalid edge count.\n");

	// Final Verdict!
	if (v_vertex && v_loop && v_edge)
		printf("\nBravo! Your solution is correct.\n");
	else
		printf("\nYour solution is incorrect. Head onto www.looppuzzle.com to get better.\n");

    return 0;
}