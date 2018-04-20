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
 * @author Guy Tremblay
 */
public class ReduceByKeyJava {
  static final int DEFAULT_NB = 100;

  public static void main(String[] args) throws IOException {
    int nb = DEFAULT_NB;

    if (args.length >= 1) {
      nb = Integer.parseInt(args[0]);
    }

    Integer[] elems = new Integer[nb * (nb+1) / 2];
    Integer[] expectedResult = new Integer[nb+1];
    int next = 0;
    for ( int i = 1; i <= nb; i++ ) {
      for (int k = 1; k <= i; k++) {
        elems[next] = i;
        next += 1;
      }
      expectedResult[i] = i;
    }

    long startTime = System.nanoTime();
        
    Map<Integer,Integer> result =
      Arrays.stream(elems)
      .parallel()
      .map( val -> new SimpleEntry<Integer,Integer>(val, 1) )
      .collect( toMap(e -> e.getKey(), e -> e.getValue(), (v1, v2) -> v1 + v2) );
    
    long duration = (System.nanoTime() - startTime);
    
    double milliseconds = (double) duration / 1000000;
    System.err.println("Temps Java: " + milliseconds + " ms");

    for (Map.Entry<Integer, Integer> e : result.entrySet()) {
      if ( !e.getValue().equals(expectedResult[e.getKey()]) ) {
        System.out.println( "*** Result pas ok pour " + e.getKey() + ": " + e.getValue() + " != " + expectedResult[e.getKey()] );
        break;
      }
    }
  }
}
  

