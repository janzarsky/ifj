// exit 0
// output 0 1 15 10
class Main {
    static void run() {
        hero.approval();
    }
}

class mages {
    static int morrigan() {
        return 0;
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
        int approves;
        approves = mages.morrigan();
        ifj16.print(approves + " ");
        approves = mages.wynne();
        ifj16.print(approves + " ");
        approves = mages.anders();
        ifj16.print(approves + " ");
        approves = mages.bethany();
        ifj16.print(approves + "\n");
    }
}
