// exit 0
// output 5 5.5 asdf 5 5 asdf 1 5.5 asdf 5 2.5 asdf 5 5.5 asdf 1 2.5 asdf 1 1 asdf end
class Main {
    static void run() {
       int x = 1;
       double y = 2.5;
       String z = "asdf";

       my_fun(5, 5.5, "asdf");
       my_fun(5, 5, "asdf");

       my_fun(x, 5.5, "asdf");
       my_fun(5, y, "asdf");
       my_fun(5, 5.5, z);
       my_fun(x, y, z);

       my_fun(x, x, z);

       ifj16.print("end\n");
    }
    static void my_fun(int a, double b, String c) {
        ifj16.print(a + " " + b + " " + c + " ");
    }
}
