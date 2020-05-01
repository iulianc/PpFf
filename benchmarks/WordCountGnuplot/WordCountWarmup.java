import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.Files;
import java.text.DecimalFormat;
import java.util.AbstractMap.SimpleEntry;
import java.util.Arrays;
import java.util.Map;
import java.util.concurrent.TimeUnit;
import java.util.stream.Collectors;
import static java.util.stream.Collectors.counting;
import static java.util.stream.Collectors.groupingBy;
import static java.util.stream.Collectors.toMap;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import static java.util.stream.Collectors.groupingBy;
import static java.util.stream.Collectors.toMap;


/**
 *
 * @author iciobanu
 */
public class WordCountWarmup {
        static final String DEFAULT_INPUT_FILE = "testdata/78792Words.txt";
  
        public static void main(String[] args) throws IOException {
                boolean debug = false;
                String inputFile = DEFAULT_INPUT_FILE;
                int outWords = 0;

                if (args.length >= 1) {
                        inputFile = args[0];
                }

                // utiliser pour verifier le bon fonctionnement
                // du programme
                if (args.length >= 2) {
                        if(Integer.parseInt(args[1]) == 1){
                                debug = true;
                        }
                }

                // Warmup
                List<String> wordsCount_ = 
                        Files.lines(Paths.get(inputFile))
                        .parallel()
                        .flatMap( line -> Arrays.stream(line.trim().split(" ")) )
                        .filter( word -> word.length() > 0 )
                        .collect( Collectors.toList() );


                long startTime = System.nanoTime();
                List<Map.Entry<String,Integer>> wordsCount = null;
                wordsCount = Files.lines(Paths.get(inputFile))
                        .parallel()
                        .flatMap( line -> Arrays.stream(line.trim().split(" ")) )
                        .map( word -> word.replaceAll("[^a-zA-Z]", "").toLowerCase() )
                        .filter( word -> word.length() > 0 )
                        .map( word -> new SimpleEntry<>(word, 1) )
                        .collect( toMap(e -> e.getKey(), e -> e.getValue(), (v1, v2) -> v1 + v2) )
                        .entrySet().stream()
                        .collect( Collectors.toList() );  

                long duration = (System.nanoTime() - startTime);
                double milliseconds = (double) duration / 1000000;

                if(!debug){
                        String outputResult = String.format("%6.0f ", milliseconds);
                        System.out.print(outputResult); 
                } else{
                        wordsCount.forEach( x -> 
                                            System.out.println( x.getKey() + " => " + x.getValue() ) ); 	
                }
        }
}
