import java.lang.Exception;

import java.util.Scanner;

import java.io.InputStream;
import java.io.FileInputStream;

public class Cat { 
    public static void cat_by_line(InputStream src) {
        try {
            Scanner s = new Scanner(src);

            while (s.hasNextLine()) {
                String line = s.nextLine();
                System.out.println(line);
            }

            s.close();
        } catch (Exception e) {}
    }

    public static void main(String argv[]) {
        if (argv.length > 0) {
            for (int idx = 0; idx < argv.length; idx++) {
                try {
                    FileInputStream src = new FileInputStream(argv[idx]);

                    cat_by_line(src);
                    src.close();
                } catch (Exception e) {
                    System.out.println(e);
                }
            }
        } else {
            cat_by_line(System.in);
        }
    }
}
