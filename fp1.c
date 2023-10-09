#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int src;
    int dest;
    int weight;
} Edge;

typedef struct {
    Edge* elements;
    int size;
    int capacity;
} Heap;

typedef struct Node {
    int dest;
    int weight;
    struct Node* next;
} Node;

typedef struct {
    int num_vertices;
    int num_edges;
    Node** adj_list;
} Graph;

Graph* createGraph(int num_vertices, int num_edges) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->num_edges = num_edges;
    graph->adj_list = (Node**)malloc(num_vertices * sizeof(Node*));
    for (int i = 0; i < num_vertices; i++) {
        graph->adj_list[i] = NULL;
    }
    return graph;
}

void addEdge(Graph* graph, int src, int dest, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = graph->adj_list[src];
    graph->adj_list[src] = newNode;

    newNode = (Node*)malloc(sizeof(Node));
    newNode->dest = src;
    newNode->weight = weight;
    newNode->next = graph->adj_list[dest];
    graph->adj_list[dest] = newNode;
}

Heap* createHeap(int capacity) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->elements = (Edge*)malloc(capacity * sizeof(Edge));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heapify(Heap* heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->elements[left].weight < heap->elements[smallest].weight) {
        smallest = left;
    }
    if (right < heap->size && heap->elements[right].weight < heap->elements[smallest].weight) {
        smallest = right;
    }

    if (smallest != index) {
        Edge temp = heap->elements[index];
        heap->elements[index] = heap->elements[smallest];
        heap->elements[smallest] = temp;
        heapify(heap, smallest);
    }
}

void buildHeap(Heap* heap, int num_edges) {
    int i = num_edges / 2 - 1;
    for (; i >= 0; i--) {
        heapify(heap, i);
    }
}

void insert(Heap* heap, Edge edge) {
    heap->size++;
    int i = heap->size - 1;
    while (i > 0 && edge.weight < heap->elements[(i - 1) / 2].weight) {
        heap->elements[i] = heap->elements[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->elements[i] = edge;
}

Edge extractMin(Heap* heap) {
    Edge minEdge = heap->elements[0];
    heap->elements[0] = heap->elements[heap->size - 1];
    heap->size--;
    heapify(heap, 0);
    return minEdge;
}

int find(int parent[], int vertex) {
    if (parent[vertex] == -1) {
        return vertex;
    }
    return find(parent, parent[vertex]);
}

void unionSets(int parent[], int rank[], int x, int y) {
    int xRoot = find(parent, x);
    int yRoot = find(parent, y);

    if (rank[xRoot] < rank[yRoot]) {
        parent[xRoot] = yRoot;
    } else if (rank[yRoot] < rank[xRoot]) {
        parent[yRoot] = xRoot;
    } else {
        parent[yRoot] = xRoot;
        rank[xRoot]++;
    }
}

void createMST(Graph* graph) {
    int num_vertices = graph->num_vertices;
    int num_edges = graph->num_edges;
    Heap* minHeap = createHeap(num_edges);
    Edge mst[num_vertices - 1];

    // Add all the edges to the min heap
    for (int i = 0; i < num_vertices; i++) {
        Node* temp = graph->adj_list[i];
        while (temp != NULL) {
            if (temp->dest > i) {
                Edge edge;
                edge.src = i;
                edge.dest = temp->dest;
                edge.weight = temp->weight;
                insert(minHeap, edge);
            }
            temp = temp->next;
        }
    }

    // Create a disjoint set for all vertices
    int parent[num_vertices];
    int rank[num_vertices];
    memset(parent, -1, num_vertices * sizeof(int));
    memset(rank, 0, num_vertices * sizeof(int));

    int mstIndex = 0;
    while (mstIndex < num_vertices - 1 && minHeap->size > 0) {
        Edge edge = extractMin(minHeap);
        int srcRoot = find(parent, edge.src);
        int destRoot = find(parent, edge.dest);

        if (srcRoot != destRoot) {
            mst[mstIndex] = edge;
            mstIndex++;
            unionSets(parent, rank, srcRoot, destRoot);
        }
    }

    // Calculate the total cost of the minimum spanning tree
    int totalCost = 0;
    for (int i = 0; i < num_vertices - 1; i++) {
        totalCost += mst[i].weight;
    }

    // Write the minimum spanning tree edges to the output file
    FILE* outFile = fopen("fp1_result.txt", "w");
    if (outFile == NULL) {
        printf("Error opening the output file.\n");
        return;
    }

    for (int i = 0; i < num_vertices - 1; i++) {
        fprintf(outFile, "%d %d %d\n", mst[i].src, mst[i].dest, mst[i].weight);
    }

    // Write the total cost and connection status to the output file
    fprintf(outFile, "%d\n", totalCost);
    fprintf(outFile, "%s\n", (mstIndex == num_vertices - 1) ? "CONNECTED" : "DISCONNECTED");

    fclose(outFile);

    // Clean up memory
    free(minHeap->elements);
    free(minHeap);
}

int main(int argc, char* argv[]) {
    // Check the number of command-line arguments
    if (argc != 2) {
        printf("usage: %s input_filename\n", argv[0]);
        return 0;
    }

    // Read the input file name from the command-line arguments
    char* inputFilename = argv[1];

    // Open the input file
    FILE* inFile = fopen(inputFilename, "r");
    if (inFile == NULL) {
        printf("The input file does not exist.\n");
        return 0;
    }

    // Read the number of vertices and edges from the input file
    int num_vertices, num_edges;
    fscanf(inFile, "%d", &num_vertices);
    fscanf(inFile, "%d", &num_edges);

    // Create a graph
    Graph* graph = createGraph(num_vertices, num_edges);

    // Read the edges from the input file and add them to the graph
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        fscanf(inFile, "%d %d %d", &src, &dest, &weight);
        addEdge(graph, src, dest, weight);
    }

    // Close the input file
    fclose(inFile);

    // Measure the running time
    clock_t start = clock();

    // Create the minimum spanning tree
    createMST(graph);

    // Calculate the running time
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Print the running time
    printf("output written to fp1_result.txt.\n");
    printf("running time: %f seconds\n", cpu_time_used);

    return 0;
}

