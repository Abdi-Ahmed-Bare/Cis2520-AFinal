#include "graph.h"

// Function to display the menu
void prompt(void)
{
    printf("\nMenu:\n");
    printf("1. Display Adjacency List\n");
    printf("2. Perform Breadth-First Search (BFS)\n");
    printf("3. Perform Depth-First Search (DFS)\n");
    printf("4. Find Shortest Path using Dijkstra's Algorithm\n");
    printf("5. Exit\n");
}

// Read the graph from a file
Graph *readGraph(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return NULL;
    }

    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (!graph)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Find the number of vertices by reading the first line
    char line[1024];
    if (fgets(line, sizeof(line), file) == NULL)
    {
        fprintf(stderr, "Error: Failed to read the first line\n");
        free(graph);
        fclose(file);
        return NULL;
    }

    // Count the number of vertices by counting spaces in the first line
    int numVertices = 0;
    char *token = strtok(line, " \n");
    while (token != NULL)
    {
        numVertices++;
        token = strtok(NULL, " \n");
    }

    graph->numVertices = numVertices;

    // Rewind the file to start reading the matrix
    fseek(file, 0, SEEK_SET);

    // Initialize the adjacency matrix with 0
    for (int i = 0; i < graph->numVertices; i++)
    {
        for (int j = 0; j < graph->numVertices; j++)
        {
            graph->adjMatrix[i][j] = 0;
        }
    }

    // Read the adjacency matrix from the file
    for (int i = 0; i < graph->numVertices; i++)
    {
        for (int j = 0; j < graph->numVertices; j++)
        {
            fscanf(file, "%d", &graph->adjMatrix[i][j]);
        }
    }

    fclose(file);
    return graph;
}

// Create a new node for the adjacency list
Node *createNode(int vertex)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

// Create the adjacency list from the adjacency matrix
// Create the adjacency list from the adjacency matrix
void createAdjacencyList(Graph *graph)
{
    for (int i = 0; i < graph->numVertices; i++)
    {
        graph->adjList[i] = NULL;
        for (int j = 0; j < graph->numVertices; j++)
        {
            if (graph->adjMatrix[i][j] != 0)
            {
                Node *newNode = createNode(j + 1);  // Store 1-based vertex number
                
                // Add the new node at the end of the list (not at the front)
                if (graph->adjList[i] == NULL)
                {
                    graph->adjList[i] = newNode;  // First node
                }
                else
                {
                    Node *temp = graph->adjList[i];
                    while (temp->next != NULL)
                    {
                        temp = temp->next;  // Traverse to the last node
                    }
                    temp->next = newNode;  // Add the new node at the end
                }
            }
        }
    }
}

// Display the adjacency list with edge weights
void displayAdjacencyList(Graph *graph)
{
    createAdjacencyList(graph);
    printf("Adjacency List:\n");
    for (int i = 0; i < graph->numVertices; i++)
    {
        printf("Vertex %d:", i + 1);  // Display 1-based index for vertices
        Node *current = graph->adjList[i];
        while (current)
        {
            printf(" -> %d (%d)", current->vertex, graph->adjMatrix[i][current->vertex - 1]);  // Print vertex and weight
            current = current->next;
        }
        printf(" NULL\n");
    }
}

//FROM HERE THERE IS AN ISSUE: 
void bfs(Graph *graph, int startVertex)
{
    bool visited[MAX_VERTICES] = {false};
    int queue[MAX_VERTICES];
    int front = -1, rear = -1;

    visited[startVertex - 1] = true; 
    queue[++rear] = startVertex - 1;

    printf("Final BFS Order:\n");

    while (front != rear)
    {
        int currentVertex = queue[++front];
        printf("%d ", currentVertex + 1); 

        Node *temp = graph->adjList[currentVertex];
        while (temp)
        {
            if (!visited[temp->vertex - 1]) 
            {
                visited[temp->vertex - 1] = true;
                queue[++rear] = temp->vertex - 1;
            }
            temp = temp->next;
        }
    }
    printf("\n");
}

void dfsUtil(Graph *graph, int vertex, bool *visited);


// Depth-First Search (DFS)
void dfs(Graph *graph, int startVertex)
{
    bool visited[MAX_VERTICES] = {false};
    //printf("DFS Order:\n");
    dfsUtil(graph, startVertex - 1, visited); // Adjust to 0-based index
    printf("\n");
}


void dfsUtil(Graph *graph, int vertex, bool *visited)
{
    visited[vertex] = true;
    printf("%d ", vertex + 1); // Print 1-based index

    Node *temp = graph->adjList[vertex];
    while (temp)
    {
        if (!visited[temp->vertex - 1]) // Adjust to 0-based index
        {
            dfsUtil(graph, temp->vertex - 1, visited);
        }
        temp = temp->next;
    }
}

// Dijkstra's Algorithm
void dijkstra(Graph *graph, int startVertex)
{
    int dist[MAX_VERTICES];
    bool visited[MAX_VERTICES] = {false};
    int n = graph->numVertices;

    for (int i = 0; i < n; i++)
    {
        dist[i] = INT_MAX;
    }
    dist[startVertex - 1] = 0;  // Convert to 0-based index

    for (int i = 0; i < n - 1; i++)
    {
        int minDist = INT_MAX, u = -1;
        for (int j = 0; j < n; j++)
        {
            if (!visited[j] && dist[j] < minDist)
            {
                minDist = dist[j];
                u = j;
            }
        }

        if (u == -1) break;  // No reachable vertex

        visited[u] = true;

        for (int v = 0; v < n; v++)
        {
            if (graph->adjMatrix[u][v] && !visited[v] && dist[u] != INT_MAX &&
                dist[u] + graph->adjMatrix[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph->adjMatrix[u][v];
            }
        }
    }

    //printf("\nShortest distance from vertex %d:\n", startVertex);
    for (int i = 0; i < n; i++)
    {
        printf("Shortest distance from vertex %d to vertex %d: %d\n", startVertex, i + 1, dist[i]);
    }
}


/**
 * Frees the memory allocated for the graph, including the adjacency list.
 * @param graph Pointer to the Graph structure to be freed.
 */
void freeGraph(Graph *graph)
{
    // Implement the function logic here

    for (int i = 0; i < graph->numVertices; i++)
    {
        Node *temp = graph->adjList[i];
        while (temp)
        {
            Node *next = temp->next;
            free(temp);
            temp = next;
        }
    }

    free(graph);
}