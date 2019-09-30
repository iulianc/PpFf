
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


/**
 *
 * @author ciubanui
 */
public class StockPrice {
    static final int DEFAULT_NB_ITERATIONS = 5;
    static final String DEFAULT_INPUT_FILE = "../testdata/stock_options_64K.txt";

    
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

		
      if (args.length >= 1) {
      	inputFile = args[0];
      }
   
      List<Map.Entry<String, Double>> stockPrice = null;

		long startTime = System.nanoTime();
       

      stockPrice = Files.lines(Paths.get(inputFile))
					.parallel()
               .map(ligne -> StockPrice.getOptionData(ligne))
               .map( opData -> new SimpleEntry<>(opData.StockName, opData.BlkSchlsEqEuroNoDiv()) )
               .collect( toMap(e -> e.getKey(), e -> e.getValue(), (v1, v2) -> v1 > v2 ? v1 : v2) )
               .entrySet().stream()
               .collect( Collectors.toList() );  


     	long duration = (System.nanoTime() - startTime);
		double milliseconds = (double) duration / 1000000;
    	String outputResult = String.format("%6.0f ", milliseconds);
    	System.out.print(outputResult); 
        
    }
    
}
