/*
 * Copyright 2006-2022 the Pacemaker project contributors
 *
 * The version control history for this file may have further details.
 *
 * This source code is licensed under the GNU Lesser General Public License
 * version 2.1 or later (LGPLv2.1+) WITHOUT ANY WARRANTY.
 */

#ifndef PCMK__OPTIONS_INTERNAL__H
#  define PCMK__OPTIONS_INTERNAL__H

#  ifndef PCMK__CONFIG_H
#    define PCMK__CONFIG_H
#    include <config.h>   // HAVE_GETOPT, _Noreturn
#  endif

#  include <glib.h>     // GHashTable
#  include <stdbool.h>  // bool

/*
 * Command-line option handling
 *
 * This will all eventually go away as everything is converted to use GOption
 */

#  ifdef HAVE_GETOPT_H
#    include <getopt.h>
#  else
#    define no_argument 0
#    define required_argument 1
#  endif

enum pcmk__cli_option_flags {
    pcmk__option_default    = (1 << 0),
    pcmk__option_hidden     = (1 << 1),
    pcmk__option_paragraph  = (1 << 2),
    pcmk__option_example    = (1 << 3),
};

typedef struct pcmk__cli_option_s {
    /* Fields from 'struct option' in getopt.h */
    /* name of long option */
    const char *name;
    /*
     * one of no_argument, required_argument, and optional_argument:
     * whether option takes an argument
     */
    int has_arg;
    /* if not NULL, set *flag to val when option found */
    int *flag;
    /* if flag not NULL, value to set *flag to; else return value */
    int val;

    /* Custom fields */
    const char *desc;
    long flags;
} pcmk__cli_option_t;

void pcmk__set_cli_options(const char *short_options, const char *usage,
                           pcmk__cli_option_t *long_options,
                           const char *app_desc);
int pcmk__next_cli_option(int argc, char **argv, int *index,
                          const char **longname);
_Noreturn void pcmk__cli_help(char cmd, crm_exit_t exit_code);
void pcmk__cli_option_cleanup(void);


/*
 * Environment variable option handling
 */

const char *pcmk__env_option(const char *option);
void pcmk__set_env_option(const char *option, const char *value);
bool pcmk__env_option_enabled(const char *daemon, const char *option);


/*
 * Cluster option handling
 */

typedef struct pcmk__cluster_option_s {
    const char *name;
    const char *alt_name;
    const char *type;
    const char *values;
    const char *default_value;

    bool (*is_valid)(const char *);

    const char *description_short;
    const char *description_long;

} pcmk__cluster_option_t;

const char *pcmk__cluster_option(GHashTable *options,
                                 pcmk__cluster_option_t *option_list, int len,
                                 const char *name);

char *pcmk__format_option_metadata(const char *name, const char *desc_short,
                                   const char *desc_long,
                                   pcmk__cluster_option_t *option_list, int len);

void pcmk__validate_cluster_options(GHashTable *options,
                                    pcmk__cluster_option_t *option_list,
                                    int len);

bool pcmk__valid_interval_spec(const char *value);
bool pcmk__valid_boolean(const char *value);
bool pcmk__valid_number(const char *value);
bool pcmk__valid_positive_number(const char *value);
bool pcmk__valid_quorum(const char *value);
bool pcmk__valid_script(const char *value);
bool pcmk__valid_percentage(const char *value);

// from watchdog.c
long pcmk__get_sbd_timeout(void);
bool pcmk__get_sbd_sync_resource_startup(void);
long pcmk__auto_watchdog_timeout(void);
bool pcmk__valid_sbd_timeout(const char *value);

// Constants for environment variable names
#define PCMK__ENV_BLACKBOX                  "blackbox"
#define PCMK__ENV_CLUSTER_TYPE              "cluster_type"
#define PCMK__ENV_DEBUG                     "debug"
#define PCMK__ENV_LOGFACILITY               "logfacility"
#define PCMK__ENV_LOGFILE                   "logfile"
#define PCMK__ENV_LOGPRIORITY               "logpriority"
#define PCMK__ENV_MCP                       "mcp"
#define PCMK__ENV_NODE_START_STATE          "node_start_state"
#define PCMK__ENV_PHYSICAL_HOST             "physical_host"
#define PCMK__ENV_QUORUM_TYPE               "quorum_type"
#define PCMK__ENV_SHUTDOWN_DELAY            "shutdown_delay"
#define PCMK__ENV_STDERR                    "stderr"

// Constants for cluster option names
#define PCMK__OPT_NODE_HEALTH_BASE          "node-health-base"
#define PCMK__OPT_NODE_HEALTH_GREEN         "node-health-green"
#define PCMK__OPT_NODE_HEALTH_RED           "node-health-red"
#define PCMK__OPT_NODE_HEALTH_STRATEGY      "node-health-strategy"
#define PCMK__OPT_NODE_HEALTH_YELLOW        "node-health-yellow"

// Constants for meta-attribute names
#define PCMK__META_ALLOW_UNHEALTHY_NODES    "allow-unhealthy-nodes"

// Constants for enumerated values for various options
#define PCMK__VALUE_CUSTOM                  "custom"
#define PCMK__VALUE_GREEN                   "green"
#define PCMK__VALUE_MIGRATE_ON_RED          "migrate-on-red"
#define PCMK__VALUE_NONE                    "none"
#define PCMK__VALUE_ONLY_GREEN              "only-green"
#define PCMK__VALUE_PROGRESSIVE             "progressive"
#define PCMK__VALUE_RED                     "red"
#define PCMK__VALUE_YELLOW                  "yellow"

#endif // PCMK__OPTIONS_INTERNAL__H
