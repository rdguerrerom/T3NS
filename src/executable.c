#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <argp.h>

#include "io.h"
#include "io_to_disk.h"
#include "macros.h"
#include "network.h"
#include "bookkeeper.h"
#include "hamiltonian.h"
#include "hamiltonian_qc.h"
#include "optimize_network.h"
#include "options.h"
#include "debug.h"

/* ========================================================================== */
/* ==================== DECLARATION STATIC FUNCTIONS ======================== */
/* ========================================================================== */

static void initialize_program(int argc, char *argv[], struct siteTensor **T3NS, 
                               struct rOperators **rops, 
                               struct optScheme * scheme);

static void cleanup_before_exit(struct siteTensor **T3NS, 
                                struct rOperators **rops, 
                                struct optScheme * const scheme);

static void destroy_T3NS(struct siteTensor **T3NS);

static void destroy_all_rops(struct rOperators **rops);

/* ========================================================================== */

int main(int argc, char *argv[])
{
        struct siteTensor *T3NS;
        struct rOperators *rops;
        struct optScheme scheme;
        long long t_elapsed;
        double d_elapsed;
        struct timeval t_start, t_end;

        gettimeofday(&t_start, NULL);

        /* line by line write-out */
        setvbuf(stdout, NULL, _IOLBF, BUFSIZ);

        initialize_program(argc, argv, &T3NS, &rops, &scheme);

        execute_optScheme(T3NS, rops, &scheme);

        cleanup_before_exit(&T3NS, &rops, &scheme);
        printf("SUCCESFULL END!\n");
        gettimeofday(&t_end, NULL);

        t_elapsed = (t_end.tv_sec - t_start.tv_sec) * 1000000LL + 
                t_end.tv_usec - t_start.tv_usec;
        d_elapsed = t_elapsed * 1e-6;

        printf("elapsed time for calculation in total: %lf sec\n", d_elapsed);
        return EXIT_SUCCESS;
}

/* ========================================================================== */ 
/* ===================== DEFINITION STATIC FUNCTIONS ======================== */
/* ========================================================================== */

const char *argp_program_version     = "T3NS 1.0";
const char *argp_program_bug_address = "<Klaas.Gunst@UGent.be>";

/* A description of the program */
static char doc[] =
"T3NS -- An implementation of the three-legged tree tensor networks for\n"
"        fermionic systems.";

/* A description of the arguments we accept. */
static char args_doc[] = "INPUTFILE";

/* The options we understand. */
static struct argp_option options[] = {
        {0} /* options struct needs to be closed by a { 0 } option */
};

/* Used by main to communicate with parse_opt. */
struct arguments {
        char *args[1];                /* netwfile */
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
        /* Get the input argument from argp_parse, which we
         *      know is a pointer to our arguments structure. */
        struct arguments *arguments = state->input;

        switch (key) {
        case ARGP_KEY_ARG:

                /* Too many arguments. */
                if (state->arg_num >= 1)
                        argp_usage(state);

                arguments->args[state->arg_num] = arg;
                break;

        case ARGP_KEY_END:
                /* Not enough arguments. */
                if (state->arg_num < 1)
                        argp_usage(state);
                break;

        default:
                return ARGP_ERR_UNKNOWN;
        }

        return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

static void initialize_program(int argc, char *argv[], struct siteTensor **T3NS, 
                               struct rOperators **rops, 
                               struct optScheme * scheme)
{
        long long t_elapsed;
        double d_elapsed;
        struct timeval t_start, t_end;

        struct arguments arguments;

        gettimeofday(&t_start, NULL);
        init_bookie();
        init_netw();

        /* Default values. */
        /* no arguments to initialize */

        /* Parse our arguments; every option seen by parse_opt will be
         * reflected in arguments. */
        argp_parse(&argp, argc, argv, 0, 0, &arguments);

        read_inputfile(arguments.args[0], scheme);
        assert(scheme->nrRegimes != 0);
        create_list_of_symsecs(scheme->regimes[0].minD);

        random_init(T3NS, rops);

        gettimeofday(&t_end, NULL);

        t_elapsed = (t_end.tv_sec - t_start.tv_sec) * 1000000LL + 
                t_end.tv_usec - t_start.tv_usec;
        d_elapsed = t_elapsed * 1e-6;
        printf("elapsed time for preparing calculation: %lf sec\n", d_elapsed);
}

static void cleanup_before_exit(struct siteTensor **T3NS, 
                                struct rOperators **rops, 
                                struct optScheme * const scheme)
{
        destroy_network();
        destroy_bookkeeper();
        destroy_T3NS(T3NS);
        destroy_all_rops(rops);
        destroy_hamiltonian();
        destroy_optScheme(scheme);
}

static void destroy_T3NS(struct siteTensor **T3NS)
{
        int i;
        for (i = 0; i < netw.sites; ++i)
                destroy_siteTensor(&(*T3NS)[i]);
        safe_free(*T3NS);
}

static void destroy_all_rops(struct rOperators **rops)
{
        int i;
        for (i = 0; i < netw.nr_bonds; ++i)
                destroy_rOperators(&(*rops)[i]);
        safe_free(*rops);
}
