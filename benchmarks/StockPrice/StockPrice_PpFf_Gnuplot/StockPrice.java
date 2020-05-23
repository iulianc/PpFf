
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
import java.util.HashMap;
import java.util.function.*;

/**
 *
 * @author ciubanui & Guy Tremblay
 */

class GroupByKey implements Consumer<Map.Entry<String,Double>> {
    @SuppressWarnings("unchecked")
    private Map<String,Double> map = (Map) new HashMap();
    
    public void accept( Map.Entry<String,Double> entry ) {
        Double newVal = entry.getValue();
        Double current = map.get(entry.getKey());
        if (current == null || newVal > current) {
            map.put(entry.getKey(), newVal);
        }
    }
    
    public void combine( GroupByKey other ) {
        other.map.entrySet().forEach( this::accept );
    }

    public Map<String,Double> toMap() {
        return map;
    }
}

public class StockPrice {
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
        
        return new OptionData(StockName, s, strike, r, divq, v, t, OptionType, divs, DGrefval);
    }

    static public Map.Entry<String,Double> calculateStockPrice( OptionData opData )  {
        return new SimpleEntry<String,Double>(opData.StockName, opData.BlkSchlsEqEuroNoDiv());
    }
    
    /**
     * @param args the command line arguments
     */
	public static void main(String[] args) throws IOException {        
        int avecWarmup = Integer.parseInt(args[0]);
        String inputFile = args.length >= 2 ? args[1] : DEFAULT_INPUT_FILE;

        // Utilise pour verifier le bon fonctionnement du programme
        boolean emitOutput = args.length >= 3 && Integer.parseInt(args[2]) == 1;

		// Code bidon pour rechauffement: on emet le nombre d'elements
		// produits, pour etre certain que le resultat soit utilise
		// (pour eviter le dead code elimination?).
        if (avecWarmup == 1) {
            int stockPrice_ = 
                Files.lines(Paths.get(inputFile))
                .parallel()
                .map(StockPrice::getOptionData)
                .collect( Collectors.toList() )
                .size();
            System.err.println( stockPrice_ );
        } else if (avecWarmup >= 2) {
            int stockPrice_ = 
                Files.lines(Paths.get(inputFile))
                .limit( (long) Math.pow(10, avecWarmup) )
                .parallel()
                .map(StockPrice::getOptionData)
                .map( StockPrice::calculateStockPrice )
                .collect( GroupByKey::new, GroupByKey::accept, GroupByKey::combine )
                .toMap()
                .entrySet()
                .size();

            System.err.println( stockPrice_ );
        }

        // Execution du *vrai* programme.
		long startTime = System.nanoTime();

		Map<String, Double> stockPrice =
            Files.lines( Paths.get(inputFile) )
            .parallel()
            .map( StockPrice::getOptionData )
            .map( StockPrice::calculateStockPrice )
            .collect( GroupByKey::new, GroupByKey::accept, GroupByKey::combine )
            .toMap();

     	long duration = (System.nanoTime() - startTime);
		double milliseconds = (double) duration / 1000000;


        // On emet le temps ou le resultat complet, selon le cas.
		if (!emitOutput) {
			String outputResult = String.format("%6.0f ", milliseconds);
			System.out.print(outputResult); 
		} else {
      		stockPrice
                .entrySet()
                .forEach( x ->
                          System.out.println( x.getKey() +
                                              " => " +
                                              ( String.format("%.4f", x.getValue() ).replace(",",".") ) ) );
		}
    }
}
