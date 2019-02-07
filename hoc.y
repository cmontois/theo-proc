/*############################################################################*/
/*                  A N A L Y S E   S Y N T A X I Q U E                       */
/******************************************************************************/
/**
 *  \file       hoc.y
 *  \brief      Implémentation des règles syntaxiques avec génération de code
 *              pour l'évaluation d'expressions algébriques et fonctions
 *
 *
 *  \date       21 Décembre 2018
 *  \author     Samir El Khattabi
 *  \version    1.0
 *
 */
%{
#define _HOC_Y_
#include "hoc.h"
instMac *adr;
%}
/******************************************************************************/
/*      D E F I N I T I O N   DES  T O K E N S                                */
/******************************************************************************/
/**
 *  \note       Déclaration des tokens avec le type adéquat (si token valué)
 */
%token NUMINT CSTINT VARINT
%token NUMFLO VARFLO CSTFLO
%token UNDEF
%token PREDEF
%token ADD SUB DIV MUL POW NOT AND OR LE LT GT GE EQ NE IF EL WHI ASG
%token AO AF PV
%token PRINT
/******************************************************************************/
/*      T Y P A G E   DES  U N I T E S   S Y N T A X I Q U E S                */

/******************************************************************************/
/*  A S S O C I A T I V I T Es   DES  O P E R T E U R S  &  P R I O R I T Es  */
/******************************************************************************/
/**
 *  \note       Associativité à gauche de même priorité
 */
%left ASG
%left OR
%left AND
%left LE LT GT GE EQ NE
%left ADD SUB
/******************************************************************************/
/**
 *  \note       Associativité à gauche de priorité superieure
 */

%left  MUL DIV
%left  NOT
/******************************************************************************/
/**
 *  \note       Surcharge de l'opération soustraction par le signe
 */
%left  UNARYMINUS
/******************************************************************************/
/**
 *  \note       Associativité à droite
 */
%right POW
/******************************************************************************/
/*      G R A M M A I R E                                                     */
%%
list:   vide
    | list '\n'         { prompt(); }
    | list stmt '\n'    { code(STOP); return 1;}
    | list expr '\n'    { code2(printExpr, STOP); return 1; }
    | list error '\n'   { yyerrok; prompt(); }
    ;
	
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

vide:
    ;

varNum : VARINT { code2(varPush, (instMac)$1); }
    | VARFLO { code2(varPush, (instMac)$1); }
    | UNDEF  { code2(varPush, (instMac)$1); }
    ;

stmt : assgn
	| expr 
	| condStmt
	| stmtList
    | PRINT { code(printSymbolList); }
    ;
	
stmtList : vide
	| stmt PV
	| AO stmtList stmt AF
	
	;
	
bool : comp symbolBool comp {code((instMac)$2->value.vfunc);}
	;

comp : expr symbolComp expr {code((instMac)$2->value.vfunc);}
	
assgn : varNum ASG expr { code(varAssign);}
	;
	
condStmt :IF { adr=progPtr;code((instMac)$1->value.vfunc);code3(STOP,STOP,STOP);printf("if code adr %p\n",adr); }
			'(' expr ')' {code(STOP);*(adr+1)=(instMac)progPtr; printf("adr=%p avec %p\n",adr+1,*(adr+1));} 
			stmt end { *(adr+2)=*(adr+3)=(instMac)progPtr;printf("adr=%p avec %p\n",adr+3,*(adr+3));}
		| IF { adr=progPtr;code((instMac)$1->value.vfunc);code3(STOP,STOP,STOP);printf("if code adr %p\n",adr); }
			'(' expr ')' {code(STOP);*(adr+1)=(instMac)progPtr; printf("adr=%p avec %p\n",adr+1,*(adr+1));} 
			stmt end { *(adr+2)=(instMac)progPtr;printf("adr=%p avec %p\n",adr+2,*(adr+2));} 
			EL stmt end {*(adr+3)=(instMac)progPtr;printf("adr=%p avec %p\n",adr+3,*(adr+3));}
	;
end:	{ code(STOP);}
	;

expr : NUMINT { code2(intPush, (instMac)$1); }
    | NUMFLO { code2(floPush, (instMac)$1); }
    | CSTINT { code3(varPush, (instMac)$1, varEval); }
    | CSTFLO { code3(varPush, (instMac)$1, varEval); }
    | varNum { code (varEval); }
    | '(' expr ')'
    | expr symbolOp expr { code((instMac)$2->value.vfunc);}
	| comp
	| bool
	| symbolNot expr {code ((instMac)$1->value.vfunc);}
    | '-' expr %prec UNARYMINUS { code(negate); }
    | PREDEF '(' expr ')' { code2(predef, (instMac)$1); }
    ;
%%
/******************************************************************************/
/*      V A R I A B L E S   G L O B A L E S                                   */
/******************************************************************************/
/**
 *  \var        progName    Référence le nom de l'exécutable de l'application
 */
char *progName;
/******************************************************************************/
/**
 *  \var        lineNo      Référence le numéro de la ligne courante
 */
int lineNo = 0;
/******************************************************************************/
/*      P R O G R A M M E   P R I N C I P A L                                 */
int main (int argc, char *argv[])
{
    progName = argv[0];
    fprintf(stdout,"\033[30m\033[1mWelcome to the basic calculator.\nImplemented by Clément MONTOIS (LA3).\nVersion 1.1, Junnary 5, 2019\n\033[0m");
    installDefaultSymbols();
	printf("Beginning execution\n");
    for (initCode(),prompt(); yyparse(); initCode(),prompt()) execute(baseProg);
	printf("End execution\n");
    fprintf(stdout, "\nSee ya !\n");
    exit(EXIT_SUCCESS);
}
/*------\\//------\\//------\\//------\\//------\\//------\\//------\\/ F I N */
