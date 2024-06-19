#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <limits>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

// Structure to represent a star
struct Star {
    string name;
    int x, y, z, weight, profit;
};

// Structure to represent an edge
struct Edge {
    string from, to;
    double distance;
};

// Function to read stars from a file
vector<Star> readStars(const string& filename) {
    vector<Star> stars;
    ifstream file(filename);
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        Star star;
        ss >> star.name >> star.x >> star.y >> star.z >> star.weight >> star.profit;
        if (!ss.fail()) {
            stars.push_back(star);
        }
    }
    
    return stars;
}

// Function to read edges from a file
vector<Edge> readEdges(const string& filename) {
    vector<Edge> edges;
    ifstream file(filename);
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        Edge edge;
        ss >> edge.from >> edge.to >> edge.distance;
        if (!ss.fail()) {
            edges.push_back(edge);
        }
    }
    
    return edges;
}


// Dijkstra's algorithm to find the shortest paths from a source star
pair<unordered_map<string, double>, unordered_map<string, string>> dijkstra(const string& source, const vector<Star>& stars, const vector<Edge>& edges) {
    unordered_map<string, vector<pair<string, double>>> graph;
    
    // Build the graph
    for (const auto& edge : edges) {
        graph[edge.from].emplace_back(edge.to, edge.distance);
        graph[edge.to].emplace_back(edge.from, edge.distance);
    }

    
    // Priority queue to select the next node with the shortest distance
    priority_queue<pair<double, string>, vector<pair<double, string>>, greater<>> pq;
    unordered_map<string, double> distances;
    unordered_map<string, string> previous;
    
    for (const auto& star : stars) {
        distances[star.name] = numeric_limits<double>::infinity();
        previous[star.name] = ""; // Initialize with empty string
    }
    
    if (distances.find(source) == distances.end()) {
        cerr << "Source node " << source << " not found in the stars data." << endl;
        return {distances, previous};
    }

    distances[source] = 0.0;
    pq.emplace(0.0, source);
    
    while (!pq.empty()) {
        double currentDistance = pq.top().first;
        string currentNode = pq.top().second;
        pq.pop();
        
        if (currentDistance > distances[currentNode]) continue;
        
        for (const auto& neighbor : graph[currentNode]) {
            double distance = currentDistance + neighbor.second;
            if (distance < distances[neighbor.first]) {
                distances[neighbor.first] = distance;
                previous[neighbor.first] = currentNode; // Track the previous node
                pq.emplace(distance, neighbor.first);
            }
        }
    }
    
    return {distances, previous};
}

// Function to reconstruct the path from source to each node
vector<string> reconstructPath(const string& target, const unordered_map<string, string>& previous) {
    vector<string> path;
    for (string at = target; !at.empty(); at = previous.at(at)) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

// Function to store the shortest paths in a text file
void storeShortestPaths(const string& source, const unordered_map<string, double>& distances, const unordered_map<string, string>& previous) {
    ofstream file("./Q3_AnswerOutput/dijkstra_shortestPaths.txt");
    file << fixed << setprecision(2); // Set precision to 2 decimal places
    file << "Shortest distances from " << source << ":\n";

    // Sort distances by star name
    vector<pair<string, double>> sortedDistances(distances.begin(), distances.end());
    sort(sortedDistances.begin(), sortedDistances.end());

    for (const auto& [star, distance] : sortedDistances) {
        if (distance == numeric_limits<double>::infinity()) {
            file << "To " << star << ": inf units\n";
        } else {
            file << "To " << star << ": " << distance << " units";
            vector<string> path = reconstructPath(star, previous);
            if (!path.empty()) {
                file << " (path: ";
                for (size_t i = 0; i < path.size() - 1; ++i) {
                    file << path[i] << " -> ";
                }
                file << path.back() << ")";
            }
            file << "\n";
        }
    }
    file.close();
}

void printAllShortestPaths(const string& source, const unordered_map<string, double>& distances, const unordered_map<string, string>& previous) {
    cout << "Shortest distances from " << source << ":\n";
    // Sort distances by star name
    vector<pair<string, double>> sortedDistances(distances.begin(), distances.end());
    sort(sortedDistances.begin(), sortedDistances.end());

    for (const auto& [star, distance] : sortedDistances) {
        if (distance == numeric_limits<double>::infinity()) {
            cout << "To " << star << ": inf units\n";
        } else {
            cout << "To " << star << ": " << distance << " units";
            vector<string> path = reconstructPath(star, previous);
            if (!path.empty()) {
                cout << " (path: ";
                for (size_t i = 0; i < path.size() - 1; ++i) {
                    cout << path[i] << " -> ";
                }
                cout << path.back() << ")";
            }
            cout << "\n";
        }
    }
}

int main() {
    // Read stars and edges from the files
    vector<Star> stars = readStars("./Q1_dataset2Output/stars.txt");
    vector<Edge> edges = readEdges("./Q1_dataset2Output/edges.txt");

    // Run Dijkstra's algorithm from Star A
    string source = "Star_A";
    auto [distances, previous] = dijkstra(source, stars, edges);

    printAllShortestPaths(source, distances, previous);
    
    // Store the shortest paths in a text file
    storeShortestPaths(source, distances, previous);
    
    cout << "Shortest paths have been stored in './Q3_AnswerOutput/dijkstra_shortestPaths.txt'.\n";
    
    return 0;
}