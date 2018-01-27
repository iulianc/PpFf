

template< typename T >
class AggregateOperatorMax{
    public:
		void compute(T *out, T *in){
			//std::cout << "Max compute" << std::endl;

			if (*in > *out)
				*out = *in;
		}

		void reduce(T *out, T *in){
			//std::cout << "Max reduce" << std::endl;
			compute(out, in);
		}
};
