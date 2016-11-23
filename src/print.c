/**
 * @file print.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:12:17 2012
 * @brief Printing stuff for SFS.
 *
 * Printing stuff for SFS.
 */

#include "print.h"

#include <stdio.h>

void sfs_print_atom( object o )
{

    switch (o->type)
    {
    case SFS_NUMBER:

        switch (((o->this).number).numtype)
        {
        case NUM_REAL:
            printf("%g",((o->this).number).this.real);
            break;

        case NUM_INTEGER:
            printf("%d",((o->this).number).this.integer);
            break;

        case NUM_UINTEGER:
            printf("%u",((o->this).number).this.integer);
            break;

        case NUM_COMPLEX:
            printf("COMPLEX pas encore defini\n");
            break;

        case NUM_UNDEF:
            printf("UNDEF");
            break;

        case NUM_PINFTY:
            printf("+inf");
            break;

        case NUM_MINFTY:
            printf("-inf");
            break;
        }
        break;

    case SFS_CHARACTER:
        ;
        DEBUG_MSG("print char");
        char car=(o->this).character;

        if (car=='\n')
        {
            printf("#\\newline");
        }

        else if (car<32)
        {
            printf("#\\");
        }

        else if (car==' ')
        {
            printf("#\\space");
        }

        else
        {
            printf("#\\%c",(o->this).character);
        }
        break;


    case SFS_STRING:
        printf("%c",'"');
        printf("%s",(o->this).string);
        printf("%c",'"');
        break;


    case SFS_NIL:
        printf("()");
        break;

    case SFS_BOOLEAN:
        if ( o==true )
        {
            printf("%s","#t");
        }
        else if ( o==false )
        {
            printf("%s","#f");
        }

        break;

    case SFS_SYMBOL:
        printf("%s",(o->this).symbol);
        break;
    }

    return ;
}

void sfs_print_pair( object o )
{

    if (  o ->this.pair.car -> type == SFS_PAIR)
    {
        printf("(");
        sfs_print_pair(o ->this.pair.car);
    }

    else if (o -> this.pair.car -> type == SFS_NIL)
    {
        printf("()");

    }

    else
    {
        sfs_print( o -> this.pair.car );
    }


    if (o -> this.pair.cdr -> type == SFS_NIL)
    {
        printf(")");
    }

    else if (o -> this.pair.cdr -> type == SFS_PAIR)
    {
        printf(" ");
        sfs_print( (o -> this).pair.cdr );
    }

    else
    {
        printf(" . ");
        sfs_print( (o -> this).pair.cdr );
        printf(")");
    }


    return;
}

void sfs_print( object o )
{

    if ( o->type == SFS_PAIR )
    {
        /*printf("(");*/
        sfs_print_pair( o );
        /*printf(")");*/
    }

    else
    {

        sfs_print_atom( o );
    }

}


void sfs_print_env(object ENV)
{
    object env=ENV;
    object noeud=ENV;
    if (cdr(env)==nil)
    {
        printf("Environnement vide\n");
    }

    int cpt=1;

    while (env != nil)
    {
        noeud=cdr(env);
        printf("Env %d:\n",cpt);
        while (noeud != nil)
        {

            /*if ( (car(noeud))->type == SFS_PAIR )*/

                sfs_print_atom(car(car(noeud)));
                printf(" -> ");
                if (  cdr(car(noeud))->type == SFS_PAIR ) {printf("(");}
                sfs_print(cdr(car(noeud)));
                printf("\n");

            noeud=cdr(noeud);
        }
        env=car(env);
    }
}
