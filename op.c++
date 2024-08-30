#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;
class Course {
public:
    string name, code;
    int CreditHours, minStudentLevel;

    Course() : name(""), code(""), CreditHours(0), minStudentLevel(0) {}
    Course(string name, int minStudentLevel, int CreditHours)
        : name(name), CreditHours(CreditHours), minStudentLevel(minStudentLevel) {}

    friend ostream& operator<<(ostream &os, const Course &course) {
        os << "Name: " << course.name.substr(0, course.name.size() - 3) 
           << "\nCode: " << course.name 
           << "\nLevel: " << course.minStudentLevel 
           << "\nCredit Hours: " << course.CreditHours;
        return os;
    }
};

class NormalCourse : public Course {
public:
    NormalCourse(const string& name, int minLevel, int creditHours)
        : Course(name + "_N" + to_string(minLevel), minLevel, creditHours) {}
};

class SummerCourse : public Course {
public:
    SummerCourse(const string& name, int minLevel, int creditHours)
        : Course(name + "_S" + to_string(minLevel), minLevel, creditHours) {}
};

class Student {
public:
    string name;
    int id, noOfCourses, level;
    vector<double> Grades;
    vector<Course*> courseList;

    Student() : id(0), noOfCourses(0), level(0) {}

    virtual ~Student() {
        for (Course* course : courseList) {
            delete course;
        }
    }

    void printRegisteredCourses() const {
        for (size_t i = 0; i < courseList.size(); ++i) {
            if (courseList[i]) {
                cout << *courseList[i] << "\n";
            }
        }
    }

    virtual float CalculateExpenses() const = 0;

    double getTotalGPA() const {
        int totalCreditHours = 0;
        double totalPoints = 0;
        for (size_t i = 0; i < courseList.size(); ++i) {
            if (courseList[i]) {
                double gpa = getCourseGPA(Grades[i]);
                totalPoints += gpa * courseList[i]->CreditHours;
                totalCreditHours += courseList[i]->CreditHours;
            }
        }
        return (totalCreditHours ? totalPoints / totalCreditHours : 0.0);
    }

    double getCourseGPA(double grade) const {
        if (grade >= 93) return 4.0;
        if (grade >= 90) return 3.7;
        if (grade >= 87) return 3.3;
        if (grade >= 83) return 3.0;
        if (grade >= 80) return 2.7;
        if (grade >= 77) return 2.3;
        if (grade >= 73) return 2.0;
        if (grade >= 70) return 1.7;
        if (grade >= 67) return 1.3;
        if (grade >= 63) return 1.0;
        if (grade >= 60) return 0.7;
        return 0.0;
    }

    bool RegisterCourse(Course *course) {
        if (level >= course->minStudentLevel && noOfCourses < 6) {
            courseList.push_back(course);
            ++noOfCourses;
            return true;
        }
        return false;
    }

    friend ostream& operator<<(ostream &os, const Student &student) {
        os << "Student Name: " << student.name
           << "\nID: " << student.id
           << "\nLevel: " << student.level << "\n";
        return os;
    }
};

class SpecialProgramStudent : public Student {
public:
    float CalculateExpenses() const override {
        int totalCreditHours = 0;
        for (const auto& course : courseList) {
            if (course) {
                totalCreditHours += course->CreditHours;
            }
        }
        return totalCreditHours * 500;
    }
};

class GeneralProgramStudent : public Student {
public:
    float CalculateExpenses() const override {
        return 1000 * level;
    }
};

vector<Course*> COURSES = {
    new NormalCourse("Math", 1, 2),
    new NormalCourse("Programming_1", 1, 3),
    new NormalCourse("Technical_Writing", 1, 3),
    new NormalCourse("Electronics", 2, 4),
    new NormalCourse("Data_Structures", 2, 3),
    new NormalCourse("OOP", 2, 3),
    new NormalCourse("Algorithms", 3, 2),
    new NormalCourse("Operating_Systems", 3, 3),
    new NormalCourse("Databases", 3, 3),
    new NormalCourse("Machine_Learning", 4, 3),
    new NormalCourse("Compilers", 4, 4),
    new NormalCourse("Programming_Concepts", 4, 4),
    new SummerCourse("Math", 1, 2),
    new SummerCourse("Compilers", 4, 4),
    new SummerCourse("Algorithms", 3, 2),
    new SummerCourse("OOP", 2, 3)
};

Course* searchCourse(const string& code) {
    auto it = find_if(COURSES.begin(), COURSES.end(), [&code](Course* course) {
        return course->name == code;
    });
    return (it != COURSES.end()) ? *it : nullptr;
}

int main() {
    int totalNumberOfStudents, studentType, grade, noOfCourses;
    string code;

    cout << "Total Number of students: ";
    cin >> totalNumberOfStudents;
    cin.ignore();

    while (totalNumberOfStudents--) {
        cout << "For each student:\n";
        cout << "Enter student type (0: general, 1: special): ";
        cin >> studentType;
        cin.ignore();

        Student* student = nullptr;
        if (studentType == 0) {
            student = new GeneralProgramStudent();
        } else if (studentType == 1) {
            student = new SpecialProgramStudent();
        } else {
            cout << "Invalid student type.\n";
            continue;
        }
        
        cout << "Enter name: ";
        getline(cin, student->name);
        
        cout << "Enter ID: ";
        cin >> student->id;
        cout << "Enter student level: ";
        cin >> student->level;
        

        cout << "Number of courses to be registered: ";
        cin >> noOfCourses;
          student->Grades.resize(noOfCourses);
        cout << "Courses to be registered: \n";
        for (int i = 0; i < noOfCourses; ++i) {
            bool checkRegistration = false;
            cout << "Course " << i + 1 << ": \n";
            cout << "Code: ";
            cin>>code;

            Course* course = searchCourse(code);
            if (course) {
                checkRegistration = student->RegisterCourse(course);
                if (!checkRegistration) {
                    cout << "Error: You cannot register in this course\n";
                } else {
                    cout << "Grade: ";
                    cin >> grade;
                    cin.ignore();
                    student->Grades[student->noOfCourses - 1] = grade;
                }
            } else {
                cout << "No Course with this code\n";
            }
        }

        cout << "\n\n" << *student;
        cout << "\n\nRegistered Courses: \n";
        for (size_t i = 0; i < student->courseList.size(); ++i) {
            cout << "Course " << i + 1 << ": \n";
            if (student->courseList[i]) {
                cout << *student->courseList[i] << "\nGrade: " << student->Grades[i] << "\n";
            }
        }
        cout << "\n\nOverall GPA: " << fixed << setprecision(2) << student->getTotalGPA() << "\n";
        cout << "Student Expenses: " << student->CalculateExpenses() << "\n\n";

        delete student;
    }

    for (Course* course : COURSES) {
        delete course;
    }

    return 0;
}