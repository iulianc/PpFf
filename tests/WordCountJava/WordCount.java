
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
import java.io.FileWriter;
import java.io.IOException;


/**
 *
 * @author iciobanu
 */
public class WordCount {

    /**
     * @param args the command line arguments

     */
    public static void main(String[] args) throws IOException {
    
	Map<String, Integer> wordCount = null;
	String inputFile = "";
	String outputFile = "";

	inputFile = "/home/iuly/RepositoryFastFlow/PpFf_Catch/tests/WordCount/testdata/lorem.txt";
	int iterations = 5;

	if(args.length > 1)
	{
		iterations = Integer.parseInt(args[0]);
		inputFile = args[1];

		if(args.length > 2)
		{
			outputFile = args[2];
		}
	}
	
        
        //System.out.println(inputFile);

        //benchmark 
        long startTime = System.nanoTime();
        
	
	for(int i = 0; i < iterations; ++i){
		// Methode1
		Path path = Paths.get(inputFile);

		wordCount = Files.lines(path).flatMap(line -> Arrays.stream(line.trim().split(" ")))
		        .parallel()
		        .map(word -> word.replaceAll("[^a-zA-Z]", "").toLowerCase().trim())
		        .filter(word -> word.length() > 0)
		        .map(word -> new SimpleEntry<>(word, 1))
		        .collect(toMap(e -> e.getKey(), e -> e.getValue(), (v1, v2) -> v1 + v2));


//        //Methode2
//        Path path = Paths.get(inputFile);
//        Map<String, Long> wordCount = Files.lines(path)
//                //.parallel()
//                .flatMap(line -> Arrays.stream(line.trim().split(" ")))
//                .map(word -> word.replaceAll("[^a-zA-Z]", "").toLowerCase().trim())
//                .filter(word -> word.length() > 0)
//                .map(word -> new SimpleEntry<>(word, 1))
//                .collect(groupingBy(SimpleEntry::getKey, counting()));
        
        
//        //Methode3 Faster
//        Path path = Paths.get(inputFile);
//        Map<String, Integer> wordCount = Files.lines(path)
//                .parallel()
//                .flatMap(line -> Arrays.stream(line.trim().split(" ")))
//                .map(word -> word.replaceAll("[^a-zA-Z]", "").toLowerCase().trim())
//                .filter(word -> word.length() > 0)
//                .collect(Collectors.toMap(s -> s, s -> 1, Integer::sum));   
	}        

     

        
    long difference = System.nanoTime() - startTime;
	long duration = difference / iterations;
	System.out.println(duration + "ns");

	double seconds = (double)duration / 1000000000;
	System.out.println("Total execution time in seconds: " + seconds);
	
	wordCount.forEach((k, v) -> System.out.println(String.format("%s ==>> %d", k, v))); 
	
	if(outputFile != "")
	{
		//WordCount wordCount = new WordCount();
		try (BufferedWriter bw = new BufferedWriter(new FileWriter(outputFile))) {
			for (Map.Entry<String, Integer> entry : wordCount.entrySet())
			{
			    bw.write(String.format("%s ==>> %d\n", entry.getKey(), entry.getValue()));
			}
		} catch (IOException e) {
			//e.printStackTrace();
		}		
	}       
    }

	static void writeToFile(BufferedWriter bw, String content) throws IOException
	{
		bw.write(content);
	}
    
}


