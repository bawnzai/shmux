/*
** Copyright (C) 2003 Christophe Kalt
**
** This file is part of shmux,
** see the LICENSE file for details on your rights.
*/

#include "os.h"

#include "byteset.h"
#include "ctype.h"

static char const rcsid[] = "@(#)$Id: byteset.c,v 1.3 2003-04-19 00:52:56 kalt Exp $";

extern char *myname;

static char sets[2][256];

/*
** byteset_init
**	Initialize set[] from a range definition string
*/
void
byteset_init(set, definition)
int set;
char *definition;
{
    char *str, *tok, *dash;
    int i, j;

    assert( set == 0 || set == 1 );
    assert( definition != NULL );

    i = 0;
    while (i < 256)
	sets[set][i++] = 0;

    str = strdup(definition);
    tok = strtok(str, ",");
    while (tok != NULL)
      {
	if (tok[0] == '-')
	    i = 0;
	else if (isdigit((int) tok[0]) != 0)
	    i = atoi(tok);
	else
	    i = -1;

	j = i;
	dash = index(tok, '-');
	if (dash != NULL)
	  {
	    if (*(dash + 1) != '\0')
	      {
		if (isdigit((int) *(dash + 1)) != 0)
		    j = atoi(dash + 1);
		else
		    j = -1;
	      }
	    else
		j = 255;
	  }

	if (i < 0 || i > 255 || j < 0 || j > 255 || j < i) {
	  fprintf(stderr, "%s: Invalid range: %s\n", myname, tok);
	  exit(1);
	}

	while (i <= j)
	    sets[set][i++] = 1;

	tok = strtok(NULL, ",");
      }
    free(str);
}

/*
** byteset_test
**	Is a byte included in the set?
*/
int
byteset_test(set, byte)
int set, byte;
{
    assert( set == 0 || set == 1 );
    assert( byte >= 0 && byte <= 255 );

    return sets[set][byte] == 0;
}
