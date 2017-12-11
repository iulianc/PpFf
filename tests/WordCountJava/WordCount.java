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
  static final int DEFAULT_NB_ITERATIONS = 5;
  static final String DEFAULT_INPUT_FILE = "testdata/78792Words.txt";
  
  public static void main(String[] args) throws IOException {
    int nbIterations = DEFAULT_NB_ITERATIONS;
    String inputFile = DEFAULT_INPUT_FILE;
    String outputFile = "";

    if (args.length > 1) {
      nbIterations = Integer.parseInt(args[0]);
      inputFile = args[1];
      if (args.length > 2) {
        outputFile = args[2];
      }
    }
	
    ArrayList<ArrayList<String>> container = new ArrayList<ArrayList<String>>();
    ArrayList<String> innerContainer = new ArrayList<String>(); 

    try (BufferedReader br = new BufferedReader(new FileReader(inputFile))) {
        String line;
        while ((line = br.readLine()) != null) {
          String[] words = line.split("\\s");
          for (String word : words) {
            innerContainer.add(word);
          }
        }
      }
    container.add(innerContainer);
        
    //benchmark 
    long startTime = System.nanoTime();
        
    List<Map.Entry<String,Integer>> wordsCount = null;
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
    
    //System.out.println(duration + "ns");
    //double microseconds = (double) duration / 1000;
    //System.out.println("Total execution time in microseconds: " + microseconds); 
    double milliseconds = (double) duration / 1000000;
    System.out.println("Total execution time in milliseconds: " + milliseconds); 			             
        
    /*  wordsCount.forEach( x -> 
           System.out.println( "\"" + x.getKey() + "\" => " + x.getValue() ) );   
    */   

    if (outputFile != "") {
      try (BufferedWriter bw = new BufferedWriter(new FileWriter(outputFile))) {
          for (Map.Entry<String, Integer> entry : wordsCount) {
            bw.write(String.format("%s => %d\n", entry.getKey(), entry.getValue()));
          }
        } catch (IOException e) {
        e.printStackTrace();
      }		
    }
  }
}


