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

class Reduce implements Consumer<String> {
    private int nbChars = 0;
    private int nbMots = 0;
    private long maxHash = 0;

    private long compute_hash( String s ) {
        final long p = 31;
        final long m = ((long) 1e9) + 9;
        long hash_value = 0;
        long p_pow = 1;
        for (int i = 0; i < s.length(); i++ ) {
            char c = s.charAt(i);
            hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
            p_pow = (p_pow * p) % m;
        }
        return hash_value;
    }

    public void accept( String s ) {
        nbMots += 1;
        nbChars += s.length();
        long hash = compute_hash( s );
        maxHash = hash > maxHash ? hash : maxHash;
    }
    
    public void combine( Reduce other ) {
        nbChars += other.nbChars();
        nbMots += other.nbMots();
        maxHash = maxHash > other.maxHash() ? maxHash : other.maxHash();
    }

    public int nbChars() {
        return nbChars;
    }
    
    public int nbMots() {
        return nbMots;
    }

    public long maxHash() {
        return maxHash;
    }
}

public class WC {
    static final String DEFAULT_INPUT_FILE = "testdata/78792Words.txt";
  
    public static Stream<String> splitInWords(String line) {
        return Arrays.stream(line.trim().split(" "));
    }
    
    public static String toLowerCaseLetters(String word) {
        return word.replaceAll("[^a-zA-Z]", "").toLowerCase();
    }
    
    public static boolean notEmpty( String word ) {
        return word.length() > 0;
    }
    
    public static void main(String[] args) throws IOException {
        int avecWarmup = Integer.parseInt(args[0]);
        String inputFile = args.length >= 2 ? args[1] : DEFAULT_INPUT_FILE;

        // Utilise pour verifier le bon fonctionnement du programme
        boolean emitOutput = args.length >= 3 && Integer.parseInt(args[2]) == 1;

		// Code bidon pour rechauffement: on emet le nombre d'elements
		// produits, pour etre certain que le resultat soit utilise
		// (pour eviter le dead code elimination?).
        if (avecWarmup == 1) {
            int wordsCount_ = 
                Files.lines(Paths.get(inputFile))
                .parallel()
                .flatMap( WC::splitInWords )
                .map( WC::toLowerCaseLetters )
                .filter( WC::notEmpty )
                .collect( Collectors.toList() )
                .size();
            System.err.println( wordsCount_ );
        } else if (avecWarmup >= 2) {
            Reduce wc = 
                Files.lines( Paths.get(inputFile) )
                .limit( (long) Math.pow(10, avecWarmup) )
                .parallel()
                .flatMap( WC::splitInWords )
                .map( WC::toLowerCaseLetters )
                .filter( WC::notEmpty )
                .collect( Reduce::new, Reduce::accept, Reduce::combine );
            System.err.println( wc.nbMots() + wc.nbChars() + wc.maxHash() );
        }

        // Execution du *vrai* programme.
        long startTime = System.nanoTime();
        
        Reduce wc = 
            Files.lines( Paths.get(inputFile) )
            .parallel()
            .flatMap( WC::splitInWords )
            .map( WC::toLowerCaseLetters )
            .filter( WC::notEmpty )
            .collect( Reduce::new, Reduce::accept, Reduce::combine );
        
        long duration = (System.nanoTime() - startTime);
        double milliseconds = (double) duration / 1000000;
        
        // On emet le temps ou le resultat complet, selon le cas.
        if (!emitOutput) {
            String outputResult = String.format("%6.0f ", milliseconds);
            System.out.print(outputResult); 
        } else {
            System.out.println( wc.nbMots() + " " + wc.nbChars() + " " + wc.maxHash() );
        }
    }
}
