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
public class MapFilterMapJava {
  static final int DEFAULT_NB = 100;

  public static int sommeJusqua( int n ) {
    int res = 0;
    for (int i = 1; i <= n; i++) {
        res += i / 20 % 2 == 0 ? 1 : 2;
    }
    return res;
  }

  public static void main(String[] args) throws IOException {
    int nb = DEFAULT_NB;

    if (args.length >= 1) {
      nb = Integer.parseInt(args[0]);
    }

    int[] elems = new int[nb];
    for ( int i = 0; i < nb; i++ ) {
      elems[i] = i;
    }

    long startTime = System.nanoTime();
        
    int[] result =
      Arrays.stream(elems)
      .parallel()
      .map( (n) -> n * 10 )
      .filter( (n) -> n % 20 == 0 )
      .map( (n) -> sommeJusqua(n) )
      .toArray();
    
    long duration = (System.nanoTime() - startTime);
    
    double milliseconds = (double) duration / 1000000;
    System.err.println("Temps Java: " + milliseconds + " ms");
    
    //for (int i = 0; i < result.length; i++) {
    //  System.out.println( result[i] );
    //}
  }
}
  

