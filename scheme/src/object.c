
/**
 * @file object.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:02:57 2012
 * @brief Object definitions for SFS.
 *
 * Object definitions for SFS.
 */

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



object make_pair(char* symb) {

	object t = make_object( SFS_PAIR );

	(t->this).pair.car = nil ;
	(t->this).pair.cdr = nil ;

	return t;
}
