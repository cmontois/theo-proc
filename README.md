# High Order Calculator

Implémenté par Clément MONTOIS (LA3)

Remarque: 
- 0 correspond à false et 1 à true
- Pour la lisibilité des exemples basiques les parties de DEBUG ont été désactivées mais elles sont activées de base. Des exemples dans la partie gestion des opérateurs vous montreront les parties DEBUG afin de témoigner de la génération de code

## DONE
- expressions comparaison (< / > / <= / => / == / !=)
- expressions logiques (NOT (!) / AND (&&) / OR (||))
- séparateurs ( {} / ; )
- mots clés ( if / else )

## TODO
- mot clé while


## CHANGELOG

### Version 2.0

Objectif:

Quelle(s) structure(s) est(sont) nécessaire(s) pour 
supporter l’implémentation des fonctions.
Apportez les modifications essentielles 
à l’analyseur syntaxique et à l’interpréteur pour 
prendre en charge l’implémentation des fonctions. 

### Version 1.7

Objectif:

Affichez le code généré avant son exécution. 

### Version 1.6

Objectif:


Modifiez votre interpréteur pour tracer 
les unités syntaxiques reconnues
A la génération (compilation du langage), 
rendez la trace conditionnée par la définition d’un symbole.

### Version 1.5

Objectif: 


Réalisez  les  tests  avec  captures  d’écran de  la  version  1.5 
(votre) montrant  le  bon fonctionnement des traitements 
conditionnels & itératifs.

```

```


#### If & Else

Condition if/else - if

```
hoc> if (1 <= 2) {a=1;} else {a=2;}
hoc> a
= 1
```


Condition if/else - else


```
hoc> if (2 <= 1) {a=1;} else {a=2;}
hoc> a
= 1
```

#### While

Not implemented yet.


## Méthodologie de développement

### Ajout d’une nouvelle structure

Cette structure permet nottament l'association d'un type et d'une fonction.

defSymbols.c
```
static struct {char *name; short type ; void (*vfunc)();} predefsOper[] = {
	"=",      ASG,varAssign,
    "+",      ADD,add,
    "-",      SUB,sub,
    "/",      DIV,Div,
    "*",      MUL,mul,   
	"^",  	  POW,power,
	"!",	  NOT,not_code,
	"&&",	  AND,and_code,
	"||",     OR,or_code,
	"<",	  LT,lt,
	"<=",	  LE,le,
	">",      GT,gt,
	">=",     GE,ge,
	"==",	  EQ,eq,
	"!=",     NE,ne,
	"if",     IF,if_code,
	"else",   EL,else_code,
	"while",  WHI,while_code,
    NULL,     0
};
```

Reconnaissance en tant que symbole:
- des opérateurs (normaux, logiques & comparaisons)
```
"=",      ASG,varAssign,
"+",      ADD,add,
"-",      SUB,sub,
"/",      DIV,Div,
"*",      MUL,mul,   
"^",  	  POW,power,
"!",	  NOT,not_code,
"&&",	  AND,and_code,
"||",     OR,or_code,
"<",	  LT,lt,
"<=",	  LE,le,
">",      GT,gt,
">=",     GE,ge,
"==",	  EQ,eq,
"!=",     NE,ne,
```

- des mots clés

```
"if",     IF,if_code,
"else",   EL,else_code,
"while",  WHI,while_code,
NULL,     0
```

### Déclaration d’un nouveau type pour reconnaitre un symbole avec une fonction void (*vfunc)();


hoc.h
```
typedef union {
    /**     valeur d'un token de type VARINT, NUMINT         */
    int     iValue;
    /**     valeur d'un token de type VARFLO, NUMFLO, UNDEF  */
    double  fValue;
    /**     valeur d'un token de type PREDEF                 */
    double (*func)();
	/**     valeur d'un token                                */
	void 	(*vfunc) ();
} value_t;
```

Ajout de la fonction void pour gérer les fonctions de type void appelées pour chaque symbole.

### Installation de la nouvelle structure

codeHoc.c
```
void installDefaultSymbols(void)
{
    int i;
    symbol_ptr_t sp;
    value_t  v;
    
    fprintf(stdout,"Loading: Float Constants ");
    for (i = 0; floConsts[i].constName != NULL;  i++) {
        v.fValue = floConsts[i].constValue;
        sp = installSymbol(floConsts[i].constName, CSTFLO, v);
    }
    fprintf(stdout,"(done)\nLoading: Float Functions ");
    for (i = 0; predefs[i].predefName != NULL; i++) {
        v.func = predefs[i].predefFuncPtr;
        sp = installSymbol(predefs[i].predefName, PREDEF, v);
    }
	fprintf(stdout,"(done)\nLoading: Predef Constants ");
    for (i = 0; predefsOper[i].name != NULL; i++) {
        v.vfunc = predefsOper[i].vfunc;
        sp = installSymbol(predefsOper[i].name,predefsOper[i].type, v);
    }
	
    fprintf(stdout,"(done).\n");
}
```

Ajout dans l’installation des symboles de la structure des opérandes.

### Déclaration des nouvelles fonctions

hoc.h
```
/******************************************************************************/
/**
 *  \fn         void if_code(void);
 *  \brief      
 */
void if_code(void);


/******************************************************************************/
/**
 *  \fn         void else_code(void);
 *  \brief     
 */
void else_code(void);

/******************************************************************************/
/**
 *  \fn         void while_code(void);
 *  \brief      
 */
void while_code(void);
```

### Ecriture du contenu des nouvelles fonctions

codeHoc.c
```
/******************************************************************************/
/**
 *  \fn         void if_code(void);
 *  \brief     
 */
void if_code(void){
	instMac* adr=PC; 
	printf("Execute adr +3 \n");
	execute(adr+3);
	

	data_t condition=pop();
	
	if(condition.fValue) {
		printf("Conditions is ok \n");
		execute(*((instMac**)adr));
		printf("adr=%p avec %p\n",adr+2,*((instMac**)(adr+2)));
		
	}
	else {
		printf("Conditions is not ok\n");
		execute(*((instMac**)(adr+1)));
		printf("adr=%p avec %p\n",adr+1,*((instMac**)(adr+1)));
	}
	
	PC=*((instMac**)(adr+2));
	printf("adr=%p avec %p\n",adr,*((instMac**)(adr)));
};
```

### Ajout des nouveaux token


hoc.y
```
%token ADD SUB DIV MUL POW NOT AND OR LE LT GT GE EQ NE IF EL WHI ASG
%token AO AF PV
%token PRINT
```

Vérification de l'installation des symboles à l'execution du programme
```
Loading: Predef Constants (done).
```

### Ajout de l’ordre de priorité des opérandes


hoc.y
```
/**
 *  \note       Associativité à gauche de même priorité
 */
%left ASG
%left OR
%left AND
%left LE LT GT GE EQ NE
%left ADD SUB

/**
 *  \note       Associativité à gauche de priorité superieure
 */

%left  MUL DIV
%left  NOT

/**
 *  \note       Surcharge de l'opération soustraction par le signe
 */
%left  UNARYMINUS

/**
 *  \note       Associativité à droite
 */
%right POW
```

On définit la priorité pour les nouveaux opérateurs par rapport aux autres.

### Ranger les symboles dans une même catégorie


hoc.y
```
symbolOp : ADD 
	|SUB 
	|MUL 
	|DIV 
	|POW 
	;
	
symbolComp : LE 
	|LT 
	|GE 
	|GT 
	|EQ 
	|NE 
	;

symbolBool : AND 
	|OR  
	;
symbolNot : NOT
	;
```

Rangement en différentes parties pour mieux reconnaître les opérateurs d’une même catégorie.

## Gestion des opérateurs et test

### Opérateurs classiques

#### Reconnaissance pour les symbolOp

hoc.y
```
expr symbolOp expr { code((instMac)$2->value.vfunc);}
```

#### Ajout des opérateurs dans le lexème de l’analyseur

hoc.l
```
oper 		[+^*\/-]
```
Reconnaissance des opérateurs + - / * ^


#### Ajout de nouvelles règles lexicales

hoc.l
```
{oper}		return _installIdentifier();
```
Installation des symboles

#### Test de la génération de code et de l'éxecution des SymbolOp

Opérateur "+"
```
hoc> 1+2
[yylex()        ] NAME_=[              ] @SYMB=[     0x1bd02b0] TYPE_=[        NUMINT]  VALUE=[             1]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x1bd02b0]
[yylex()        ] result : +NAME_=[             +] @SYMB=[     0x1bcb790] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x1bd0300] TYPE_=[        NUMINT]  VALUE=[             2]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[     0x1bd0300]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x4043c9]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x1bd02b0] TYPE_=[        NUMINT]  VALUE=[             1]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[     0x1bd0300] TYPE_=[        NUMINT]  VALUE=[             2]
[execute()      ] @INST=[      0x608de0] INST_=[      0x4043c9] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x4049f7] @PILE=[      0x608480]
= 3
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

Opérateur "-"
```
hoc> 2-1
[yylex()        ] NAME_=[              ] @SYMB=[     0x1bd0350] TYPE_=[        NUMINT]  VALUE=[             2]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x1bd0350]
[yylex()        ] result : -NAME_=[             -] @SYMB=[     0x1bcb7e0] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x1bd03a0] TYPE_=[        NUMINT]  VALUE=[             1]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[     0x1bd03a0]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x404405]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x1bd0350] TYPE_=[        NUMINT]  VALUE=[             2]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[     0x1bd03a0] TYPE_=[        NUMINT]  VALUE=[             1]
[execute()      ] @INST=[      0x608de0] INST_=[      0x404405] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x4049f7] @PILE=[      0x608480]
= 1
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

Opérateur "*"
```
hoc> 2*4
[yylex()        ] NAME_=[              ] @SYMB=[     0x1bd03f0] TYPE_=[        NUMINT]  VALUE=[             2]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x1bd03f0]
[yylex()        ] result : *NAME_=[             *] @SYMB=[     0x1bcb880] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x1bd0440] TYPE_=[        NUMINT]  VALUE=[             4]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[     0x1bd0440]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x404441]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x1bd03f0] TYPE_=[        NUMINT]  VALUE=[             2]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[     0x1bd0440] TYPE_=[        NUMINT]  VALUE=[             4]
[execute()      ] @INST=[      0x608de0] INST_=[      0x404441] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x4049f7] @PILE=[      0x608480]
= 8
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

Opérateur "/"
```
hoc> 9/2
[yylex()        ] NAME_=[              ] @SYMB=[     0x1bd0490] TYPE_=[        NUMINT]  VALUE=[             9]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x1bd0490]
[yylex()        ] result : /NAME_=[             /] @SYMB=[     0x1bcb830] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x1bd04e0] TYPE_=[        NUMINT]  VALUE=[             2]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[     0x1bd04e0]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x40447d]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x1bd0490] TYPE_=[        NUMINT]  VALUE=[             9]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[     0x1bd04e0] TYPE_=[        NUMINT]  VALUE=[             2]
[execute()      ] @INST=[      0x608de0] INST_=[      0x40447d] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x4049f7] @PILE=[      0x608480]
= 4.5
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
```

Opérateur "^"
```
hoc> 4^2
[yylex()        ] NAME_=[              ] @SYMB=[     0x2512170] TYPE_=[        NUMINT]  VALUE=[             4]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x2512170]
[yylex()        ] result : ^NAME_=[             ^] @SYMB=[     0x250d8d0] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x25121c0] TYPE_=[        NUMINT]  VALUE=[             2]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[     0x25121c0]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x4044e1]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x2512170] TYPE_=[        NUMINT]  VALUE=[             4]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[     0x25121c0] TYPE_=[        NUMINT]  VALUE=[             2]
[execute()      ] @INST=[      0x608de0] INST_=[      0x4044e1] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x4049f7] @PILE=[      0x608480]
= 16
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```


Opérateur "log"
```
hoc> log(1)
[yylex()        ] result : logNAME_=[           log] @SYMB=[     0x250d6f0] TYPE_=[        PREDEF]      FADDR=[ 0x400db0]
[yylex()        ] NAME_=[              ] @SYMB=[     0x2512210] TYPE_=[        NUMINT]  VALUE=[             1]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x2512210]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x40498a]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[     0x250d6f0]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x2512210] TYPE_=[        NUMINT]  VALUE=[             1]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x40498a] @PILE=[      0x608480]
[predef()       ] NAME_=[           log] @SYMB=[     0x250d6f0] TYPE_=[        PREDEF]  FADDR=[      0x400db0]
[execute()      ] @INST=[      0x608de0] INST_=[      0x4049f7] @PILE=[      0x608480]
= 0
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
```

### Opérateurs comparaisons

#### Reconnaissance pour les symbolComp

hoc.y
```
expr symbolComp expr {code((instMac)$2->value.vfunc);}
```

#### Ajout des opérateurs dans le lexème de l’analyseur

hoc.l
```
lt 		    "<"   
le			"<="
gt			">"
ge			">="
eq			"=="
ne			"!="
not			"!"
```

#### Ajout de nouvelles règles lexicales

hoc.l
```
{lt} 		return _installIdentifier();
{le} 		return _installIdentifier();
{gt} 		return _installIdentifier();
{ge} 		return _installIdentifier();
{eq} 		return _installIdentifier();
{ne} 		return _installIdentifier();
```

#### Test de la génération de code et de l'éxecution des SymbolOp

Opérateur "<"
```
hoc> 1 < 2
[yylex()        ] NAME_=[              ] @SYMB=[     0x15a9170] TYPE_=[        NUMINT]  VALUE=[             1]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x15a9170]
[yylex()        ] result : <NAME_=[             <] @SYMB=[     0x15a4a10] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x15a91c0] TYPE_=[        NUMINT]  VALUE=[             2]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[     0x15a91c0]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x404662]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x15a9170] TYPE_=[        NUMINT]  VALUE=[             1]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[     0x15a91c0] TYPE_=[        NUMINT]  VALUE=[             2]
[execute()      ] @INST=[      0x608de0] INST_=[      0x404662] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x4049f7] @PILE=[      0x608480]
= 1
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

Opérateur "<="
```
hoc> 2<=2
[yylex()        ] NAME_=[              ] @SYMB=[     0x15a9210] TYPE_=[        NUMINT]  VALUE=[             2]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x15a9210]
[yylex()        ] result : <=NAME_=[            <=] @SYMB=[     0x15a4a60] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x15a9260] TYPE_=[        NUMINT]  VALUE=[             2]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[     0x15a9260]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x4046ae]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x15a9210] TYPE_=[        NUMINT]  VALUE=[             2]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[     0x15a9260] TYPE_=[        NUMINT]  VALUE=[             2]
[execute()      ] @INST=[      0x608de0] INST_=[      0x4046ae] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x4049f7] @PILE=[      0x608480]
= 1
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

Opérateur ">"
```
hoc> 4 > 5
[yylex()        ] NAME_=[              ] @SYMB=[     0x16c1170] TYPE_=[        NUMINT]  VALUE=[             4]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x16c1170]
[yylex()        ] result : >NAME_=[             >] @SYMB=[     0x16bcab0] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x16c11c0] TYPE_=[        NUMINT]  VALUE=[             5]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[     0x16c11c0]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x4046fa]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x16c1170] TYPE_=[        NUMINT]  VALUE=[             4]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[     0x16c11c0] TYPE_=[        NUMINT]  VALUE=[             5]
[execute()      ] @INST=[      0x608de0] INST_=[      0x4046fa] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x4049f7] @PILE=[      0x608480]
= 0
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

Opérateur ">="
```
hoc> 8 >= 5
[yylex()        ] NAME_=[              ] @SYMB=[     0x16c1210] TYPE_=[        NUMINT]  VALUE=[             8]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x16c1210]
[yylex()        ] result : >=NAME_=[            >=] @SYMB=[     0x16bcb00] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x16c1260] TYPE_=[        NUMINT]  VALUE=[             5]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[     0x16c1260]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x404746]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x16c1210] TYPE_=[        NUMINT]  VALUE=[             8]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[     0x16c1260] TYPE_=[        NUMINT]  VALUE=[             5]
[execute()      ] @INST=[      0x608de0] INST_=[      0x404746] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x4049f7] @PILE=[      0x608480]
= 1
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

Opérateur "=="
```
hoc> 5 == 2
[yylex()        ] NAME_=[              ] @SYMB=[     0x16c12b0] TYPE_=[        NUMINT]  VALUE=[             5]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x16c12b0]
[yylex()        ] result : ==NAME_=[            ==] @SYMB=[     0x16bcb50] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x16c1300] TYPE_=[        NUMINT]  VALUE=[             2]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[     0x16c1300]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x404792]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x16c12b0] TYPE_=[        NUMINT]  VALUE=[             5]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[     0x16c1300] TYPE_=[        NUMINT]  VALUE=[             2]
[execute()      ] @INST=[      0x608de0] INST_=[      0x404792] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x4049f7] @PILE=[      0x608480]
= 0
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

Opérateur "!="
```
hoc> 6 != 13
[yylex()        ] NAME_=[              ] @SYMB=[     0x16c1350] TYPE_=[        NUMINT]  VALUE=[             6]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x16c1350]
[yylex()        ] result : !=NAME_=[            !=] @SYMB=[     0x16bcba0] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x16c13a0] TYPE_=[        NUMINT]  VALUE=[            13]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[     0x16c13a0]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x4047e4]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x16c1350] TYPE_=[        NUMINT]  VALUE=[             6]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[     0x16c13a0] TYPE_=[        NUMINT]  VALUE=[            13]
[execute()      ] @INST=[      0x608de0] INST_=[      0x4047e4] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x4049f7] @PILE=[      0x608480]
= 1
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

Opérateur "!"
```
hoc> !0
[yylex()        ] result : !NAME_=[             !] @SYMB=[     0x16bc920] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x16c1440] TYPE_=[        NUMINT]  VALUE=[             0]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x16c1440]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x40452e]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x16c1440] TYPE_=[        NUMINT]  VALUE=[             0]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x40452e] @PILE=[      0x608480]
[execute()      ] @INST=[      0x608dd8] INST_=[      0x4049f7] @PILE=[      0x608480]
= 1
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```


### Opérateurs booléens

#### Reconnaissance pour les symbolBool

Reconnaissance NOT AND & OR entre deux comparaisons (nommées bool)

hoc.y
```
bool : comp symbolBool comp {code((instMac)$2->value.vfunc);};


comp : expr symbolComp expr {code((instMac)$2->value.vfunc);}

expr : 
	| comp
	| bool

```

#### Ajout des opérateurs dans le lexème de l’analyseur

hoc.l
```
not			"!"
and 		"&&"
or	 		"||"
```

#### Ajout de nouvelles règles lexicales

hoc.l
```
{not} 		return _installIdentifier();
{and} 		return _installIdentifier();
{or} 		return _installIdentifier();
```

#### Test de la génération de code et de l'éxecution des SymbolOp

Opérateur "&&"
```
hoc> 4 > 2 && 8 > 4
[yylex()        ] NAME_=[              ] @SYMB=[      0x8b5210] TYPE_=[        NUMINT]  VALUE=[             4]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[      0x8b5210]
[yylex()        ] result : >NAME_=[             >] @SYMB=[      0x8b0ab0] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[      0x8b5260] TYPE_=[        NUMINT]  VALUE=[             2]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[      0x8b5260]
[yylex()        ] result : &&NAME_=[            &&] @SYMB=[      0x8b0970] TYPE_=[
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x4046fa]
[yylex()        ] NAME_=[              ] @SYMB=[      0x8b52b0] TYPE_=[        NUMINT]  VALUE=[             8]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[      0x8b52b0]
[yylex()        ] result : >NAME_=[             >] @SYMB=[      0x8b0ab0] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[      0x8b5300] TYPE_=[        NUMINT]  VALUE=[             4]
[code()-symb/fct] @PROG=[      0x608df8] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608e00] @CODE=[      0x8b5300]
[code()-symb/fct] @PROG=[      0x608e08] @CODE=[      0x4046fa]
[code()-symb/fct] @PROG=[      0x608e10] @CODE=[      0x40457a]
[code()-symb/fct] @PROG=[      0x608e18] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608e20] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[      0x8b5210] TYPE_=[        NUMINT]  VALUE=[             4]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[      0x8b5260] TYPE_=[        NUMINT]  VALUE=[             2]
[execute()      ] @INST=[      0x608de0] INST_=[      0x4046fa] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[      0x8b52b0] TYPE_=[        NUMINT]  VALUE=[             8]
[execute()      ] @INST=[      0x608df8] INST_=[      0x4040fc] @PILE=[      0x608488]
[intPush()      ] NAME_=[              ] @SYMB=[      0x8b5300] TYPE_=[        NUMINT]  VALUE=[             4]
[execute()      ] @INST=[      0x608e08] INST_=[      0x4046fa] @PILE=[      0x608490]
[execute()      ] @INST=[      0x608e10] INST_=[      0x40457a] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608e18] INST_=[      0x4049f7] @PILE=[      0x608480]
= 1
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

Opérateur "||"
```
hoc> 1==1 || 1 > 5
[yylex()        ] NAME_=[              ] @SYMB=[     0x1b86170] TYPE_=[        NUMINT]  VALUE=[             1]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[     0x1b86170]
[yylex()        ] result : ==NAME_=[            ==] @SYMB=[     0x1b81b50] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x1b861c0] TYPE_=[        NUMINT]  VALUE=[             1]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[     0x1b861c0]
[yylex()        ] result : ||NAME_=[            ||] @SYMB=[     0x1b819c0] TYPE_=[
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x404792]
[yylex()        ] NAME_=[              ] @SYMB=[     0x1b86210] TYPE_=[        NUMINT]  VALUE=[             1]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[     0x1b86210]
[yylex()        ] result : >NAME_=[             >] @SYMB=[     0x1b81ab0] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[     0x1b86260] TYPE_=[        NUMINT]  VALUE=[             5]
[code()-symb/fct] @PROG=[      0x608df8] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608e00] @CODE=[     0x1b86260]
[code()-symb/fct] @PROG=[      0x608e08] @CODE=[      0x4046fa]
[code()-symb/fct] @PROG=[      0x608e10] @CODE=[      0x4045ee]
[code()-symb/fct] @PROG=[      0x608e18] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608e20] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[     0x1b86170] TYPE_=[        NUMINT]  VALUE=[             1]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[     0x1b861c0] TYPE_=[        NUMINT]  VALUE=[             1]
[execute()      ] @INST=[      0x608de0] INST_=[      0x404792] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[     0x1b86210] TYPE_=[        NUMINT]  VALUE=[             1]
[execute()      ] @INST=[      0x608df8] INST_=[      0x4040fc] @PILE=[      0x608488]
[intPush()      ] NAME_=[              ] @SYMB=[     0x1b86260] TYPE_=[        NUMINT]  VALUE=[             5]
[execute()      ] @INST=[      0x608e08] INST_=[      0x4046fa] @PILE=[      0x608490]
[execute()      ] @INST=[      0x608e10] INST_=[      0x4045ee] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608e18] INST_=[      0x4049f7] @PILE=[      0x608480]
= 1
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

Opérateur "!"
```
hoc> ! (4 < 2)
[yylex()        ] result : !NAME_=[             !] @SYMB=[      0x8b0920] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[      0x8b5170] TYPE_=[        NUMINT]  VALUE=[             4]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[      0x8b5170]
[yylex()        ] result : <NAME_=[             <] @SYMB=[      0x8b0a10] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[      0x8b51c0] TYPE_=[        NUMINT]  VALUE=[             2]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[      0x8b51c0]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x404662]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0x40452e]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608df8] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[      0x8b5170] TYPE_=[        NUMINT]  VALUE=[             4]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[      0x8b51c0] TYPE_=[        NUMINT]  VALUE=[             2]
[execute()      ] @INST=[      0x608de0] INST_=[      0x404662] @PILE=[      0x608488]
[execute()      ] @INST=[      0x608de8] INST_=[      0x40452e] @PILE=[      0x608480]
[execute()      ] @INST=[      0x608df0] INST_=[      0x4049f7] @PILE=[      0x608480]
= 1
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

### Conditions

#### Reconnaissance pour les symbolBool

hoc.y
```
stmt : assgn
	| expr 
	| condStmt
	| stmtList
    | PRINT { code(printSymbolList); }
    ;
	
stmtList : vide
	| stmt PV
	| AO stmtList stmt AF
```

#### Ajout des opérateurs dans le lexème de l’analyseur

hoc.l
```
if			"(if)"
else		"(else)"
while		"(while)"
```

#### Ajout de nouvelles règles lexicales

hoc.l
```
{if}		return _installIdentifier();
{else}		return _installIdentifier();
{while}		return _installIdentifier();
```

#### Test de la génération de code et de l'éxecution des SymbolOp

Opérateur "if"
```
hoc> if ( 4 > 2 ) { a = 8; }
[yylex()        ] result : ifNAME_=[            if] @SYMB=[      0xc18bf0] TYPE_=[
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x404836]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[         (nil)]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[         (nil)]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[         (nil)]
if code adr 0x608dc0
[yylex()        ] NAME_=[              ] @SYMB=[      0xc1d170] TYPE_=[        NUMINT]  VALUE=[             4]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0xc1d170]
[yylex()        ] result : >NAME_=[             >] @SYMB=[      0xc18ab0] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[      0xc1d1c0] TYPE_=[        NUMINT]  VALUE=[             2]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608df8] @CODE=[      0xc1d1c0]
[code()-symb/fct] @PROG=[      0x608e00] @CODE=[      0x4046fa]
[code()-symb/fct] @PROG=[      0x608e08] @CODE=[         (nil)]
adr=0x608dc8 avec 0x608e10
[yylex()        ] result : aNAME_=[             a] @SYMB=[      0xc1d210] TYPE_=[        UNDEF_]
[code()-symb/fct] @PROG=[      0x608e10] @CODE=[      0x4041a0]
[code()-symb/fct] @PROG=[      0x608e18] @CODE=[      0xc1d210]
[yylex()        ] result : =NAME_=[             =] @SYMB=[      0xc18740] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[      0xc1d260] TYPE_=[        NUMINT]  VALUE=[             8]
[code()-symb/fct] @PROG=[      0x608e20] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608e28] @CODE=[      0xc1d260]
[code()-symb/fct] @PROG=[      0x608e30] @CODE=[      0x404306]
[code()-symb/fct] @PROG=[      0x608e38] @CODE=[         (nil)]
adr=0x608dd8 avec 0x608e40
[code()-symb/fct] @PROG=[      0x608e40] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x404836] @PILE=[      0x608478]
Execute adr +3
[execute()      ] @INST=[      0x608de0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[      0xc1d170] TYPE_=[        NUMINT]  VALUE=[             4]
[execute()      ] @INST=[      0x608df0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[      0xc1d1c0] TYPE_=[        NUMINT]  VALUE=[             2]
[execute()      ] @INST=[      0x608e00] INST_=[      0x4046fa] @PILE=[      0x608488]
Conditions is ok
[execute()      ] @INST=[      0x608e10] INST_=[      0x4041a0] @PILE=[      0x608478]
[varPush()      ] NAME_=[             a] @SYMB=[      0xc1d210] TYPE_=[        UNDEF_]
[execute()      ] @INST=[      0x608e20] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[      0xc1d260] TYPE_=[        NUMINT]  VALUE=[             8]
[execute()      ] @INST=[      0x608e30] INST_=[      0x404306] @PILE=[      0x608488]
[varAssign()    ] NAME_=[             a] @SYMB=[      0xc1d210] TYPE_=[        UNDEF_]
adr=0x608dd8 avec 0x608e40
adr=0x608dc8 avec 0x608e10
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc> a
[yylex()        ] result : aNAME_=[             a] @SYMB=[      0xc1d210] TYPE_=[        VARFLO]        VALUE=[        8]
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x4041a0]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[      0xc1d210]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[      0x404254]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x4041a0] @PILE=[      0x608478]
[varPush()      ] NAME_=[             a] @SYMB=[      0xc1d210] TYPE_=[        VARFLO]  VALUE=[             8]
[execute()      ] @INST=[      0x608dd0] INST_=[      0x404254] @PILE=[      0x608480]
[varEval()      ] NAME_=[             a] @SYMB=[      0xc1d210] TYPE_=[        VARFLO]  VALUE=[             8]
[execute()      ] @INST=[      0x608dd8] INST_=[      0x4049f7] @PILE=[      0x608480]
= 8
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

Opérateur "if/else"
```
hoc> if (1 <= 2) {a=1;} else {a=2;}
[yylex()        ] result : ifNAME_=[            if] @SYMB=[      0xc18bf0] TYPE_=[
[code()-symb/fct] @PROG=[      0x608dc0] @CODE=[      0x404836]
[code()-symb/fct] @PROG=[      0x608dc8] @CODE=[         (nil)]
[code()-symb/fct] @PROG=[      0x608dd0] @CODE=[         (nil)]
[code()-symb/fct] @PROG=[      0x608dd8] @CODE=[         (nil)]
if code adr 0x608dc0
[yylex()        ] NAME_=[              ] @SYMB=[      0xc1d670] TYPE_=[        NUMINT]  VALUE=[             1]
[code()-symb/fct] @PROG=[      0x608de0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608de8] @CODE=[      0xc1d670]
[yylex()        ] result : <=NAME_=[            <=] @SYMB=[      0xc18a60] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[      0xc1d6c0] TYPE_=[        NUMINT]  VALUE=[             2]
[code()-symb/fct] @PROG=[      0x608df0] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608df8] @CODE=[      0xc1d6c0]
[code()-symb/fct] @PROG=[      0x608e00] @CODE=[      0x4046ae]
[code()-symb/fct] @PROG=[      0x608e08] @CODE=[         (nil)]
adr=0x608dc8 avec 0x608e10
[yylex()        ] result : aNAME_=[             a] @SYMB=[      0xc1d210] TYPE_=[        VARFLO]        VALUE=[        1]
[code()-symb/fct] @PROG=[      0x608e10] @CODE=[      0x4041a0]
[code()-symb/fct] @PROG=[      0x608e18] @CODE=[      0xc1d210]
[yylex()        ] result : =NAME_=[             =] @SYMB=[      0xc18740] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[      0xc1d710] TYPE_=[        NUMINT]  VALUE=[             1]
[code()-symb/fct] @PROG=[      0x608e20] @CODE=[      0x4040fc]
[code()-symb/fct] @PROG=[      0x608e28] @CODE=[      0xc1d710]
[code()-symb/fct] @PROG=[      0x608e30] @CODE=[      0x404306]
[yylex()        ] result : elseNAME_=[          else] @SYMB=[      0xc18c40] TYPE_=[
[code()-symb/fct] @PROG=[      0x608e38] @CODE=[         (nil)]
adr=0x608dd8 avec 0x608e40
[SYTX-E-000: yyparse    ] #syntax error#
[yylex()        ] result : aNAME_=[             a] @SYMB=[      0xc1d210] TYPE_=[        VARFLO]        VALUE=[        1]
[yylex()        ] result : =NAME_=[             =] @SYMB=[      0xc18740] TYPE_=[
[yylex()        ] NAME_=[              ] @SYMB=[      0xc1d760] TYPE_=[        NUMINT]  VALUE=[             2]
hoc> a
[yylex()        ] result : aNAME_=[             a] @SYMB=[      0xc1d210] TYPE_=[        VARFLO]        VALUE=[        1]
[code()-symb/fct] @PROG=[      0x608e40] @CODE=[      0x4041a0]
[code()-symb/fct] @PROG=[      0x608e48] @CODE=[      0xc1d210]
[code()-symb/fct] @PROG=[      0x608e50] @CODE=[      0x404254]
[code()-symb/fct] @PROG=[      0x608e58] @CODE=[      0x4049f7]
[code()-symb/fct] @PROG=[      0x608e60] @CODE=[         (nil)]
[execute()      ] @INST=[      0x608dc0] INST_=[      0x404836] @PILE=[      0x608478]
Execute adr +3
[execute()      ] @INST=[      0x608de0] INST_=[      0x4040fc] @PILE=[      0x608478]
[intPush()      ] NAME_=[              ] @SYMB=[      0xc1d670] TYPE_=[        NUMINT]  VALUE=[             1]
[execute()      ] @INST=[      0x608df0] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[      0xc1d6c0] TYPE_=[        NUMINT]  VALUE=[             2]
[execute()      ] @INST=[      0x608e00] INST_=[      0x4046ae] @PILE=[      0x608488]
Conditions is ok
[execute()      ] @INST=[      0x608e10] INST_=[      0x4041a0] @PILE=[      0x608478]
[varPush()      ] NAME_=[             a] @SYMB=[      0xc1d210] TYPE_=[        VARFLO]  VALUE=[             1]
[execute()      ] @INST=[      0x608e20] INST_=[      0x4040fc] @PILE=[      0x608480]
[intPush()      ] NAME_=[              ] @SYMB=[      0xc1d710] TYPE_=[        NUMINT]  VALUE=[             1]
[execute()      ] @INST=[      0x608e30] INST_=[      0x404306] @PILE=[      0x608488]
[varAssign()    ] NAME_=[             a] @SYMB=[      0xc1d210] TYPE_=[        VARFLO]  VALUE=[             1]
adr=0x608dd8 avec 0x608e40
adr=0x608dc8 avec 0x608e10
[execute()      ] @INST=[      0x608e40] INST_=[      0x4041a0] @PILE=[      0x608478]
[varPush()      ] NAME_=[             a] @SYMB=[      0xc1d210] TYPE_=[        VARFLO]  VALUE=[             1]
[execute()      ] @INST=[      0x608e50] INST_=[      0x404254] @PILE=[      0x608480]
[varEval()      ] NAME_=[             a] @SYMB=[      0xc1d210] TYPE_=[        VARFLO]  VALUE=[             1]
[execute()      ] @INST=[      0x608e58] INST_=[      0x4049f7] @PILE=[      0x608480]
= 1
[initCode()     ] @PROG=[      0x608dc0] @PILE=[      0x608480]
hoc>
```

Opérateur "while" (Not implemented yet)
```
```