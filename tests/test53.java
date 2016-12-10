// exit 0
// output 32 28.64 012abcdef
class Main {
    static int a = 5;
    static int aa = 5*a;
    static int aaa = aa+7;

    static double b = 4.2;
    static double bb = 3.2*b;
    static double bbb = bb + 15.2;

    static String c = "abc";
    static String cc = c + "def";
    static String ccc = "012" + cc;

    static void run() {
        ifj16.print(aaa + " ");
        ifj16.print(bbb + " ");
        ifj16.print(ccc + "\n");
    }
}
