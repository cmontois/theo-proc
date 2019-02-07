/*############################################################################*/
/*                  G E S T I O N  DES  S Y M B O L E S                       */
/******************************************************************************/
/**
 *  \file       symbol.c
 *  \brief      Fichier d'implémentations en vue de la gestion des symboles
 *              Structures de données & prototypes
 *
 *  \date       21 Décembre 2018
 *  \author     Samir El Khattabi
 *  \version    1.0
 *
 */
#define _SYMBOL_C_
#include "hoc.h"
/******************************************************************************/
/*     V A R I A B L Es  G L O B A L Es                                       */
/*                  G E S T I O N  DES  S Y M B O L E S                       */
/******************************************************************************/
/**
 *  \var        Déclaration de la table des symboles (TS)
 *  \note       La table des symboles sera gérée en liste
 */
static symbol_ptr_t symbolList = SYMBOL_NULL;
/******************************************************************************/
/*     I M P L E M E N T A T I O N  DES  F O N C T I O N S                    */
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
symbol_ptr_t installSymbol(char *tokenName, short tokenType, value_t tokenValue)
{
    symbol_ptr_t sp;
    
    sp          = (symbol_ptr_t) malloc(sizeof(symbol_t));
    sp->name    = malloc(strlen(tokenName)+1);
    strcpy(sp->name, tokenName);
    sp->type    = tokenType;
    sp->value   = tokenValue;
    sp->next    = symbolList;
    symbolList  = sp;
    return sp;
}
/******************************************************************************/
/**
 *  \fn         symbol_ptr_t lookUpSymbol(char *tokenName);
 *  \brief      Rechercher un symbole dans la liste des symboles
 *  \param      tokenName   nom du symbole recherché
 *  \return     pointeur sur le symbole recherché ou NULL si non trouvé
 */
symbol_ptr_t lookUpSymbol(char *tokenName)
{
    symbol_ptr_t sp;
    
    for (sp = symbolList; sp != SYMBOL_NULL; sp = sp->next)
        if (strcmp(sp->name, tokenName) == 0) return sp;
    return SYMBOL_NULL;             // token non trouvé
}
/******************************************************************************/
/**
 *  \fn         void printSymbolList(void);
 *  \brief      Afficher TOUS (sauf certains) les symboles définis
 */
void printSymbolList(void)
{
    symbol_ptr_t sp;
    
    printf("Constantes Réelles:\n");
    for (sp = symbolList; sp != SYMBOL_NULL; sp = sp->next)
        if (sp->type == CSTFLO) printf("\t%s\t= %.8g\n", sp->name, sp->value.fValue);
    printf("Variables Réelles:\n");
    for (sp = symbolList; sp != SYMBOL_NULL; sp = sp->next)
        if (sp->type == VARFLO) printf("\t%s\t= %.8g\n", sp->name, sp->value.fValue);
    printf("Constantes Réelles:\n");
    for (sp = symbolList; sp != SYMBOL_NULL; sp = sp->next)
        if (sp->type == CSTINT) printf("\t%s\t= %.8g\n", sp->name, sp->value.fValue);
    printf("Variables Entières:\n");
    for (sp = symbolList; sp != SYMBOL_NULL; sp = sp->next)
        if (sp->type == VARINT) printf("\t%s\t= %8i\n", sp->name, sp->value.iValue);
    printf("Non définis :\n\t");
    for (sp = symbolList; sp != SYMBOL_NULL; sp = sp->next)
        if (sp->type == UNDEF)
            printf("%s ", sp->name);
    printf("\nFonctions prédéfinies :\n\t");
    for (sp = symbolList; sp != SYMBOL_NULL; sp = sp->next)
        if (sp->type == PREDEF)
            printf("%s ", sp->name);
    printf("\n");
}
/*------\\//------\\//------\\//------\\//------\\//------\\//------\\/ F I N */
