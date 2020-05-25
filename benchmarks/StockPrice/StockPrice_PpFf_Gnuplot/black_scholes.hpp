/*
 * Copyright (c) 2019 alpha group, CS department, University of Torino.
 * 
 * This file is part of pico 
 * (see https://github.com/alpha-unito/pico).
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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

  // local private working variables for the calculation
  // double xStockPrice;
  // double xStrikePrice;
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

StockAndPrice* getOptionDataAndCalculateStockPrice(std::string* str) {
    OptionData* opt = getOptionData(str);

    StockAndPrice* stockAndPrice = new StockAndPrice();
    stockAndPrice->StockName = opt->StockName;
    stockAndPrice->StockPrice = BlkSchlsEqEuroNoDiv(opt->s, opt->strike, opt->r, opt->v, 
                                                    opt->t, opt->OptionType, 0);
	
    return stockAndPrice;
}

#endif /* EXAMPLES_STOCK_MARKET_BLACK_SCHOLES_HPP_ */
