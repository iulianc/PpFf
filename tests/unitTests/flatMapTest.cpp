
#include "catch.hpp"
#include <string>
#include <vector>
#include "../../src/pp/Pipe.hpp"
#include "Employee.hpp"
#include "utility.hpp"

typedef std::vector< std::string > LANGUAGES;
LANGUAGES GetLanguages(Employee employee){
	std::vector< std::string > *languages = new std::vector< std::string >;
	*languages = employee.languages;

	//return languages;
	return employee.languages;
}

TEST_CASE( "FlatCollectionTypeVector", "MapOperator" ) {
	std::vector< int > elems_collection1(3);
	std::vector< int > elems_collection2(3);
	std::vector< int > elems_collection3(3);
	std::vector< std::vector<int> > collection = {elems_collection1, elems_collection2, elems_collection3};
	unsigned int noElems = 3;
	std::vector< int > expectedResult(9);

	expectedResult = {0,3,6,1,4,7,2,5,8};
	std::vector< int > currentResult;

	for(unsigned int i = 0; i < collection.size(); i++){
		for(unsigned int j = 0; j < collection[i].size(); j++){
			collection[i][j] = i + j*3;
		}
	};

	typedef std::vector< int > vec_type;

	pp::Pipe pipe;
	currentResult = pipe.source< vec_type >(collection.begin(), collection.end())
					.flatMap< vec_type, int >()
					.collect< int, std::vector >();

//	for(unsigned int i = 0; i < currentResult.size(); i++){
//		std::cout <<"ElementFlatMap: "<< currentResult[i]<<"\n";
//	};

	for(unsigned int i = 0; i < expectedResult.size(); i++){
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}

TEST_CASE( "FlatCollectionTypeDeque", "MapOperator" ) {
	std::deque< int > elems_collection1(3);
	std::deque< int > elems_collection2(3);
	std::deque< int > elems_collection3(3);
	std::deque< std::deque<int> > collection = {elems_collection1, elems_collection2, elems_collection3};
	unsigned int noElems = 3;
	std::vector< int > expectedResult(9);

	expectedResult = {0,3,6,1,4,7,2,5,8};
	std::vector< int > currentResult;

	for(unsigned int i = 0; i < collection.size(); i++){
		for(unsigned int j = 0; j < collection[i].size(); j++){
			collection[i][j] = i + j*3;
		}
	};

	typedef std::deque< int > deque_type;

	pp::Pipe pipe;
	currentResult = pipe.source< deque_type >(collection.begin(), collection.end())
					.flatMap< deque_type, int >()
					.collect< int, std::vector >();

	for(unsigned int i = 0; i < expectedResult.size(); i++){
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}

TEST_CASE( "FlatCollectionParallel", "MapOperator" ) {
	std::vector< int > elems_collection1(3);
	std::vector< int > elems_collection2(3);
	std::vector< int > elems_collection3(3);
	std::vector< std::vector<int> > collection = {elems_collection1, elems_collection2, elems_collection3};
	unsigned int noElems = 3;
	std::vector< int > expectedResult(9);

	expectedResult = {0,1,2,3,4,5,6,7,8};
	std::vector< int > currentResult;

	for(unsigned int i = 0; i < collection.size(); i++){
		for(unsigned int j = 0; j < collection[i].size(); j++){
			collection[i][j] = i + j*3;
		}
	};
	typedef std::vector< int > vec_type;

	pp::Pipe pipe;
	currentResult = pipe.source< vec_type >(collection.begin(), collection.end())
					.parallel(4)
					.flatMap< vec_type, int >()
					.collect< int, std::vector >();

	std::sort(currentResult.begin(), currentResult.end());

	for(unsigned int i = 0; i < expectedResult.size(); i++){
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}

TEST_CASE( "FlatCollectionApplyingFunction", "MapOperator" ) {
	std::vector< Employee > employees(3);
	std::vector< std::string > expectedResult(10);

	expectedResult = {"French", "English", "Chinese", "Arabic", "French", "Spanish", "Portuguese"};
	std::vector< std::string > currentResult;

	for(unsigned int i = 0; i < employees.size(); i++){
		Employee employee;
		employee.name = "Employee" + ConvertNumberToString(i);
		employees[i] = employee;
	};

	employees[0].languages = {"French", "English"};
	employees[1].languages = {"Chinese", "Arabic", "French"};
	employees[2].languages = {"Spanish", "Portuguese"};

	typedef LANGUAGES (*mapF)(Employee);

	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(employees.begin(), employees.end())
					.flatMap< Employee, std::string, LANGUAGES, mapF >(GetLanguages)
					.collect< std::string, std::vector >();

//	for(unsigned int i = 0; i < expectedResult.size(); i++){
//		std::cout<<currentResult[i]<<std::endl;
//	};

	for(unsigned int i = 0; i < currentResult.size(); i++){
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}

TEST_CASE( "FlatCollectionApplyingLambdaFunctionParallel", "MapOperator" ) {
	std::vector< Employee > employees(3);
	std::vector< std::string > expectedResult(10);

	expectedResult = {"Arabic", "Chinese", "English", "French", "French", "Portuguese", "Spanish" };
	std::vector< std::string > currentResult;

	for(unsigned int i = 0; i < employees.size(); i++){
		Employee employee;
		employee.name = "Employee" + ConvertNumberToString(i);
		employees[i] = employee;
	};

	employees[0].languages = {"French", "English"};
	employees[1].languages = {"Chinese", "Arabic", "French"};
	employees[2].languages = {"Spanish", "Portuguese"};


	typedef LANGUAGES (*mapF)(Employee);

	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(employees.begin(), employees.end())
					.parallel(4)
					.flatMap< Employee, std::string, LANGUAGES, mapF >(([](Employee empl){ return empl.languages;}))
					.collect< std::string, std::vector >();


	std::sort(currentResult.begin(), currentResult.end());
	for(unsigned int i = 0; i < expectedResult.size(); i++){
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}


