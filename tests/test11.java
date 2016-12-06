// exit 0
// output 0 1 2 3 4 
class Main {
    static void run() {
        int x = 5;
        int result;
        result = recurse_fun(x);
        ifj16.print("\n");
    }
    static int recurse_fun(int a) {
        a = a - 1;

        if (a != 0) {
            recurse_fun(a);
        }
        else {
            a = a + 0;
        }

        ifj16.print(a + " ");

        return a;
    }
}
