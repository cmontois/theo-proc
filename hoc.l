/*############################################################################*/
/*                  A N A L Y S E   L E X I C A L E                           */
/******************************************************************************/
/**
 *  \file       hoc.l
 *  \brief      Implémentation des règles lexicales
 *              pour l'évaluation d'expressions algébriques et fonctions
 *
 *  \date       21 Décembre 2018
 *  \author     Samir El Khattabi
 *  \version    1.0
 *
 */
%{
#define _HOC_L_
#include "hoc.h"
%}
/******************************************************************************/
/*     C O N T E X T E  DE  L ' A N A L Y S E U R                             */
%option noyywrap
/******************************************************************************/
/*     L E X E M M E S  DE  L ' A N A L Y S E U R                             */
entier      [0-9]+
reel        [0-9]+\.?|[0-9]*\.[0-9]+
ident       [a-zA-Z][a-zA-Z0-9]*
asg			"="
oper 		[+^*\/-]
lt 		    "<"   
le			"<="
gt			">"
ge			">="
eq			"=="
ne			"!="
not			"!"
and 		"&&"
or	 		"||"
if			"(if)"
else		"(else)"
while		"(while)"
ao          "{"
af			"}"
pv			";"

/******************************************************************************/
/*     R E G L E S   L E X I C A L E S  DE  L ' A N A L Y S E U R             */
%%
[ \t]       ;
{entier}    return _installNumber(NUMINT);
{reel}      return _installNumber(NUMFLO);
"PRINT"     return PRINT;
{ident}     return _installIdentifier();
{asg}       return _installIdentifier();
{oper}		return _installIdentifier();
{lt} 		return _installIdentifier();
{le} 		return _installIdentifier();
{gt} 		return _installIdentifier();
{ge} 		return _installIdentifier();
{eq} 		return _installIdentifier();
{ne} 		return _installIdentifier();
{not} 		return _installIdentifier();
{and} 		return _installIdentifier();
{or} 		return _installIdentifier();
{if}		return _installIdentifier();
{else}		return _installIdentifier();
{while}		return _installIdentifier();
{ao}		return AO;
{af}		return AF;
{pv}		return PV;
\n          return '\n';
.           return yytext[0];
%%
/******************************************************************************/
/*     I M P L E M E N T A T I O N  DES  F O N C T I O N S                    */
/*                  A N A L Y S E   L E X I C A L E                           */
/******************************************************************************/
/**
 *  \fn         void _dbgSymbolFlex(symbol_ptr_t sp);
 *  \brief      Afficher les informations relatives à un symbole
 *  \param      sp  pointeur sur le symbole
 */
void _dbgSymbolFlex(symbol_ptr_t sp)
{
    #ifdef DEBUG_L
    fprintf(stderr,"\033[32m[yylex()\t] ");
    dbgSymbol(sp);
    fprintf(stderr,"\033[0m");
    #endif
}
/******************************************************************************/
/**
 *  \fn         int _installNumber(int tokenType);
 *  \brief      Installer les informations relatives à un nombre
 *  \param      sp  pointeur sur le symbole
 */
int _installNumber(int tokenType)
{
    value_t v;
    
    switch (tokenType) {
        case NUMINT: v.iValue = atoi(yytext);
                     yylval = installSymbol("", NUMINT, v);
                     break;
        case NUMFLO: v.fValue = atof(yytext);
                     yylval = installSymbol("", NUMFLO, v);
                     break;
    }
    _dbgSymbolFlex(yylval);
    return yylval->type;
}
/******************************************************************************/
/**
 *  \fn         int _installNumber(int tokenType);
 *  \brief      Installer les informations relatives à un symbole
 *  \param      sp  pointeur sur le symbole
 */
int _installIdentifier(void)
{
    symbol_ptr_t sp;
    printf("result : %s",yytext);
    if ((sp = lookUpSymbol(yytext)) == SYMBOL_NULL)
        sp = installSymbol(yytext, UNDEF, (value_t)0);
    yylval= sp;
    _dbgSymbolFlex(yylval);
	
    return sp->type;
}
/*------\\//------\\//------\\//------\\//------\\//------\\//------\\/ F I N */
