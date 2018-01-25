

template< typename Out, typename In >
class AggregateOperatorCount{
    public:
        void compute(Out *out, In *in){
            //std::cout << "Count compute" << std::endl;
            *out += 1;
        }

        void reduce(Out *out, Out *in){
            //std::cout << "Count reduce" << std::endl;
            *out += *in;
        }
};
