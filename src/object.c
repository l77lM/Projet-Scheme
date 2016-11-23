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


object make_object( uint type )
{

    object t = sfs_malloc( sizeof( *t ) );

    t->type = type;

    return t;
}

object make_primitive(object (*ptrPrim)(object))
{
    object prim=make_object(SFS_PRIMITIVE);

    (prim->this).primitive.function=ptrPrim;

    return prim;
}


object make_nil( void )
{

    object t = make_object( SFS_NIL );

    t->this.special = NULL;

    return t;
}


object make_boolean( uint test )
{

    object t = make_object( SFS_BOOLEAN );

    if (test==1)
    {
        t->this.special = false;
    }

    if (test==0)
    {
        t->this.special = true;
    }

    else
    {
        WARNING_MSG("Booleen non reconnu");
        return NULL ;
    }
    return t;
}



object init_boolean( void )
{

    object t = make_object( SFS_BOOLEAN );

    t->this.special = NULL;

    return t;
}



object make_symbol(char* symb)
{

    object test=exist_symb(symb);

    if (test!=nil)
    {
        return test;
    }

    else
    {
        return(add_table(symb));
    }


    /*
    object t = make_object( SFS_SYMBOL );

    strcpy( t->this.symbol , symb);

    return t;
    */
}

int est_entier(double d)
{
    /* Renvoie 0 si c'est un entier, 1 sinon */
    int i= (int)d;
    if (i == d)
    {
        return 0;
    }
    else
    {
        return 1;
    }

}

object make_integer(int N)
{

    object t = make_object( SFS_NUMBER );

    t->type = SFS_NUMBER;
    (t->this.number).numtype = NUM_INTEGER;
    (t->this.number).this.integer = N;


    return t;
}


object make_infty(int i)
{

    object t = make_object( SFS_NUMBER );

    t->type = SFS_NUMBER;
    if (i==-1)
    {
        (t->this.number).numtype = NUM_MINFTY;
    }
    if (i==0)
    {
        (t->this.number).numtype = NUM_PINFTY;
    }

    return t;
}


object make_real(double X)
{

    object t = make_object( SFS_NUMBER );

    t->type = SFS_NUMBER;
    (t->this.number).numtype = NUM_REAL;
    (t->this.number).this.real = X;


    return t;
}

object make_undef(void)
{

    object t = make_object( SFS_NUMBER );

    t->type = SFS_NUMBER;
    (t->this.number).numtype = NUM_UNDEF;

    return t;
}


object make_character(char car)
{

    object t = make_object( SFS_CHARACTER );

    t->this.character = car;

    return t;
}



object make_string(char* str)
{

    object t = make_object( SFS_STRING );

    strncpy((t->this).string, str, strlen(str));

    return t;
}



object make_pair()
{

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

    (newnoeud->this).pair.cdr=cdr(env);

    (VAR->this).pair.car = var ;
    (VAR->this).pair.cdr = val ;

    (newnoeud->this).pair.car = VAR;

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

        if (0== strcmp( (test->this.pair.car)->this.symbol, symb->this.symbol ))
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
    }

    object test=is_symb(env,symb);

    if (test!=nil)
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


int est_ident(char* c1,char* c2)
{
    /* Renvoie 0 si les chaines sont identiques */
    return strcmp( c1, c2);
}


object car(object paire)
{
    if (paire->type==SFS_PAIR)
    {
        return paire->this.pair.car;
    }
    else
    {
        DEBUG_MSG("CAR is not a pair");
        return NULL;
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
        DEBUG_MSG("CDR is not a pair");
        return NULL;
    }
}




object predicat(object A)
{
    return (sfs_eval(car(cdr(A))));
}

object consequence(object A)
{
    return (sfs_eval(car(cdr(cdr(A)))));
}

object alternative(object A)
{
    object test=cdr(cdr(cdr(A)));
    object output=nil;

    if (test!=nil)
    {
        output=sfs_eval(car(test));
        if (cdr(test)!=nil)
        {
            WARNING_MSG("Too many arguments for IF");
        }
    }
    return output;
}




object add_table(char* nomsymb)
{
    /* Crée un nouvel objet symbole au début de la table et renvoie son adresse */
    object noeud=make_pair();

    object symb = make_object( SFS_SYMBOL );
    strcpy( symb->this.symbol, nomsymb);

    noeud->this.pair.cdr=cdr(table_symbol);

    noeud->this.pair.car=symb;

    table_symbol->this.pair.cdr=noeud;

    return symb;
}



object exist_symb(char* nomsymb)
{
    /* Chercher un symbole dans la table de symbole. Cette fonction retourne l'adresse de l'objet symbol ou nil si le symbol est absent de la table */

    object test=nil;
    object ptr=cdr(table_symbol);


    while (ptr!=nil)
    {

        if (0== est_ident( (ptr->this.pair.car)->this.symbol, nomsymb ))
        {
            return test;
        }
        ptr=cdr(ptr);
    }
    return nil;
}

void affiche_table()
{
    object test=cdr(table_symbol);
    while (test != nil)
    {
        sfs_print(car(test));
        printf("\t");
        test=cdr(test);
    }
    printf("\n");
}


void init_table()
{
    add_table("define");
    add_table("set!");
    add_table("and");
    add_table("or");
    add_table("if");
    add_table("+");
    add_table("-");
    add_table("*");
    add_table("/");
}

char* symbol(object symb)
{
    return symb->this.symbol;
}


object cherche_erreur(object A)
{
    if (A==Error)
    {
        DEBUG_MSG("cherche_erreur trouve erreur");
        return Error;
    }
    else if (A->type==SFS_PAIR)
    {
        DEBUG_MSG("cherche_erreur paire");
        object res=cherche_erreur(car(A));
        if (res==Error)
        {
            return Error;
        }
        res=cherche_erreur(cdr(A));
        if (res==Error)
        {
            return Error;
        }
        else
        {
            return nil;
        }
    }
    else
    {
        DEBUG_MSG("cherche_erreur test");
        return nil;
    }
}
