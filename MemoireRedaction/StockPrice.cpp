//black_scholes.hpp

#ifndef EXAMPLES_STOCK_MARKET_BLACK_SCHOLES_HPP_
#define EXAMPLES_STOCK_MARKET_BLACK_SCHOLES_HPP_

#include <cmath>


// Cumulative Normal Distribution Function
// See Hull, Section 11.8, P.243-244
#define inv_sqrt_2xPI 0.39894228040143270286

double CNDF(double InputX) {
  int sign;

  double OutputX;
  double xInput;
  double xNPrimeofX;
  double expValues;
  double xK2;
  double xK2_2, xK2_3;
  double xK2_4, xK2_5;
  double xLocal, xLocal_1;
  double xLocal_2, xLocal_3;

  // Check for negative value of InputX
  if (InputX < 0.0) {
    InputX = -InputX;
    sign = 1;
  } else
    sign = 0;

  xInput = InputX;

  // Compute NPrimeX term common to both four & six decimal accuracy calcs
  expValues = exp(-0.5 * InputX * InputX);
  xNPrimeofX = expValues;
  xNPrimeofX = xNPrimeofX * inv_sqrt_2xPI;

  xK2 = 0.2316419 * xInput;
  xK2 = 1.0 + xK2;
  xK2 = 1.0 / xK2;
  xK2_2 = xK2 * xK2;
  xK2_3 = xK2_2 * xK2;
  xK2_4 = xK2_3 * xK2;
  xK2_5 = xK2_4 * xK2;

  xLocal_1 = xK2 * 0.319381530;
  xLocal_2 = xK2_2 * (-0.356563782);
  xLocal_3 = xK2_3 * 1.781477937;
  xLocal_2 = xLocal_2 + xLocal_3;
  xLocal_3 = xK2_4 * (-1.821255978);
  xLocal_2 = xLocal_2 + xLocal_3;
  xLocal_3 = xK2_5 * 1.330274429;
  xLocal_2 = xLocal_2 + xLocal_3;

  xLocal_1 = xLocal_2 + xLocal_1;
  xLocal = xLocal_1 * xNPrimeofX;
  xLocal = 1.0 - xLocal;

  OutputX = xLocal;

  if (sign) {
    OutputX = 1.0 - OutputX;
  }

  return OutputX;
}

double BlkSchlsEqEuroNoDiv(double sptprice, double strike, double rate,
                           double volatility, double time, int otype,
                           float timet) {
#ifdef GRAIN
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
#endif

  double OptionPrice;
  double xRiskFreeRate;
  double xVolatility;
  double xTime;
  double xSqrtTime;

  double logValues;
  double xLogTerm;
  double xD1;
  double xD2;
  double xPowerTerm;
  double xDen;
  double d1;
  double d2;
  double FutureValueX;
  double NofXd1;
  double NofXd2;
  double NegNofXd1;
  double NegNofXd2;

  // xStockPrice = sptprice;
  // xStrikePrice = strike;
  xRiskFreeRate = rate;
  xVolatility = volatility;

  xTime = time;
  xSqrtTime = sqrt(xTime);

  logValues = log(sptprice / strike);

  xLogTerm = logValues;

  xPowerTerm = xVolatility * xVolatility;
  xPowerTerm = xPowerTerm * 0.5;

  xD1 = xRiskFreeRate + xPowerTerm;
  xD1 = xD1 * xTime;
  xD1 = xD1 + xLogTerm;

  xDen = xVolatility * xSqrtTime;
  xD1 = xD1 / xDen;
  xD2 = xD1 - xDen;

  d1 = xD1;
  d2 = xD2;

  NofXd1 = CNDF(d1);
  NofXd2 = CNDF(d2);

  FutureValueX = strike * (exp(-(rate) * (time)));
  if (otype == 0) {
    OptionPrice = (sptprice * NofXd1) - (FutureValueX * NofXd2);
  } else {
    NegNofXd1 = (1.0 - NofXd1);
    NegNofXd2 = (1.0 - NofXd2);
    OptionPrice = (FutureValueX * NegNofXd2) - (sptprice * NegNofXd1);
  }

  return OptionPrice;
}

struct OptionData {
  std::string StockName;
  double s;       // spot price
  double strike;  // strike price
  double r;       // risk-free interest rate
  double divq;    // dividend rate
  double v;       // volatility
  double t;  // time to maturity or option expiration in years (1yr = 1.0, 6mos
             // = 0.5, 3mos = 0.25, ..., etc)
  int OptionType;   // Option type.  "P"=PUT, "C"=CALL
  double divs;      // dividend vals (not used in this test)
  double DGrefval;  // DerivaGem Reference Value
};


//StockPrice.cpp

#include <iostream>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <functional>
#include <regex>
#include <chrono>
#include <ctime>
#include <ratio>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include<fstream>
#include <locale>
#include <Flow.hpp>
#include <unordered_map>
#include <ctype.h>
#include <iomanip>

#include "black_scholes.hpp"

using namespace PpFf;

#define DEFAULT_NB_ITERATIONS 1
#define DEFAULT_INPUT_FILE "stock_options_64K.txt"
#define DEFAULT_NB_THREADS 1
#define DEFAULT_OUTPUT_FILE_RESULT "stock_output_result.txt"


struct StockAndPrice {
    std::string StockName;
    double StockPrice;
};

OptionData* getOptionData(std::string* data) {
    std::string name;
    OptionData* opt = new OptionData();
    char otype;
    std::stringstream ins(*data);

    /* read stock option data */
    ins >> opt->StockName;
    ins >> opt->s >> opt->strike >> opt->r >> opt->divq;
    ins >> opt->v >> opt->t >> otype >> opt->divs >> opt->DGrefval;
    opt->OptionType = (otype == 'P');
    
    return opt;
}

StockAndPrice* calculateStockPrice(OptionData* opt) {
    StockAndPrice* stockAndPrice = new StockAndPrice();
    stockAndPrice->StockName = opt->StockName;
    stockAndPrice->StockPrice = BlkSchlsEqEuroNoDiv(opt->s, opt->strike, opt->r, opt->v, 
                                                    opt->t, opt->OptionType, 0);
	
    return stockAndPrice;
}

int main(int argc, char* argv[]) {
    uint32_t nbIterations = DEFAULT_NB_ITERATIONS;
    std::string inputFile = DEFAULT_INPUT_FILE;
	 std::string outputFileResult = DEFAULT_OUTPUT_FILE_RESULT;
    uint32_t nbThreads = DEFAULT_NB_THREADS;

    if (argc >= 2) {
        inputFile = argv[1];
    }

    if (argc >= 3) {
        nbIterations = atoi(argv[2]);
    }
    
    if (argc >= 4) {
        nbThreads = atoi(argv[3]);
    }

    if (argc >= 5) {
        outputFileResult = argv[4];
    }
    
    Reducer<StockAndPrice, double> reducer(0.0, 
                                           [](double maxPrice, StockAndPrice sp) {
                                               return sp.StockPrice > maxPrice ? sp.StockPrice : maxPrice; 
                                           },
                                           [](double max, double workerResult) { 
                                               return workerResult > max ? workerResult : max; 
                                           });
    
    auto begin = std::chrono::high_resolution_clock::now();

    std::unordered_map<std::string, double> currentResult;
    for (uint32_t i = 0; i < nbIterations; ++i) {
        currentResult = 
            Flow
            ::source(inputFile)
            .parallel(nbThreads)
            .map<std::string, OptionData>(getOptionData)
            .map<OptionData, StockAndPrice>(calculateStockPrice)
            .reduceByKey<StockAndPrice, std::string, double>(reducer, 
                                                             [](StockAndPrice* sp) { return &(sp->StockName); });
        
    }
    auto end = std::chrono::high_resolution_clock::now();
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    
    fprintf( stderr, "Temps C++  (%3d it.; %2d thr.): %5ld ms {%5ld ms/it. }\n",
             nbIterations, nbThreads, duration_ms, duration_ms / nbIterations );
    
  	 // Write result to file
	 std::ofstream ofs;

  	 ofs.open (outputFileResult, std::ofstream::out | std::ofstream::app);
  	 
	 char buffer[100];
	 snprintf(buffer, sizeof(buffer), "Temps C++  (%3d it.; %2d thr.): %5ld ms {%5ld ms/it. }\n", 
												nbIterations, nbThreads, duration_ms, duration_ms / nbIterations);
	 ofs << buffer;
	 ofs.close();

    std::map<std::string, double> orderedResult;
    for (auto it = currentResult.begin(); it != currentResult.end(); it++) {
    	orderedResult[it->first] = it->second;
    }
    
    for (auto it = orderedResult.begin(); it != orderedResult.end(); it++) {
        std::string currentResultKey = it->first;
        double currentResultValue = it->second;
        std::cout << currentResultKey << " => " << std::fixed << std::setprecision(4) << currentResultValue << std::endl;
    }
   
    return 0;
}
