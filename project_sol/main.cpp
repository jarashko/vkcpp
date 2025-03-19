#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <stdexcept>
using namespace std;

class Graph {
private:
    vector<vector<int>> adjList;
    int vertexCount;

public:
    explicit Graph(int vertices = 0) : vertexCount(vertices) {
        if (vertices < 0) {
            throw invalid_argument("Invalid vertex count.");
        }
        adjList.resize(vertices);
    }
    void addEdge(int u, int v) {
        if (u < 0 || u >= vertexCount || v < 0 || v >= vertexCount) {
            throw out_of_range("Invalid vertex ID.");
        }
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }
    void resize(int vertices) {
        if (vertices < 0) {
            throw invalid_argument("Invalid vertex count.");
        }
        vertexCount = vertices;
        adjList.resize(vertices);
    }

    const vector<vector<int>>& getAdjList() const { return adjList; }
    int getVertexCount() const { return vertexCount; }
};

class BFSAlg {
public:
    static vector<int> findDistance(const Graph& graph, int start) {
        int vertexCount = graph.getVertexCount();
        if (start < 0 || start >= vertexCount) {
            throw invalid_argument("Invalid start node.");
        }
        vector<int> distances(vertexCount, -1);
        queue<int> traversalQueue;
        distances[start] = 0;
        traversalQueue.push(start);

        while (!traversalQueue.empty()) {
            int current = traversalQueue.front();
            traversalQueue.pop();
            for (int neighbor : graph.getAdjList()[current]) {
                if (distances[neighbor] == -1) {
                    distances[neighbor] = distances[current] + 1;
                    traversalQueue.push(neighbor);
                }
            }
        }
        return distances;
    }
};

void readGraph(const string& filename, Graph& graph, int& startNode) {
    ifstream input(filename);
    if (!input) {
        throw runtime_error("File not found.");
    }
    int vertexCount, edgeCount;
    input >> vertexCount >> edgeCount;
    if (vertexCount <= 0 || edgeCount < 0) {
        throw invalid_argument("Invalid input.");
    }
    graph.resize(vertexCount);
    for (int i = 0; i < edgeCount; ++i) {
        int u, v;
        if (!(input >> u >> v)) {
            throw runtime_error("Edge read error.");
        }
        graph.addEdge(u, v);
    }
    if (!(input >> startNode) || startNode < 0 || startNode >= vertexCount) {
        throw runtime_error("Invalid start node.");
    }
}

int main() {
    try {
        Graph graph;
        int startNode;
        readGraph("graph.txt", graph, startNode);
        vector<int> distances = BFSAlg::findDistance(graph, startNode);
        cout << "Distances from node " << startNode << ":\n";
        for (size_t i = 0; i < distances.size(); ++i) {
            if (i != startNode) {
                cout << "Node " << i << ": " << distances[i] << "\n";
            }
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}