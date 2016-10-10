/** @file p2x-opts.hh
 *  @brief The header file for the command line option parser
 *  generated by GNU Gengetopt version 2.22.6
 *  http://www.gnu.org/software/gengetopt.
 *  DO NOT modify this file, since it can be overwritten
 *  @author GNU Gengetopt by Lorenzo Bettini */

#ifndef P2X_OPTS_H
#define P2X_OPTS_H

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h> /* for FILE */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef CMDLINE_PARSER_PACKAGE
/** @brief the program name (used for printing errors) */
#define CMDLINE_PARSER_PACKAGE "p2x"
#endif

#ifndef CMDLINE_PARSER_PACKAGE_NAME
/** @brief the complete program name (used for help and version) */
#define CMDLINE_PARSER_PACKAGE_NAME "p2x"
#endif

#ifndef CMDLINE_PARSER_VERSION
/** @brief the program version */
#define CMDLINE_PARSER_VERSION VERSION
#endif

/** @brief Where the command line options are stored */
struct gengetopt_args_info
{
  const char *help_help; /**< @brief Print help and exit help description.  */
  const char *full_help_help; /**< @brief Print help, including hidden options, and exit help description.  */
  const char *version_help; /**< @brief Print version and exit help description.  */
  char ** verbose_arg;	/**< @brief Control messages by bit mask (default='error|warning').  */
  char ** verbose_orig;	/**< @brief Control messages by bit mask original value given at command line.  */
  unsigned int verbose_min; /**< @brief Control messages by bit mask's minimum occurreces */
  unsigned int verbose_max; /**< @brief Control messages by bit mask's maximum occurreces */
  const char *verbose_help; /**< @brief Control messages by bit mask help description.  */
  int debug_flag;	/**< @brief Enable debugging (default=off).  */
  const char *debug_help; /**< @brief Enable debugging help description.  */
  char * prec_list_arg;	/**< @brief Precedence file list.  */
  char * prec_list_orig;	/**< @brief Precedence file list original value given at command line.  */
  const char *prec_list_help; /**< @brief Precedence file list help description.  */
  char ** ignore_arg;	/**< @brief Add an item to ignore.  */
  char ** ignore_orig;	/**< @brief Add an item to ignore original value given at command line.  */
  unsigned int ignore_min; /**< @brief Add an item to ignore's minimum occurreces */
  unsigned int ignore_max; /**< @brief Add an item to ignore's maximum occurreces */
  const char *ignore_help; /**< @brief Add an item to ignore help description.  */
  char ** binary_arg;	/**< @brief Add a binary operator.  */
  char ** binary_orig;	/**< @brief Add a binary operator original value given at command line.  */
  unsigned int binary_min; /**< @brief Add a binary operator's minimum occurreces */
  unsigned int binary_max; /**< @brief Add a binary operator's maximum occurreces */
  const char *binary_help; /**< @brief Add a binary operator help description.  */
  char ** right_arg;	/**< @brief Add a right associative operator.  */
  char ** right_orig;	/**< @brief Add a right associative operator original value given at command line.  */
  unsigned int right_min; /**< @brief Add a right associative operator's minimum occurreces */
  unsigned int right_max; /**< @brief Add a right associative operator's maximum occurreces */
  const char *right_help; /**< @brief Add a right associative operator help description.  */
  char ** unary_arg;	/**< @brief Add a unary operator.  */
  char ** unary_orig;	/**< @brief Add a unary operator original value given at command line.  */
  unsigned int unary_min; /**< @brief Add a unary operator's minimum occurreces */
  unsigned int unary_max; /**< @brief Add a unary operator's maximum occurreces */
  const char *unary_help; /**< @brief Add a unary operator help description.  */
  char ** postfix_arg;	/**< @brief Add a postfix operator.  */
  char ** postfix_orig;	/**< @brief Add a postfix operator original value given at command line.  */
  unsigned int postfix_min; /**< @brief Add a postfix operator's minimum occurreces */
  unsigned int postfix_max; /**< @brief Add a postfix operator's maximum occurreces */
  const char *postfix_help; /**< @brief Add a postfix operator help description.  */
  char ** item_arg;	/**< @brief Add an item.  */
  char ** item_orig;	/**< @brief Add an item original value given at command line.  */
  unsigned int item_min; /**< @brief Add an item's minimum occurreces */
  unsigned int item_max; /**< @brief Add an item's maximum occurreces */
  const char *item_help; /**< @brief Add an item help description.  */
  char ** brace_arg;	/**< @brief Scope start and end token.  */
  char ** brace_orig;	/**< @brief Scope start and end token original value given at command line.  */
  unsigned int brace_min; /**< @brief Scope start and end token's minimum occurreces */
  unsigned int brace_max; /**< @brief Scope start and end token's maximum occurreces */
  const char *brace_help; /**< @brief Scope start and end token help description.  */
  int list_token_flag;	/**< @brief List token types (default=off).  */
  const char *list_token_help; /**< @brief List token types help description.  */
  int list_classes_flag;	/**< @brief List token classes (default=off).  */
  const char *list_classes_help; /**< @brief List token classes help description.  */
  int scan_only_flag;	/**< @brief Scan only, do not parse (default=off).  */
  const char *scan_only_help; /**< @brief Scan only, do not parse help description.  */
  char ** scanner_arg;	/**< @brief Select scanner class (default='strings').  */
  char ** scanner_orig;	/**< @brief Select scanner class original value given at command line.  */
  unsigned int scanner_min; /**< @brief Select scanner class's minimum occurreces */
  unsigned int scanner_max; /**< @brief Select scanner class's maximum occurreces */
  const char *scanner_help; /**< @brief Select scanner class help description.  */
  int stdin_tty_flag;	/**< @brief Read from stdin, even if it is a TTY (default=off).  */
  const char *stdin_tty_help; /**< @brief Read from stdin, even if it is a TTY help description.  */
  char * outfile_arg;	/**< @brief Write output to file.  */
  char * outfile_orig;	/**< @brief Write output to file original value given at command line.  */
  const char *outfile_help; /**< @brief Write output to file help description.  */
  char ** input_encoding_arg;	/**< @brief Input encoding (default='utf-8').  */
  char ** input_encoding_orig;	/**< @brief Input encoding original value given at command line.  */
  unsigned int input_encoding_min; /**< @brief Input encoding's minimum occurreces */
  unsigned int input_encoding_max; /**< @brief Input encoding's maximum occurreces */
  const char *input_encoding_help; /**< @brief Input encoding help description.  */
  int indent_flag;	/**< @brief Indent (default=on).  */
  const char *indent_help; /**< @brief Indent help description.  */
  char ** indent_unit_arg;	/**< @brief Indentation unit (default=' ').  */
  char ** indent_unit_orig;	/**< @brief Indentation unit original value given at command line.  */
  unsigned int indent_unit_min; /**< @brief Indentation unit's minimum occurreces */
  unsigned int indent_unit_max; /**< @brief Indentation unit's maximum occurreces */
  const char *indent_unit_help; /**< @brief Indentation unit help description.  */
  int newline_as_br_flag;	/**< @brief Emit newline text as ca:br element of ca:text (default=on).  */
  const char *newline_as_br_help; /**< @brief Emit newline text as ca:br element of ca:text help description.  */
  int newline_as_entity_flag;	/**< @brief Emit newline text as &#xa; character entity (default=off).  */
  const char *newline_as_entity_help; /**< @brief Emit newline text as &#xa; character entity help description.  */
  int merged_flag;	/**< @brief Collect children of equal operator chains, output all binary nodes in MERGED mode (default=off).  */
  const char *merged_help; /**< @brief Collect children of equal operator chains, output all binary nodes in MERGED mode help description.  */
  int strict_flag;	/**< @brief Strict output mode: paren children always indicated by null elements (default=off).  */
  const char *strict_help; /**< @brief Strict output mode: paren children always indicated by null elements help description.  */
  char * output_mode_arg;	/**< @brief Write output as XML/JSON/MATLAB.  */
  char * output_mode_orig;	/**< @brief Write output as XML/JSON/MATLAB original value given at command line.  */
  const char *output_mode_help; /**< @brief Write output as XML/JSON/MATLAB help description.  */
  int write_recursive_flag;	/**< @brief Recursive output writing (default=off).  */
  const char *write_recursive_help; /**< @brief Recursive output writing help description.  */
  int attribute_line_flag;	/**< @brief Emit attribute line with source line (default=on).  */
  const char *attribute_line_help; /**< @brief Emit attribute line with source line help description.  */
  int attribute_column_flag;	/**< @brief Emit attribute column with source column (default=on).  */
  const char *attribute_column_help; /**< @brief Emit attribute column with source column help description.  */
  int attribute_char_flag;	/**< @brief Emit attribute column with source char (default=off).  */
  const char *attribute_char_help; /**< @brief Emit attribute column with source char help description.  */
  int attribute_precedence_flag;	/**< @brief Emit attribute precedence with token precedence (default=off).  */
  const char *attribute_precedence_help; /**< @brief Emit attribute precedence with token precedence help description.  */
  int attribute_mode_flag;	/**< @brief Emit attribute mode with token mode (default=off).  */
  const char *attribute_mode_help; /**< @brief Emit attribute mode with token mode help description.  */
  int attribute_type_flag;	/**< @brief Emit attribute type with token type (default=on).  */
  const char *attribute_type_help; /**< @brief Emit attribute type with token type help description.  */
  int attribute_id_flag;	/**< @brief Emit attribute id with token id (default=off).  */
  const char *attribute_id_help; /**< @brief Emit attribute id with token id help description.  */
  
  unsigned int help_given ;	/**< @brief Whether help was given.  */
  unsigned int full_help_given ;	/**< @brief Whether full-help was given.  */
  unsigned int version_given ;	/**< @brief Whether version was given.  */
  unsigned int verbose_given ;	/**< @brief Whether verbose was given.  */
  unsigned int debug_given ;	/**< @brief Whether debug was given.  */
  unsigned int prec_list_given ;	/**< @brief Whether prec-list was given.  */
  unsigned int ignore_given ;	/**< @brief Whether ignore was given.  */
  unsigned int binary_given ;	/**< @brief Whether binary was given.  */
  unsigned int right_given ;	/**< @brief Whether right was given.  */
  unsigned int unary_given ;	/**< @brief Whether unary was given.  */
  unsigned int postfix_given ;	/**< @brief Whether postfix was given.  */
  unsigned int item_given ;	/**< @brief Whether item was given.  */
  unsigned int brace_given ;	/**< @brief Whether brace was given.  */
  unsigned int list_token_given ;	/**< @brief Whether list-token was given.  */
  unsigned int list_classes_given ;	/**< @brief Whether list-classes was given.  */
  unsigned int scan_only_given ;	/**< @brief Whether scan-only was given.  */
  unsigned int scanner_given ;	/**< @brief Whether scanner was given.  */
  unsigned int stdin_tty_given ;	/**< @brief Whether stdin-tty was given.  */
  unsigned int outfile_given ;	/**< @brief Whether outfile was given.  */
  unsigned int input_encoding_given ;	/**< @brief Whether input-encoding was given.  */
  unsigned int indent_given ;	/**< @brief Whether indent was given.  */
  unsigned int indent_unit_given ;	/**< @brief Whether indent-unit was given.  */
  unsigned int newline_as_br_given ;	/**< @brief Whether newline-as-br was given.  */
  unsigned int newline_as_entity_given ;	/**< @brief Whether newline-as-entity was given.  */
  unsigned int merged_given ;	/**< @brief Whether merged was given.  */
  unsigned int strict_given ;	/**< @brief Whether strict was given.  */
  unsigned int output_mode_given ;	/**< @brief Whether output-mode was given.  */
  unsigned int write_recursive_given ;	/**< @brief Whether write-recursive was given.  */
  unsigned int attribute_line_given ;	/**< @brief Whether attribute-line was given.  */
  unsigned int attribute_column_given ;	/**< @brief Whether attribute-column was given.  */
  unsigned int attribute_char_given ;	/**< @brief Whether attribute-char was given.  */
  unsigned int attribute_precedence_given ;	/**< @brief Whether attribute-precedence was given.  */
  unsigned int attribute_mode_given ;	/**< @brief Whether attribute-mode was given.  */
  unsigned int attribute_type_given ;	/**< @brief Whether attribute-type was given.  */
  unsigned int attribute_id_given ;	/**< @brief Whether attribute-id was given.  */

  char **inputs ; /**< @brief unamed options (options without names) */
  unsigned inputs_num ; /**< @brief unamed options number */
} ;

/** @brief The additional parameters to pass to parser functions */
struct cmdline_parser_params
{
  int override; /**< @brief whether to override possibly already present options (default 0) */
  int initialize; /**< @brief whether to initialize the option structure gengetopt_args_info (default 1) */
  int check_required; /**< @brief whether to check that all required options were provided (default 1) */
  int check_ambiguity; /**< @brief whether to check for options already specified in the option structure gengetopt_args_info (default 0) */
  int print_errors; /**< @brief whether getopt_long should print an error message for a bad option (default 1) */
} ;

/** @brief the purpose string of the program */
extern const char *gengetopt_args_info_purpose;
/** @brief the usage string of the program */
extern const char *gengetopt_args_info_usage;
/** @brief the description string of the program */
extern const char *gengetopt_args_info_description;
/** @brief all the lines making the help output */
extern const char *gengetopt_args_info_help[];
/** @brief all the lines making the full help output (including hidden options) */
extern const char *gengetopt_args_info_full_help[];

/**
 * The command line parser
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser (int argc, char **argv,
  struct gengetopt_args_info *args_info);

/**
 * The command line parser (version with additional parameters - deprecated)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param override whether to override possibly already present options
 * @param initialize whether to initialize the option structure my_args_info
 * @param check_required whether to check that all required options were provided
 * @return 0 if everything went fine, NON 0 if an error took place
 * @deprecated use cmdline_parser_ext() instead
 */
int cmdline_parser2 (int argc, char **argv,
  struct gengetopt_args_info *args_info,
  int override, int initialize, int check_required);

/**
 * The command line parser (version with additional parameters)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param params additional parameters for the parser
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_ext (int argc, char **argv,
  struct gengetopt_args_info *args_info,
  struct cmdline_parser_params *params);

/**
 * Save the contents of the option struct into an already open FILE stream.
 * @param outfile the stream where to dump options
 * @param args_info the option struct to dump
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_dump(FILE *outfile,
  struct gengetopt_args_info *args_info);

/**
 * Save the contents of the option struct into a (text) file.
 * This file can be read by the config file parser (if generated by gengetopt)
 * @param filename the file where to save
 * @param args_info the option struct to save
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_file_save(const char *filename,
  struct gengetopt_args_info *args_info);

/**
 * Print the help
 */
void cmdline_parser_print_help(void);
/**
 * Print the full help (including hidden options)
 */
void cmdline_parser_print_full_help(void);
/**
 * Print the version
 */
void cmdline_parser_print_version(void);

/**
 * Initializes all the fields a cmdline_parser_params structure 
 * to their default values
 * @param params the structure to initialize
 */
void cmdline_parser_params_init(struct cmdline_parser_params *params);

/**
 * Allocates dynamically a cmdline_parser_params structure and initializes
 * all its fields to their default values
 * @return the created and initialized cmdline_parser_params structure
 */
struct cmdline_parser_params *cmdline_parser_params_create(void);

/**
 * Initializes the passed gengetopt_args_info structure's fields
 * (also set default values for options that have a default)
 * @param args_info the structure to initialize
 */
void cmdline_parser_init (struct gengetopt_args_info *args_info);
/**
 * Deallocates the string fields of the gengetopt_args_info structure
 * (but does not deallocate the structure itself)
 * @param args_info the structure to deallocate
 */
void cmdline_parser_free (struct gengetopt_args_info *args_info);

/**
 * The config file parser (deprecated version)
 * @param filename the name of the config file
 * @param args_info the structure where option information will be stored
 * @param override whether to override possibly already present options
 * @param initialize whether to initialize the option structure my_args_info
 * @param check_required whether to check that all required options were provided
 * @return 0 if everything went fine, NON 0 if an error took place
 * @deprecated use cmdline_parser_config_file() instead
 */
int cmdline_parser_configfile (const char *filename,
  struct gengetopt_args_info *args_info,
  int override, int initialize, int check_required);

/**
 * The config file parser
 * @param filename the name of the config file
 * @param args_info the structure where option information will be stored
 * @param params additional parameters for the parser
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int cmdline_parser_config_file (const char *filename,
  struct gengetopt_args_info *args_info,
  struct cmdline_parser_params *params);

/**
 * Checks that all the required options were specified
 * @param args_info the structure to check
 * @param prog_name the name of the program that will be used to print
 *   possible errors
 * @return
 */
int cmdline_parser_required (struct gengetopt_args_info *args_info,
  const char *prog_name);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* P2X_OPTS_H */
