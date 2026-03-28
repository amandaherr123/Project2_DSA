//
// Created by lrpau on 3/27/2026.
//
#include <vector>
#include "merge_sort.h"
using namespace std;

void merge_halves(vector<pair<float, float>>& arr, int l, int m, int r) {
    int n1 = m-l+1;
    int n2 = r-m;
    vector<pair<float,float>> L(n1);
    vector<pair<float,float>> R(n2);
    for (int i=0; i<n1; i++) {
        L[i] = arr[l+i];
    }
    for (int i=0; i<n2; i++) {
        R[i] = arr[m+1+i];
    }
    int i=0;
    int j=0;
    int k=l;
    while (i<n1&&j<n2) {
        if (L[i].first<R[j].first) {
            arr[k]=L[i];
            i++;
        }
        else {
            arr[k]=R[j];
            j++;
        }
        k++;
    }
    while (i<n1) {
        arr[k]=L[i];
        i++;
        k++;
    }
    while (j<n2) {
        arr[k]=R[j];
        j++;
        k++;
    }
}

void merge_sort(vector<pair<float,float>>& arr, int l, int r) {
    if (l<r) {
        int m =l+(r-l)/2;
        merge_sort(arr, l, m);
        merge_sort(arr, m+1, r);
        merge_halves(arr, l, m, r);
    }
}