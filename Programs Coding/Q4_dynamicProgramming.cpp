#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Star {
    string name;
    int weight;
    int profit;
};

vector<Star> readStars(const string& filename) {
    ifstream file(filename);
    vector<Star> stars;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Star star;
        string dummy;
        ss >> star.name >> dummy >> dummy >> dummy >> star.weight >> star.profit;
        stars.push_back(star);
    }

    return stars;
}

void printSegment(ofstream &outputFile, const vector<vector<int>>& dp, int startCol, int endCol, int n) {
    int colWidth = 6;
    string maxRowLabel = "{a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t}";
    int firstColWidth = maxRowLabel.size() + 2;

    // Print the header row
    outputFile << setw(firstColWidth) << " ";
    cout << setw(firstColWidth) << " ";
    for (int w = startCol; w <= endCol; ++w) {
        outputFile << setw(colWidth) << w;
        cout << setw(colWidth) << w;
    }
    outputFile << endl;
    cout << endl;

    // Print the DP matrix segment
    for (int i = 0; i <= n; ++i) {
        string rowLabel = "{";
        for (int j = 1; j <= i; ++j) {
            rowLabel += (j == 1 ? "" : ",") + string(1, 'a' + j - 1);
        }
        rowLabel += "}";    

        outputFile << setw(firstColWidth) << rowLabel;
        cout << setw(firstColWidth) << rowLabel;

        for (int w = startCol; w <= endCol; ++w) {
            outputFile << setw(colWidth) << dp[i][w];
            cout << setw(colWidth) << dp[i][w];
        }
        outputFile << endl;
        cout << endl;
    }
}

void knapsack(const vector<Star>& stars, int maxWeight, const string& outputFileName) {
    int n = stars.size();
    vector<vector<int>> dp(n + 1, vector<int>(maxWeight + 1, 0));

    // Build the dp matrix
    for (int i = 1; i <= n; ++i) {
        for (int w = 1; w <= maxWeight; ++w) {
            if (stars[i - 1].weight <= w) {
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - stars[i - 1].weight] + stars[i - 1].profit);
            } else { 
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    ofstream outputFile(outputFileName);

    // Output the dp matrix in segments
    int segmentSize = 20;
    for (int startCol = 0; startCol <= maxWeight; startCol += segmentSize) {
        int endCol = min(startCol + segmentSize - 1, maxWeight);

        outputFile << string(70, '-') << endl;
        outputFile << "Segment [" << startCol << "- " << endCol << "]:" << endl;
        outputFile << string(70, '-') << endl;

        cout << string(70, '-') << endl;
        cout << "Segment [" << startCol << "- " << endCol << "]:" << endl;
        cout << string(70, '-') << endl;

        printSegment(outputFile, dp, startCol, endCol, n);

        outputFile << endl;
        cout << endl;
    }

    // Find the stars to visit
    int w = maxWeight;
    vector<Star> resultStars;
    for (int i = n; i > 0 && w > 0; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            resultStars.push_back(stars[i - 1]);
            w -= stars[i - 1].weight;
        }
    }

    // Output the result & Calculate the total profit and weight
    int totalProfit = 0;
    int totalWeight = 0;

    outputFile << "Stars to visit:" << endl;
    for (const Star& star : resultStars) {
        totalProfit += star.profit;
        totalWeight += star.weight;
        outputFile << star.name << " - Weight: " << star.weight << "kg, Profit: " << star.profit << endl;
        cout << star.name << " - Weight: " << star.weight << "kg, Profit: " << star.profit << endl;
    }

    outputFile << "--------------------------------" << endl;
    outputFile << "Total Weight: " << totalWeight << "kg" << endl;
    outputFile << "Total Profit: " << totalProfit << endl;

    cout << "--------------------------------" << endl;
    cout << "Total Weight: " << totalWeight << "kg" << endl;
    cout << "Total Profit: " << totalProfit << endl;

    outputFile.close();
}

int main() {
    int maxWeight = 800;
    string starsFile = "./Q1_dataset2Output/stars.txt";
    string outputFileName = "./Q4_AnswerOutput/knapsack_output.txt";

    vector<Star> stars = readStars(starsFile);

    // Output generated stars
    cout << "Generated Stars:" << endl;
    for (const Star& star : stars) {
        cout << star.name << " - Weight: " << star.weight << "kg, Profit: " << star.profit << endl;
    }
    cout << endl;

    // Solve the knapsack problem
    knapsack(stars, maxWeight, outputFileName);

    cout << "Results saved to " << outputFileName << endl;

    return 0;
}
