class Main {
    static void run() {
        int a;
        double b;
        a = b + 5;
        my_func1();
    }
}
class aclass {
    static void my_func1() {
        int a;
        a = my_func2();
    }
    static int my_func2() {
        return 5;
    }
}
