import java.io.File;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

public class Filewalker {

	public int CountLinesFromFile(String path){
		int linesFile = 0;
		BufferedReader reader;
		//System.out.print(path + ": ");

		try {
			reader = new BufferedReader(new FileReader(path));
			String line = reader.readLine();
			while (line != null) {
				linesFile++;
				line = reader.readLine();
			}
			reader.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		//System.out.println(linesFile);
		return linesFile;	
	}

    public void walk( String path ) {
		  int totalLinesModule = 0;
        File root = new File( path );
        File[] list = root.listFiles();
		  ArrayList<String> dirs = new ArrayList<String>();

		  System.out.println("Module: " + path);

        if (list == null) return;

        for ( File f : list ) {
            if ( f.isDirectory() ) {
                //walk( f.getAbsolutePath() );
					 dirs.add( f.getAbsolutePath() );
            }
            else {
					totalLinesModule += CountLinesFromFile(f.getAbsolutePath());
            }
        }

		  System.out.println("Total lines module: " + totalLinesModule);
		  System.out.println("");

		  for(String dirName : dirs){
				totalLinesModule = 0;
		  		walk(dirName);
		  }
    }

    public static void main(String[] args) {
        final String dir = System.getProperty("user.dir");
        //System.out.println("current dir = " + dir);

        Filewalker fw = new Filewalker();
        fw.walk(dir);
    }

}