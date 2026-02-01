#include <iostream>
#include <vector>
#include <string>

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

int main(){
    vector<int> arr = {
        39, 12, 5, 18, 7, 33, 2, 29, 14, 1,
        26, 10, 31, 6, 22, 17, 9, 25, 8, 30,
        13, 21, 4, 28, 16, 11, 35, 3, 24, 19,
        27, 15, 37, 0, 23, 34, 20, 38, 32, 36
    };

    hybridSort(arr);
    for(int i=0; i<arr.size();i++){
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
