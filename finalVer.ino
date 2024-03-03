#include <Adafruit_NeoPixel.h> //Allumage des LEDs 
#include "TouchScreen.h"       // Tactile : position de l'appui et pression
 
//nombre de LEDs
int X=120;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(X, 6, NEO_GRB + NEO_KHZ800);
// Parameter 32 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


//Entrées de la plaque tactile 
#define YP A0  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM A1   // can be a digital pin
#define XP A2  // can be a digital pin

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 0);

const int bouton = 2;
const int rougeB = 3;
const int verteB = 4;
const int bleueB = 5;

int game = 1;
int val = 0;
int ancien_val = 1;

int nbPlayer = 0;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//affiche l'ecran pour choisir le jeu/le nombre de joueur
int choice[46] = {5,16,27,38,49,60,71,82,93,104,115, 13,30,31,35,52,56,57,58,78,79,80,95,97,100,101,102, 18,19,20,23,40,41,42,47,62,63,64,84,85,86,89,91,106,107,108};

//numéro des LEDs à allumer pour l'affichage de l'écran de victoire 
int winOrange[19] = {54,55,76,78,74,80,72,59,50, 48,61,70,83,62,68,67,64,45,86};
int winJaune[7] = {49,60,71,82,44,65,87};
int winRouge[22] = {32,31,30,29,28,27,26,25,24,23,22, 99,100,101,102,103,104,105,106,107,108,109};
//certaines LEDs seront à éteindre après l'affichage de l'écran de victoire 
int winDelete[9] = {0,22,44,45,66,88, 109,87,65};
int tieDelete[11] = {0,22,44,45,66,88, 109,87,65, 21,43};

int loseVert[17] = {33,54,55,76,77,78,40,38,39,49,60,61,62,69,84,83,82};
int loseBleu[22] = {35,36,37,50,52,57,59,72,74,79,80,81,41,42,43,46,63,64,68,85,86,87};

int tieRouge[22] = {99,100,101,102,103,104,105,106,107,108,109, 11,12,13,14,15,16,17,18,19,20,21};
int tieOrange[17] ={33,34,35,53,56,75,78, 39,40,41,48,61,62,70,83,84,85};
int tieJaune[13] = {36,37,38,50,59,72,80,81,82, 43,44,65,87};


int casesAllumees[9] = {10, 10, 10, 10, 10, 10, 10, 10, 10};  // liste de toutes les cases allumées 
int casesJ1[9] = {10, 10, 10, 10, 10, 10, 10, 10, 10};        // liste des cases allumées par le J1
int casesJ2[9] = {10, 10, 10, 10, 10, 10, 10, 10, 10};        // liste des cases allumées par le J2


int grille[65] = {1,2,3,4,5,6,7,8,9,10,11,14,17,20,23 ,32, 33, 29, 26, 34, 35, 36, 37, 38, 39, 40, 41, 42, 45, 48, 51, 54, 55, 58, 61, 64, 67, 68, 69, 70, 71, 72, 73, 74, 75 ,76, 77, 80, 83, 86, 89, 92, 95, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108};

//pour chaque case allumable : {x_min,x_max, y_min,y_max, LEDs qui la constitue}
int cases[9][8] = {{500,600,  680,810, 18,19,24,25},
                  {370,470,  680,810, 16,15,27,28},
                  {200,300,  680,810, 12,13,31,30},
                  {500,600,  460,580, 47,46,63,62}, 
                  {370,470,  460,580, 50,49,60,59},
                  {200,300,  460,580, 53,52,57,56},
                  {500,600,  230,350, 85,84,91,90},
                  {370,470,  230,350, 82,81,94,93},
                  {200,300,  230,350, 79,78,97,96}};


int Score1[5] = {21,22,43,44,65};  //liste des LEDs où afficher le score du J1
int Score2[5] = {88,87,66,65,44};  //liste des LEDs où afficher le score du J2



//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Souvenir[100] = {0,0,1,1,0,1,0,1,0,0,0,1,1,0,0,0,1,1,1,0,1,0,0,1,0,0,1,1,0,1,0,1,0,1,0,0,0,1,0,1, 1, 1, 1, 1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8,7};

//chaque combinaison de cases permettant de gagner
int combiWin[8][3]= {{0,1,2}, 
                  {3,4,5},
                  {6,7,8},
                  {0,3,6},
                  {1,4,7},
                  {2,5,8},
                  {2,4,6},
                  {0,4,8}};


int score = 1;
bool ok = true;
int allumee;
int current = 0;
int x; 
int y;
int index;
int ind;


int scoreJ1 = 0;  //enregistre le score du J1
int scoreJ2 = 0; //enregistre le score du J2

bool caseOk=true;  //permet de vérifier si une case n'est pas déjà allumée 

int joueur = -1;   // 1 = J1 (jaune) et -1 = J2 (rouge)


//-------------------------------------------------------------AFFICHAGE----------------------------------------------------------------------//


void resetAll(){
  for (int i = 0; i<121; i++){
    strip.setPixelColor(i, 0,0,0);
  }
  strip.show();
}

//permet d'éteindre les cases pour recommencer une manche
void reset(){
  for (int i=0; i<9; i++){
    for (int j=4; j<8; j++){
      strip.setPixelColor(cases[i][j], 0,0,0);
    }
  }
  strip.show();
}

//permet d'éteindre les LEDs affichant le score pour recommencer une partie
void resetScore(){
  for (int i = 0; i<5; i++){
    strip.setPixelColor(Score1[i], 0,0,0);
    strip.setPixelColor(Score2[i], 0,0,0);
  }
}

void affichage1P(){
  for (int i = 0; i<65; i++){
   strip.setPixelColor(grille[i], 0,0,255); 
  }
  strip.setPixelColor(0, 0,0,0);
  strip.show();
}

//allume en début de manche la grille/plateau (en orange)
void affichage2P(){
  for(int i = 0; i < 65; i++ ) { 
    strip.setPixelColor(grille[i], 255,69,0);       
  } 
  strip.setPixelColor(0, 255,69, 0);
  strip.setPixelColor(109, 255,69, 0);
  strip.show(); // on affiche 
}

//allume les LEDs qui montre le score des joueurs 
void afficheScore(){
  for (int j=0; j<scoreJ1; j++){
    strip.setPixelColor(Score1[j], 255, 255, 0);
  }
  for (int k=0; k<scoreJ2; k++){
    strip.setPixelColor(Score2[k], 255, 0, 0);
  }
  strip.show();
}

void affichageLose(){
  strip.clear();
  for (int i=0; i<22; i++){
    strip.setPixelColor(loseBleu[i], 0,0,255);
  }
  for (int i=0; i<17; i++){
    strip.setPixelColor(loseVert[i], 0,255,0);
  }
  strip.show();
  delay(3000);
  resetAll();
  for(int i=0; i<score; i++){
    strip.setPixelColor(i, 0,255,0);
    delay(100);
    strip.show();
  }
  delay(3000);
}

//affiche un écran de victoire quand une manche est gagnée
void affichageWin(){
  strip.clear();
  for (int i=0; i<19; i++){
    strip.setPixelColor(winOrange[i], 255,69,0);
  }
  for (int i=0; i<7; i++){
    strip.setPixelColor(winJaune[i], 255,255,0);
  }
  for (int i=0; i<22; i++){
    strip.setPixelColor(winRouge[i], 255,0,0);
  }
  strip.show();
  delay(3000);
  for (int i=0; i<9; i++){
    strip.setPixelColor(winDelete[i], 0,0,0);
  }
  affichage2P();
  afficheScore();
}

// affiche TIE ! lors d'une égalité
void affichageTie(){
  strip.clear();
  for (int i=0; i<17; i++){
    strip.setPixelColor(tieOrange[i], 255,69,0);
  }
  for (int i=0; i<13; i++){
    strip.setPixelColor(tieJaune[i], 255,255,0);
  }
  for (int i=0; i<22; i++){
    strip.setPixelColor(tieRouge[i], 255,0,0);
  }
  strip.show();
  delay(3000);
  for (int i=0; i<11; i++){
    strip.setPixelColor(tieDelete[i], 0,0,0);
  }
  affichage2P();
  afficheScore();
}

//----------------------------------POSITION--------------------------------------------------------------------------------------------------------------------------------------------------


//prend une position en x et en y et renvoie l'index de la case correspondant à cette position
int indexCase(float x, float y){
  if (x>650 ||(y <679 && y>581)){
    return 10;
  }
  else{ 
    bool breakk = false;
    ind = 0;
    while (breakk == false){
      if (cases[ind][0]<x && x<cases[ind][1] && cases[ind][2]<y && y<cases[ind][3]){
        breakk = true;
        return ind;
      }
      ind += 1;
    }
  }
}


int posY(){
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  digitalWrite(16, HIGH);
  digitalWrite(17, LOW); 
  return analogRead(1); 
}

int posX(){
  pinMode(17, INPUT);
  pinMode(16, INPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  digitalWrite(14, HIGH);
  digitalWrite(15, LOW); 
  return analogRead(3); 
}



//--------------------CHOIX DU JEU------------------------------------------------------------------------------------------------------------------------------------------------------

void choix(){
  for (int i =0; i<11; i++){
    strip.setPixelColor(choice[i], 255,0,0);
  }
  for (int i =11; i<27; i++){
    strip.setPixelColor(choice[i], 255,69,0);
  }
  for (int i =27; i<46; i++){
    strip.setPixelColor(choice[i], 0,0,255);
  }
  strip.show();
  x = posX();
  y = posY();
  if (x<650 && x>600  && y<270 && y>200){
    nbPlayer = 2;
  }
  else if (x<300 && x>250  && y<270 && y>200){
    nbPlayer = 1;
  }
  
}


//-------------------JEU 1P---------------------------------------------------------------------------------------------------------------------------------------

void listeRandom(){
  for (int i=0; i<100; i++){
    Souvenir[i] = random (9);
  }
}

void verifyCase1P(int index, int cur){
  if (index<9 && index>=0){
    if (index==Souvenir[cur]){
      current +=1;
      for (int i=4; i<8; i++){
        strip.setPixelColor(cases[index][i], 255,0,0);
        strip.show();
      }
      delay(100);
      reset();
      if (current == score){
        ok = true;
        score += 1;
      }
    }
    else{
      affichageLose();
      score = 1;
      current = 0;
      listeRandom();
      ok = true;
      reset();
    }
  } 
}

void off1P(){
  listeRandom();
  reset();
  score = 1;
  ok = true;
  current =0;
  resetScore();
  for (int i=0; i<65; i++){
    strip.setPixelColor(grille[i], 0,0,0);
  }
  strip.setPixelColor(109, 0,0, 0);
  strip.show();
}

//-------------------JEU 2P---------------------------------------------------------------------------------------------------------------------------------------


//vérifie si une case est jouable, c'est-à-dire qu'elle n'est pas allumées
void verifyCase2P(int index){
  for (int i =0; i<9; i++){
    if (index == casesAllumees[i] || index>8){
      caseOk = false;
    }
  }
}


// remplace un 10 dans les listes des cases allumées par la case qui vient d'être jouée.
void caseJouee(int index){
  int index10 = 0;
  //on remplace d'abord dans la liste de toutes les cases allumées.
  for (int i=0; i<9; i++){
    if (casesAllumees[i]>=10){ 
      index10=i;
    }
  } 
  casesAllumees[index10] = index;
  //ensuite on remplace dans la liste du joueur qui vient de jouer
  if (joueur == 1){
    casesJ1[index10]=index;
  }
  else{
    casesJ2[index10]=index;
  }
}


//remplit les listes des cases jouées par des 10 pour recommencer la partie.
void cases10(){
  for (int i=0; i<9; i++){
    casesAllumees[i] =10;
    casesJ1[i] =10;
    casesJ2[i]=10;
  }
}

void verifyList(){
  for (int i =0; i<9; i++){
    if (casesAllumees[i]>10){
      casesAllumees[i]=10;
    }
  }
}

//--------------------FIN DE PARTIE----------------------//


//vérifie si un joueur a gagné ou si toutes les cases sont allumées sans gagnant : renvoie true sinon false.
bool verifyEnd(){

  // On vérifie si un joueur a gagné : on parcourt la liste des combinaisons gagnantes et on vérifie si une d'entres elles apparaît dans la liste des cases allumées par un joueur.
  for (int i=0; i<8; i++){
    int ok =0;
    for (int j=0; j<3; j++){
      for (int k=0; k<9; k++){
        if (joueur == 1 && combiWin[i][j]==casesJ1[k]){
          ok += 1;
          if (ok==3){
            cases10();
            scoreJ1 += 1;
            affichageWin();
            if (scoreJ1 == 5){
              joueur = -1;
              scoreJ1 = 0;
              scoreJ2 = 0;
              resetScore();
            }
            return true;
          }
        }
        else if (joueur == -1 && combiWin[i][j]==casesJ2[k]){
          ok += 1;
          if (ok==3){
            cases10();
            scoreJ2 += 1;
            affichageWin();
            if (scoreJ2 == 5){
              joueur = -1;
              scoreJ2 = 0;
              scoreJ1 = 0;
              resetScore();
            }
            return true;
          }
        }
      }
    }
  }
  // on vérifie ensuite si toutes les cases sont allumées : dans ce cas personne ne peut gagner : on met fin à la partie 
  int count = 0;
  for (int h = 0; h<9; h++){
    if (casesAllumees[h]<10){
      count += 1;
    }
  }
  if (count ==9){
    cases10();
    affichageTie();
    return true;
  }
  // on return false : la partie n'est pas finie.
  return false;
}


void off2P(){
  cases10();
  reset();
  scoreJ1 = 0;
  scoreJ2 = 0;
  resetScore();
  for (int i=0; i<65; i++){
    strip.setPixelColor(grille[i], 0,0,0);
  }
  strip.setPixelColor(109, 0,0, 0);
  strip.show();
}



//------------------------------SETUP-------------------------------------------------------------------------------------------------------------

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  Serial.begin(9600);
  pinMode(bouton,INPUT);
  pinMode(4, OUTPUT);
  strip.show(); // Initialise toute les led à 'off'
  digitalWrite(4, HIGH);
  randomSeed(analogRead(5));
  listeRandom();
}


//----------LOOP-------------------------------------------------------------------------------------------------------------------------------------------------



void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(bouton);
  if ((val == LOW)&&(ancien_val==HIGH)){
    game = 1-game;
    delay(20);
  }
  ancien_val = val;
  if (game ==0){
    if (nbPlayer ==1){
      off1P();
    }
    else{
      off2P();
    }
    nbPlayer=0;
  }
  else{

    if (nbPlayer == 0){
      choix();
      resetAll();
    }
    else{

      //jeu 1P
      if (nbPlayer==1){

        affichage1P();
        if (ok==true){
          current = 0;
          for (int j=0; j<score;j++){
            allumee = Souvenir[j];
            for (int i=4; i<8; i++){
              strip.setPixelColor(cases[allumee][i], 0,255,0);
              strip.show();
            }
            strip.show();
            delay(1500);
            reset();
            delay(200);
          }
        }
        ok = false;
        x = posX();
        y = posY();
        index = indexCase(x,y);
        verifyCase1P(index, current);
      }

      //jeu 2P Morpion
      else{ 

        caseOk = true;
        affichage2P();
        afficheScore();
        x = posX();
        y = posY();
        int index = indexCase(x,y);
        verifyCase2P(index);
        if (caseOk==true){
          joueur = -joueur;
          caseJouee(index);
          for (int j=4; j<8;j++){
            if (joueur == 1){
              strip.setPixelColor(cases[index][j], 255,255,0);
            }
            else{
              strip.setPixelColor(cases[index][j], 255,0,0);
            }
          } 
        }
        if (verifyEnd()==true){
          reset();
        } 
      }
    }

  }

}
