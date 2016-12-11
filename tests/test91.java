// exit 0
class Main
{
	static void run(){

	   Cullen.attacked();

	}
}


class Cullen{


static int c_health = 100;
static int c_stamina = 100;
static int c_damage = 15;

static String weapon;
static String helmet;
static String armor;

static int c_alive = 1;

static void attacked(){

while (c_alive == 1) {
        c_health = c_health - pride_demon.d_damage;
        pride_demon.d_health = pride_demon.d_health - c_damage;

        if (c_health <= 0 ) { c_alive = 0; }
	else {ifj16.print("Cullen zije" + "\n");}
        if (pride_demon.d_health <= 0 ) {pride_demon.d_alive = 0;}
	else {ifj16.print("demon zije" + "\n");}
}
}
}

class pride_demon {

static int d_health = 90;
static int d_mana = 90;
static int d_damage = 17;

static int d_alive = 1;

}
