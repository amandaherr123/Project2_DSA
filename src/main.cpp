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
    int linkedin_connections;
    float leadership_score;
    bool volunteer=false;
    float salary;
    bool hasSalary=false;
};

// CSV LOADER AND PARSER
vector<Student> loadCSV(const string& filename) {
    vector<Student> students;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error opening file.\n";
        return students;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string temp;
        Student s;
        getline(ss,temp,','); //ignore student ID

        getline(ss, temp, ','); s.gpa = stof(temp);
        getline(ss, temp, ','); s.age = stoi(temp);
        getline(ss, temp, ','); s.internships = stoi(temp);
        getline(ss, s.major, ',');
        getline(ss,temp,','); s.linkedin_connections = stoi(temp);
        getline(ss,temp,','); s.leadership_score = stof(temp);
        getline(ss, temp,',');
        if (temp=="Yes") {
            s.volunteer = true;
        }
        else {
            s.volunteer = false;
        }
        getline(ss, temp, ',');
        if (temp=="") {
            s.hasSalary = false;
        }
        else {
            s.hasSalary = true;
            s.salary = stof(temp);
        }
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
    dist += pow((a.linkedin_connections - b.linkedin_connections)/50.0, 2);
    dist += pow((a.leadership_score - b.leadership_score)/10.0, 2);

    //match bonus/penalty
    if (a.major != b.major) dist += 5;
    if (a.volunteer!=b.volunteer) dist += 3;
    return sqrt(dist);
}

// SALARY ESTIMATION
float estimateSalaryMerge(vector<Student>& students, Student& input, int k = 10) {

    vector<pair<float, float>> distances; // (distance, salary)

    for (int i=0; i<students.size(); i++) {
        float dist = calculateDistance(students[i], input);
        distances.push_back({dist,(float)i});
    }
    // Sort by distance (closest first)
    merge_sort(distances,0,distances.size()-1);

    // Take average of top K THAT HAVE A SALARY
    float sum = 0;
    int students_used=0;

    for (int i=0; i<distances.size() && students_used<k; i++) {
        int index=(int)distances[i].second;
        if (students[index].hasSalary) {
            sum+=students[index].salary;
            students_used++;
        }
    }
    if (students_used==0) {
        return 0;
    }
    return sum / students_used;
}

//Same function, but sorts similarity scores using heap
float estimateSalaryHeap(vector<Student>& students, Student& input, int k = 10) {

    vector<pair<float, float>> distances; // (distance, salary)

    for (int i=0; i<students.size(); i++) {
        float dist = calculateDistance(students[i], input);
        distances.push_back({dist,(float)i});
    }
    // Sort by distance (closest first)
    heapSort(distances);

    // Take average of top K THAT HAVE A SALARY
    float sum = 0;
    int students_used=0;

    for (int i=0; i<distances.size() && students_used<k; i++) {
        int index=(int)distances[i].second;
        if (students[index].hasSalary) {
            sum+=students[index].salary;
            students_used++;
        }
    }
    if (students_used==0) {
        return 0;
    }
    return sum / students_used;
}

float lakhsToUSD(float lakhs) {
    return lakhs*1054.14; //curent lakh to usd ratio (lakh is 100,000 rupees)
}

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
    string volunteerInput;

    cout << "\nEnter your GPA (Out of 10): ";
    cin >> input.gpa;

    cout << "Enter your Age: ";
    cin >> input.age;

    cout << "Enter your internship count: ";
    cin >> input.internships;

    cout << "Enter your Major (CSE, IT, CE, etc.): ";
    cin.ignore();
    getline(cin, input.major);

    cout << "Enter number of LinkedIn connections: ";
    cin >> input.linkedin_connections;

    cout << "Enter estimated leadeship score (0-100)";
    cin >> input.leadership_score;

    cout << "Volunteer experience? (Yes or No): ";
    cin >> volunteerInput;
    if (volunteerInput=="Yes") {
        input.volunteer = true;
    }
    else {
        input.volunteer = false;
    }

    // SALARY ESTIMATION AND TIMING
    auto start_time_Merge=chrono::high_resolution_clock::now();
    float estimatedSalaryMerged = estimateSalaryMerge(students, input);
    auto end_time_Merge=chrono::high_resolution_clock::now();
    auto time_merge=chrono::duration_cast<chrono::microseconds>(end_time_Merge - start_time_Merge);

    auto start_time_Heap=chrono::high_resolution_clock::now();
    float estimatedSalaryHeap = estimateSalaryHeap(students, input);
    auto end_time_Heap=chrono::high_resolution_clock::now();
    auto time_heap=chrono::duration_cast<chrono::microseconds>(end_time_Heap - start_time_Heap);

    cout << "\nEstimated salary with merge sort: "<< lakhsToUSD(estimatedSalaryMerged) << endl;
    cout << "Time Taken: " << time_merge.count() << " microseconds" << endl;

    cout << "\nEstimated salary with heap sort: "<< lakhsToUSD(estimatedSalaryHeap) << endl;
    cout << "Time Taken: " << time_heap.count() << " microseconds" << endl;
    return 0;
}