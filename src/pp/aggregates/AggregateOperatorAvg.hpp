
template< typename Out, typename In >
class AggregateOperatorAvg{
    public:
		void compute(Out *out, In *in){
			//std::cout << "Avg compute" << std::endl;
			out->first += *in;
			out->second += 1;
		}

		void reduce(Out *out, Out *in){
			//std::cout << "Avg reduce" << std::endl;
			out->first += in->first;
			out->second += in->second;
		}
};

