#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

#define MAX_NODES 100
#define INF 99999

int N; // Number of nodes
int graph[MAX_NODES][MAX_NODES]; // Adjacency matrix representing the graph
int dist[MAX_NODES][MAX_NODES]; // Distance matrix
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for synchronization

// Function prototypes
void initialize();
void floydWarshall();
void *threadFunction(void *arg);

// Function to initialize the graph and distance matrices
void initialize() {
    // Read input graph and initialize matrices
    int M;
    scanf("%d %d", &N, &M); // Read N and M from input
    // Initialize dist matrix with INF
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) {
                dist[i][j] = 0; // Distance to self is 0
            } else {
                dist[i][j] = INF; // Initialize other distances to INF
            }
            graph[i][j] = INF; // Initialize graph matrix with INF
        }
    }
    // Read edges and weights from input and update the graph matrix
    for (int i = 0; i < M; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        // Adjust indices to 0-based indexing
        u--; 
        v--;
        graph[u][v] = w;
        graph[v][u] = w; // Since it's an undirected graph
        dist[u][v] = w; // Initialize distance with edge weight
        dist[v][u] = w; // Since it's an undirected graph
    }
}

// Function to execute Floyd-Warshall algorithm using multi-threading
void floydWarshall() {
    pthread_t threads[N];
    for (int k = 0; k < N; k++) {
        // Create a thread for each row of i values
        pthread_create(&threads[k], NULL, threadFunction, (void *)(intptr_t)k);
    } 
        // Wait for all threads to finish
    for (int k = 0; k < N; k++)
    {
        pthread_join(threads[k], NULL);
    }
}

// Function executed by each thread
void *threadFunction(void *arg) {
    int row = (intptr_t)arg; // Get the row index
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            pthread_mutex_lock(&mutex); // Lock mutex before updating dist matrix
            if (dist[i][row] != INF && dist[row][j] != INF && dist[i][row] + dist[row][j] < dist[i][j]) {
                dist[i][j] = dist[i][row] + dist[row][j];
            }
            pthread_mutex_unlock(&mutex); // Unlock mutex after updating dist matrix
        }
    }
    pthread_exit(NULL);
}

// Main function
int main() {
    // Initialize the graph and distance matrices
    initialize();
    // Run the multi-threaded Floyd-Warshall algorithm
    floydWarshall();
    // Print the final distance matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (dist[i][j] == INF) {
                printf("INF");
            } else {
                printf("%d", dist[i][j]);
            }
            if (j < N - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
    return 0;
}
