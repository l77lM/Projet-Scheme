
/**
 * @file object.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:02:57 2012
 * @brief Object definitions for SFS.
 *
 * Object definitions for SFS.
 */

#include <stdio.h>
#include <string.h>

#include "object.h"
#include "mem.h"


object make_object( uint type ) {

    object t = sfs_malloc( sizeof( *t ) );

    t->type = type;

    return t;
}




object make_nil( void ) {

    object t = make_object( SFS_NIL );

    t->this.special = NULL;

    return t;
}


object make_boolean( uint test ) {

	object t = make_object( SFS_BOOLEAN );

	if (test==1) { t->this.special = false; }

	if (test==0) { t->this.special = true; }

	else {
		printf("\nBooleen non reconnu\n");
		return NULL ;
	}
	return t;
}



object init_boolean( void ) {

	object t = make_object( SFS_BOOLEAN );

	t->this.special = NULL;

	return t;
}



object make_symbol(char* symb) {

	object t = make_object( SFS_SYMBOL );

	strcpy( t->this.symbol , symb);

	return t;
}



object make_integer(int N) {

	object t = make_object( SFS_NUMBER );

	t->type = SFS_NUMBER;
	(t->this.number).numtype = NUM_INTEGER;
	(t->this.number).this.integer = N;


	return t;
}

object make_infty(int i){

	object t = make_object( SFS_NUMBER );

	t->type = SFS_NUMBER;
	if (i==0)  { (t->this.number).numtype = NUM_MINFTY; }
	if (i==-1) { (t->this.number).numtype = NUM_PINFTY; }

	return t;
}


object make_real(double X) {

	object t = make_object( SFS_NUMBER );

	t->type = SFS_NUMBER;
	(t->this.number).numtype = NUM_REAL;
	(t->this.number).this.real = X;


	return t;
}



object make_character(char car) {

	object t = make_object( SFS_CHARACTER );

	t->this.character = car;

	return t;
}



object make_string(char* str) {

	object t = make_object( SFS_STRING );

	strncpy((t->this).string , str , strlen(str));

	return t;
}



object make_pair() {

	object t = make_object( SFS_PAIR );

	(t->this).pair.car = nil ;
	(t->this).pair.cdr = nil ;

	return t;
}




object make_newENV(object env)
{
    /* Crée et renvoie l'adresse d'un nouvel environnement superieur à l'environnement courant */
    object New= make_pair();
    New->this.pair.car=env;
    return New;
}



object add_symb(object env,object var,object val)
{
    /* Cette fonction ajoute un symbole en début d'environement (masquage) et retourne l'adresse de la paire contenant la variable et sa valeur */

    object VAR= make_pair();
    object newnoeud= make_pair();

    newnoeud->this.pair.cdr=cdr(env);

    (VAR->this).pair.car = var ;
    (VAR->this).pair.cdr = val ;

    (env->this).pair.cdr = newnoeud ;

    return VAR;

}

object is_symb(object env,object symb)
{
    /* Chercher un symbole dans UN environnement. Cette fonction retourne l'adresse de la paire contenant le symbol et sa valeur ou nil si le symbol est absent de l'environnement */

    object test=nil;
    object ptr=cdr(env);

    while (ptr!=nil)
    {
        test=car(ptr);

        if (0== strcmp( (test->this.pair.car)->this.symbol , symb->this.symbol ))
        {
            return test;
        }
        ptr=cdr(ptr);
    }
    return nil;
}

object cherche_symbol(object env,object symb)
{
    /* Cherche un symbole dans l'environnement et les environnements inferieurs  */
    if (env==nil)
    {
        return nil;
        /* printf("Symbol not found"); */
    }

    object test=is_symb(env,symb)

    else if (test!=nil)
    {
        return test;
    }
    else
    {
        return cherche_symbol(car(env),symb);
    }


}

object modif_symbole_env(object env,object symb,object val)
{
    /* Cette fonction retourne l'adresse de la paire contenant le symbole et sa valeur modifiée. Renvoie nil si le symbole n'existe pas */

    object test=cherche_symbol(env,symb);

    if (test!=nil)
    {
        test->this.pair.cdr=val;
    }
    return test;
}


int est_ident(char* c1,char* c2)  /* Si renvoie 0 alors les chaines sont identiques */
{
    return strcmp( c1 , c2);
}


object car(object paire)
{
    if (paire->type==SFS_PAIR)
    {
        return paire->this.pair.car;
    }
    else
    {
        printf("Pas pair");
        return nil;
    }
}

object cdr(object paire)
{
    if (paire->type==SFS_PAIR)
    {
        return paire->this.pair.cdr;
    }
    else
    {
        printf("Pas pair");
        return nil;
    }
}
