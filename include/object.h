/**
 * @file object.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 17:49:46 2012
 * @brief Object definition for SFS.
 *
 * Object definition for SFS.
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "number.h"


typedef struct object_t
{

    uint type;

    union
    {

        num              number;
        char             character;
        string           string;
        string           symbol;

        struct pair_t
        {
            struct object_t *car;
            struct object_t *cdr;
        }                pair;

        struct object_t *special;

    } this;

} *object;




object make_object( uint type );
object make_nil( void );
object make_boolean( uint test );
object init_boolean( void );
object make_symbol(char* symb);
object make_integer(int N);
object make_character(char car);
object make_string(char* str);
object make_pair();
object make_infty(int i);

object make_newENV(object env);
object add_symb(object base,object var,object val);
object is_symb(object base,object symb);
object cherche_symbol(object env,object symb);
object modif_symbole_env(object base,object symb,object val);

int est_ident(char* c1,char* c2);
object car(object paire);
object cdr(object paire);

#define SFS_NUMBER       0x00
#define SFS_CHARACTER    0x01
#define SFS_STRING       0x02
#define SFS_PAIR         0x03
#define SFS_NIL          0x04
#define SFS_BOOLEAN      0x05
#define SFS_SYMBOL       0x06



extern object nil;

extern object true;

extern object false;

extern object empty_list;

extern object meta_env;

#ifdef __cplusplus
}
#endif

#endif /* _OBJECT_H_ */
