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

// This code implements a pipeline for batch processing of stocks.
// It first computes a price for each option from a text file,
// then it extracts the maximum price for each stock name.

#include <algorithm>
#include <cassert>
#include <chrono>
#include <ctime>
#include <ratio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>
#include <iomanip>

#include "black_scholes.hpp"
#include "defs.h"

int main(int argc, char** argv) {

  std::string in_fname, out_fname;
  in_fname = "/home/iuly/PpFf_JapetServer/tests/StockPrice/testdata/stock_options_64K.txt";
  out_fname = "/home/iuly/PpFf_JapetServer/tests/StockPrice/testdata/pico_result.txt";

  /* parse command line */
    if (argc >= 2) {
        in_fname = argv[1];
    }

    if (argc >= 3) {
        out_fname = argv[2];
    }


  /* read options from the input file */
  std::ifstream in_file(in_fname);


  std::map<std::string, StockAndPrice> red;

  auto begin = std::chrono::high_resolution_clock::now();

  while (in_file.good()) {
    std::string opt_line;
    std::getline(in_file, opt_line, '\n');



    std::string name;
    OptionData opt;
    char otype;
    std::stringstream ins(opt_line);

    /* read stock name */
    ins >> name;

    /* read stock option data */
    ins >> opt.s >> opt.strike >> opt.r >> opt.divq;
    ins >> opt.v >> opt.t >> otype >> opt.divs >> opt.DGrefval;
    opt.OptionType = (otype == 'P');

    StockPrice res = black_scholes(opt);

    if (red.find(name) != red.end())
      red[name] = std::max(StockAndPrice(name, res), red[name]);
    else
      red[name] = StockAndPrice(name, res);

  }

   auto end = std::chrono::high_resolution_clock::now();
   long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

   std::cerr << "Temps C++:  " << duration_ms << " ms" << std::endl;

  /* print results */
  std::ofstream out_file(out_fname);
  //for (auto item : red) out_file << item.second.to_string() << std::endl;
  for (auto item : red) out_file << item.second.Key() << " => " << std::fixed << std::setprecision(4) << item.second.Value() << std::endl;


  return 0;
}
