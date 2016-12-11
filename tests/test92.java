// exit 0
class Main
{
	static void run(){


       Cullen.equip_inquisitor_armor();
       Cullen.set_armor_stats();
	   Cullen.attacked();
	   Bethany.cullen_buff();

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

while (pride_demon.d_alive == 1) {
        c_health = c_health - pride_demon.d_damage;
        pride_demon.d_health = pride_demon.d_health - c_damage;

        if (c_health <= 20) {Bethany.heal_cullen();} else {}

        if (c_health <= 0 ) { c_alive = 0; }
	else {ifj16.print("Cullen zije" + "\n");}
        if (pride_demon.d_health <= 0 ) {pride_demon.d_alive = 0;}
	else {ifj16.print("demon zije" + "\n");}
}
}

static void equip_templar_armor(){

armor = "Templar Knight Commander Armor";

}

static void equip_inquisitor_armor(){

armor = "Inquisitor Armor";

}

static void set_armor_stats(){

 int result = ifj16.compare(armor, "Templar Knight Commander Armor");

if (result == 0) { c_health = c_health + 25; c_damage = c_damage + 5; } else {

    result = ifj16.compare(armor, "Inquisitor Armor");

    if (result == 0) {c_health = c_health + 5; c_damage = c_damage + 25;} else {}
}
}
}

class pride_demon {

static int d_health = 90;
static int d_mana = 90;
static int d_damage = 17;

static int d_alive = 1;

}

class Bethany{

static int b_health = 100;
static int b_damage = 100;

static void heal_cullen(){

Cullen.c_health = Cullen.c_health + 20;

}

static void heal_self(){

if (b_health <= 15) { b_health = b_health + 25;} else {}

}

static void cullen_buff(){

Cullen.c_health = Cullen.c_health + 10;
Cullen.c_damage = Cullen.c_damage + 7;

}

}
