/*############################################################################*/
/*          M A C H I N E   D'E X E C U T I O N  DE  C O D E                  */
/******************************************************************************/
/**
 *  \file       code.c
 *  \brief      Fichier d'implémentations pour la génération & exécution de code
 *              Structures de données & prototypes
 *
 *  \date       21 Décembre 2018
 *  \author     Samir El Khattabi
 *  \version    1.0
 *
 */
#define _CODE_C_
#include "hoc.h"
/******************************************************************************/
/*      C O N S T A N T E S   S Y M B O L I Q U E S                           */
/*          M A C H I N E   D'E X E C U T I O N  DE  C O D E                  */
/******************************************************************************/
/**
 *  \def        MAX_STACK
 *  \brief      Taille maximale de la pile d'exécution
 */
#define MAX_STACK 256
/******************************************************************************/
/**
 *  \def        MAX_PROG
 *  \brief      Nombre maximal d'instructions d'un programme
 */
#define MAX_PROG 2000
/******************************************************************************/
/*     V A R I A B L E S  G L O B A L E S                                     */
/*          M A C H I N E   D'E X E C U T I O N  DE  C O D E                  */
/******************************************************************************/
/**
 *  \var        stack   pile d'exécution
 */
static data_t stack[MAX_STACK];
/******************************************************************************/
/**
 *  \var        stackPtr    prochain emplacement libre dans la pile d'exécution
 */
static data_t *stackPtr;
/******************************************************************************/
/**
 *  \var        prog        machine d'exécution
 */
instMac prog[MAX_PROG];
/******************************************************************************/
/**
 *  \var        progPtr   prochain emplacement libre dans la machine d'exécution
 */
instMac *progPtr;
/******************************************************************************/
/**
 *  \var        PC          Compteur d'exécution
 */
instMac *PC;
/******************************************************************************/
/**
 *  \var        baseProg    Début de sous programme
 */
instMac *baseProg;
/******************************************************************************/
/**
 *  \var        returning   indique une demande de fin d'exécution
 */
int returning;
/******************************************************************************/
/*     I M P L E M E N T A T I O N  DES  F O N C T I O N S                    */
/*          M A C H I N E   D'E X E C U T I O N  DE  C O D E                  */
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
void push(data_t d)
{
    if (stackPtr >= &stack[MAX_STACK])
        printError("EXEC-E-001: push","Stack overflow");
    *stackPtr++ = d;
}
/******************************************************************************/
/**
 *  \fn         data_t pop(void);
 *  \brief      Dépiler le sommet de la pile d'exécution
 *  \note       Ce traitement est invoqué (généralement) après l'analyse
 *              syntaxique pour dépiler une donnée en vue d'une opération
 *              en phase d'interprétation
 *  \return     donnée au sommet de la pile
 */
data_t pop(void)
{
    if (stackPtr <= stack)
        printError("EXEC-E-002: pop","Stack empty");
    return *--stackPtr;
}
/******************************************************************************/
/**
 *  \fn         _dbgCode(instMac *p);
 *  \brief      Afficher les informations relatives à une instruction machine
 */
void _dbgCode(instMac *p)
{
    #ifdef DEBUG_C
    fprintf(stderr,"\033[33m[code()-symb/fct] @PROG=[%14p] @CODE=[%14p]\033[0m\n",p,*p);
    #endif
}
/******************************************************************************/
/**
 *  \fn         instMac *code(instMac pSy_pFct);
 *  \brief      Ajouter une instruction au programme en cours de génération
 *  \note       Ce traitement est invoqué  par l'analyseur syntaxique pour
 *              générer une instruction machine qui correspond soit à un pointeur
 *              sur un symbole soit un pointeur sur une fonction.
 *  \param      pSy_pFct    ptr sur symbole ou ptr sur fonction
 *  \return     pointeur sur l'instruction insérée
 */
instMac *code(instMac pSy_pFct)
{    instMac *currentProgPtr = progPtr;
     
    if (progPtr >= &prog[MAX_PROG])
        printError("SENS-E-001: code", "Bigest program");
    *progPtr++ = pSy_pFct;
    _dbgCode(currentProgPtr);
    return currentProgPtr;
}
/******************************************************************************/
/**
 *  \fn         _dbgExec(instMac *adr);
 *  \brief      Afficher les informations relatives à une instruction machine
 */
void _dbgExec(instMac *adr)
{
    #ifdef DEBUG_E
    fprintf(stderr,"\033[35m[execute()\t] @INST=[%14p] INST_=[%14p] @PILE=[%14p]\033[0m\n", adr, *adr,stackPtr-1);
    #endif
}
/******************************************************************************/
/**
 *  \fn         void initCode(void);
 *  \brief      Initialise les variables globales pour la génération & exécution
 *              de code
 */
void initCode(void)
{
    stackPtr  = stack;
    progPtr   = baseProg = prog;
    returning = 0;
    #ifdef DEBUG_E
    fprintf(stderr,"\033[36m[initCode()\t] @PROG=[%14p] @PILE=[%14p]\033[0m\n", progPtr, stackPtr);
    #endif
}
/******************************************************************************/
/**
 *  \fn         void execute(instMac *p);
 *  \brief      Exécuter le programme pointé par p
 *  \note       Ce traitement est invoqué  après analyseur syntaxique &
 *              génération de code. Il s'agit d'une interprétation du programme.
 *  \param      p   adresse de début du programme à éxecuter
 */
void execute(instMac *p)
{
    for (PC = p; *PC != STOP && !returning; ){
        _dbgExec(PC);
        (*(*PC++)) ();
    }
    if (returning)
        printError("SENS-E-000: execute", "Returning");
}
/*------\\//------\\//------\\//------\\//------\\//------\\//------\\/ F I N */
