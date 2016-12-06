// exit 0
// output 1 10.2 asdfqwertyjklmn 3 11.2 asdfqwertyjklmnjklmn
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

        ifj16.print(a + " ");
        ifj16.print(b + " ");
        ifj16.print(c + " ");
        ifj16.print(la + " ");
        ifj16.print(lb + " ");
        ifj16.print(lc + "\n");
    }
}
