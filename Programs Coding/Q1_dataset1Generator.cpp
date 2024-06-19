#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Extract all the digits from the ID
vector<int> extractDigits(int id) {
    vector<int> digits;
    while (id > 0) {
        digits.push_back(id % 10);
        id /= 10;
    }
    return digits;
}


// Generate all the posible combinations from the extracted digits in 2, 3, 4 digits
vector<int> generateCombinations(vector<int>& digits) {
    vector<int> combinations;
    // 2-digits combination
    for(size_t i = 0; i < digits.size(); ++i) {
        for(size_t j = 0; j < digits.size(); ++j) {
            if(i != j) {
                combinations.push_back((digits[i]*10) + digits[j]);
            }
        }
    }
    //3-digits combination
    for(size_t i = 0; i < digits.size(); ++i){
        for(size_t j = 0; j < digits.size(); ++j) {
            for(size_t k = 0; k < digits.size(); ++k) {
                if(i != j && i != k && i != k) {
                    combinations.push_back((digits[i] * 100) + (digits[j] * 10) + digits[k]);
                }
            }
        }
    }
    //4-digits combinations
    for(size_t i = 0; i < digits.size(); ++i) {
        for(size_t j = 0; j < digits.size(); ++j) {
            for(size_t k = 0; k < digits.size(); ++k) {
                for(size_t l = 0; l < digits.size(); l++) {
                    if(i != j && i != k && i != l && j != k && j != l && k != l) {
                        combinations.push_back((digits[i] * 1000) + (digits[j] * 100) + (digits[k] * 10) + digits[l]);
                    }
                }
            }
        }
    }

    return combinations;
}

// Get a random number combinations
int getRandomCombinations(const vector<int>& combinations) {
    int randomIdx = rand() % combinations.size();
    return combinations[randomIdx];
}


// Generate datasets
vector<vector<int>> generateDataset(vector<int>& combinations) {
    srand(time(0));
    vector<vector<int>> datasets;
    vector<int> sizes = {100, 1000, 10000, 100000, 250000, 500000};

    for(int size: sizes) {
        vector<int> dataset;
        for(size_t i = 0; i < size; ++i) {
            dataset.push_back(getRandomCombinations(combinations));
        }
        datasets.push_back(dataset);
    }

    return datasets;
} 


void storeDatasets(const vector<vector<int>>& datasets) {
    for(size_t i = 0; i < datasets.size(); ++i) {
        string fileName = "./Q1_dataset1Output/dataset_" + to_string(i+1) + ".txt";
        ofstream outfile(fileName);
        for(int value: datasets[i]) {
            outfile << value << endl;
        }
        outfile.close();
        cout << "Dataset " << i + 1 << " stored in " << fileName << endl;
    }
}



int main() {
    int leaderID = 1211102398;

    vector<int> digits = extractDigits(leaderID);

    vector<int> combinations = generateCombinations(digits);

    vector<vector<int>> datasets = generateDataset(combinations);

    storeDatasets(datasets);

    return 0;
}