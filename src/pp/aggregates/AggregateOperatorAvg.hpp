
template< typename Out, typename In >
class AggregateOperatorAvg{
    public:
		void compute(Out *out, In *in){
			//std::cout << "Avg compute" << std::endl;
			out->first = out->first + *in;
			out->second = out->second + increment;
		}

		void reduce(Out *out, Out *in){
			//std::cout << "Avg reduce" << std::endl;
			out->first = out->first + in->first;
			out->second = out->second + in->second;
		}

	private:
		int increment = 1;
};

