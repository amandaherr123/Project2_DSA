//
// Created by jupal on 3/27/2026.
//
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <chrono>
#include "heap_sort.h"
#include "merge_sort.h"

using namespace std;

// STRUCT
struct Student {
    float gpa;
    int age;
    int internships;
    string major;
    string universityType;
    float salary;
};

// CSV LOADER
vector<Student> loadCSV(const string& filename) {
    vector<Student> students;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error opening file.\n";
        return students;
    }

    

    string line;
    getline(file, line);

    getline(file,line);
    cout<< "first data line: " << line << endl; //<-- DELETE

    while (getline(file, line)) {
        stringstream ss(line);
        string temp;
        Student s;

        getline(ss, temp, ','); s.gpa = stof(temp);
        getline(ss, temp, ','); s.age = stoi(temp);
        getline(ss, temp, ','); s.internships = stoi(temp);
        getline(ss, s.major, ',');
        getline(ss, s.universityType, ',');
        getline(ss, temp, ','); s.salary = stof(temp);

        students.push_back(s);
    }

    file.close();
    return students;
}


// SIMILARITY FUNCTION
float calculateDistance(const Student& a, const Student& b) {
    float dist = 0;

    dist += pow(a.gpa - b.gpa, 2);
    dist += pow(a.age - b.age, 2);
    dist += pow(a.internships - b.internships, 2);


    //match bonus/penalty
    if (a.major != b.major) dist += 5;
    if (a.universityType != b.universityType) dist += 3;
    if (a.internships != b.internships) dist += 4; 

    return sqrt(dist);
}


// SALARY ESTIMATION
float estimateSalaryMerge(vector<Student>& students, Student& input, int k = 5) {

    vector<pair<float, float>> distances; // (distance, salary)

    for (auto& s : students) {
        float dist = calculateDistance(s, input);
        distances.push_back({dist, s.salary});
    }

    // Sort by distance (closest first)
    merge_sort(distances,0,distances.size()-1);

    // Take average of top K
    float sum = 0;
    for (int i = 0; i < k; i++) {
        sum += distances[i].second;
    }

    return sum / k;
}

//Same function, but sorts similarity scores using heap
float estimateSalaryHeap(vector<Student>& students, Student& input, int k = 5) {

    vector<pair<float, float>> distances; // (distance, salary)

    for (auto& s : students) {
        float dist = calculateDistance(s, input);
        distances.push_back({dist, s.salary});
    }

    // Sort by distance (closest first)
    heapSort(distances);

    // Take average of top K
    float sum = 0;
    for (int i = 0; i < k; i++) {
        sum += distances[i].second;
    }

    return sum / k;
}


// Replace these with teammates implementations
/* saving just in case, but both implementations are complete - Luke
void mergeSortWrapper(vector<Student>& students) {
    sort(students.begin(), students.end(), [](Student a, Student b) {
        return a.salary < b.salary;
    });
}

void heapSortWrapper(vector<Student>& students) {
    sort(students.begin(), students.end(), [](Student a, Student b) {
        return a.salary < b.salary;
    });
}
*/


// MAIN
int main() {
     //DELETE COLUMNS IN DATASET
    string filename = "../data/student_placement_prediction_dataset_2026.csv"; // <-- change to file name
    vector<Student> students = loadCSV(filename);

    if (students.empty()) {
        cout << "Dataset is empty.\n";
        return 0;
    }

    cout << "Loaded " << students.size() << " students.\n";

    //USER INPUT
    Student input;
    cout << "\nEnter your GPA (Out of 10): ";
    cin >> input.gpa;

    cout << "Enter your Age: ";
    cin >> input.age;

    cout << "Enter your internship count: ";
    cin >> input.internships;

    cout << "Enter your Major (CSE, IT, CE, etc.): ";
    cin.ignore();
    getline(cin, input.major);

    cout << "Enter University Tier (T1-T3): ";
    getline(cin, input.universityType);

    // ESTIMATION
    float estimatedSalaryMerged = estimateSalaryMerge(students, input);
    float estimatedSalaryHeap = estimateSalaryHeap(students, input);
    cout << "\nEstimated Salary with Merge Sort: $" << estimatedSalaryMerged << endl;
    cout << "Estimated Salary with Heap Sort: $" << estimatedSalaryHeap << endl;

    return 0;
}