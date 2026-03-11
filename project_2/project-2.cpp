#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
using namespace std;

// part a
// stored : adjacency matrix (graph), pq: array
#include <vector>
#include <climits>
using namespace std;

// part a
// stored: adjacency matrix, pq: array
vector<int> dijkstra_a(vector<vector<int>> &graph, int src)
{
    int V = graph.size();

    vector<int> dist(V, INT_MAX);
    vector<bool> visited(V, false);

    // Distance from source to itself is 0
    dist[src] = 0;

    // Repeat V times
    for (int count = 0; count < V; count++)
    {
        int u = -1;
        int minDist = INT_MAX;

        // Array-based priority queue:
        // find the unvisited vertex with the smallest distance
        for (int i = 0; i < V; i++)
        {
            if (!visited[i] && dist[i] < minDist)
            {
                minDist = dist[i];
                u = i;
            }
        }

        // If no reachable unvisited vertex remains, stop
        if (u == -1)
            break;

        visited[u] = true;

        // Scan the whole row of the adjacency matrix
        for (int v = 0; v < V; v++)
        {
            // graph[u][v] != INT_MAX means there is an edge
            if (!visited[v] && graph[u][v] != INT_MAX && dist[u] != INT_MAX)
            {
                if (dist[u] + graph[u][v] < dist[v])
                {
                    dist[v] = dist[u] + graph[u][v];
                }
            }
        }
    }

    return dist;
}

// part b
// stored : adjacency list, pq: min heap
vector<int> dijkstra_b(vector<vector<pair<int, int>>> &adj, int src)
{

    int V = adj.size();

    // Min-heap (priority queue) storing pairs of (distance, node)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    vector<int> dist(V, INT_MAX);

    // Distance from source to itself is 0
    dist[src] = 0;
    pq.emplace(0, src);

    // Process the queue until all reachable vertices are finalized
    while (!pq.empty())
    {
        auto top = pq.top();
        pq.pop();

        int d = top.first;
        int u = top.second;

        // If this distance not the latest shortest one, skip it
        if (d > dist[u])
            continue;

        // Explore all neighbors of the current vertex
        for (auto &p : adj[u])
        {
            int v = p.first;
            int w = p.second;

            // If we found a shorter path to v through u, update it
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                pq.emplace(dist[v], v);
            }
        }
    }

    // Return the final shortest distances from the source
    return dist;
}


// generate a random dense adjacency matrix (every pair of nodes connected)
vector<vector<int>> generateDenseMatrix(int V)
{
    vector<vector<int>> g(V, vector<int>(V, INT_MAX));
    for (int i = 0; i < V; i++)
    {
        g[i][i] = 0;
        for (int j = i + 1; j < V; j++)
        {
            int w = rand() % 100 + 1;
            g[i][j] = g[j][i] = w;
        }
    }
    return g;
}

// generate a random sparse adjacency matrix (each node has around 5 neighbours)
vector<vector<int>> generateSparseMatrix(int V)
{
    vector<vector<int>> g(V, vector<int>(V, INT_MAX));
    for (int i = 0; i < V; i++)
    {
        g[i][i] = 0;
        for (int k = 0; k < 5; k++)
        {
            int j = rand() % V;
            if (i != j)
            {
                int w = rand() % 100 + 1;
                g[i][j] = g[j][i] = w;
            }
        }
    }
    return g;
}

// convert an adjacency matrix to an adjacency list (for dijkstra_b)
vector<vector<pair<int, int>>> matrixToAdj(vector<vector<int>> &graph)
{
    int V = graph.size();
    vector<vector<pair<int, int>>> adj(V);
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            if (graph[i][j] != INT_MAX && i != j)
                adj[i].push_back({j, graph[i][j]});
    return adj;
}

// run analysis and save them in CSV files
void runEmpirical()
{
    srand(42);

    vector<int> sizes = {100, 200, 400, 800, 1600, 3200, 6400, 12800};
    int RUNS = 3;

    ofstream csv_a("results_a.csv");
    ofstream csv_b("results_b.csv");

    // CSV headers
    csv_a << "V,E_dense,Time_A_Dense_ms,E_sparse,Time_A_Sparse_ms\n";
    csv_b << "V,E_dense,Time_B_Dense_ms,E_sparse,Time_B_Sparse_ms\n";

    cout << "\n=== Empirical Analysis ===\n";
    cout << fixed << setprecision(4);
    cout << left
         << setw(8)  << "V"
         << setw(16) << "Dense_A(ms)"
         << setw(16) << "Dense_B(ms)"
         << setw(16) << "Sparse_A(ms)"
         << setw(16) << "Sparse_B(ms)"
         << "\n"
         << string(72, '-') << "\n";

    for (int V : sizes)
    {
        double total_a_dense = 0, total_b_dense = 0;
        double total_a_sparse = 0, total_b_sparse = 0;
        long long e_dense = 0, e_sparse = 0;

        for (int run = 0; run < RUNS; run++)
        {
            // dense graph analysis
            auto dense = generateDenseMatrix(V);
            auto adj_dense = matrixToAdj(dense);

            e_dense = (long long)V * (V - 1) / 2;

            auto t1 = chrono::high_resolution_clock::now();
            dijkstra_a(dense, 0);
            auto t2 = chrono::high_resolution_clock::now();
            total_a_dense += chrono::duration<double, milli>(t2 - t1).count();

            t1 = chrono::high_resolution_clock::now();
            dijkstra_b(adj_dense, 0);
            t2 = chrono::high_resolution_clock::now();
            total_b_dense += chrono::duration<double, milli>(t2 - t1).count();

            // sparse graph analysis
            auto sparse = generateSparseMatrix(V);
            auto adj_sparse = matrixToAdj(sparse);

            long long edge_count = 0;
            for (int i = 0; i < V; i++)
                for (int j = i + 1; j < V; j++)
                    if (sparse[i][j] != INT_MAX)
                        edge_count++;
            e_sparse = edge_count;

            t1 = chrono::high_resolution_clock::now();
            dijkstra_a(sparse, 0);
            t2 = chrono::high_resolution_clock::now();
            total_a_sparse += chrono::duration<double, milli>(t2 - t1).count();

            t1 = chrono::high_resolution_clock::now();
            dijkstra_b(adj_sparse, 0);
            t2 = chrono::high_resolution_clock::now();
            total_b_sparse += chrono::duration<double, milli>(t2 - t1).count();
        }

        double avg_a_dense  = total_a_dense  / RUNS;
        double avg_b_dense  = total_b_dense  / RUNS;
        double avg_a_sparse = total_a_sparse / RUNS;
        double avg_b_sparse = total_b_sparse / RUNS;

        // write to CSVs
        csv_a << V << "," << e_dense << "," << avg_a_dense << ","
              << e_sparse << "," << avg_a_sparse << "\n";

        csv_b << V << "," << e_dense << "," << avg_b_dense << ","
              << e_sparse << "," << avg_b_sparse << "\n";

        cout << left
             << setw(8)  << V
             << setw(16) << avg_a_dense
             << setw(16) << avg_b_dense
             << setw(16) << avg_a_sparse
             << setw(16) << avg_b_sparse
             << "\n";
    }

    csv_a.close();
    csv_b.close();

    cout << "\nResults saved to results_a.csv and results_b.csv\n";
}

int main()
{
    int src = 0;

    // part a example data
    vector<vector<int>> graph = {
        {0, 4, 8, INT_MAX, INT_MAX},
        {4, 0, 3, INT_MAX, 6},
        {8, 3, 0, 2, INT_MAX},
        {INT_MAX, INT_MAX, 2, 0, 10},
        {INT_MAX, 6, INT_MAX, 10, 0}};

    vector<int> result_a = dijkstra_a(graph, src);

    // part b example data
    vector<vector<pair<int, int>>> adj(5);
    adj[0] = {{1, 4}, {2, 8}};
    adj[1] = {{0, 4}, {4, 6}, {2, 3}};
    adj[2] = {{0, 8}, {3, 2}, {1, 3}};
    adj[3] = {{2, 2}, {4, 10}};
    adj[4] = {{1, 6}, {3, 10}};

    vector<int> result_b = dijkstra_b(adj, src);

    // test print results
    cout << "result_a : ";
    for (int d : result_a)
        cout << d << " ";
    cout << "\n";

    cout << "result_b : ";
    for (int d : result_b)
        cout << d << " ";
    cout << "\n";

    // run empirical analysis and save CSVs
    runEmpirical();

    return 0;
}