/*############################################################################*/
/*          G E N E R A T I O N  DE  C O D E                                  */
/******************************************************************************/
/**
 *  \file       codeHoc.c
 *  \brief      Fichier d'implémentations pour la génération de code
 *              <LI> Symboles Numériques : nombres, constantes, variables</LI>
 *              <LI> Opérations Algébriques</LI>
 *              <LI> Appels Fonctions prédéfinies</LI>
 *              <LI> Affichage résultat</LI>
 *
 *  \date       21 Décembre 2018
 *  \author     Samir El Khattabi
 *  \version    1.0
 *
 */
#define _CODE_HOC_C_
#include "hoc.h"
/*############################################################################*/
/*     I M P L E M E N T A T I O N  DES  F O N C T I O N S                    */
/*          G E N E R A T I O N  DE  C O D E  :                               */
/*                  T A B L E  DES  S Y M B O L E S   N U M E R I Q U E S     */
/******************************************************************************/
/**
 *  \fn         void installDefaultSymbols(void);
 *  \brief      Installer des symboles au lancement de l'application
 *              déclaration de constantes numériques & fonctions mathématiques
 */
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
/******************************************************************************/
/**
 *  \fn         void _dbgSymbolExec(char *trmt, symbol_ptr_t sp);
 *  \brief      Afficher les informations relatives à un symbole lors de
 *              l'exécution d'un traitement
 */
void _dbgSymbolExec(char *trmt, symbol_ptr_t sp)
{
    #ifdef DEBUG_E
    fprintf(stderr,"\033[34m[%s()\t] ", trmt);
    dbgSymbol(sp);
    fprintf(stderr,"\033[0m");
    #endif
}
/*############################################################################*/
/*     I M P L E M E N T A T I O N  DES  F O N C T I O N S                    */
/*          G E N E R A T I O N  DE  C O D E  :                               */
/*                  S Y M B O L E S   N U M E R I Q U E S                     */
/******************************************************************************/
/**
 *  \fn         void intPush(void);
 *  \brief      Empiler la valeur d'un entier sur la pile d'exécution
 */
void intPush(void)
{   data_t d;
    _dbgSymbolExec("intPush",(symbol_ptr_t)*PC);
    d.fValue = (float)(((symbol_ptr_t) *PC++)->value.iValue);
    push(d);
}
/******************************************************************************/
/**
 *  \fn         void floPush(void);
 *  \brief      Empiler la valeur d'un réel sur la pile d'exécution
 */
void floPush(void)
{   data_t d;
    _dbgSymbolExec("floPush",(symbol_ptr_t)*PC);
    d.fValue = ((symbol_ptr_t) *PC++)->value.fValue;
    push(d);
}
/******************************************************************************/
/**
 *  \fn         void varPush(void);
 *  \brief      Empiler le symbole adressant une variable sur la pile d'exécution
 */
void varPush(void)
{   data_t d;
    
    d.symbol = ((symbol_ptr_t) *PC++);
    _dbgSymbolExec("varPush",d.symbol);
    sprintf(msgErr,"[%s] is not a variable",d.symbol->name);
    
    if (d.symbol->type!=VARFLO && d.symbol->type!=CSTFLO &&
        d.symbol->type!=VARINT && d.symbol->type!=CSTFLO &&
        d.symbol->type!=UNDEF)
        printError("EXEC-E-301: varPush", msgErr);
    push(d);
}
/******************************************************************************/
/**
 *  \fn         void varEval(void);
 *  \brief      Dépiler le symbole se trouvant au sommet de la pile d'exécution
 *              et empiler sa valeur
 */
void varEval(void)
{   data_t d = pop();
    _dbgSymbolExec("varEval",d.symbol);
    if (d.symbol->type == UNDEF) {
        sprintf(msgErr,"[%s] is undefined variable",d.symbol->name);
        printError("EXEC-E-302: varEval", msgErr);
    } else if (d.symbol->type==VARINT || d.symbol->type==CSTINT)
        d.fValue = (float)d.symbol->value.iValue;
    else d.fValue = d.symbol->value.fValue;
    push(d);
}
/******************************************************************************/
/**
 *  \fn         void varAssign(void);
 *  \brief      Dépiler le symbole se trouvant au sommet de la pile d'exécution
 *              et lui affecter la valeur du nouveau sommet
 */
void varAssign(void)
{   data_t d2 = pop(), d1 = pop();
    _dbgSymbolExec("varAssign",d1.symbol);
    sprintf(msgErr,"[%s] is not a variable, can't assign",d1.symbol->name);
    if (d1.symbol->type!=VARFLO && d1.symbol->type!=VARINT
        && d1.symbol->type!=UNDEF)
        printError("EXEC-E-303: varAssign", msgErr);
    else if (d1.symbol->type==VARINT) {
        d1.symbol->value.iValue = d2.iValue;
        d1.symbol->type    = VARINT;
    } else {
        d1.symbol->value.fValue = d2.fValue;
        d1.symbol->type    = VARFLO;
    }
}
/*############################################################################*/
/*     I M P L E M E N T A T I O N  DES  F O N C T I O N S                    */
/*          G E N E R A T I O N  DE  C O D E  :                               */
/*                  O P E R A T I O N S   A L G E B R I Q U E S               */
/******************************************************************************/
/**
 *  \fn         void add(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une addition et empiler le résultat
 */
void add(void)
{   data_t d2 = pop(), d1 = pop();
    d1.fValue = d1.fValue + d2.fValue;
    push(d1);
}
/******************************************************************************/
/**
 *  \fn         void sub(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une soustraction et empiler le résultat
 */
void sub(void)
{   data_t d2 = pop(), d1 = pop();
    d1.fValue = d1.fValue - d2.fValue;
    push(d1);
}
/******************************************************************************/
/**
 *  \fn         void mul(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une multiplication et empiler le résultat
 */
void mul(void)
{   data_t d2 = pop(), d1 = pop();
    d1.fValue = d1.fValue * d2.fValue;
    push(d1);
}
/******************************************************************************/
/**
 *  \fn         void Div(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une division et empiler le résultat
 */
void Div(void)
{   data_t d2 = pop(), d1 = pop();
    if (d2.fValue == 0.0)
        printError("EXEC-E-304: Div", "Division by zero");
    d1.fValue = d1.fValue / d2.fValue;
    push(d1);
}
/******************************************************************************/
/**
 *  \fn         void power(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une élévation à la puissance et empiler
 *              le résultat
 */
void power(void)
{   data_t d2 = pop(), d1 = pop();
    d1.fValue = pow(d1.fValue, d2.fValue);
    push(d1);
}

 /******************************************************************************/
/**
 *  \fn         void not_code(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer un contraire et empiler
 *              le résultat
 */
void not_code(void) {
	data_t d1=pop();
	data_t d;
	d.fValue=!d1.fValue;
	push(d);
	
};

/******************************************************************************/
/**
 *  \fn         void and_code(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer un ET et empiler
 *              le résultat
 */
void and_code(void) {
	data_t d2=pop(),d1=pop();
	data_t d;
	
	d.fValue=d1.fValue&&d2.fValue;
	push(d);
};

/******************************************************************************/
/**
 *  \fn         void or_code(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer un OU  et empiler
 *              le résultat
 */
void or_code(void){
	data_t d2=pop(),d1=pop();
	data_t d;
	d.fValue=d1.fValue||d2.fValue;
	push(d);
};

/******************************************************************************/
/**
 *  \fn         void lt(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une comparaison inférieur et empiler
 *              le résultat
 */
void lt(void){
	data_t d2=pop(),d1=pop();
	data_t d;
	d.fValue=d1.fValue<d2.fValue;
	push(d);
		

};
/******************************************************************************/
/**
 *  \fn         void le(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une comparaison inférieur égale et empiler
 *              le résultat
 */
void le(void){
	data_t d2=pop(),d1=pop();
	data_t d;
	d.fValue=d1.fValue<=d2.fValue;
	push(d);

};
/******************************************************************************/
/**
 *  \fn         void gt(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une comparaison supérieur et empiler
 *              le résultat
 */
void gt(void){
	data_t d2=pop(),d1=pop();
	data_t d;
	d.fValue=d1.fValue>d2.fValue;
	push(d);
};
/******************************************************************************/
/**
 *  \fn         void ge(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une comparaison supérieur égale et empiler
 *              le résultat
 */
void ge(void){
	data_t d2=pop(),d1=pop();
	data_t d;
	d.fValue=d1.fValue>=d2.fValue;
	push(d);
	
};

/******************************************************************************/
/**
 *  \fn         void eq(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, effectuer une comparaison égale et empiler
 *              le résultat
 */
void eq(void){
	data_t d2=pop(),d1=pop();
	data_t d;
	d.fValue=d1.fValue==d2.fValue;
	push(d);
};
/******************************************************************************/
/**
 *  \fn         void ne(void);
 *  \brief      Dépiler le nombre se trouvant au sommet de la pile
 *              d'exécution, effectuer une négation et empiler
 *              le résultat
 */
void ne(void){
	data_t d2=pop(),d1=pop();
	data_t d;
	d.fValue=d1.fValue!=d2.fValue;
	push(d);
};

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


/******************************************************************************/
/**
 *  \fn         void while_code(void);
 *  \brief      
 */
void while_code(void){
	instMac* adr=PC; 
	printf("Execute adr +3 \n");
	execute(adr+3);
	
	data_t condition=pop();
	
	if(condition.fValue) {
		printf("Conditions is ok \n");
	}
	else {
		printf("Conditions is not ok\n");
        //Loop
	}
	
    printf("Not implemented yet\n");
    
};

/******************************************************************************/
/**
 *  \fn         void else_code(void);
 *  \brief      
 */
void else_code(void){

};
/******************************************************************************/
/**
 *  \fn         void negate(void);
 *  \brief      Dépiler les deux nombres se trouvant au sommet de la pile
 *              d'exécution, inverser le signe et empiler le résultat
 */
 

void negate(void)
{   data_t d = pop();
    d.fValue = - d.fValue;
    push(d);
}
/*############################################################################*/
/*     I M P L E M E N T A T I O N  DES  F O N C T I O N S                    */
/*          G E N E R A T I O N  DE  C O D E  :                               */
/*                  A P P E L S  F O N C T I O N S                            */
/******************************************************************************/
/**
 *  \fn         void predef(void);
 *  \brief      Dépiler le paramètre (se trouvant au sommet de la pile) de la
 *              fonction pointée par PC et empiler le résultat
 */
void predef(void)
{   data_t d = pop();
    _dbgSymbolExec("predef",(symbol_ptr_t) *PC);
    d.fValue = (*(double (*)()) ((symbol_ptr_t) *PC++)->value.func) (d.fValue);
    push(d);
}

/*############################################################################*/
/*     I M P L E M E N T A T I O N  DES  F O N C T I O N S                    */
/*          G E N E R A T I O N  DE  C O D E  :                               */
/*                  A F F I C H A G E                                         */
/******************************************************************************/
/**
 *  \fn         void printExpr(void);
 *  \brief      Dépiler le nombre se trouvant au sommet de la pile et l'afficher
 */
void printExpr(void)
{   data_t d = pop();
    fprintf(stdout,"= %.8g\n", d.fValue);
}
/******************************************************************************/
double Log(double x) {
    return checkResultCall(log(x));
}
/*------\\//------\\//------\\//------\\//------\\//------\\//------\\/ F I N */
