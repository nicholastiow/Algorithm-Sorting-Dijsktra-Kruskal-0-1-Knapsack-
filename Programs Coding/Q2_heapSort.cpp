#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <queue>

using namespace std;

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
        string filename = "./Q2_AnswerOutput/heapSorted_dataset1/" + algorithm + "_sorted_dataset_" + to_string(i + 1) + ".txt";
        ofstream outfile(filename);
        for (size_t j = 0; j < datasets[i].size(); ++j) {
            outfile << datasets[i][j] << endl;
        }
        outfile.close();
        cout << "Sorted dataset " << i + 1 << " using " << algorithm << " stored in " << filename << endl;
    }
}

// Function to store sorting times in a txt file
void storeSortingTimes(const vector<double>& times, const string& algorithm) {
    string filename = "./Q2_AnswerOutput/heapSorted_dataset1/" + algorithm + "_sorting_times.txt";
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

    // Measure time for inserting all data into the priority queue
    auto start_insert = chrono::high_resolution_clock::now();
    vector<priority_queue<int, vector<int>, greater<int>>> heaps;

    for (const auto& dataset : datasets) {
        priority_queue<int, vector<int>, greater<int>> heap;
        for (const auto& elem : dataset) {
            heap.push(elem);
        }
        heaps.push_back(move(heap));
    }


    auto end_insert = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_insert = end_insert - start_insert;
    cout << "Time taken to insert all data into the priority queue: " << duration_insert.count() << " seconds." << endl;


    // Measure time for dequeuing all data from the priority queue, row by row
    vector<vector<int>> sorted_datasets;
    vector<double> sortingTimes;
    
    for (size_t i = 0; i < heaps.size(); ++i) {
        auto start_dequeue = chrono::high_resolution_clock::now();
        vector<int> sorted_dataset;

        while (!heaps[i].empty()) {
            sorted_dataset.push_back(heaps[i].top());
            heaps[i].pop();
        }

        auto end_dequeue = chrono::high_resolution_clock::now();
        chrono::duration<double> duration_dequeue = end_dequeue - start_dequeue;
        cout << "Time taken to dequeue dataset " << i + 1 << " from the priority queue: " << duration_dequeue.count() << " seconds." << endl;
        
        sortingTimes.push_back(duration_dequeue.count());
        sorted_datasets.push_back(sorted_dataset);
    }

    // Store sorted datasets
    storeSortedDatasets(sorted_datasets, "heap");

    // Store sorting times
    storeSortingTimes(sortingTimes, "heap");

    return 0;
}
