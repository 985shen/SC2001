#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <iomanip>
#include <climits>
#include <chrono>

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

double secondsNow(){
    using clock = chrono::high_resolution_clock;
    return chrono::duration<double>(clock::now().time_since_epoch()).count();
}

void saveResults(const string& filename, const vector<int>& inputs, 
                         const vector<long long>& comparisons, 
                         const vector<double>& times, const string& header) {
    ofstream file(filename);
    file << header << "\n";
    for (size_t i = 0; i < inputs.size(); i++) {
        file << inputs[i] << "," << comparisons[i] << "," << fixed << setprecision(6) << times[i] << "\n";
    }
    file.close();
    cout << "Results saved to " << filename << endl;
}

void testPartCI() {
    // fixed S, vary n
    cout << "\n=== Part (Ci): Fixed S=32, varying n ===" << endl;
    
    vector<int> sizes = {1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int maxValue = 10000000;

    vector<int> inputSizes;
    vector<long long> compCounts;
    vector<double> cpuTimes;
    
    cout << left << setw(15) << "Input n" << setw(20) << "Comparisons" << "CPU Time (s)" << endl;
    cout << string(60, '-') << endl;
    
    for (int size : sizes) {
        vector<int> arr = generateRandomArray(size, maxValue);
        
        // measure CPU time
        double startTime = secondsNow();
        hybridSort(arr);
        double endTime = secondsNow();
        double cpuTime = endTime - startTime;
        
        cout << setw(15) << size << setw(20) << comparisonCount << fixed << setprecision(6) << cpuTime << endl;
        
        inputSizes.push_back(size);
        compCounts.push_back(comparisonCount);
        cpuTimes.push_back(cpuTime);
    }
    
    saveResults("results_ci.csv", inputSizes, compCounts, cpuTimes, "Input n,Comparisons,CPU Time (s)");
}

void testPartCII() {
    // fixed n, vary S
    cout << "\n=== Part (cii): Fixed n=1000000, varying S ===" << endl;
    
    vector<int> sValues = {10, 20, 30, 40, 50, 60 ,70 ,80, 90, 100};
    int fixedSize = 1000000;
    int maxValue = 10000000;

    vector<int> sVals;
    vector<long long> compCounts;
    vector<double> cpuTimes;
    
    cout << left << setw(15) << "S value" << setw(20) << "Comparisons" << "CPU Time (s)" << endl;
    cout << string(60, '-') << endl;
    
    for (int s : sValues) {
        vector<int> arr = generateRandomArray(fixedSize, maxValue);
        
        // measure CPU time
        double startTime = secondsNow();
        hybridSort(arr, s);
        double endTime = secondsNow();
        double cpuTime = endTime - startTime;
        
        cout << setw(15) << s << setw(20) << comparisonCount << fixed << setprecision(6) << cpuTime << endl;
        
        sVals.push_back(s);
        compCounts.push_back(comparisonCount);
        cpuTimes.push_back(cpuTime);
    }
    
    saveResults("results_cii.csv", sVals, compCounts, cpuTimes, "S value,Comparisons,CPU Time (s)");
}

void testPartCIII() {
    // vary both S and n
    cout << "\n=== Part (ciii): Finding optimal value of S ===" << endl;
    
    vector<int> sizes = {10000, 100000, 1000000, 10000000};
    vector<int> sValues = {10, 20, 30, 40, 50, 60 ,70 ,80, 90, 100};
    int maxValue = 10000000;
    
    ofstream file("results_ciii.csv");

    file << "Input n";
    for (int s : sValues) {
        file << ", S= " << s << " Comparisons, CPU Time (s)" << s;
    }
    file << "\n";
    
    for (int size : sizes) {
        cout << "\nTesting size: " << size << endl;
        file << size;
        
        long long minComparisons = LLONG_MAX;
        int optimalS = 0;
        
        for (int s : sValues) {
            vector<int> arr = generateRandomArray(size, maxValue);
            
            // measure CPU time
            double startTime = secondsNow();
            hybridSort(arr, s);
            double endTime = secondsNow();
            double cpuTime = endTime - startTime;
            
            // save number of comparisons and CPU time in seconds
            file << "," << comparisonCount << "," << fixed << setprecision(6) << cpuTime;
            
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


// -------------------- Original Mergesort (Part D) --------------------

void mergeSortImpl(vector<int>& arr, vector<int>& temp, int left, int right){
    int diff = right - left + 1;
    if (diff <= 1) return;

    int mid = left + (right - left) / 2;
    mergeSortImpl(arr, temp, left, mid);
    mergeSortImpl(arr, temp, mid + 1, right);
    merge(arr, temp, left, mid, right); // uses comparisonCount
}

void mergeSort(vector<int>& arr){
    if (arr.empty()) return;
    comparisonCount = 0;
    vector<int> temp(arr.size());
    mergeSortImpl(arr, temp, 0, static_cast<int>(arr.size()) - 1);
}

// -------------------- Part (d): Hybrid vs Original Mergesort --------------------

void testPartCIV(int optimalS){
    cout << "\n=== Part (d): Hybrid vs Original Mergesort on n=10000000 ===" << endl;
    cout << "Using S=" << optimalS << " for HybridSort" << endl;

    int n = 10000000;
    int maxValue = 10000000;

    // using same dataset for both algorithms
    vector<int> base = generateRandomArray(n, maxValue);
    vector<int> arrHybrid = base;
    vector<int> arrMerge = base;

    // Hybrid sort timing + comparisons
    double t0 = secondsNow();
    hybridSort(arrHybrid, optimalS);
    double t1 = secondsNow();
    long long hybridComps = comparisonCount;
    double hybridTime = t1 - t0;

    // Original mergesort timing + comparisons
    double t2 = secondsNow();
    mergeSort(arrMerge);
    double t3 = secondsNow();
    long long mergeComps = comparisonCount;
    double mergeTime = t3 - t2;

    cout << left << setw(20) << "Algorithm" << setw(20) << "Comparisons" << "CPU Time (s)" << endl;
    cout << string(60, '-') << endl;
    cout << left << setw(20) << "HybridSort" << setw(20) << hybridComps << fixed << setprecision(6) << hybridTime << endl;
    cout << left << setw(20) << "MergeSort"  << setw(20) << mergeComps  << fixed << setprecision(6) << mergeTime << endl;

    ofstream file("results_d.csv");
    file << "Algorithm,S,Comparisons,CPU Time (s)\n";
    file << "HybridSort," << optimalS << "," << hybridComps << "," << fixed << setprecision(6) << hybridTime << "\n";
    file << "MergeSort,0," << mergeComps << "," << fixed << setprecision(6) << mergeTime << "\n";
    file.close();
    cout << "Results saved to results_d.csv" << endl;
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
    cout << "Testing part d" << endl;
    int optimalS = 10;
    testPartCIV(optimalS);
    cout << "\nPart d test complete\n" << endl;
    
    cout << "\n=====================================" << endl;
    cout << "All tests completed!" << endl;
    cout << "=====================================" << endl;
    return 0;
}