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
 * Emet liste des mots et leur nombre d'occurrences sur stdout.
 *
 * @author iciobanu
 * @author Guy Tremblay
 */
public class WordCount {
  static final int DEFAULT_NB_ITERATIONS = 5;
  static final String DEFAULT_INPUT_FILE = "testdata/78792Words.txt";
  
  public static void main(String[] args) throws IOException {
    int nbIterations = DEFAULT_NB_ITERATIONS;
    String inputFile = DEFAULT_INPUT_FILE;

    if (args.length > 1) {
      nbIterations = Integer.parseInt(args[0]);
      inputFile = args[1];
    }
	
    ArrayList<ArrayList<String>> container = new ArrayList<ArrayList<String>>();
    ArrayList<String> innerContainer = new ArrayList<String>(); 

    try (BufferedReader br = new BufferedReader(new FileReader(inputFile))) {
        String line;
        while ((line = br.readLine()) != null) {
          String[] words = line.split("\\s+");
          for (String word : words) {
            innerContainer.add(word);
          }
        }
      }
    container.add(innerContainer);
        
    //benchmark 
    long startTime = System.nanoTime();
        
    List<Map.Entry<String,Integer>> wordsCount = null;
    // NOTE (GUY T.): Il faudrait que le premier etage genere un flux de lignes.
    // Et que le temps d'execution mesure aussi la lecture du fichier, comme pour la version C++.
    for (int i = 0; i < nbIterations; ++i) {
      wordsCount = container
        .stream()
        .parallel()
        .flatMap( line -> line.stream() )
        .map( word -> word.replaceAll("[^a-zA-Z]", "").toLowerCase().trim() )
        .filter( word -> word.length() > 0 )
        .map( word -> new SimpleEntry<>(word, 1) )
        .collect( toMap(e -> e.getKey(), e -> e.getValue(), (v1, v2) -> v1 + v2) )
        .entrySet()
        .stream()
        .sorted( Comparator.comparing(Map.Entry::getKey) )
        .collect( Collectors.toList() );  
      
      /*           
      //Methode2
      wordsCount = container.stream()
      .parallel()
      .flatMap(line->line.stream())
      .map(word -> word.replaceAll("[^a-zA-Z]", "").toLowerCase().trim())
      .filter(word -> word.length() > 0)
      .collect(Collectors.toMap(s -> s, s -> 1, Integer::sum))  
      .entrySet()
      .stream()
      .sorted( Comparator.comparing(Map.Entry::getKey) )
      .collect( Collectors.toList() );  
      */	                              
    }
        
    long duration = (System.nanoTime() - startTime) / nbIterations;
    
    double milliseconds = (double) duration / 1000000;
    System.err.println( "Temps Java: " + milliseconds + " ms" );
    
    wordsCount.forEach( x -> 
                        System.out.println( x.getKey() + " => " + x.getValue() ) ); 
  }
}


