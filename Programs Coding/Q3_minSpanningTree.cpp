#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <unordered_map>
#include <sstream>
#include <cstdlib>

using namespace std;

// Structure to represent a star
struct Star {
    string name;
    int x, y, z;
    int weight, profit;
};

// Structure to represent an edge
struct Edge {
    string from, to;
    double distance;
};

// Union-Find structure to support Kruskal's algorithm
class UnionFind {
    private:
        vector<int> parent;
        vector<int> rank;
        
    public:
        UnionFind(int n) : parent(n), rank(n, 0) {
            for (int i = 0; i < n; ++i)
                parent[i] = i;
        }

        int find(int u) {
            if (parent[u] != u)
                parent[u] = find(parent[u]);
            return parent[u];
        }

        bool unite(int u, int v) {
            int rootU = find(u);
            int rootV = find(v);
            if (rootU != rootV) {
                if (rank[rootU] > rank[rootV]) {
                    parent[rootV] = rootU;
                } else if (rank[rootU] < rank[rootV]) {
                    parent[rootU] = rootV;
                } else {
                    parent[rootV] = rootU;
                    ++rank[rootU];
                }
                return true;
            }
            return false;
        }
};

// Function to calculate distance between two stars
double calculateDistance(const Star &star1, const Star &star2) {
    return sqrt(pow(star2.x - star1.x, 2) + pow(star2.y - star1.y, 2) + pow(star2.z - star1.z, 2));
}

// Function to read stars from a file
vector<Star> readStars(const string &filename) {
    vector<Star> stars;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        Star star;
        iss >> star.name >> star.x >> star.y >> star.z >> star.weight >> star.profit;
        stars.push_back(star);
    }
    return stars;
}

// Function to read edges from a file
vector<Edge> readEdges(const string &filename) {
    vector<Edge> edges;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        Edge edge;
        iss >> edge.from >> edge.to >> edge.distance;
        edges.push_back(edge);
    }
    return edges;
}

// Kruskal's algorithm to find the MST
vector<Edge> kruskalMST(const vector<Edge> &edges, int numberOfNodes) {
    vector<Edge> mst;
    UnionFind uf(numberOfNodes);
    auto sortedEdges = edges;

    sort(sortedEdges.begin(), sortedEdges.end(), [](const Edge &a, const Edge &b) {
        return a.distance < b.distance;
    });

    for (const auto &edge : sortedEdges) {
        int u = edge.from.back() - 'A';
        int v = edge.to.back() - 'A';
        if (uf.unite(u, v)) {
            mst.push_back(edge);
            if (mst.size() == numberOfNodes - 1)
                break;
        }
    }
    return mst;
}

// Function to save MST edges to a file
void saveMST(const vector<Edge> &mst, const string &filename) {
    ofstream file(filename);
    for (const auto &edge : mst) {
        file << edge.from << " " << edge.to << " " << edge.distance << endl;
    }
}

// Function to print edges
void printEdges(const vector<Edge> &edges) {
    cout << "Edges in the Minimum Spanning Tree:\n";
    for (const auto &edge : edges) {
        cout << edge.from << " - " << edge.to << " : " << edge.distance << endl;
    }
}

int main() {
    vector<Star> stars = readStars("./Q1_dataset2Output/stars.txt");
    vector<Edge> edges = readEdges("./Q1_dataset2Output/edges.txt");

    vector<Edge> mst = kruskalMST(edges, stars.size());

    saveMST(mst, "./Q3_AnswerOutput/kruskal_mstEdges.txt");
    printEdges(mst);

    cout << "\nMinimum Spanning Tree stored in './Q3_AnswerOutput/kruskal_mstEdges.txt'\n";



    return 0;
}
