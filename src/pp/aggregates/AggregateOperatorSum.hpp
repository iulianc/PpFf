

template< typename T >
class AggregateOperatorSum{
    public:
		void compute(T *out, T *in){
			//std::cout << "Sum compute" << std::endl;
			*out = *out + *in;
		}

		void reduce(T *out, T *in){
			//std::cout << "Sum reduce" << std::endl;
			compute(out, in);
		}

	private:
		//const std::string _type = "FunctionAggregateSum";
};
