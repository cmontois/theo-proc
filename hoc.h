/**
 *  \file       hoc.h
 *  \brief      Fichier de spécifications globales de l'application
 *
 *  \date       21 Décembre 2018
 *  \author     Samir El Khattabi
 *  \version    1.0
 *
 */
#ifndef _HOC_H_
#define _HOC_H_
/******************************************************************************/
/*     I N C L U D Es  S T A N D A R Ds                                       */
/******************************************************************************/
#ifndef _STD_H_
#define _STD_H_
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#endif  //  _STD_H_
/******************************************************************************/
#if defined(_CODE_HOC_C_) // || defined(_HOC_Y_)
#ifndef _MATH_H_
#define _MATH_H_
#include <math.h>
#endif  //  _MATH_H_
#endif  //  Sélection de l'inclusion
/******************************************************************************/
#if defined(_IO_C_)
#ifndef _ERRNO_H_
#define _ERRNO_H_
#include <errno.h>
#endif  //  _ERRNO_H_
#endif  //  Sélection de l'inclusion
/******************************************************************************/
/*      C O N S T A N T E S   S Y M B O L I Q U E S   G L O B A L E S         */
/******************************************************************************/
#define COULEUR(param)  fprintf(stdout,"\033[%sm",param)
/*############################################################################*/
/*      D E F I N I T I O N S    D E     T Y P E S                            */
/*                  G E S T I O N  DES  S Y M B O L E S                       */
/******************************************************************************/
/**
 *  \typedef    value_t
 *  \brief      Structure UNION qui acceuille la valeur d'un token
 *  \note       On gére 3 types de valeurs.
 */
typedef union {
    /**     valeur d'un token de type VARINT, NUMINT                          */
    int     iValue;
    /**     valeur d'un token de type VARFLO, NUMFLO, UNDEF                   */
    double  fValue;
    /**     valeur d'un token de type PREDEF                                  */
    double (*func)();
	/**     valeur d'un token */
	void (*vfunc) ();
} value_t;
/******************************************************************************/
/**
 *  \typedef    Définition de la structure symbol_t et symbol_ptr_t
 *  \brief      Liste de symboles
 *  \note       Structure gérée en liste, avec ajout en tête de liste
 */
typedef struct symbol {
    /**     nom du symbole : identifiant                                      */
    char *name;
    /**     type du symbole : VARINT, CSTINT, NUMINT, VARFLO, CSTFLO, NUMFLO,
     *                        UNDEF, PREDEF,                                  */
    short type;
    /**     valeur du symbole : iValue, fValue, func                          */
    value_t value;
    /**     pointeur sur symbole suivant                                      */
    struct symbol *next;
}   symbol_t,* symbol_ptr_t;
/*############################################################################*/
/*      D E F I N I T I O N S    D E     T Y P E S                            */
/*          G E N E R A T I O N  &  E X E C U T I O N  DE  C O D E            */
#if defined(_CODE_C_) || defined(_CODE_HOC_C_) || defined(_HOC_Y_)
/******************************************************************************/
/**
 *  \union      data
 *  \brief      l'interpréteur manipule des valeurs ou des symboles
 */
union data {
    /**     valeur d'un donnée flottante : NUMFLO,                            */
    double       fValue;
    /**     valeur d'un donnée entière : NUMINT,                              */
    int          iValue;
    /**     adresse d'un symbole : VARINT, CSTINT, VARFLO, CSTFLO, UNDEF,
     *                             PREDEF,                                    */
   symbol_ptr_t symbol;
};
/**
 *  \typedef    data_t
 *  \brief      Définition de data_t :type de données de la pile de l'interpréteur
 */
typedef union data data_t;
/******************************************************************************/
/**
 *  \typedef    instMac
 *  \brief      Définition de instMac : instruction machine
 *  \note       Une instruction machine correpond à une adresse mémoire de
 *              logement du code à éxécuter
 *
 *              ICI, POUR l'INSTANT :
 *              on ne s'intéresse qu'à des appels de fonctions sans résultat
 *              PLUS TARD :
 *              des appels de fonctions avec résultat générique (void *)
 */
typedef void (*instMac) ();
/******************************************************************************/
#endif      //  _CODE_H_
/*############################################################################*/
/*      C O N S T A N T E S   S Y M B O L I Q U E S   S P E C I F I Q U Es    */
/*                  G E S T I O N  DES  S Y M B O L E S                       */
/******************************************************************************/
/**
 *  \def        SYMBOL_NULL
 *  \brief      Pointeur NULL sur symbole
 */
#define SYMBOL_NULL ((symbol_ptr_t) 0)
/******************************************************************************/
/**
 *  \def        YYSTYPE
 *  \brief      Type de la variavle externe yylval portant la valeur du token
 *              reconnu par l'AL
 */
#define YYSTYPE symbol_ptr_t
/*############################################################################*/
/*      C O N S T A N T E S   S Y M B O L I Q U E S   S P E C I F I Q U Es    */
/*          M A C H I N E   D'E X E C U T I O N  DE  C O D E                  */
/******************************************************************************/
/**
 *  \def        STOP
 *  \brief      Instruction de fin d'un bloc d'instructions
 */
#define STOP (instMac) 0
/******************************************************************************/
/*     I N C L U D Es  S P E C I F I Q U Es                                   */
/******************************************************************************/
#if !defined(_HOC_Y_)
    #ifndef _HOC_TAB_H_
        #define _HOC_TAB_H_
        #include "hoc.tab.h"
    #endif  //  _HOC_TAB_H_
#endif  //  Sélection des inclusions
/*############################################################################*/
/*     P R O T O T Y P E S   D E   F O N C T I O N S                          */
/*                  G E S T I O N  DES  S Y M B O L E S                       */
/******************************************************************************/
/**
 *  \fn         symbol_ptr_t installSymbol(char *tokenName,
 *                                         short tokenType, value_t tokenValue);
 *  \brief      Insérer un nouveau symbole en tête de la liste des symboles
 *  \note       Ce traitement est invoqué (généralement) par l'analyseur lexical
 *  \param      tokenName   nom du symbole à insérer
 *  \param      tokenType   nom du symbole à insérer
 *  \param      tokenValue   nom du symbole à insérer
 *  \return     pointeur sur le symbole inséré (nouvelle tête de liste)
 */
symbol_ptr_t installSymbol(char *tokenName, short tokenType, value_t tokenValue);
/******************************************************************************/
/**
 *  \fn         symbol_ptr_t lookUpSymbol(char *tokenName);
 *  \brief      Rechercher un symbole dans la liste des symboles
 *  \param      tokenName   nom du symbole recherché
 *  \return     pointeur sur le symbole recherché ou NULL si non trouvé
 */
symbol_ptr_t lookUpSymbol(char *tokenName);
/******************************************************************************/
/**
 *  \fn         void printSymbolList(void);
 *  \brief      Afficher les symboles définis
 */
void printSymbolList(void);
/*############################################################################*/
/*     P R O T O T Y P E S   D E   F O N C T I O N S                          */
/*                  G E S T I O N  DES  S O R T I E S                         */
#if !defined(_SYMBOL_C_)
/******************************************************************************/
/*  \fn         void dbgSymbol(symbol_ptr_t sp);
 *  \brief      Afficher les informations relatives à un symbole
 *  \param      sp  pointeur sur le symbole
 */
void dbgSymbol(symbol_ptr_t sp);
/******************************************************************************/
/**
 *  \fn         void prompt(void);
 *  \brief      Afficher le prompt
 */
void prompt(void);
/*############################################################################*/
/*     P R O T O T Y P E S   D E   F O N C T I O N S                          */
/*                  G E S T I O N  DES  E R R E U R S                         */
/******************************************************************************/
/**
 *  \fn         void yyerror(char *msg);
 *  \brief      Fonction invoquée par l'analyseur syntaxique
 *  \parm       msg     Message d'erreur
 */
void yyerror(char *msg);
/******************************************************************************/
/**
 *  \fn         void printError(char *tagErr, char *msgErr);
 *  \brief      Afficher un message d'erreur formaté
 *  \param      tagErr TYPE-SEVERITE-NUM
 *  \parm       msgErr  Message d'erreur
 */
void printError(char * iTagErr, char * iStrErr);
/******************************************************************************/
/**
 *  \fn         void checkTypeSymbol(symbol_ptr_t sy, short type);
 *  \brief      Vérifier le type d'un symbole et afficher un message d'erreur
 *              adéquat
 *  \param      sy      Symbole
 *  \param      type    Type du symbole
 */
void checkTypeSymbol(symbol_ptr_t sy, short type);
/******************************************************************************/
/**
 *  \fn         void checkResultCall(double fctCall);
 *  \brief      Vérifier que l'appel d'une fonction ne provoque pas d'erreur
 *              de calcul et afficher un message d'erreur si nécessaire
 *  \param      resfctCall    Appel de fonction
 */
double checkResultCall(double fctCall);
#endif      //  _IO_H_
/*############################################################################*/
/*     P R O T O T Y P E S   D E   F O N C T I O N S                          */
/*          M A C H I N E   D'E X E C U T I O N  DE  C O D E                  */
#if defined(_CODE_C_) || defined(_CODE_HOC_C_) || defined(_HOC_Y_)
/******************************************************************************/
/**
 *  \fn         void push(data_t d);
 *  \brief      Empiler une donnée sur la pile d'exécution
 *  \note       Ce traitement est invoqué (généralement) après l'analyse
 *              syntaxique pour empiler la donnée en vue d'une opération
 *              (à venir) en phase d'interprétation
 *  \param      d   donnée à empiler
 *  \return     néant
 */
void push(data_t d);
/******************************************************************************/
/**
 *  \fn         data_t pop(void);
 *  \brief      Dépiler le sommet de la pile d'exécution
 *  \note       Ce traitement est invoqué (généralement) après l'analyse
 *              syntaxique pour dépiler une donnée en vue d'une opération
 *              en phase d'interprétation
 *  \return     donnée au sommet de la pile
 */
data_t pop(void);
/******************************************************************************/
/**
 *  \fn         debugCode(instMac *p, char * couleur);
 *  \brief      Afficher les informations relatives à une instruction machine
 */
instMac *code(instMac pSy_pFct);
/******************************************************************************/
/**
 *  \fn         void initCode(void);
 *  \brief      Initialise les variables globales pour la génération & exécution
 *              de code
 */
void initCode(void);
/******************************************************************************/
/**
 *  \fn         void execute(instMac *p);
 *  \brief      Exécuter le programme pointé par p
 *  \note       Ce traitement est invoqué  après analyseur syntaxique &
 *              génération de code. Il s'agit d'une interprétation du programme.
 *  \param      p    adresse de début du programme à éxecuter
 */
void execute(instMac *p);
/******************************************************************************/
/*      M A C R Os  F O N C T I O Ns                                          */
#define code2(c1, c2)         code(c1); code(c2)
#define code3(c1, c2, c3)     code(c1); code(c2); code(c3)
/******************************************************************************/
#endif      //  _CODE_H_
/*############################################################################*/
/*     P R O T O T Y P E S   D E   F O N C T I O N S                          */
/*          G E N E R A T I O N  DE  C O D E  :                               */
/*                  T A B L E  DES  S Y M B O L E S                           */
#if defined(_CODE_HOC_C_) || defined(_HOC_Y_)
/******************************************************************************/
/**
 *  \fn         void installDefaultSymbols(void);
 *  \brief      Installer des symboles au lancement de l'application :
 *              déclaration de constantes numériques & fonctions mathématiques
 */
void installDefaultSymbols(void);
/*############################################################################*/
/*          G E N E R A T I O N  DE  C O D E  :                               */
/*                  S Y M B O L E S   N U M E R I Q U E S                     */
/******************************************************************************/
/**
 *  \fn         void intPush(void);
 *  \brief      Empiler la valeur d'un entier sur la pile d'exécution
 */
void intPush(void);
/******************************************************************************/
/**
 *  \fn         void floPush(void);
 *  \brief      Empiler la valeur d'un réel sur la pile d'exécution
 */
void floPush(void);
/******************************************************************************/
/**
 *  \fn         void varPush(void);
 *  \brief      Empiler le symbole adressant une variable sur la pile d'exécution
 */
void varPush(void);
/******************************************************************************/
/**
 *  \fn         void varEval(void);
 *  \brief      Dépiler le symbole se trouvant au sommet de la pile d'exécution
 *              et empiler sa valeur
 */
void varEval(void);
/******************************************************************************/
/**
 *  \fn         void varAssign(void);
 *  \brief      Dépiler le symbole se trouvant au sommet de la pile d'exécution
 *              et lui affecter la valeur du nouveau sommet
 */
void varAssign(void);
/*############################################################################*/
/*     P R O T O T Y P E S   D E   F O N C T I O N S                          */
/*          G E N E R A T I O N  DE  C O D E  :                               */
/*                  O P E R A T I O N S   A L G E B R I Q U E S               */
/******************************************************************************/
/**
 *  \fn         void add(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une addition et empiler le résultat
 */
void add(void);
/******************************************************************************/
/**
 *  \fn         void sub(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une soustraction et empiler le résultat
 */
void sub(void);
/******************************************************************************/
/**
 *  \fn         void mul(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une multiplication et empiler le résultat
 */
void mul(void);
/******************************************************************************/
/**
 *  \fn         void Div(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une division et empiler le résultat
 */
void Div(void);
/******************************************************************************/
/**
 *  \fn         void power(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une élévation à la puissance et empiler
 *              le résultat
 */
void power(void);

/******************************************************************************/
/**
 *  \fn         void not_code(void);
 *  \brief     
 */
void not_code(void);

/******************************************************************************/
/**
 *  \fn         void and_code(void);
 *  \brief     
 */
void and_code(void);

/******************************************************************************/
/**
 *  \fn         void or_code(void);
 *  \brief      
 */
void or_code(void);

/******************************************************************************/
/**
 *  \fn         void lt(void);
 *  \brief      
 */
void lt(void);
/******************************************************************************/
/**
 *  \fn         void le(void);
 *  \brief      
 */
void le(void);
/******************************************************************************/
/**
 *  \fn         void gt(void);
 *  \brief      
 */
void gt(void);

/******************************************************************************/
/**
 *  \fn         void ge(void);
 *  \brief      
 */
void ge(void);


/******************************************************************************/
/**
 *  \fn         void eq(void);
 *  \brief     
 */
void eq(void);

/******************************************************************************/
/**
 *  \fn         void ne(void);
 *  \brief      
 */
void ne(void);


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



/******************************************************************************/
/**
 *  \fn         void negate(void);
 *  \brief      Dépiler le sommet de la pile d'exécution,
 *              inverser le signe et empiler le résultat
 */
void negate(void);
/*############################################################################*/
/*     P R O T O T Y P E S   D E   F O N C T I O N S                          */
/*          G E N E R A T I O N  DE  C O D E  :                               */
/*                  O P E R A T I O N S   A L G E B R I Q U E S               */
/******************************************************************************/
/**
 *  \fn         void predef(void);
 *  \brief      Dépiler le paramètre (se trouvant au sommet de la pile) de la
 *              fonction pointée par PC et empiler le résultat
 */
void predef(void);

/*############################################################################*/
/*     P R O T O T Y P E S   D E   F O N C T I O N S                          */
/*          G E N E R A T I O N  DE  C O D E  :                               */
/*                  O P E R A T I O N S   A L G E B R I Q U E S               */
/******************************************************************************/
/**
 *  \fn         void printExpr(void);
 *  \brief      Dépiler le nombre se trouvant au sommet de la pile et l'afficher
 */
void printExpr(void);
/******************************************************************************/
#endif      //  _CODE_HOC_H_
/*############################################################################*/
/*     P R O T O T Y P E S   D E   F O N C T I O N S                          */
/*                  A N A L Y S E   L E X I C A L E                           */
#if defined(_HOC_L_)
/******************************************************************************/
/**
 *  \fn         void _dbgSymbolFlex(symbol_ptr_t sp);
 *  \brief      Afficher les informations relatives à un symbole
 *  \param      sp  pointeur sur le symbole
 */
void _dbgSymbolFlex(symbol_ptr_t sp);
/******************************************************************************/
/**
 *  \fn         int _installNumber(int tokenType);
 *  \brief      Installer les informations relatives à un nombre
 *  \param      sp  pointeur sur le symbole
 */
int _installNumber(int tokenType);
/******************************************************************************/
/**
 *  \fn         int _installIdentifier(void);
 *  \brief      Installer les informations relatives à un symbole
 *  \param      sp  pointeur sur le symbole
 */
int _installIdentifier(void);
#endif      //  _HOC_L_
/*############################################################################*/
/*     P R O T O T Y P E S   D E   F O N C T I O N S                          */
/*                  A N A L Y S E   S Y N T A X I Q U E                       */
#if defined(_HOC_Y_)
/******************************************************************************/
/**
 *  \fn         int yylex(void);
 *  \brief      Analyseur lexical
 *  \return     int  indique le numero du token identifié
 */
extern int yylex(void);
/******************************************************************************/
#endif  //  _HOC_Y_
/******************************************************************************/
/*     V A R I A B L E S    E X T E R N E S                                   */
/******************************************************************************/
/**
 *  \extern     stack   pile d'exécution
 */
#if defined(_CODE_HOC_C_)
    extern data_t stack[];
#endif
/******************************************************************************/
/**
 *  \extern     stackPtr    prochain emplacement libre dans la pile d'exécution
 */
#if defined(_CODE_HOC_C_)
    extern data_t *stackPtr;
#endif
/******************************************************************************/
/**
 *  \extern     prog        machine d'exécution
 */
#if defined(_CODE_HOC_C_)
    extern instMac prog[];
#endif
/******************************************************************************/
/**
 *  \extern     progPtr   prochain emplacement libre dans la machine d'exécution
 */
#if defined(_CODE_HOC_C_) || defined(_HOC_Y_)
    extern instMac *progPtr;
#endif
/******************************************************************************/
/**
 *  \extern     PC          Compteur d'exécution
 */
#if defined(_CODE_HOC_C_)
    extern instMac *PC;
#endif
/******************************************************************************/
/**
 *  \extern     baseProg    Début de sous programme
 */
#if  defined(_CODE_HOC_C_) || defined(_HOC_Y_)
    extern instMac *baseProg;
#endif
/******************************************************************************/
/**
 *  \extern     returning   indique une demande de fin d'exécution
 */
#if defined(_IO_C_)
    extern int returning;
#endif
/******************************************************************************/
/**
 *  \extern     msgErr      indique le message de l'erreur courante
 */
#if defined(_CODE_HOC_C_)
    extern char msgErr[];
#endif
/******************************************************************************/
/**
 *  \extern     errno   indique le numéro d'erreur OS courante
 */
#if defined(_IO_C_)
    extern int errno;
#endif
/******************************************************************************/
#if defined(_CODE_HOC_C_)
#include "defSymbols.c"
#endif
/******************************************************************************/
#endif  //  _HOC_H_
/*------\\//------\\//------\\//------\\//------\\//------\\//------\\/ F I N */
