/*         ______   ___    ___ 
 *        /\  _  \ /\_ \  /\_ \ 
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___ 
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      Emulation for libc routines that may be missing on some platforms.
 *
 *      By Michael Bukin.
 *
 *      Henrik Stokseth added _al_sane_realloc() function.
 *
 *      _al_srand() and _al_rand() functions based on code by Paul Pridham.
 *
 *      See readme.txt for copyright information.
 */


#include "allegro.h"
#include "allegro/internal/aintern.h"


static int _al_rand_seed = 0;



#ifdef ALLEGRO_NO_STRLWR

/* _alemu_strlwr:
 *  Convert all upper case characters in string to lower case.
 */
char *_alemu_strlwr(char *string)
{
   char *p;
   ASSERT(string);

   for (p=string; *p; p++)
      *p = utolower(*p);

   return string;
}

#endif



#ifdef ALLEGRO_NO_STRUPR

/* _alemu_strupr:
 *  Convert all lower case characters in string to upper case.
 */
char *_alemu_strupr(char *string)
{
   char *p;
   ASSERT(string);

   for (p=string; *p; p++)
      *p = utoupper(*p);

   return string;
}

#endif



#ifdef ALLEGRO_NO_STRICMP

/* _alemu_stricmp:
 *  Case-insensitive comparison of strings.
 */
int _alemu_stricmp(AL_CONST char *s1, AL_CONST char *s2)
{
   int c1, c2;
   ASSERT(s1);
   ASSERT(s2);

   do {
      c1 = utolower(*(s1++));
      c2 = utolower(*(s2++));
   } while ((c1) && (c1 == c2));

   return c1 - c2;
}

#endif



#ifdef ALLEGRO_NO_MEMCMP

/* _alemu_memcmp:
 *  Comparison of two memory blocks.
 */
int _alemu_memcmp(AL_CONST void *s1, AL_CONST void *s2, size_t num)
{
   size_t i;
   ASSERT(s1);
   ASSERT(s2);
   ASSERT(num >= 0);

   for (i=0; i<num; i++)
      if (((unsigned char *)s1)[i] != ((unsigned char *)s2)[i])
	 return ((((unsigned char *)s1)[i] < ((unsigned char *)s2)[i]) ? -1 : 1);

   return 0;
}

#endif



/* _al_sane_realloc:
 *  realloc() substitution with guaranteed behaviour.
 */
void *_al_sane_realloc(void *ptr, size_t size)
{
   void *tmp_ptr;

   tmp_ptr = NULL;

   if (ptr && size) {
      tmp_ptr = realloc(ptr, size);
      if (!tmp_ptr && ptr) free(ptr);
   }
   else if (!size) {
      tmp_ptr = NULL;
      if (ptr) free(ptr);
   }
   else if (!ptr) {
      tmp_ptr = malloc(size);
   }
   
   return tmp_ptr;
}



/* _al_srand:
 *  Seed initialization routine for rand() replacement.
 */
void _al_srand(int seed)
{
   _al_rand_seed = seed;
}



/* _al_rand:
 *  Simple rand() replacement with guaranteed randomness in the lower 16 bits.
 */
int _al_rand()
{
   _al_rand_seed = (_al_rand_seed + 1) * 1103515245 + 12345;
   return ((_al_rand_seed >> 16) & _AL_RAND_MAX);
}
