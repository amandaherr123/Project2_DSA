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

using namespace std;

// STRUCT
struct Student {
    float gpa;
    int age;
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

    while (getline(file, line)) {
        stringstream ss(line);
        string temp;
        Student s;

        getline(ss, temp, ','); s.gpa = stof(temp);
        getline(ss, temp, ','); s.age = stoi(temp);
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

    //match bonus/penalty
    if (a.major != b.major) dist += 5;
    if (a.universityType != b.universityType) dist += 3;

    return sqrt(dist);
}

// SALARY ESTIMATION
float estimateSalary(vector<Student>& students, Student& input, int k = 5) {

    vector<pair<float, float>> distances; // (distance, salary)

    for (auto& s : students) {
        float dist = calculateDistance(s, input);
        distances.push_back({dist, s.salary});
    }

    // Sort by distance (closest first)
    sort(distances.begin(), distances.end());

    // Take average of top K
    float sum = 0;
    for (int i = 0; i < k; i++) {
        sum += distances[i].second;
    }

    return sum / k;
}

// Replace these with teammates implementations
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

// MAIN
int main() {

    string filename = "students.csv"; // <-- change to file name
    vector<Student> students = loadCSV(filename);

    if (students.empty()) {
        cout << "Dataset is empty.\n";
        return 0;
    }

    cout << "Loaded " << students.size() << " students.\n";

    //USER INPUT
    Student input;
    cout << "\nEnter your GPA: ";
    cin >> input.gpa;

    cout << "Enter your Age: ";
    cin >> input.age;

    cout << "Enter your Major: ";
    cin.ignore();
    getline(cin, input.major);

    cout << "Enter University Type: ";
    getline(cin, input.universityType);

    // ESTIMATION
    float estimatedSalary = estimateSalary(students, input);

    cout << "\nEstimated Salary: $" << estimatedSalary << endl;


    return 0;
}