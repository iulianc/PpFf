#include "catch.hpp"
#include "../../src/Flow.hpp"
#include <string>
#include <vector>
#include "Employee.hpp"
#include "Student.hpp"
#include "utility.hpp"
#include <algorithm>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <ff/ff.hpp>

using namespace ff;

//unordered_map
TEST_CASE( "GroupByDepStudentsAverageFF", "GroupByKeyOperator" ) {
    typedef std::vector<Student> VALUE;
    typedef std::unordered_map<std::string, VALUE> CONTAINER;

    unsigned int noStudents = 10;
    VALUE students;
    for (unsigned int i = 0; i < noStudents; i++) {
        Student student("Student" + ConvertNumberToString(i),
                          "B",
                          "IT",
							"Montreal");
        students.push_back(student);
    };


    students[0].average = students[1].average = students[2].average = "A";
    students[3].average = students[4].average = "A";
    students[5].average = students[6].average = "B+";
    students[7].average = students[8].average = "A";
    students[9].average = "C";

    students[0].departement = students[1].departement = students[2].departement = "IT";
    students[3].departement = students[4].departement = "Accounting";
    students[5].departement = students[6].departement = "Chemistry";
    students[7].departement = students[8].departement = "Art";
    students[9].departement = "History";


    CONTAINER expectedResult =
        { 
			{"IT", {students[0], students[1], students[2]}},
          	{"Accounting", {students[3], students[4]}},
          	{"Art", {students[7], students[8]}}
        };

	struct printElementsFlow : ff_node_t<Student, void> {
		void* svc(Student* task) {
			std::cout << task->name << "=>" << task->average << std::endl;
			return GO_ON;
		}
	};

	struct source : ff_node {
		VALUE students;
		source(VALUE students) : students(students){}

		void* svc(void*) {
			for (auto &elem: students) {
				ff_send_out(&elem);
			}	

			return EOS;	
		}
	};

	struct findStudentsAverage : ff_node_t<Student, Student> {
		Student* svc(Student* task) {
			if(task->average == "A") {
				return task;
			}

			return GO_ON;
		}
	};


	struct groupByDepartement : ff_node_t<Student, void> {
		CONTAINER& container;
		groupByDepartement(CONTAINER& container) : container(container){}
		
		void* svc(Student* task) {
			auto mapIt = container.find(task->departement);
              
			if (mapIt != container.end()) {
				mapIt->second.push_back(*task);
			} else {
				VALUE students = {*task};
				container[task->departement] = students;
			}

			return GO_ON;		
		}
	};

	CONTAINER result;
	ff_Pipe<> ffp( new source(students),
					new findStudentsAverage,
					new groupByDepartement(result) );


	if (ffp.run_and_wait_end() < 0) 
		error("running pipe");

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        VALUE resultValue = result[it->first];
        VALUE expectedResultValue = it->second;

        REQUIRE(resultValue.size() == expectedResultValue.size());
        for (unsigned int i = 0; i < expectedResultValue.size(); i++) {
            REQUIRE(resultValue[i].name == expectedResultValue[i].name);
        }
    }
}



TEST_CASE( "GroupByDepStudentsCityFF", "GroupByKeyOperator" ) {
    typedef std::vector<Student> VALUE;
    typedef std::unordered_map<std::string, VALUE> CONTAINER;

    unsigned int noStudents = 10;
    VALUE students;
    for (unsigned int i = 0; i < noStudents; i++) {
        Student student("Student" + ConvertNumberToString(i),
                          "B",
                          "IT",
							"Montreal");
        students.push_back(student);
    };


    students[0].average = students[1].average = students[2].average = "A";
    students[3].average = students[4].average = "A";
    students[5].average = students[6].average = "B+";
    students[7].average = students[8].average = "A";
    students[9].average = "C";

    students[0].departement = students[1].departement = students[2].departement = "IT";
    students[3].departement = students[4].departement = "Accounting";
    students[5].departement = students[6].departement = "Chemistry";
    students[7].departement = students[8].departement = "Art";
    students[9].departement = "History";

    students[0].city_address = students[1].city_address = students[2].city_address = "Montreal";
    students[3].city_address = students[4].city_address = "Quebec";
    students[5].city_address = students[6].city_address = "Longueuil";
    students[7].city_address = students[8].city_address = "Montreal";
    students[9].city_address = "Saskatchewan";


    CONTAINER expectedResult =
        { 
			{"IT", {students[0], students[1], students[2]}},
          	{"Art", {students[7], students[8]}}
        };

	struct printElementsFlow : ff_node_t<Student, void> {
		void* svc(Student* task) {
			std::cout << task->name << "=>" << task->average << std::endl;
			return GO_ON;
		}
	};

	struct source : ff_node {
		VALUE students;
		source(VALUE students) : students(students){}

		void* svc(void*) {
			for (auto &elem: students) {
				ff_send_out(&elem);
			}	

			return EOS;	
		}
	};

	struct findStudentsCity : ff_node_t<Student, Student> {
		Student* svc(Student* task) {
			if(task->city_address == "Montreal") {
				return task;
			}

			return GO_ON;
		}
	};

	struct groupByDepartement : ff_node_t<Student, void> {
		CONTAINER& container;
		groupByDepartement(CONTAINER& container) : container(container){}
		
		void* svc(Student* task) {
			auto mapIt = container.find(task->departement);
              
			if (mapIt != container.end()) {
				mapIt->second.push_back(*task);
			} else {
				VALUE students = {*task};
				container[task->departement] = students;
			}

			return GO_ON;		
		}
	};

	CONTAINER result;
	ff_Pipe<> ffp( new source(students),
					new findStudentsCity,
					new groupByDepartement(result) );


	if (ffp.run_and_wait_end() < 0) 
		error("running pipe");

/*
	for (auto it = result.begin(); it != result.end(); it++) {
    	std::string currentResultKey = it->first;
		VALUE currentResultValue = it->second;

		std::cout << currentResultKey << " => ";
		for (unsigned int i = 0; i < currentResultValue.size(); i++) {
			std::cout << currentResultValue[i].name << "; ";
		}

		std::cout << std::endl;
	}

*/

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        VALUE resultValue = result[it->first];
        VALUE expectedResultValue = it->second;

        REQUIRE(resultValue.size() == expectedResultValue.size());
        for (unsigned int i = 0; i < expectedResultValue.size(); i++) {
            REQUIRE(resultValue[i].name == expectedResultValue[i].name);
        }
    }
}



TEST_CASE( "GroupByDepStudentsAverageParallelFF", "GroupByKeyOperator" ) {
    typedef std::vector<Student> VALUE;
    typedef std::unordered_map<std::string, VALUE> CONTAINER;

    unsigned int noStudents = 10;
    VALUE students;
    for (unsigned int i = 0; i < noStudents; i++) {
        Student student("Student" + ConvertNumberToString(i),
                          "B",
                          "IT",
							"Montreal");
        students.push_back(student);
    };


    students[0].average = students[1].average = students[2].average = "A";
    students[3].average = students[4].average = "A";
    students[5].average = students[6].average = "B+";
    students[7].average = students[8].average = "A";
    students[9].average = "C";

    students[0].departement = students[1].departement = students[2].departement = "IT";
    students[3].departement = students[4].departement = "Accounting";
    students[5].departement = students[6].departement = "Chemistry";
    students[7].departement = students[8].departement = "Art";
    students[9].departement = "History";


    CONTAINER expectedResult =
        { 
			{"IT", {students[0], students[1], students[2]}},
          	{"Accounting", {students[3], students[4]}},
          	{"Art", {students[7], students[8]}}
        };

	struct printElementsFlow : ff_node_t<Student, void> {
		void* svc(Student* task) {
			std::cout << task->name << "=>" << task->average << std::endl;
			return GO_ON;
		}
	};

	struct source : ff_node {
		VALUE students;
		source(VALUE students) : students(students){}

		void* svc(void*) {
			for (auto &elem: students) {
				ff_send_out(&elem);
			}	

			return EOS;	
		}
	};

	struct findStudentsAverage : ff_node_t<Student, Student> {
		Student* svc(Student* task) {
			if(task->average == "A") {
				return task;
			}

			return GO_ON;
		}
	};


	struct groupByDepartement : ff_node_t<Student, void> {
		CONTAINER& container;
		groupByDepartement(CONTAINER& container) : container(container){}
		
		void* svc(Student* task) {
			auto mapIt = container.find(task->departement);
              
			if (mapIt != container.end()) {
				mapIt->second.push_back(*task);
			} else {
				VALUE students = {*task};
				container[task->departement] = students;
			}

			return GO_ON;		
		}
	};

	std::vector<ff_node*> workers;
	for ( uint32_t i = 0; i < 4; i++ ) {
		workers.push_back( new ff_Pipe<>(new findStudentsAverage) );
	}

	CONTAINER result;
	ff_Pipe<> ffp( new source(students),
					new ff_farm(workers),
					new groupByDepartement(result) );


	if (ffp.run_and_wait_end() < 0) 
		error("running pipe");

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        VALUE resultValue = result[it->first];
        VALUE expectedResultValue = it->second;

		std::sort(resultValue.begin(), resultValue.end(), [](Student s1, Student s2)->bool {return s1.name < s2.name; } );

        REQUIRE(resultValue.size() == expectedResultValue.size());
        for (unsigned int i = 0; i < expectedResultValue.size(); i++) {
            REQUIRE(resultValue[i].name == expectedResultValue[i].name);
        }
    }
}

