
struct Employee {
public:
    int age;
    std::string name;
    int salary;
    std::string toString() { return name; };
    std::vector<std::string> languages;

    Employee() {
        age = 0;
        name = "";
        salary = 0;
    };

    Employee(std::string _name) {
        age = 0;
        name = _name;
        salary = 0;
    };   

    Employee(int _age, std::string _name, int _salary) {
        age = _age;
        name = _name;
        salary = _salary;
    };   
};
