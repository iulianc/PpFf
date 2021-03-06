
struct Employee {
public:
    int age;
    std::string name;
    int salary;
    std::string job_title;
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

    Employee(int _age, std::string _name, int _salary, std::string _job_title) {
        age = _age;
        name = _name;
        salary = _salary;
        job_title = _job_title;
    };

    Employee(int _age, std::string _name, int _salary, std::vector<std::string> _languages) {
        age = _age;
        name = _name;
        salary = _salary;
        languages = _languages;
    };
};
