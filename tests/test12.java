class Main {
    static int a;
    static double b;
    static String c;

    static void run() {
        int la;
        double lb;
        String lc;

        a = 1 + 0;
        b = 5 + 4.2;
        c = "asdf" + "qwerty";

        la = a + a * 2;
        b = b + a;
        c = c + "jklmn";

        la = a + a*2;
        lb = b + a;
        lc = c + "jklmn";

        ifj16.print("a = " + a + "\n");
        ifj16.print("b = " + b + "\n");
        ifj16.print("c = " + c + "\n");
        ifj16.print("la = " + la + "\n");
        ifj16.print("lb = " + lb + "\n");
        ifj16.print("lc = " + lc + "\n");
    }
}
