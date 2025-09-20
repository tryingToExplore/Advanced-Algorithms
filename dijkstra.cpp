
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <map>
#include <string>
#include <queue>

using namespace std;

struct Edge {
    int destination;
    double weight;
    
    Edge(int dest, double w) : destination(dest), weight(w) {}
};

class Graph {
private:
    int numVertices;
    vector<vector<Edge>> adjacencyList;
    map<string, int> nodeToIndex;
    vector<string> indexToNode;
    
public:
    Graph() : numVertices(0) {}
    
    void addVertex(const string& name) {
        if (nodeToIndex.find(name) == nodeToIndex.end()) {
            nodeToIndex[name] = numVertices;
            indexToNode.push_back(name);
            adjacencyList.resize(++numVertices);
        }
    }
    
    void addEdge(const string& from, const string& to, double weight) {
        addVertex(from);
        addVertex(to);
        
        int fromIndex = nodeToIndex[from];
        int toIndex = nodeToIndex[to];
        
        adjacencyList[fromIndex].push_back(Edge(toIndex, weight));
        adjacencyList[toIndex].push_back(Edge(fromIndex, weight));
    }
    
    int getNumVertices() const {
        return numVertices;
    }
    
    string getVertexName(int index) const {
        if (index >= 0 && index < numVertices) {
            return indexToNode[index];
        }
        return "";
    }
    
    int getVertexIndex(const string& name) const {
        auto it = nodeToIndex.find(name);
        return (it != nodeToIndex.end()) ? it->second : -1;
    }
    
    vector<string> getAllVertices() const {
        return indexToNode;
    }
    
    const vector<Edge>& getNeighbors(int vertexIndex) const {
        if (vertexIndex >= 0 && vertexIndex < numVertices) {
            return adjacencyList[vertexIndex];
        }
        static vector<Edge> empty;
        return empty;
    }
    
    void displayGraph() const {
        cout << "\nGraph Representation (Adjacency List):" << endl;
        cout << "=====================================" << endl;
        for (int i = 0; i < numVertices; i++) {
            cout << indexToNode[i] << " -> ";
            for (const Edge& edge : adjacencyList[i]) {
                cout << "(" << indexToNode[edge.destination] 
                     << ", " << edge.weight << ") ";
            }
            cout << endl;
        }
        cout << endl;
    }
};
 
class DijkstraAlgorithm {
private:
    const Graph* graph;
    
public:
    DijkstraAlgorithm(const Graph* g) : graph(g) {}
    
    pair<double, vector<string>> findShortestPath(const string& source, const string& destination) {
        int sourceIndex = graph->getVertexIndex(source);
        int destIndex = graph->getVertexIndex(destination);
        
        if (sourceIndex == -1) {
            throw invalid_argument("Source vertex '" + source + "' not found in graph");
        }
        if (destIndex == -1) {
            throw invalid_argument("Destination vertex '" + destination + "' not found in graph");
        }
        if (sourceIndex == destIndex) {
            return {0.0, {source}};
        }
        
        int numVertices = graph->getNumVertices();
        
        vector<double> distances(numVertices, numeric_limits<double>::max());
        vector<int> parent(numVertices, -1);
        
        distances[sourceIndex] = 0.0;
        
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        pq.push({0.0, sourceIndex});
        
        while (!pq.empty()) {
            double currentDist = pq.top().first;
            int currentVertex = pq.top().second;
            pq.pop();
            
            if (currentDist > distances[currentVertex]) {
                continue;
            }
            
            if (currentVertex == destIndex) {
                break;
            }
            
            for (const Edge& edge : graph->getNeighbors(currentVertex)) {
                int neighbor = edge.destination;
                double newDist = currentDist + edge.weight;
                
                if (newDist < distances[neighbor]) {
                    distances[neighbor] = newDist;
                    parent[neighbor] = currentVertex;
                    pq.push({newDist, neighbor});
                }
            }
        }
        
        if (distances[destIndex] == numeric_limits<double>::max()) {
            return {-1.0, {}};
        }
        
        vector<string> path;
        int current = destIndex;
        while (current != -1) {
            path.push_back(graph->getVertexName(current));
            current = parent[current];
        }
        reverse(path.begin(), path.end());
        
        return {distances[destIndex], path};
    }
    
    vector<double> findShortestDistances(const string& source) {
        int sourceIndex = graph->getVertexIndex(source);
        
        if (sourceIndex == -1) {
            throw invalid_argument("Source vertex '" + source + "' not found in graph");
        }
        
        int numVertices = graph->getNumVertices();
        vector<double> distances(numVertices, numeric_limits<double>::max());
        
        distances[sourceIndex] = 0.0;
        
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        pq.push({0.0, sourceIndex});
        
        while (!pq.empty()) {
            double currentDist = pq.top().first;
            int currentVertex = pq.top().second;
            pq.pop();
            
            if (currentDist > distances[currentVertex]) {
                continue;
            }
            
            for (const Edge& edge : graph->getNeighbors(currentVertex)) {
                int neighbor = edge.destination;
                double newDist = currentDist + edge.weight;
                
                if (newDist < distances[neighbor]) {
                    distances[neighbor] = newDist;
                    pq.push({newDist, neighbor});
                }
            }
        }
        
        return distances;
    }
};
 
Graph createSampleGraph() {
    Graph graph;
    
    graph.addVertex("A");
    graph.addVertex("B");
    graph.addVertex("C");
    graph.addVertex("D");
    graph.addVertex("E");
    graph.addVertex("F");
    
    graph.addEdge("A", "B", 4.0);
    graph.addEdge("A", "C", 2.0);
    graph.addEdge("B", "C", 1.0);
    graph.addEdge("B", "D", 5.0);
    graph.addEdge("C", "D", 8.0);
    graph.addEdge("C", "E", 10.0);
    graph.addEdge("D", "E", 2.0);
    graph.addEdge("D", "F", 6.0);
    graph.addEdge("E", "F", 3.0);
    
    return graph;
}

void displayShortestPath(const string& source, const string& destination, 
                        double cost, const vector<string>& path) {
    cout << "\n" << string(50, '=') << endl;
    cout << "SHORTEST PATH RESULT" << endl;
    cout << string(50, '=') << endl;
    
    if (cost == -1.0) {
        cout << "No path exists from '" << source << "' to '" << destination << "'" << endl;
    } else {
        cout << "Source: " << source << endl;
        cout << "Destination: " << destination << endl;
        cout << "Shortest Distance: " << fixed << setprecision(2) << cost << endl;
        cout << "Path: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    }
    cout << string(50, '=') << endl;
}

Graph createUserGraph() {
    Graph graph;
    int numVertices, numEdges;
    
    cout << "\nEnter the number of vertices: ";
    cin >> numVertices;
    
    cout << "Enter vertex names:" << endl;
    for (int i = 0; i < numVertices; i++) {
        string vertexName;
        cout << "Vertex " << (i + 1) << ": ";
        cin >> vertexName;
        graph.addVertex(vertexName);
    }
    
    cout << "\nEnter the number of edges: ";
    cin >> numEdges;
    
    cout << "Enter edges (from to weight):" << endl;
    for (int i = 0; i < numEdges; i++) {
        string from, to;
        double weight;
        cout << "Edge " << (i + 1) << ": ";
        cin >> from >> to >> weight;
        graph.addEdge(from, to, weight);
    }
    
    return graph;
}
 
int main() {
    cout << "Dijkstra's Algorithm Implementation" << endl;
    cout << "===================================" << endl;
    
    Graph graph;
    int choice;
    
    cout << "\nChoose graph input method:" << endl;
    cout << "1. Use sample graph" << endl;
    cout << "2. Create custom graph" << endl;
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;
    
    if (choice == 1) {
        graph = createSampleGraph();
        cout << "\nSample graph created successfully!" << endl;
    } else if (choice == 2) {
        graph = createUserGraph();
        cout << "\nCustom graph created successfully!" << endl;
    } else {
        cout << "Invalid choice. Using sample graph." << endl;
        graph = createSampleGraph();
    }
    
    graph.displayGraph();
    
    DijkstraAlgorithm dijkstra(&graph);
    
    while (true) {
        cout << "\n" << string(40, '-') << endl;
        cout << "DIJKSTRA'S ALGORITHM MENU" << endl;
        cout << string(40, '-') << endl;
        cout << "1. Find shortest path between two vertices" << endl;
        cout << "2. Find shortest distances from a vertex to all others" << endl;
        cout << "3. Display graph" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice (1-4): ";
        
        cin >> choice;
        
        switch (choice) {
            case 1: {
                string source, destination;
                cout << "\nEnter source vertex: ";
                cin >> source;
                cout << "Enter destination vertex: ";
                cin >> destination;
                
                try {
                    auto [cost, path] = dijkstra.findShortestPath(source, destination);
                    displayShortestPath(source, destination, cost, path);
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            
            case 2: {
                string source;
                cout << "\nEnter source vertex: ";
                cin >> source;
                
                try {
                    vector<double> distances = dijkstra.findShortestDistances(source);
                    cout << "\nShortest distances from '" << source << "':" << endl;
                    cout << string(40, '-') << endl;
                    
                    for (int i = 0; i < graph.getNumVertices(); i++) {
                        string vertexName = graph.getVertexName(i);
                        if (distances[i] == numeric_limits<double>::max()) {
                            cout << vertexName << ": Unreachable" << endl;
                        } else {
                            cout << vertexName << ": " << fixed << setprecision(2) 
                                 << distances[i] << endl;
                        }
                    }
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }
            
            case 3:
                graph.displayGraph();
                break;
                
            case 4:
                cout << "\nThank you for using Dijkstra's Algorithm!" << endl;
                return 0;
                
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    
    return 0;
}