#include "graph.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int choice;
    Graph *graph = readGraph(argv[1]);
    if (graph == NULL)
    {
        return 1;
    }

    createAdjacencyList(graph);

    do
    {
        // Display the options
        prompt();

        // Get user input
        printf("Enter your choice: ");

        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            displayAdjacencyList(graph);
            break;

        case 2:
            bfs(graph, 1);  // Start BFS from vertex 1
            break;

        case 3:
            dfs(graph, 1);  // Perform DFS starting from vertex 1
            break;

        case 4:
            dijkstra(graph, 1);  // Perform Dijkstra's Algorithm starting from vertex 1
            break;

        case 5:
            printf("Exiting...\n");
            break;

        default:
            printf("Invalid choice.\n");
            break;
        }

    } while (choice != 5);

    freeGraph(graph);  // Free the graph memory when done
    return 0;
}
