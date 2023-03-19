import java.lang.Exception;

import java.util.Scanner;

import java.io.InputStream;
import java.io.FileInputStream;
import java.io.OutputStream;
import java.io.FileOutputStream;

public class CP { 
    public static void cp_by_line(InputStream src, OutputStream dst) {
        try {
            Scanner s = new Scanner(src);

            while (s.hasNextLine()) {
                String line = s.nextLine();

                dst.write(line.getBytes(), 0, line.length());
                dst.write('\n');
            }

            s.close();
        } catch (Exception e) {}
    }

    public static void main(String argv[]) {
        if (argv.length > 0) {
            try {
                FileInputStream src = new FileInputStream(argv[0]);

                if (argv.length == 1) {
                    cp_by_line(src, System.out);
                } else {
                    try {
                        FileOutputStream dst = new FileOutputStream(argv[1]);
                        
                        cp_by_line(src, dst);
                        dst.close();
                    } catch (Exception e) {
                        System.out.println(e.getMessage());
                    }
                }

                src.close();
            } catch (Exception e) {
                System.out.println(e.getMessage());
            }
        } else {
            cp_by_line(System.in, System.out);
        }
    }
}
