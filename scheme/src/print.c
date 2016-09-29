
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

void sfs_print_atom( object o ) {
    switch (o->type)
    {
        case SFS_NUMBER:
            switch (((o->this).number).numtype)
            {
                case NUM_REAL:
                    printf("%f",((o->this).number).this.real);
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
                    printf("UNDEF pas encore defini\n");
                    break;

                case NUM_PINFTY:
                    printf("PINFTY pas encore defini\n");
                    break;

                case NUM_MINFTY:
                    printf("MINFTY pas encore defini\n");
                    break;
            }

            break;

        case SFS_CHARACTER:
            printf("%c",(o->this).character);
            break;


        case SFS_STRING:
            printf("%c",'"');
            printf("%s",(o->this).string);
            printf("%c",'"');
            break;


        case SFS_NIL:
            printf("%s","()");
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
            else
            {
            }
            break;

        case SFS_SYMBOL:
            printf("%s",(o->this).symbol);
            break;
    }
    return;
}

void sfs_print_pair( object o ) {     /* a verifier !!!!!! */
    printf("%c",'(');
    sfs_print( (o -> this).pair.car );
    printf(" ");
    sfs_print( (o -> this).pair.cdr );
    printf("%c",')');
    return;
}

void sfs_print( object o ) {

    if ( SFS_PAIR == o->type ) {
        sfs_print_pair( o );
    }

    else {
        sfs_print_atom( o );
    }

}
