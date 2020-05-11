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
public class WordCount {
    static final String DEFAULT_INPUT_FILE = "testdata/78792Words.txt";
  
    public static void main(String[] args) throws IOException {
        boolean avecWarmup = Integer.parseInt(args[0]) != 0;
        String inputFile = args.length >= 2 ? args[1] : DEFAULT_INPUT_FILE;

        // Utilise pour verifier le bon fonctionnement du programme
        boolean emitOutput = args.length >= 3 && Integer.parseInt(args[2]) == 1;

		// Code bidon pour rechauffement: on emet le nombre d'elements
		// produits, pour etre certain que le resultat soit utilise
		// (pour eviter le dead code elimination?).
        if (avecWarmup) {
            int wordsCount_ = 
                Files.lines(Paths.get(inputFile))
                .parallel()
                .flatMap( line -> Arrays.stream(line.trim().split(" ")) )
                .filter( word -> word.length() > 0 )
                .collect( Collectors.toList() )
                .size();
            System.err.println( wordsCount_ );
        }

        // Execution du *vrai* programme.
        long startTime = System.nanoTime();
        
        List<Map.Entry<String,Integer>> wordsCount = 
            Files.lines(Paths.get(inputFile))
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
        
        // On emet le temps ou le resultat complet, selon le cas.
        if (!emitOutput) {
            String outputResult = String.format("%6.0f ", milliseconds);
            System.out.print(outputResult); 
        } else {
            wordsCount.forEach( x -> 
                                System.out.println( x.getKey() + " => " + x.getValue() ) );     
        }
    }
}
