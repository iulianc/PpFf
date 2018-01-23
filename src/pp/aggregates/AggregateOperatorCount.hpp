

template< typename Out >
class AggregateOperatorCount{
    public:
        void compute(Out *out){
            //std::cout << "Count compute" << std::endl;
            *out = *out + increment;
        }

        void reduce(Out *out, Out *in){
            //std::cout << "Count reduce" << std::endl;
            *out = *out + *in;
        }

	private:
        Out increment = 1;
};
