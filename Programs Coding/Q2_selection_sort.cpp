#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>

using namespace std;

// Selection Sort function
template<typename T>
void selectionSort(vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex])
                minIndex = j;
        }
        swap(arr[i], arr[minIndex]);
    }
}

// Function to read datasets from text files
vector<vector<int>> readDatasets(const vector<string>& filenames) {
    vector<vector<int>> datasets;
    for (const auto& filename : filenames) {
        ifstream infile(filename);
        vector<int> dataset;
        int number;
        while (infile >> number) {
            dataset.push_back(number);
        }
        datasets.push_back(dataset);
    }
    return datasets;
}

// Function to store sorted datasets in txt files
void storeSortedDatasets(const vector<vector<int>>& datasets, const string& algorithm) {
    for (size_t i = 0; i < datasets.size(); ++i) {
        string filename = "./Q2_AnswerOutput/selectionSorted_dataset1/" + algorithm + "_sorted_dataset_" + to_string(i + 1) + ".txt";
        ofstream outfile(filename);
        for (size_t j = 0; j < datasets[i].size(); ++j) {
            outfile << datasets[i][j] << endl;
        }
        outfile.close();
        cout << "Sorted dataset " << i + 1 << " using " << algorithm << " stored in " << filename << endl;
    }
}

// Function to store the timing for each dataset
void storeSortingTimes(const vector<double>& times, const string& algorithm) {
    string filename = "./Q2_AnswerOutput/selectionSorted_dataset1/" + algorithm + "_sorting_times.txt";
    ofstream outfile(filename);
    for (size_t i = 0; i < times.size(); ++i) {
        outfile << "Dataset " << i + 1 << ": " << times[i] << " seconds" << endl;
    }
    outfile.close();
    cout << "Sorting times stored in " << filename << endl;
}

int main() {
    // Filenames of datasets to be read
    vector<string> filenames = {
        "./Q1_dataset1Output/dataset_1.txt", "./Q1_dataset1Output/dataset_2.txt", "./Q1_dataset1Output/dataset_3.txt", 
        "./Q1_dataset1Output/dataset_4.txt", "./Q1_dataset1Output/dataset_5.txt", "./Q1_dataset1Output/dataset_6.txt"
    };

    // Read datasets
    vector<vector<int>> datasets = readDatasets(filenames);

    // Vector to store sorting times for each dataset
    vector<double> sortingTimes;

    // Sort datasets using Selection Sort and record time for each dataset
    for (auto& dataset : datasets) {
        auto start = chrono::high_resolution_clock::now();
        selectionSort(dataset);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        sortingTimes.push_back(duration.count());
    }

    // Output the sorting times for each dataset
    for (size_t i = 0; i < sortingTimes.size(); ++i) {
        cout << "Time taken to sort dataset " << i + 1 << ": " << sortingTimes[i] << " seconds." << endl;
    }

    // Store sorted datasets
    storeSortedDatasets(datasets, "selection");

    // Store sorting times
    storeSortingTimes(sortingTimes, "selection");

    return 0;
}
