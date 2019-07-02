//OptionData.java

import java.lang.*;
import java.math.BigDecimal;

public class OptionData {
    static final double inv_sqrt_2xPI = 0.39894228040143270286;
    static final BigDecimal bd_inv_sqrt_2xPI = new BigDecimal(0.39894228040143270286);
    String StockName;
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
    
    // Getter
    public String getName() {
        return StockName;
    }

    // Setter
    public void setName(String value) {
        this.StockName = value;
    }
  
    public OptionData(String StockName, double s, double strike, double r, double divq, double v, double t, int OptionType, double divs, double DGrefval){
        this.StockName = StockName;
        this.s = s;
        this.strike = strike;
        this.r = r;
        this.divq = divq;
        this.v = v;
        this.t = t;
        this.OptionType = OptionType;
        this.divs = divs;
        this.DGrefval = DGrefval;
    }
    
    private double CNDF(double InputX) {
        boolean sign;

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
          sign = true;
        } else
          sign = false;

        xInput = InputX;

        // Compute NPrimeX term common to both four & six decimal accuracy calcs
        expValues = Math.exp(-0.5 * InputX * InputX);
        xNPrimeofX = expValues;
        xNPrimeofX = xNPrimeofX * inv_sqrt_2xPI;
        
        //System.out.println(bd_inv_sqrt_2xPI);

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
    
    public double BlkSchlsEqEuroNoDiv(){
        return this.BlkSchlsEqEuroNoDiv(this.s, this.strike, this.r, this.v, this.t, this.OptionType, 0);
    }
    
    private double BlkSchlsEqEuroNoDiv(double sptprice, double strike, double rate,
                               double volatility, double time, int otype,
                               float timet) {

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
          xSqrtTime = Math.sqrt(xTime);

          logValues = Math.log(sptprice / strike);

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

          FutureValueX = strike * (Math.exp(-(rate) * (time)));
          if (otype == 0) {
            OptionPrice = (sptprice * NofXd1) - (FutureValueX * NofXd2);
          } else {
            NegNofXd1 = (1.0 - NofXd1);
            NegNofXd2 = (1.0 - NofXd2);
            OptionPrice = (FutureValueX * NegNofXd2) - (sptprice * NegNofXd1);
          }

          return OptionPrice;
    }    
}



//StockPrice.java

import java.nio.file.Paths;
import java.nio.file.Files;
import java.util.AbstractMap.SimpleEntry;
import java.util.Map;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;
import java.io.IOException;
import java.util.List;
import static java.util.stream.Collectors.toMap; 
import java.io.*;  

public class StockPrice {
    static final int DEFAULT_NB_ITERATIONS = 5;
    static final String DEFAULT_INPUT_FILE = "../testdata/stock_options_64K.txt";
	 static final String DEFAULT_OUTPUT_FILE_RESULT = "../testdata/stock_output_result.txt";
    
    static public OptionData getOptionData(String data) {
        String structData[] = data.trim().split("\\t|\\s");
        String StockName = structData[0].trim();
        double s = Double.parseDouble(structData[1].trim());
        double strike = Double.parseDouble(structData[2].trim());
        double r = Double.parseDouble(structData[3].trim());
        double divq = Double.parseDouble(structData[4].trim());
        double v = Double.parseDouble(structData[5].trim());
        double t = Double.parseDouble(structData[6].trim());
        int OptionType = (structData[7].trim().charAt(0) == 'P') ? 1 : 0;
        double divs = Double.parseDouble(structData[8].trim());
        double DGrefval = Double.parseDouble(structData[9].trim());
        
        OptionData opt = new OptionData(StockName, s, strike, r, divq, v, t, OptionType, divs, DGrefval);

        return opt;
    }


    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws IOException {        
        int nbIterations = DEFAULT_NB_ITERATIONS;
        String inputFile = DEFAULT_INPUT_FILE;
		  String outputFileResult = DEFAULT_OUTPUT_FILE_RESULT;

        if (args.length >= 1) {
          inputFile = args[0];
        }

        if (args.length >= 2) {
          nbIterations = Integer.parseInt(args[1]);
        }

        if (args.length >= 3) {
          outputFileResult = args[2];
        }
    
        List<Map.Entry<String, Double>> stockPrice = null;

	long startTime = System.nanoTime();
       
	for (int i = 0; i < nbIterations; ++i) {
          stockPrice = Files.lines(Paths.get(inputFile))
				.parallel()
                .map(ligne -> StockPrice.getOptionData(ligne))
                .map( opData -> new SimpleEntry<>(opData.StockName, opData.BlkSchlsEqEuroNoDiv()) )
                .collect( toMap(e -> e.getKey(), e -> e.getValue(), (v1, v2) -> v1 > v2 ? v1 : v2) )
                .entrySet().stream()
                .collect( Collectors.toList() );  
	}

     	long duration = (System.nanoTime() - startTime);
    
    	double milliseconds = (double) duration / 1000000;
		String outputResult = "Temps Java (" + String.format("%3d", nbIterations) + " it.):         " + String.format("%6.0f", milliseconds) + " ms" + " {" + String.format("%5.0f", milliseconds / nbIterations) + " ms/it. }";
    	System.err.println(outputResult); 

		// Write the result to file
    	try {
			BufferedWriter writer = new BufferedWriter(new FileWriter(outputFileResult, true));
			writer.write(outputResult);
			writer.write("\n");
			writer.write("\n");
			writer.close();
		}catch (IOException e) {
			e.printStackTrace();
		}
       
      stockPrice.forEach( x ->
                      System.out.println( x.getKey() + " => " + ( String.format("%.4f", x.getValue() ).replace(",",".") ) ) );
        
    }
    
}
