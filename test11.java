class Main {
    static void run() {
        int x;
        x = 3;
        int y;
        y = 4;
        int result;
        result = 1;
        result = recurse_fun(x , y);
        x = 1;
        return;
    }
    static int recurse_fun(int a, int b) {
        a = a - 1;

        if (a != 0) {
            a = recurse_fun(a, b);
        }
        else {
            a = a + 0;
        }

        return a;
    }
}
