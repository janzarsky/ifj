// exit 0
// output -5
class Main {
    static void run() {
        int a;
        double b;
        a = b + 5;
        hero.approval();
    }
}

class mages {
    static int morrigan() {
        return -1;
    }

    static int wynne () {
        return 1;
    }

    static int anders () {
        return 15;
    }

    static int bethany () {
        return 10;
    }
}

class hero {
    static void approval() {
        int approves = morrigan();
        approves = approves + wynne ();
        approves = approves - anders();
        approves = approves + bethany();
        ifj16.print(approves + "\n");
    }
}
