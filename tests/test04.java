// exit 0
// output 8 5
class Main {
    static void run() {
        int a = 3;
        double b;
        b = a + 5;
        ifj16.print(b);
        ifj16.print(" ");
        aclass.my_func1();
        ifj16.print("\n");
    }
}
class aclass {
    static void my_func1() {
        int a;
        a = my_func2();
        ifj16.print(a);
    }
    static int my_func2() {
        return 5;
    }
}
