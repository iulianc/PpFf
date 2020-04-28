
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.Collections;


public class JavaMaps {
    static int n = 10;
    static int nbMaps = 1;
    static int granularity = 1;  
    static int nbThreads = 1;  
    
    static int mapFunc(int in){
        int res = in;
        int nb = (int)Math.pow(10, granularity);
        for (int i = 1; i <= nb; i++) {
            res += 1;
        }
        return res;
    };
    
    public static void main(String[] args) {
       
        if (args.length >= 1) {
            n = Integer.parseInt(args[0]);
    	}

    	if (args.length >= 2) {
            nbMaps = Integer.parseInt(args[1]);
    	}

    	if (args.length >= 3) {
            granularity = Integer.parseInt(args[2]);
    	}

    	if (args.length >= 4) {
            nbThreads = Integer.parseInt(args[3]);
    	}
        
        ArrayList<Integer> elems = new ArrayList<Integer>();
        ArrayList<Integer> attendus = new ArrayList<Integer>();
        List<Integer> obtenus = null;
        
        for(int i = 0; i < n; i++) {
            elems.add(i);
            attendus.add((int)(nbMaps * Math.pow(10, granularity) + i));            
        }
        
        
        long startTime = System.nanoTime();
        
        obtenus = elems.stream()
                .parallel()
                .map(JavaMaps::mapFunc)
                .collect(Collectors.toList());
                         

        long duration = (System.nanoTime() - startTime);
        double milliseconds = (double) duration / 1000000;
        
        if (obtenus.size() != attendus.size()) {
            System.out.println(String.format("Tailles pas ok: obtenus = %lu vs. attendus = %lu",
                    obtenus.size(), attendus.size() ));
        }        
        
        Collections.sort(obtenus);
             
        for (int i = 0; i < attendus.size(); i++) {
            if(!obtenus.get(i).equals(attendus.get(i))) {
                System.out.println( String.format("Pas ok pour %d: obtenus = %d vs. attendus = %d", i, obtenus.get(i), attendus.get(i)));
                break;
            }
        }        
        
        String outputResult = String.format("%5d %2d %2d %2d : %6.0f ", n, nbMaps, granularity, nbThreads, milliseconds);
        System.out.print(outputResult);         
    }
    
}
