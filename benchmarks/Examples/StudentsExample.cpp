#include <iostream>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <functional>
#include <regex>
#include <chrono>
#include <ctime>
#include <ratio>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include<fstream>
#include <locale>
#include <Flow.hpp>
#include <unordered_map>
#include <ctype.h>

using namespace PpFf;

// Nom d'un department.
typedef std::string Dept;

class Student
{
public:
    std::string name;
    Dept department;
    std::string average;
    
    Student( std::string _name, Dept _department, std::string _average ) {
        name = _name;
        department = _department;
        average = _average;
    }
};

// Conteneur pour les etudiants d'un meme department.
typedef std::vector<Student> Students;

// Conteneur pour regrouper les etudiants par department.
typedef std::unordered_map<Dept, Students> Students_by_Dept;

int main(int argc, char* argv[]) {
    Students students;
    students.push_back( Student( "S1", "Informatique", "A" ) );
    students.push_back( Student( "S2", "Informatique", "B" ) );
    students.push_back( Student( "S3", "Mathematiques", "A" ) );
    students.push_back( Student( "S4", "Mathematiques", "B" ) );
    students.push_back( Student( "S5", "Mathematiques", "A" ) );
    students.push_back( Student( "S6", "Biologie", "A" ) );
    students.push_back( Student( "S7", "Biologie", "C"  ) );
                        
    Students_by_Dept result =
        Flow
        ::source<Student>(students.begin(), students.end())
        .find<Student>( [](auto s) { return s->average == "A"; } )
        .groupByKey<Student, Dept, Student>(
           [](auto s) { return &(s->department); } );


    for ( auto student: result ) {
        std::cout << student.first << "=[";
        for( auto s: student.second ) {
            std::cout << s.name << " [" << s.department << "]: " << s.average << ", ";
        }
        std::cout << "]" << std::endl;
    }


    return 0;
}
