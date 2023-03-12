public class Argv {
    static public void main(String argv[]) {
        System.out.printf("Running %s\n", Argv.class);

        System.out.printf("Received %d user input arguments: \n", argv.length);
        for (int idx = 0; idx < argv.length; idx ++) {
            System.out.printf("    argv[%d] = %s\n", idx, argv[idx]);
        }
    }
}
