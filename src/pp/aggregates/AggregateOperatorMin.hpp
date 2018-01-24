

template< typename T >
class AggregateOperatorMin{
    public:
		void compute(T *out, T *in){
			//std::cout << "Min compute" << std::endl;
			if (*in < *out)
				*out = *in;
		}

		void reduce(T *out, T *in){
			//std::cout << "Min reduce" << std::endl;
			compute(out, in);
		}

	private:
};
