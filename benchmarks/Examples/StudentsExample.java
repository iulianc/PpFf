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

class Student {
    public String name;
    public String departement;
    public String average;
    
    Student( String name, String departement, String average ) {
        this.name = name;
        this.departement = departement;
        this.average = average;
    }

    public String toString() {
        return name + " [" + departement + "]: " + average;
    }
}

public class StudentsExample {
    public static void main(String[] args) throws IOException {
        List<Student> students =
            Arrays.asList( new Student( "S1", "Informatique", "A" ),
                           new Student( "S2", "Informatique", "B" ),
                           new Student( "S3", "Mathematiques", "A" ),
                           new Student( "S4", "Mathematiques", "B" ),
                           new Student( "S5", "Mathematiques", "A" ),
                           new Student( "S6", "Biologie", "A" ),
                           new Student( "S7", "Biologie", "C" )
                           );

        // Note: Pas possible d'avoir des synonymes (alias) pour les
        // types comme en C++.
        Map<String, List<Student>> studentsByDept =
            students
            .stream()
            .parallel()
            .filter( s -> s.average == "A" )
            .collect( Collectors.groupingBy( s -> s.departement ) );
        
        studentsByDept
            .entrySet()
            .forEach(System.out::println);
    }
}
