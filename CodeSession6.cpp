//
// Created by erinw on 4/14/2026.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <utility>
#include <functional>
using namespace std;

struct DijkstraResult {
    vector<int> dist;
    vector<int> parent;
    vector<int> order;
};

void reverse_vector(vector<int>& a) {
    int i = 0, j = (int)a.size() - 1;
    while (i < j) {
        int t = a[i];
        a[i] = a[j];
        a[j] = t;
        ++i; --j;
    }
}

using pii = pair<int,int>;

DijkstraResult dijkstra_shortest_path_matrix(const vector<vector<int>>& M, int s, int t) {
    int n = (int)M.size();
    const int INF = 1e9;

    DijkstraResult res;
    res.dist.assign(n, INF);
    res.parent.assign(n, -1);

    vector<bool> visited(n, false);

    priority_queue<pii, vector<pii>, greater<pii>> pq;
    res.dist[s] = 0;
    pq.push({0, s});

    int step = 0;

    while (!pq.empty()) {
        auto [du, u] = pq.top();
        pq.pop();

        if (visited[u]) continue;

        visited[u] = true;
        res.order.push_back(u);

        // TRACE OUTPUT
        cout << "Step " << step++ << ":\n";

        cout << "visited: { ";
        for (int i = 0; i < n; i++)
            if (visited[i]) cout << i << " ";
        cout << "}\n";

        cout << "dist:    [ ";
        for (int d : res.dist)
            if (d == INF) cout << "INF ";
            else cout << d << " ";
        cout << "]\n";

        cout << "parent:  [ ";
        for (int p : res.parent)
            cout << p << " ";
        cout << "]\n\n";

        if (u == t) break;

        for (int v = 0; v < n; ++v) {
            int w = M[u][v];
            if (w == 0) continue;

            int cand = res.dist[u] + w;

            if (!visited[v] && cand < res.dist[v]) {
                res.dist[v] = cand;
                res.parent[v] = u;
                pq.push({res.dist[v], v});
            }
        }
    }

    return res;
}

vector<int> reconstruct_path(const vector<int>& parent, int s, int t) {
    vector<int> path;

    for (int cur = t; cur != -1; cur = parent[cur])
        path.push_back(cur);

    reverse_vector(path);

    if (path.empty() || path.front() != s)
        return {};

    return path;
}

int main() {
    ifstream fin("mo_cities_8.txt");
    if (!fin) {
        cout << "Error: could not open mo_cities_8.txt\n";
        return 1;
    }

    vector<string> cityNames(10);
    string line;

    // READ FIRST 10 LINES (cities)
    for (int i = 0; i < 10; i++) {
        int id;
        string name;
        fin >> id >> name;
        cityNames[id] = name;
    }

    getline(fin, line); // clear leftover newline

    // Skip blank lines
    do {
        getline(fin, line);
    } while (line.size() == 0);

    vector<vector<int>> M(10, vector<int>(10, 0));

    // PARSE EDGE LIST
    int i = 0;
    while (i < (int)line.size()) {
        if (line[i] == '(') {
            i++;

            int u = 0;
            while (isdigit(line[i])) {
                u = u * 10 + (line[i] - '0');
                i++;
            }

            i++; // skip ','

            int v = 0;
            while (isdigit(line[i])) {
                v = v * 10 + (line[i] - '0');
                i++;
            }

            i++; // skip ')'
            i++; // skip '='

            int w = 0;
            while (isdigit(line[i])) {
                w = w * 10 + (line[i] - '0');
                i++;
            }

            M[u][v] = w;
            M[v][u] = w;
        } else {
            i++;
        }
    }

    // PRINT CITY LIST
    cout << "List of cities:\n\n";
    for (int i = 0; i < 10; i++)
        cout << i << "--" << cityNames[i] << "\n";
    cout << "\n";

    // PRINT MATRIX
    cout << "Weighted Adjacency Matrix (miles):\n";
    cout << "    0 1 2 3 4 5 6 7 8 9\n";

    for (int i = 0; i < 10; i++) {
        cout << i << "  ";
        for (int j = 0; j < 10; j++)
            cout << M[i][j] << " ";
        cout << "\n";
    }
    cout << "\n";

    // USER INPUT
    int start, goal;

    cout << "Enter the start city: ";
    cin >> start;

    cout << "Enter the end city: ";
    cin >> goal;

    if (start < 0 || start >= 10 || goal < 0 || goal >= 10) {
        cout << "Invalid city index.\n";
        return 1;
    }

    cout << "\nStart city: " << cityNames[start] << "\n";
    cout << "Goal city: " << cityNames[goal] << "\n\n";

    // RUN DIJKSTRA
    cout << "Trace:\n";
    DijkstraResult result = dijkstra_shortest_path_matrix(M, start, goal);

    // RECONSTRUCT PATH
    vector<int> path = reconstruct_path(result.parent, start, goal);

    if (path.empty()) {
        cout << "No path found.\n";
        return 0;
    }

    // OUTPUT RESULT
    cout << "\nShortest distance: " << result.dist[goal] << " miles\n";

    cout << "Path: ";
    for (int i = 0; i < path.size(); i++) {
        cout << cityNames[path[i]];
        if (i != path.size() - 1)
            cout << " -> ";
    }
    cout << "\n";

    return 0;
}