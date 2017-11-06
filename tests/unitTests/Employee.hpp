
struct Employee {
public:
	int age;
	std::string name;
	int salary;
	std::string toString(){return name;};
	std::vector< std::string > languages;

	Employee(){
		age = 0;
		name = "";
		salary = 0;
	}
};
