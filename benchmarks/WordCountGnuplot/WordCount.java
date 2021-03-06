import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.Files;
import java.text.DecimalFormat;
import java.util.AbstractMap.SimpleEntry;
import java.util.Arrays;
import java.util.Set;
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
import java.util.stream.*;
import java.util.HashMap;
import java.util.function.*;
import static java.util.stream.Collectors.groupingBy;
import static java.util.stream.Collectors.toMap;


/**
 *
 * @author iciobanu & Guy Tremblay
 */

class ReduceByKey implements Consumer<String> {
    @SuppressWarnings("unchecked")
        private Map<String,Integer> map = (Map) new HashMap();
    
    public void accept( String s ) {
        map.put(s, map.getOrDefault(s, 0) + 1);
    }
    
    public void combine( ReduceByKey other ) {
        for ( Map.Entry<String,Integer> entry : other.map.entrySet() ) {
            Integer value = map.getOrDefault(entry.getKey(), 0);
            map.put( entry.getKey(), value + entry.getValue() );
        } 
    }

    public Map<String,Integer> toMap() {
        return map;
    }
}

public class WordCount {
    static final String DEFAULT_INPUT_FILE = "testdata/78792Words.txt";
  
    public static Stream<String> splitInWords(String line) {
        return Arrays
            .stream( line.trim().split("[^a-zA-Z]") )
            .filter( word -> word.length() > 0 );
    }
    
    public static String toLowerCaseLetters(String word) {
        return word.toLowerCase();
    }
    
    public static boolean notEmpty( String word ) {
        return word.length() > 0;
    }
    
    public static void main(String[] args) throws IOException {
        int sorte = Integer.parseInt(args[0]);
        String inputFile = args.length >= 2 ? args[1] : DEFAULT_INPUT_FILE;

        boolean parallele = (sorte / 10) > 1;
        boolean avecWarmup = (sorte % 10) == 1;

        int nbLignesWarmup = Integer.parseInt(inputFile
                                            .replaceAll("[a-zA-Z]", "")
                                            .replaceAll("/", "")
                                            .replaceAll("[.]", "")) / 200 ;
        

        // Utilise pour verifier le bon fonctionnement du programme
        boolean emitOutput = args.length >= 3 && Integer.parseInt(args[2]) == 1;

		// Code bidon pour rechauffement: on emet le nombre d'elements
		// produits, pour etre certain que le resultat soit utilise
		// (pour eviter le dead code elimination?).
        if (avecWarmup) {
            System.err.println( "avecWarmup sur " + nbLignesWarmup + " lignes" );
            Map<String,Integer> wordsCount = 
                Files.lines( Paths.get(inputFile) )
                .limit( nbLignesWarmup )
                .parallel()
                .flatMap( WordCount::splitInWords )
                .map( WordCount::toLowerCaseLetters )
                .filter( WordCount::notEmpty )
                .collect( ReduceByKey::new, ReduceByKey::accept, ReduceByKey::combine )
                .toMap();

            System.err.println( wordsCount.entrySet().size() );
        }

        // Execution du *vrai* programme.
        long startTime = System.nanoTime();
        Map<String,Integer> wordsCount;
        if (parallele) {
            //System.err.println( "Execution avec parallel()" );
            wordsCount =
                Files.lines( Paths.get(inputFile) )
                .parallel()
                .flatMap( WordCount::splitInWords )
                .map( WordCount::toLowerCaseLetters )
                .collect( ReduceByKey::new, ReduceByKey::accept, ReduceByKey::combine )
                .toMap();
        } else {
            //System.err.println( "Execution *sans* parallel()" );
            wordsCount =
                Files.lines( Paths.get(inputFile) )
                .flatMap( WordCount::splitInWords )
                .map( WordCount::toLowerCaseLetters )
                .collect( ReduceByKey::new, ReduceByKey::accept, ReduceByKey::combine )
                .toMap();
        }
        
        long duration = (System.nanoTime() - startTime);
        double milliseconds = (double) duration / 1000000;
        
        // On emet le temps ou le resultat complet, selon le cas.
        if (!emitOutput) {
            String outputResult = String.format("%6.0f ", milliseconds);
            System.out.print(outputResult); 
        } else {
            wordsCount.entrySet().forEach( x -> 
                                System.out.println( x.getKey() + " => " + x.getValue() ) );     
        }
    }
}
