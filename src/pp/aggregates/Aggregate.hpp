#ifndef AGGREGATE_HPP
#define AGGREGATE_HPP

#include <iostream>
#include <functional>
#include "AggregateOperatorCount.hpp"
#include "AggregateOperatorSum.hpp"
#include "AggregateOperatorAvg.hpp"
#include "AggregateOperatorMax.hpp"
#include "AggregateOperatorMin.hpp"
#include "../utilities/Enumerators.hpp"


template< int I, typename In, typename V, 
          typename C = AggregateOperatorCount< V, In >,
          typename S = AggregateOperatorSum< V >,
          typename Ma = AggregateOperatorMax< V >,
          typename Mi = AggregateOperatorMin< V >,
          typename A = AggregateOperatorAvg< V, In > >
class Aggregate{};


template< typename In, typename V, typename C, typename S, typename Ma, typename Mi, typename A >
class Aggregate< Aggregates::OperatorCount, In, V, C, S, Ma, Mi, A >{
public:
    using AggregateOperator = C;

    void compute(V *out, In *in){
        aggregateOperator.compute(out, in);
    }

    void reduce(V *out, V *in){
        aggregateOperator.reduce(out, in);
    }

private:
    AggregateOperator aggregateOperator;
};


template< typename In, typename V, typename C, typename S, typename Ma, typename Mi, typename A >
class Aggregate< Aggregates::OperatorSum, In, V, C, S, Ma, Mi, A >{
public:
    using AggregateOperator = S;

    void compute(V *out, V *in){
        aggregateOperator.compute(out, in);
    }

    void reduce(V *out, V *in){
        aggregateOperator.reduce(out, in);
    }

private:
    AggregateOperator aggregateOperator;
};


template< typename In, typename V, typename C, typename S, typename Ma, typename Mi, typename A >
class Aggregate< Aggregates::OperatorMax, In, V, C, S, Ma, Mi, A >{
public:
    using AggregateOperator = Ma;

    void compute(V *out, V *in){
        aggregateOperator.compute(out, in);
    }

    void reduce(V *out, V *in){
        aggregateOperator.reduce(out, in);
    }

private:
    AggregateOperator aggregateOperator;
};

template< typename In, typename V, typename C, typename S, typename Ma, typename Mi, typename A >
class Aggregate< Aggregates::OperatorMin, In, V, C, S, Ma, Mi, A >{
public:
    using AggregateOperator = Mi;

    void compute(V *out, V *in){
        aggregateOperator.compute(out, in);
    }

    void reduce(V *out, V *in){
        aggregateOperator.reduce(out, in);
    }

private:
    AggregateOperator aggregateOperator;
};

template< typename In, typename V, typename C, typename S, typename Ma, typename Mi, typename A >
class Aggregate< Aggregates::OperatorAvg, In, V, C, S, Ma, Mi, A >{
public:
    using AggregateOperator = A;

    void compute(V *out, In *in){
        aggregateOperator.compute(out, in);
    }

    void reduce(V *out, V *in){
        aggregateOperator.reduce(out, in);
    }

private:
    AggregateOperator aggregateOperator;
};

#endif