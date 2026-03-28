#include <iostream>
#include <vector>
#include <algorithm>
#include "heap_sort.h"
using namespace std;

void heapify(vector<pair<float, float>>& arr, int n, int i){
    int largest = i;
    int left = 2*i +1;
    int right = 2*i + 2;

    if(left < n && arr[left].first > arr[largest].first)
    largest = left;

    if (right < n && arr[right].first > arr[largest].first)
    largest = right;

    if(largest !=i){
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);

    }
}

void heapSort(vector<pair<float, float>>& arr){
    int n = arr.size();
    //build heap
    for(int i = n / 2 - 1; i >= 0; i--)
    heapify(arr, n, i);
    // extract elements from heap
    for(int i = n-1; i > 0; i--){

        //move curr to end
        swap(arr[0], arr[i]);

        //heapify 
        heapify(arr,i,0);

    }

}

