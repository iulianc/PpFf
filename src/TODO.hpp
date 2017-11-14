

/*
 *
 * 1.Allow pointer for source collection
 * std::vector< Employee* >
 *
 *	Solurion:
 *	a. https://stackoverflow.com/questions/580922/identifying-primitive-types-in-templates
 *	Identifying primitive types in templates

 *	std::is_fundamental<T>::value
 *
 *
 *
 *
 * 2.Eliminate function type parameter
 * template < typename In, typename Out, typename TaskFunc >
		Pipe& map(TaskFunc taskf)

	Solution: Tesis italian guy
 *
 *
 * 3.Refactoring StageManager.hpp
 * if(!isParallel())
 *
 *
 * 4. Determine if a type is an STL container
   https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time
 * https://stackoverflow.com/questions/16337610/how-to-know-if-a-type-is-a-specialization-of-stdvector
 * https://stackoverflow.com/questions/24284879/type-traits-for-std-container
 * http://www.boost.org/doc/libs/1_62_0/libs/spirit/doc/html/spirit/advanced/customize/is_container.html
 *
 *
 * 5. Refactoring StageManager.hpp - Eliminate Parallel from stageManager with another node from Pipe
 *
 *
 * 6. Define o function #define DBG(X) X
 *
 * 7. Do reduce in parallel
 *
 */



/*
// Example program passing a pointer of pointer as parameter
#include <iostream>
#include <string>

struct Employee {
	int id;
	int age;
	std::string name;
	int salary;
};

class Utility{
    public:

    void getNameEmployee(void* emp){
        Employee **e = (Employee**)emp;

        std::cout<<(*e)->name<<std::endl;
    }
};

int main()
{
	Employee *employee = new Employee;
	employee->age = 20;
	employee->salary = 1200;
	employee->name = "Employee1";

	Employee **ppempl = &employee;

	//std::cout<<(*ppempl)->name;

	Utility u;
	u.getNameEmployee(ppempl);
}
*/


/*
// Example program cloning identity for parallel reduce Operator
#include <iostream>
#include <string>
#include <vector>

struct Employee {
	int id;
	int age;
	std::string name;
	int salary;
};

class Pipe{
    public:

    template < typename In >
    void add_stage(In &identity){
        std::cout<<identity.name<<":"<<identity.salary<<std::endl;
    }

    template < typename In >
    void add_stage_modify(In &identity){
        identity.salary = 3000;
        identity.name = "Employee2";
        std::cout<<identity.name<<":"<<identity.salary<<std::endl;
    }
};

class StageManager{
    public:

	template < typename In >
	void reduce(In &identity){
	    Pipe pipe;

// 		pipe.add_stage(identity);
// 		pipe.add_stage_modify(identity);

	    In *temp = new In(identity);

		pipe.add_stage(*temp);
		pipe.add_stage_modify(*temp);
	}
};

int main()
{

	Employee employee;
	employee.age = 20;
	employee.salary = 1200;
	employee.name = "Employee1";

    StageManager stageManager;

    stageManager.reduce<Employee>(employee);

     std::cout<<"Final: "<<employee.name<<":"<<employee.salary<<std::endl;

}
*/
