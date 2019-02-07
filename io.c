/*############################################################################*/
/*                  G E S T I O N  DES  E / S                                 */
/******************************************************************************/
/**
 *  \file       io.c
 *  \brief      Fichier d'implémentations pour la gestion des E/S
 *              <LI>Gestion des erreurs</LI>
 *
 *  \date       21 Décembre 2018
 *  \author     Samir El Khattabi
 *  \version    1.0
 *
 */
#define _IO_C_
#include "hoc.h"
/******************************************************************************/
/*     V A R I A B L E S  G L O B A L E S                                     */
/*                  G E S T I O N  DES  E R R E U R S                         */
/******************************************************************************/
/**
 *  \var        msgErr      indique le message de l'erreur courante
 */
char msgErr[256];
/*############################################################################*/
/*     I M P L E M E N T A T I O N  DES  F O N C T I O N S                    */
/*                  G E S T I O N  DES  S O R T I E S                         */
/******************************************************************************/
/**  \fn        void dbgSymbol(symbol_ptr_t sp);
 *  \brief      Afficher les informations relatives à un symbole
 *  \param      sp  pointeur sur le symbole
 */
void dbgSymbol(symbol_ptr_t sp)
{
    fprintf(stdout,"NAME_=[%14s] @SYMB=[%14p] TYPE_=[",sp->name, sp);
    switch(sp->type) {
        case NUMINT: fprintf(stdout,"%8cNUMINT]\tVALUE=[%14i]",' ',sp->value.iValue); break;
        case CSTINT: fprintf(stdout,"%8cCSTINT]\tVALUE=[%14i]",' ',sp->value.iValue); break;
        case VARINT: fprintf(stdout,"%8cVARINT]\tVALUE=[%14i]",' ',sp->value.iValue); break;
        case NUMFLO: fprintf(stdout,"%8cNUMFLO]\tVALUE=[%14.12g]",' ',sp->value.fValue); break;
        case CSTFLO: fprintf(stdout,"%8cCSTFLO]\tVALUE=[%14.12g]",' ',sp->value.fValue); break;
        case VARFLO: fprintf(stdout,"%8cVARFLO]\tVALUE=[%14.12g]",' ',sp->value.fValue); break;
        case UNDEF : fprintf(stdout,"%8cUNDEF_]",' '); break;
        case PREDEF: fprintf(stdout,"%8cPREDEF]\tFADDR=[%14p]",' ',sp->value.func); break;
    }
    fprintf(stdout,"\n");
}
/******************************************************************************/
/**
 *  \fn         void prompt(void);
 *  \brief      Afficher le prompt
 */
void prompt (void)
{
    fprintf(stdout, "hoc> ");
}
/*############################################################################*/
/*     I M P L E M E N T A T I O N  DES  F O N C T I O N S                    */
/*                  G E S T I O N  DES  E R R E U R S                         */
/******************************************************************************/
/**
 *  \fn         void yyerror(char *msg);
 *  \brief      Fonction invoquée par l'analyseur syntaxique
 *  \parm       msg     Message d'erreur
 */
void yyerror(char *msg)
{
    fprintf(stderr,"\033[31m[SYTX-E-000: yyparse\t] #\033[5m%s\033[0m\033[31m#\033[0m\n", msg);
}
/******************************************************************************/
/**
 *  \fn         void printError(char *tagErr, char *msgErr);
 *  \brief      Afficher un message d'erreur formaté
 *  \param      tagErr TYPE-SEVERITE-NUM
 *  \parm       msgErr  Message d'erreur
 */
void printError(char *tagErr, char *msgErr)
{
    fprintf(stderr,"\033[31m[%s\t] #\033[7m%s\033[0m\033[31m#\033[0m\n", tagErr, msgErr);
//    returning = 1;      //  YYERROR;
}
/******************************************************************************/
/**
 *  \fn         void checkTypeSymbol(symbol_ptr_t sy, short type);
 *  \brief      Verifier le type d'un symbole et afficher un message d'erreur
 *              adéquat
 *  \param      sy      Symbole
 *  \param      type    Type du symbole
 */
void checkTypeSymbol(symbol_ptr_t sy, short type)
{
    sprintf(msgErr,"Incompatible type of symbol [%s]",sy->name);
    if (sy->type != type)
        printError("SENS-E-000: TypeSymb", msgErr);
}
/******************************************************************************/
/**
 *  \fn         void checkResultCall(double fctCall);
 *  \brief      Verifier que l'appel d'une fonction ne provoque pas d'erreur
 *              de calcul et afficher un message d'erreur si nécessaire
 *  \param      resfctCall    Appel de fonction
 */
double checkResultCall(double fctCall)
{ char msgErr[100];
    if (errno != 0) {
        sprintf(msgErr,"Exec error [%s]",strerror(errno));
        printError("EXEC-E-000: callFct", msgErr);
    }
    return fctCall;
}
/*------\\//------\\//------\\//------\\//------\\//------\\//------\\/ F I N */
