// 1211101699 1211101452 1211102080

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

// Structure to represent a star
struct Star {
    string name;
    int x;
    int y;
    int z;
    int weight;
    int profit;
};

// Function to extract digits from the ID
vector<int> extractDigits(long long id) {
    vector<int> digits;
    while (id > 0) {
        digits.push_back(id % 10);
        id /= 10;
    }
    reverse(digits.begin(), digits.end());
    return digits;
}

// Function to generate all valid 2 and 3 digit combinations
vector<int> generateCombinations(const vector<int>& digits) {
    vector<int> combinations;
    
    // Generate 2-digit combinations
    for (size_t i = 0; i < digits.size(); ++i) {
        for (size_t j = 0; j < digits.size(); ++j) {
            if (i != j) {
                combinations.push_back(digits[i] * 10 + digits[j]);
            }
        }
    }
    
    // Generate 3-digit combinations
    for (size_t i = 0; i < digits.size(); ++i) {
        for (size_t j = 0; j < digits.size(); ++j) {
            for (size_t k = 0; k < digits.size(); ++k) {
                if (i != j && j != k && i != k) {
                    combinations.push_back(digits[i] * 100 + digits[j] * 10 + digits[k]);
                }
            }
        }
    }

    // Remove duplicates and sort the combinations
    sort(combinations.begin(), combinations.end());
    combinations.erase(unique(combinations.begin(), combinations.end()), combinations.end());

    return combinations;
}

// Function to get a random combination
int getRandomCombination(const vector<int>& combinations) {
    int randomIndex = rand() % combinations.size();
    return combinations[randomIndex];
}

// Function to generate star data
vector<Star> generateStars(int numberOfStars, const vector<int>& combinations) {
    srand(time(0));
    vector<Star> stars;
    vector<string> alps = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T"};
    for (int i = 0; i < numberOfStars; ++i) {
        Star star;
        star.name = "Star_" + alps[i];
        star.x = getRandomCombination(combinations);
        star.y = getRandomCombination(combinations);
        star.z = getRandomCombination(combinations);
        star.weight = 10 + getRandomCombination(combinations) % 300;
        star.profit = 10 + getRandomCombination(combinations) % 300;
        stars.push_back(star);
    }
    return stars;
}

// Function to generate edges ensuring each star connects to at least 3 others
vector<pair<string, string>> generateEdges(const vector<Star>& stars, int numberOfEdges) {
    vector<pair<string, string>> edges;
    vector<int> degree(stars.size(), 0);
    srand(time(0));

    // Ensure each star has at least 3 connections
    for (size_t i = 0; i < stars.size(); ++i) {
        while (degree[i] < 3) {
            int target;
            do {
                target = rand() % stars.size();
            } while (target == i || find(edges.begin(), edges.end(), make_pair(stars[i].name, stars[target].name)) != edges.end() || 
                     find(edges.begin(), edges.end(), make_pair(stars[target].name, stars[i].name)) != edges.end());

            edges.push_back(make_pair(stars[i].name, stars[target].name));
            degree[i]++;
            degree[target]++;
        }
    }

    // Generate remaining edges
    while (edges.size() < numberOfEdges) {
        int from = rand() % stars.size();
        int to;
        do {
            to = rand() % stars.size();
        } while (from == to || find(edges.begin(), edges.end(), make_pair(stars[from].name, stars[to].name)) != edges.end() || 
                 find(edges.begin(), edges.end(), make_pair(stars[to].name, stars[from].name)) != edges.end());

        edges.push_back(make_pair(stars[from].name, stars[to].name));
    }

    return edges;
}

// Function to calculate distance between sconnected stars
string calculateDistance(const Star& star1, const Star& star2) {
    double distance = sqrt(pow(star2.x - star1.x, 2) + pow(star2.y - star1.y, 2) + pow(star2.z - star1.z, 2));
    
    ostringstream oss;
    oss << fixed << setprecision(1) << distance;
    return oss.str();
}


// Function to store stars and edges into text files
void storeData(const vector<Star>& stars, const vector<pair<string, string>>& edges) {
    ofstream starFile("./Q1_dataset2Output/stars.txt");
    for (const auto& star : stars) {
        starFile << star.name << " " << star.x << " " << star.y << " " << star.z << " " << star.weight << " " << star.profit << endl;
    }
    starFile.close();

    ofstream edgeFile("./Q1_dataset2Output/edges.txt");
    for (const auto& edge : edges) {
        int fromIndex = edge.first.back() - 'A';
        int toIndex = edge.second.back() - 'A';
        edgeFile << edge.first << " " << edge.second << " " << calculateDistance(stars[fromIndex], stars[toIndex]) << endl;
    }
    edgeFile.close();
}

void printStars(const vector<Star>& stars) {
    // Print table header
    cout << "\n\nStars Information:" << endl;
    cout << string(70, '-') << endl;

    cout << left << setw(5) << "Idx"
         << left << setw(15) << "Name" 
         << right << setw(10) << "X"
         << right << setw(10) << "Y"
         << right << setw(10) << "Z"
         << right << setw(10) << "Weight"
         << right << setw(10) << "Profit" << endl;
    
    cout << string(70, '-') << endl;

    // Print each star information in a formatted way
    for (size_t i = 0; i < stars.size(); ++i) {
        const auto& star = stars[i];
        cout << left << setw(5) << i
             << left << setw(15) << star.name
             << right << setw(10) << star.x
             << right << setw(10) << star.y
             << right << setw(10) << star.z
             << right << setw(10) << star.weight
             << right << setw(10) << star.profit << endl;
    }
}

int main() {
    long long studentId1, studentId2, studentId3;
    int choice;

    cout << "Q1_dataset2Output Random Generator" << endl;
    cout << "--------------------------------------------------" << endl;

    cout << "Enter 1 to enter student IDs manually or 2 to use default IDs: ";
    cin >> choice;

    if (choice == 1) {
        cout << "Enter the 1st student ID: ";
        cin >> studentId1;

        cout << "Enter the 2nd student ID: ";
        cin >> studentId2;

        cout << "Enter the 3rd student ID: ";
        cin >> studentId3;
    } else {
        studentId1 = 1211101699;
        studentId2 = 1211101452;
        studentId3 = 1211102080;
    }

    long long groupIDSum = studentId1 + studentId2 + studentId3;
    cout << "--------------------------------------------------" << endl;
    cout << "Student IDs: " << studentId1 << " + " << studentId2 << " + " << studentId3 << endl;
    cout << "Group ID sum: " << groupIDSum << endl;
    cout << "--------------------------------------------------" << endl;

    int numberOfStars = 20;
    int numberOfEdges = 54;

    // Extract digits from the sum of group member ids
    vector<int> digits = extractDigits(groupIDSum);

    // Generate valid combinations
    vector<int> combinations = generateCombinations(digits);

    // Generate star data
    vector<Star> stars = generateStars(numberOfStars, combinations);

    // Generate edges ensuring connectivity
    vector<pair<string, string>> edges = generateEdges(stars, numberOfEdges);

    printStars(stars);

    // Store stars and edges into text files (stars.txt and edges.txt)
    storeData(stars, edges);

    cout << "\n\nDataset generated and stored in 'stars.txt' and 'edges.txt'" << endl;

    return 0;
}
