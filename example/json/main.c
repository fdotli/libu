#include <sys/stat.h>
#include <u/libu.h>
#include "json.h"

int facility = LOG_LOCAL0;

int load (const char *fn, char **ps);

int main (int ac, char *av[])
{
    char *s = NULL;
    json_lex_t *jl = NULL;
    json_obj_t *jo = NULL;

    con_err_ifm (ac != 2, "%s <file>", av[0]);

    con_err_if (load(av[1], &s));

    con_err_if (json_lex_new(s, &jl));
    con_err_if (json_lex(jl, &jo));
    json_lex_free(jl), jl = NULL;

    /* Print out what has been parsed. */
    json_obj_print(jo);
    json_obj_free(jo);

    return EXIT_SUCCESS;
err:
    if (jl) 
        u_con("%s", json_lex_geterr(jl));

    json_lex_free(jl);
    json_obj_free(jo);

    return EXIT_FAILURE;
}

int load (const char *fn, char **ps)
{
    FILE *fp = NULL;
    char *s = NULL;
    size_t slen;
    struct stat sb;

    con_return_if (fn == NULL, ~0);
    con_return_if (ps == NULL, ~0);

    con_err_sifm ((fp = fopen(fn, "r")) == NULL, "%s", fn);
    con_err_sifm (fstat(fileno(fp), &sb) == -1, "%s", fn);
    slen = sb.st_size;
    con_err_sif ((s = u_zalloc(slen + 1)) == NULL);
    con_err_sifm (fread(s, slen, 1, fp) != 1, "%s", fn);
    s[slen] = '\0';

    U_FCLOSE(fp);

    *ps = s;

    return 0;
err:
    if (s)
        u_free(s);
    if (fp)
        (void) fclose(fp);
    return ~0;
}