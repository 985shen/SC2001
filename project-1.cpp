#include <iostream>
#include <vector>
#include <string>
#include <random>

using namespace std;

constexpr int S = 32;

void insert(vector<int>& arr, int left, int right){
    // assume the first index is already sorted
    for (int i=left+1; i<=right; i++){
        int key = arr[i];
        int j=i-1;
        // keep swappng until key > j
        while (j>= left && key < arr[j]){
            arr[j+1] = arr[j];
            j--;
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

void hybridSortImpl(vector<int>& arr, vector<int>& temp, int left, int right){
    int diff = right - left + 1;
    if (diff <= 1) return;
    if (diff <= S){
        insert(arr, left, right);
        return;
    }
    int mid = left + (right - left) / 2;
    hybridSortImpl(arr, temp, left, mid);
    hybridSortImpl(arr, temp, mid + 1, right);
    merge(arr, temp, left, mid, right);
}

void hybridSort(vector<int>& arr){
    if (arr.empty()) return;
    vector<int> temp(arr.size());
    hybridSortImpl(arr, temp, 0, static_cast<int>(arr.size()) - 1);
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

int main(){
    vector<int> sizes = {
        1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000
    };

    int maxValue = 10000000;

    for (int size : sizes) {
        cout << "Testing array of size: " << size << endl;

        vector<int> arr = generateRandomArray(size, maxValue);

        hybridSort(arr);

        // verify if the arrays are sorted by checking first 20 elements
        cout << "First 20 elements: ";

        for (int i = 0; i < min(20, size); i++) {
            cout << arr[i] << " ";
        }

        cout << "\n" << endl;
    }

    return 0;
}
