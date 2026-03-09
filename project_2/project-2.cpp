#include <iostream>
#include <vector>
#include <queue>
#include <climits>
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

    return 0;
}