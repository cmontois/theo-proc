/*############################################################################*/
/*      D E C L A R A T I O N  DES  S Y M B O L E S  P R E D E F I N I S      */
/******************************************************************************/
/**
 *  \file       defSymbol.c
 *  \brief      Fichier de déclarations des symboles prédéfinis
 *              <LI>Constantes réelles</LI>
 *              <LI>Foction mathématiques prédéfinies</LI>
 *
 *  \date       21 Décembre 2018
 *  \author     Samir El Khattabi
 *  \version    1.0
 *
 */
/******************************************************************************/
/*     V A R I A B L Es  G L O B A L Es / S T A T I Q U E S                   */
/******************************************************************************/
/**
 *  \var        Déclaration d'une table de constantes numériques
 *              Chaque constante est définie par un nom et sa valeur
 */
static struct {
    char *constName; double constValue;
} floConsts[] = {
    "PI",       3.14159265358979323846,
    "E",        2.71828182845904523536,
    "GAMMA",    0.57721566490153286060,
    "DEG",      57.29577951308232087680,
    "PHI",      1.61803398874989484820,
    NULL,       0
};
/******************************************************************************/
/**
 *  \var        Déclaration d'une table de fonctions numériques prédéfinies
 *              Chaque constante est définie par un nom et sa valeur
 */
static struct {
    char *predefName; double (*predefFuncPtr) (double);
} predefs[] = {
     "sin",      sin,
     "cos",      cos,
     "tg",       tan,
     "abs",      fabs,
     /** fonction wrap du log en vue de la vérification du type de l'argument */
     "log",      log,
     NULL,     0
};

/******************************************************************************/
/**
 *  \var        Déclaration d'une table d'operateur et mots clé prédéfinies
 *              Chaque constante est définie par un nom et sa valeur
 */

 static struct {
    char *name; short type ; void (*vfunc)();
} predefsOper[] = {
	 "=",      ASG,varAssign,
     "+",      ADD,add,
     "-",      SUB,sub,
     "/",      DIV,Div,
     "*",      MUL,mul,   
	 "^",  	   POW,power,
	 "!",	   NOT,not_code,
	 "&&",	   AND,and_code,
	 "||",     OR,or_code,
	 "<",	   LT,lt,
	 "<=",	   LE,le,
	 ">",      GT,gt,
	 ">=",     GE,ge,
	 "==",	   EQ,eq,
	 "!=",     NE,ne,
	 "if",     IF,if_code,
	 "else",   EL,else_code,
	 "while",  WHI,while_code,
     NULL,     0
};

 

/*------\\//------\\//------\\//------\\//------\\//------\\//------\\/ F I N */
