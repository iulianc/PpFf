
struct Student {
public:
	std::string name;
	std::string average;
	std::string departement;
	std::string city_address;
	std::string toString() { return name; };


	Student () {
		name = "";
		average = "";
		departement = "";
		city_address = "";
	};

	Student (std::string _name, std::string _average, std::string _departement, std::string _city_address) {
		name = _name;
		average = _average;
		departement = _departement;
		city_address = _city_address;
	};
};
