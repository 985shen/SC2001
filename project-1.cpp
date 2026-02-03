#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <iomanip>
#include <climits>

using namespace std;

long long comparisonCount = 0;


void insert(vector<int>& arr, int left, int right){
    // assume the first index is already sorted
    for (int i=left+1; i<=right; i++){
        int key = arr[i];
        int j=i-1;
        // keep swappng until key > j
        while (j>= left && key < arr[j]){
            comparisonCount++;
            arr[j+1] = arr[j];
            j--;
        }
        if (j >= left) {
            comparisonCount++;
        }
        arr[j+1] = key;
    }
}

void merge(vector<int>& arr, vector<int>& temp, int left, int mid, int right){
    // copy current segment into temp once
    for (int i = left; i <= right; ++i) {
        temp[i] = arr[i];
    }

    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        comparisonCount++;
        if (temp[i] <= temp[j]) {
            arr[k++] = temp[i++];
        } else {
            arr[k++] = temp[j++];
        }
    }
    while (i <= mid) {
        arr[k++] = temp[i++];
    }
    while (j <= right) {
        arr[k++] = temp[j++];
    }
}

void hybridSortImpl(vector<int>& arr, vector<int>& temp, int left, int right, int S){
    int diff = right - left + 1;
    if (diff <= 1) return;
    if (diff <= S){
        insert(arr, left, right);
        return;
    }
    int mid = left + (right - left) / 2;
    hybridSortImpl(arr, temp, left, mid, S);
    hybridSortImpl(arr, temp, mid + 1, right, S);
    merge(arr, temp, left, mid, right);
}

void hybridSort(vector<int>& arr, int S = 32){
    if (arr.empty()) return;
    comparisonCount = 0;
    vector<int> temp(arr.size());
    hybridSortImpl(arr, temp, 0, static_cast<int>(arr.size()) - 1, S);
}

vector<int> generateRandomArray(int size, int maxValue) {
    vector<int> arr(size);
    
    // random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, maxValue);
    
    for (int i = 0; i < size; i++){
        arr[i] = dis(gen);
    }
    
    return arr;
}

void saveResults(const string& filename, const vector<pair<int, long long>>& data) {
    ofstream file(filename);
    file << "Input,Comparisons\n";
    for (const auto& i : data) {
        file << i.first << "," << i.second << "\n";
    }
    file.close();
    cout << "Results saved to " << filename << endl;
}

void testPartCI() {
    // fixed S, vary n
    cout << "\n=== Part (Ci): Fixed S=32, varying n ===" << endl;
    
    vector<int> sizes = {1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int maxValue = 10000000;
    vector<pair<int, long long>> results;
    
    cout << left << setw(15) << "Size" << "Comparisons" << endl;
    cout << string(40, '-') << endl;
    
    for (int size : sizes) {
        vector<int> arr = generateRandomArray(size, maxValue);
        hybridSort(arr);
        
        cout << setw(15) << size << comparisonCount << endl;
        results.push_back({size, comparisonCount});
    }
    
    saveResults("results_ci.csv", results);
}

void testPartCII() {
    // fixed n, vary S
    cout << "\n=== Part (cii): Fixed n=1000000, varying S ===" << endl;
    
    vector<int> sValues = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int fixedSize = 1000000;
    int maxValue = 10000000;
    vector<pair<int, long long>> results;
    
    cout << left << setw(15) << "S value" << "Comparisons" << endl;
    cout << string(40, '-') << endl;
    
    for (int s : sValues) {
        vector<int> arr = generateRandomArray(fixedSize, maxValue);
        hybridSort(arr, s);
        
        cout << setw(15) << s << comparisonCount << endl;
        results.push_back({s, comparisonCount});
    }
    
    saveResults("results_cii.csv", results);
}

void testPartCIII() {
    // vary both S and n
    cout << "\n=== Part (ciii): Finding optimal value of S ===" << endl;
    
    vector<int> sizes = {10000, 100000, 1000000, 10000000};
    vector<int> sValues = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int maxValue = 10000000;
    
    ofstream file("results_ciii.csv");
    file << "Size";
    for (int s : sValues) {
        file << ",S=" << s;
    }
    file << "\n";
    
    for (int size : sizes) {
        cout << "\nTesting size: " << size << endl;
        file << size;
        
        long long minComparisons = LLONG_MAX;
        int optimalS = 0;
        
        for (int s : sValues) {
            vector<int> arr = generateRandomArray(size, maxValue);
            hybridSort(arr, s);
            
            file << "," << comparisonCount;
            
            if (comparisonCount < minComparisons) {
                minComparisons = comparisonCount;
                optimalS = s;
            }
        }
        file << "\n";
        
        cout << "Optimal S = " << optimalS << " (comparisons: " << minComparisons << ")" << endl;
    }
    
    file.close();
    cout << "\nResults saved to results_ciii.csv" << endl;
}

int main(){
    cout << "=====================================" << endl;
    cout << "Hybrid Sort Algorithm Analysis" << endl;
    cout << "=====================================" << endl;
    
    cout << "Testing part ci" << endl;
    testPartCI();
    cout << "\nPart ci test complete\n" << endl;
    cout << "Testing part cii" << endl;
    testPartCII();
    cout << "\nPart cii test complete\n" << endl;
    cout << "Testing part ciii" << endl;
    testPartCIII();
    cout << "\nPart ciii test complete\n" << endl;
    
    cout << "\n=====================================" << endl;
    cout << "All tests completed!" << endl;
    cout << "=====================================" << endl;
    
    return 0;

}
