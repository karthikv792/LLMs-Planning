/* A Bison parser, made by GNU Bison 2.4.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006,
   2009, 2010 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 17 "src/pddl+.yacc"

/*
Error reporting:
Intention is to provide error token on most bracket expressions,
so synchronisation can occur on next CLOSE_BRAC.
Hence error should be generated for innermost expression containing error.
Expressions which cause errors return a NULL values, and parser
always attempts to carry on.
This won't behave so well if CLOSE_BRAC is missing.

Naming conventions:
Generally, the names should be similar to the PDDL2.1 spec.
During development, they have also been based on older PDDL specs,
older PDDL+ and TIM parsers, and this shows in places.

All the names of fields in the semantic value type begin with t_
Corresponding categories in the grammar begin with c_
Corresponding classes have no prefix.

PDDL grammar       yacc grammar      type of corresponding semantic val.

thing+             c_things          thing_list
(thing+)           c_thing_list      thing_list

*/

#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <ctype.h>

// This is now copied locally to avoid relying on installation
// of flex++.

//#include "FlexLexer.h"
//#include <FlexLexer.h>

#include "ptree.h"
#include "parse_error.h"

#define YYDEBUG 1

int yyerror(char *);

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", ((char *)msgid))
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) ((char *) msgid)
# endif
#endif

extern int yylex();

using namespace VAL;



/* Line 189 of yacc.c  */
#line 135 "src/pddl+.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     OPEN_BRAC = 258,
     CLOSE_BRAC = 259,
     MODULES = 260,
     OPEN_SQ = 261,
     CLOSE_SQ = 262,
     DOT = 263,
     CLASSES = 264,
     CLASS = 265,
     DEFINE = 266,
     PDDLDOMAIN = 267,
     REQS = 268,
     EQUALITY = 269,
     STRIPS = 270,
     ADL = 271,
     NEGATIVE_PRECONDITIONS = 272,
     TYPING = 273,
     DISJUNCTIVE_PRECONDS = 274,
     EXT_PRECS = 275,
     UNIV_PRECS = 276,
     QUANT_PRECS = 277,
     COND_EFFS = 278,
     FLUENTS = 279,
     OBJECTFLUENTS = 280,
     NUMERICFLUENTS = 281,
     ACTIONCOSTS = 282,
     TIME = 283,
     DURATIVE_ACTIONS = 284,
     DURATION_INEQUALITIES = 285,
     CONTINUOUS_EFFECTS = 286,
     DERIVED_PREDICATES = 287,
     TIMED_INITIAL_LITERALS = 288,
     PREFERENCES = 289,
     CONSTRAINTS = 290,
     ACTION = 291,
     PROCESS = 292,
     EVENT = 293,
     DURATIVE_ACTION = 294,
     DERIVED = 295,
     CONSTANTS = 296,
     PREDS = 297,
     FUNCTIONS = 298,
     TYPES = 299,
     ARGS = 300,
     PRE = 301,
     CONDITION = 302,
     PREFERENCE = 303,
     START_PRE = 304,
     END_PRE = 305,
     EFFECTS = 306,
     INITIAL_EFFECT = 307,
     FINAL_EFFECT = 308,
     INVARIANT = 309,
     DURATION = 310,
     AT_START = 311,
     AT_END = 312,
     OVER_ALL = 313,
     AND = 314,
     OR = 315,
     EXISTS = 316,
     FORALL = 317,
     IMPLY = 318,
     NOT = 319,
     WHEN = 320,
     WHENEVER = 321,
     EITHER = 322,
     PROBLEM = 323,
     FORDOMAIN = 324,
     INITIALLY = 325,
     OBJECTS = 326,
     GOALS = 327,
     EQ = 328,
     LENGTH = 329,
     SERIAL = 330,
     PARALLEL = 331,
     METRIC = 332,
     MINIMIZE = 333,
     MAXIMIZE = 334,
     HASHT = 335,
     DURATION_VAR = 336,
     TOTAL_TIME = 337,
     INCREASE = 338,
     DECREASE = 339,
     SCALE_UP = 340,
     SCALE_DOWN = 341,
     ASSIGN = 342,
     GREATER = 343,
     GREATEQ = 344,
     LESS = 345,
     LESSEQ = 346,
     Q = 347,
     COLON = 348,
     NUMBER = 349,
     ALWAYS = 350,
     SOMETIME = 351,
     WITHIN = 352,
     ATMOSTONCE = 353,
     SOMETIMEAFTER = 354,
     SOMETIMEBEFORE = 355,
     ALWAYSWITHIN = 356,
     HOLDDURING = 357,
     HOLDAFTER = 358,
     ISVIOLATED = 359,
     BOGUS = 360,
     NAME = 361,
     FUNCTION_SYMBOL = 362,
     INTVAL = 363,
     FLOATVAL = 364,
     AT_TIME = 365,
     PLUS = 366,
     HYPHEN = 367,
     DIV = 368,
     MUL = 369,
     UMINUS = 370
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 79 "src/pddl+.yacc"

    parse_category* t_parse_category;

    effect_lists* t_effect_lists;
    effect* t_effect;
    simple_effect* t_simple_effect;
    cond_effect*   t_cond_effect;
    forall_effect* t_forall_effect;
    timed_effect* t_timed_effect;

    quantifier t_quantifier;
    metric_spec*  t_metric;
    optimization t_optimization;

    symbol* t_symbol;
    var_symbol*   t_var_symbol;
    pddl_type*    t_type;
    pred_symbol*  t_pred_symbol;
    func_symbol*  t_func_symbol;
    const_symbol* t_const_symbol;
    class_symbol* t_class;

    parameter_symbol_list* t_parameter_symbol_list;
    var_symbol_list* t_var_symbol_list;
    const_symbol_list* t_const_symbol_list;
    pddl_type_list* t_type_list;

    proposition* t_proposition;
    pred_decl* t_pred_decl;
    pred_decl_list* t_pred_decl_list;
    func_decl* t_func_decl;
    func_decl_list* t_func_decl_list;

    goal* t_goal;
    con_goal * t_con_goal;
    goal_list* t_goal_list;

    func_term* t_func_term;
    assignment* t_assignment;
    expression* t_expression;
    num_expression* t_num_expression;
    assign_op t_assign_op;
    comparison_op t_comparison_op;

    structure_def* t_structure_def;
    structure_store* t_structure_store;

    action* t_action_def;
    event* t_event_def;
    process* t_process_def;
    durative_action* t_durative_action_def;
    derivation_rule* t_derivation_rule;

    problem* t_problem;
    length_spec* t_length_spec;

    domain* t_domain;

    pddl_req_flag t_pddl_req_flag;

    plan* t_plan;
    plan_step* t_step;

    int ival;
    double fval;

    char* cp;
    int t_dummy;

    var_symbol_table * vtab;

  class_def * t_class_def;
  //  classes_list* t_classes;




/* Line 214 of yacc.c  */
#line 364 "src/pddl+.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 376 "src/pddl+.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  17
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   967

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  116
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  129
/* YYNRULES -- Number of rules.  */
#define YYNRULES  350
/* YYNRULES -- Number of states.  */
#define YYNSTATES  810

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   370

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    15,    20,    23,    26,
      29,    32,    35,    38,    41,    43,    48,    50,    52,    57,
      60,    61,    66,    71,    74,    75,    78,    80,    85,    89,
      91,    93,    95,    97,   100,   101,   107,   111,   114,   115,
     117,   122,   127,   129,   133,   134,   139,   144,   146,   149,
     150,   153,   154,   159,   164,   166,   169,   173,   174,   176,
     178,   180,   182,   187,   189,   191,   194,   195,   198,   199,
     206,   209,   212,   215,   216,   221,   224,   227,   230,   231,
     233,   235,   237,   239,   241,   246,   248,   250,   252,   254,
     257,   260,   263,   264,   269,   274,   279,   287,   293,   299,
     301,   303,   306,   307,   312,   317,   323,   329,   333,   339,
     345,   349,   354,   362,   368,   370,   373,   374,   379,   381,
     383,   385,   387,   390,   393,   396,   397,   403,   409,   415,
     421,   427,   433,   439,   444,   447,   448,   450,   453,   455,
     457,   463,   469,   475,   481,   486,   493,   503,   513,   515,
     517,   519,   521,   524,   525,   530,   532,   537,   539,   547,
     553,   559,   565,   571,   577,   583,   588,   594,   600,   606,
     612,   614,   616,   622,   628,   630,   632,   634,   639,   644,
     646,   653,   658,   663,   665,   667,   669,   671,   673,   675,
     677,   682,   690,   694,   697,   702,   708,   713,   721,   723,
     728,   734,   739,   747,   750,   752,   757,   763,   765,   768,
     770,   775,   783,   788,   793,   798,   804,   809,   815,   821,
     828,   835,   841,   843,   848,   853,   858,   864,   872,   880,
     886,   889,   891,   894,   896,   898,   900,   905,   910,   915,
     920,   925,   930,   935,   940,   945,   950,   955,   958,   960,
     962,   964,   966,   968,   970,   972,   978,   980,   986,   999,
    1004,  1017,  1022,  1035,  1040,  1052,  1057,  1061,  1065,  1066,
    1068,  1073,  1076,  1077,  1082,  1087,  1092,  1098,  1103,  1105,
    1107,  1109,  1111,  1113,  1115,  1117,  1119,  1121,  1123,  1125,
    1127,  1129,  1131,  1133,  1135,  1137,  1139,  1141,  1143,  1145,
    1147,  1149,  1151,  1153,  1158,  1163,  1176,  1182,  1185,  1188,
    1191,  1194,  1197,  1200,  1203,  1204,  1209,  1214,  1216,  1221,
    1227,  1232,  1240,  1246,  1252,  1254,  1256,  1260,  1262,  1264,
    1266,  1271,  1275,  1279,  1283,  1287,  1291,  1293,  1296,  1298,
    1301,  1304,  1308,  1312,  1313,  1317,  1319,  1324,  1326,  1331,
    1333
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     117,     0,    -1,   118,    -1,   227,    -1,   240,    -1,     3,
      11,   120,   119,     4,    -1,     3,    11,   120,     1,    -1,
     125,   119,    -1,   226,   119,    -1,   225,   119,    -1,   206,
     119,    -1,   207,   119,    -1,   208,   119,    -1,   123,   119,
      -1,   210,    -1,     3,    12,   106,     4,    -1,   106,    -1,
     106,    -1,     3,     9,   124,     4,    -1,   121,   124,    -1,
      -1,     3,    13,   126,     4,    -1,     3,    13,     1,     4,
      -1,   126,   224,    -1,    -1,   128,   127,    -1,   128,    -1,
       3,   129,   136,     4,    -1,     3,     1,     4,    -1,   106,
      -1,    73,    -1,   106,    -1,   106,    -1,   132,   133,    -1,
      -1,     3,   135,   136,     4,   134,    -1,     3,     1,     4,
      -1,   112,    94,    -1,    -1,   106,    -1,   137,   112,   149,
     136,    -1,   137,   112,   147,   136,    -1,   137,    -1,    92,
     143,   137,    -1,    -1,   140,   112,   149,   138,    -1,   140,
     112,   147,   138,    -1,   140,    -1,   145,   139,    -1,    -1,
     146,   140,    -1,    -1,   150,   112,   149,   141,    -1,   150,
     112,   147,   141,    -1,   150,    -1,   142,   145,    -1,   142,
      92,   144,    -1,    -1,   106,    -1,   106,    -1,   106,    -1,
     106,    -1,     3,    67,   151,     4,    -1,   106,    -1,   106,
      -1,   150,   148,    -1,    -1,   151,   149,    -1,    -1,   152,
       3,    73,   188,   187,     4,    -1,   152,   181,    -1,   152,
     180,    -1,   152,   153,    -1,    -1,     3,   110,   152,     4,
      -1,   156,   154,    -1,   183,   154,    -1,   182,   154,    -1,
      -1,   159,    -1,   179,    -1,   178,    -1,   183,    -1,   182,
      -1,     3,    59,   158,     4,    -1,   157,    -1,   178,    -1,
     179,    -1,   184,    -1,   158,   178,    -1,   158,   179,    -1,
     158,   184,    -1,    -1,     3,    59,   154,     4,    -1,     3,
      59,     1,     4,    -1,     3,    59,   161,     4,    -1,     3,
     201,     3,   136,     4,   160,     4,    -1,     3,    65,   220,
     160,     4,    -1,     3,    66,   198,   164,     4,    -1,   162,
      -1,   184,    -1,   161,   160,    -1,    -1,     3,    56,   166,
       4,    -1,     3,    57,   166,     4,    -1,     3,    83,   188,
     186,     4,    -1,     3,    84,   188,   186,     4,    -1,     3,
       1,     4,    -1,     3,    83,   188,   186,     4,    -1,     3,
      84,   188,   186,     4,    -1,     3,     1,     4,    -1,     3,
      59,   165,     4,    -1,     3,   201,     3,   136,     4,   164,
       4,    -1,     3,    66,   198,   164,     4,    -1,   163,    -1,
     165,   164,    -1,    -1,     3,    59,   168,     4,    -1,   167,
      -1,   178,    -1,   179,    -1,   169,    -1,   168,   178,    -1,
     168,   179,    -1,   168,   169,    -1,    -1,     3,    87,   188,
     172,     4,    -1,     3,    83,   188,   172,     4,    -1,     3,
      84,   188,   172,     4,    -1,     3,    85,   188,   172,     4,
      -1,     3,    86,   188,   172,     4,    -1,     3,    83,   188,
     186,     4,    -1,     3,    84,   188,   186,     4,    -1,     3,
      59,   171,     4,    -1,   171,   170,    -1,    -1,   173,    -1,
      92,    81,    -1,   187,    -1,   188,    -1,     3,   111,   172,
     172,     4,    -1,     3,   112,   172,   172,     4,    -1,     3,
     114,   172,   172,     4,    -1,     3,   113,   172,   172,     4,
      -1,     3,    59,   177,     4,    -1,     3,   175,    92,    81,
     176,     4,    -1,     3,    56,     3,   175,    92,    81,   176,
       4,     4,    -1,     3,    57,     3,   175,    92,    81,   176,
       4,     4,    -1,    91,    -1,    89,    -1,    73,    -1,   185,
      -1,   177,   174,    -1,    -1,     3,    64,   203,     4,    -1,
     203,    -1,     3,    64,   205,     4,    -1,   205,    -1,     3,
     201,     3,   136,     4,   155,     4,    -1,     3,    65,   198,
     154,     4,    -1,     3,    87,   188,   185,     4,    -1,     3,
      83,   188,   185,     4,    -1,     3,    84,   188,   185,     4,
      -1,     3,    85,   188,   185,     4,    -1,     3,    86,   188,
     185,     4,    -1,     3,   112,   185,     4,    -1,     3,   111,
     185,   185,     4,    -1,     3,   112,   185,   185,     4,    -1,
       3,   114,   185,   185,     4,    -1,     3,   113,   185,   185,
       4,    -1,   187,    -1,   188,    -1,     3,   114,    80,   185,
       4,    -1,     3,   114,   185,    80,     4,    -1,    80,    -1,
     108,    -1,   109,    -1,     3,   107,   142,     4,    -1,     3,
     106,   142,     4,    -1,   107,    -1,     3,   122,     8,   107,
     142,     4,    -1,     3,   107,   142,     4,    -1,     3,   106,
     142,     4,    -1,   107,    -1,    88,    -1,    89,    -1,    90,
      -1,    91,    -1,    73,    -1,   195,    -1,     3,    59,   199,
       4,    -1,     3,   201,     3,   136,     4,   191,     4,    -1,
       3,    59,     4,    -1,     3,     4,    -1,     3,    48,   197,
       4,    -1,     3,    48,   106,   197,     4,    -1,     3,    59,
     194,     4,    -1,     3,   201,     3,   136,     4,   193,     4,
      -1,   197,    -1,     3,    48,   197,     4,    -1,     3,    48,
     106,   197,     4,    -1,     3,    59,   194,     4,    -1,     3,
     201,     3,   136,     4,   193,     4,    -1,   194,   192,    -1,
     192,    -1,     3,    48,   198,     4,    -1,     3,    48,   106,
     198,     4,    -1,   198,    -1,   196,   197,    -1,   197,    -1,
       3,    59,   196,     4,    -1,     3,   201,     3,   136,     4,
     197,     4,    -1,     3,    57,   198,     4,    -1,     3,    95,
     198,     4,    -1,     3,    96,   198,     4,    -1,     3,    97,
     187,   198,     4,    -1,     3,    98,   198,     4,    -1,     3,
      99,   198,   198,     4,    -1,     3,   100,   198,   198,     4,
      -1,     3,   101,   187,   198,   198,     4,    -1,     3,   102,
     187,   187,   198,     4,    -1,     3,   103,   187,   198,     4,
      -1,   203,    -1,     3,    64,   198,     4,    -1,     3,    59,
     200,     4,    -1,     3,    60,   200,     4,    -1,     3,    63,
     198,   198,     4,    -1,     3,   201,     3,   136,     4,   198,
       4,    -1,     3,   202,     3,   136,     4,   198,     4,    -1,
       3,   190,   185,   185,     4,    -1,   199,   191,    -1,   191,
      -1,   200,   198,    -1,   198,    -1,    62,    -1,    61,    -1,
       3,   130,   142,     4,    -1,     3,   130,   136,     4,    -1,
       3,   131,   142,     4,    -1,     3,    42,   127,     4,    -1,
       3,    42,     1,     4,    -1,     3,    43,   132,     4,    -1,
       3,    43,     1,     4,    -1,     3,    35,   197,     4,    -1,
       3,    35,     1,     4,    -1,     3,    35,   192,     4,    -1,
       3,    35,     1,     4,    -1,   210,   211,    -1,   211,    -1,
     215,    -1,   216,    -1,   217,    -1,   218,    -1,   214,    -1,
     212,    -1,     3,    10,   122,   207,     4,    -1,    40,    -1,
       3,   213,   204,   198,     4,    -1,     3,    36,   106,   223,
       3,   136,     4,    46,   191,    51,   155,     4,    -1,     3,
      36,     1,     4,    -1,     3,    38,   106,   223,     3,   136,
       4,    46,   198,    51,   155,     4,    -1,     3,    38,     1,
       4,    -1,     3,    37,   106,   223,     3,   136,     4,    46,
     198,    51,   170,     4,    -1,     3,    37,     1,     4,    -1,
       3,    39,   106,   223,     3,   136,     4,    55,   174,   219,
       4,    -1,     3,    39,     1,     4,    -1,   219,    51,   160,
      -1,   219,    47,   220,    -1,    -1,   222,    -1,     3,    59,
     221,     4,    -1,   221,   220,    -1,    -1,     3,    56,   198,
       4,    -1,     3,    57,   198,     4,    -1,     3,    58,   198,
       4,    -1,     3,    48,   106,   222,     4,    -1,     3,    48,
     222,     4,    -1,    45,    -1,    14,    -1,    15,    -1,    18,
      -1,    17,    -1,    19,    -1,    20,    -1,    21,    -1,    23,
      -1,    24,    -1,    29,    -1,    28,    -1,    27,    -1,    25,
      -1,    26,    -1,     5,    -1,    16,    -1,    22,    -1,    30,
      -1,    31,    -1,    32,    -1,    33,    -1,    34,    -1,    35,
      -1,   106,    -1,     3,    41,   138,     4,    -1,     3,    44,
     141,     4,    -1,     3,    11,     3,    68,   106,     4,     3,
      69,   106,     4,   228,     4,    -1,     3,    11,     3,    68,
       1,    -1,   125,   228,    -1,   229,   228,    -1,   230,   228,
      -1,   232,   228,    -1,   209,   228,    -1,   233,   228,    -1,
     234,   228,    -1,    -1,     3,    71,   138,     4,    -1,     3,
      70,   152,     4,    -1,    72,    -1,     3,   231,   191,     4,
      -1,     3,    77,   235,   236,     4,    -1,     3,    77,     1,
       4,    -1,     3,    74,    75,   108,    76,   108,     4,    -1,
       3,    74,    75,   108,     4,    -1,     3,    74,    76,   108,
       4,    -1,    78,    -1,    79,    -1,     3,   237,     4,    -1,
     189,    -1,   187,    -1,    82,    -1,     3,   104,   106,     4,
      -1,     3,    82,     4,    -1,   111,   236,   238,    -1,   112,
     236,   236,    -1,   114,   236,   239,    -1,   113,   236,   236,
      -1,   236,    -1,   236,   238,    -1,   236,    -1,   236,   239,
      -1,   241,   240,    -1,    28,   109,   240,    -1,    28,   108,
     240,    -1,    -1,   244,    93,   242,    -1,   242,    -1,   243,
       6,   244,     7,    -1,   243,    -1,     3,   106,   139,     4,
      -1,   109,    -1,   108,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   260,   260,   261,   262,   266,   272,   279,   280,   281,
     282,   284,   286,   288,   289,   292,   296,   299,   301,   303,
     304,   308,   315,   322,   323,   328,   330,   335,   337,   345,
     353,   355,   363,   368,   370,   374,   376,   383,   383,   386,
     399,   408,   417,   429,   431,   437,   446,   456,   461,   462,
     466,   467,   475,   482,   491,   497,   499,   501,   508,   514,
     518,   522,   526,   531,   538,   543,   545,   549,   551,   555,
     568,   570,   572,   575,   579,   585,   586,   588,   590,   599,
     600,   601,   602,   603,   607,   608,   612,   614,   616,   623,
     624,   625,   627,   631,   633,   641,   643,   651,   656,   661,
     664,   671,   672,   676,   678,   680,   684,   688,   694,   698,
     702,   708,   710,   718,   723,   729,   730,   734,   735,   739,
     741,   743,   750,   751,   752,   754,   759,   761,   763,   765,
     767,   772,   778,   784,   789,   790,   794,   795,   797,   798,
     802,   804,   806,   808,   813,   815,   818,   821,   827,   828,
     829,   837,   841,   844,   848,   853,   860,   865,   870,   875,
     880,   882,   884,   886,   888,   893,   895,   897,   899,   901,
     903,   904,   908,   910,   912,   918,   919,   922,   925,   927,
     930,   947,   949,   951,   957,   958,   959,   960,   961,   973,
     980,   982,   986,   987,   991,   993,   995,   997,  1001,  1006,
    1008,  1010,  1012,  1019,  1021,  1026,  1028,  1032,  1037,  1039,
    1044,  1046,  1049,  1051,  1053,  1055,  1057,  1059,  1061,  1063,
    1065,  1067,  1072,  1074,  1078,  1080,  1083,  1086,  1089,  1092,
    1098,  1100,  1105,  1107,  1117,  1124,  1131,  1136,  1141,  1146,
    1148,  1155,  1157,  1164,  1166,  1173,  1175,  1182,  1183,  1187,
    1188,  1189,  1190,  1191,  1192,  1195,  1204,  1210,  1219,  1230,
    1237,  1248,  1254,  1264,  1270,  1285,  1292,  1294,  1296,  1300,
    1302,  1307,  1310,  1314,  1316,  1318,  1320,  1325,  1330,  1335,
    1336,  1338,  1339,  1341,  1343,  1344,  1345,  1346,  1347,  1349,
    1352,  1355,  1356,  1357,  1359,  1368,  1371,  1374,  1376,  1378,
    1380,  1382,  1384,  1390,  1394,  1399,  1411,  1418,  1419,  1420,
    1421,  1422,  1424,  1426,  1427,  1430,  1433,  1436,  1439,  1443,
    1445,  1452,  1455,  1459,  1466,  1467,  1472,  1473,  1474,  1475,
    1476,  1478,  1482,  1483,  1484,  1485,  1489,  1490,  1495,  1496,
    1502,  1505,  1507,  1510,  1514,  1518,  1524,  1528,  1534,  1542,
    1543
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "OPEN_BRAC", "CLOSE_BRAC", "MODULES",
  "OPEN_SQ", "CLOSE_SQ", "DOT", "CLASSES", "CLASS", "DEFINE", "PDDLDOMAIN",
  "REQS", "EQUALITY", "STRIPS", "ADL", "NEGATIVE_PRECONDITIONS", "TYPING",
  "DISJUNCTIVE_PRECONDS", "EXT_PRECS", "UNIV_PRECS", "QUANT_PRECS",
  "COND_EFFS", "FLUENTS", "OBJECTFLUENTS", "NUMERICFLUENTS", "ACTIONCOSTS",
  "TIME", "DURATIVE_ACTIONS", "DURATION_INEQUALITIES",
  "CONTINUOUS_EFFECTS", "DERIVED_PREDICATES", "TIMED_INITIAL_LITERALS",
  "PREFERENCES", "CONSTRAINTS", "ACTION", "PROCESS", "EVENT",
  "DURATIVE_ACTION", "DERIVED", "CONSTANTS", "PREDS", "FUNCTIONS", "TYPES",
  "ARGS", "PRE", "CONDITION", "PREFERENCE", "START_PRE", "END_PRE",
  "EFFECTS", "INITIAL_EFFECT", "FINAL_EFFECT", "INVARIANT", "DURATION",
  "AT_START", "AT_END", "OVER_ALL", "AND", "OR", "EXISTS", "FORALL",
  "IMPLY", "NOT", "WHEN", "WHENEVER", "EITHER", "PROBLEM", "FORDOMAIN",
  "INITIALLY", "OBJECTS", "GOALS", "EQ", "LENGTH", "SERIAL", "PARALLEL",
  "METRIC", "MINIMIZE", "MAXIMIZE", "HASHT", "DURATION_VAR", "TOTAL_TIME",
  "INCREASE", "DECREASE", "SCALE_UP", "SCALE_DOWN", "ASSIGN", "GREATER",
  "GREATEQ", "LESS", "LESSEQ", "Q", "COLON", "NUMBER", "ALWAYS",
  "SOMETIME", "WITHIN", "ATMOSTONCE", "SOMETIMEAFTER", "SOMETIMEBEFORE",
  "ALWAYSWITHIN", "HOLDDURING", "HOLDAFTER", "ISVIOLATED", "BOGUS", "NAME",
  "FUNCTION_SYMBOL", "INTVAL", "FLOATVAL", "AT_TIME", "PLUS", "HYPHEN",
  "DIV", "MUL", "UMINUS", "$accept", "mystartsymbol", "c_domain",
  "c_preamble", "c_domain_name", "c_new_class", "c_class", "c_classes",
  "c_class_seq", "c_domain_require_def", "c_reqs", "c_pred_decls",
  "c_pred_decl", "c_new_pred_symbol", "c_pred_symbol",
  "c_init_pred_symbol", "c_func_decls", "c_func_decl", "c_ntype",
  "c_new_func_symbol", "c_typed_var_list", "c_var_symbol_list",
  "c_typed_consts", "c_const_symbols", "c_new_const_symbols",
  "c_typed_types", "c_parameter_symbols", "c_declaration_var_symbol",
  "c_var_symbol", "c_const_symbol", "c_new_const_symbol", "c_either_type",
  "c_new_primitive_type", "c_primitive_type", "c_new_primitive_types",
  "c_primitive_types", "c_init_els", "c_timed_initial_literal",
  "c_effects", "c_effect", "c_a_effect", "c_p_effect", "c_p_effects",
  "c_conj_effect", "c_da_effect", "c_da_effects", "c_timed_effect",
  "c_cts_only_timed_effect", "c_da_cts_only_effect",
  "c_da_cts_only_effects", "c_a_effect_da", "c_p_effect_da",
  "c_p_effects_da", "c_f_assign_da", "c_proc_effect", "c_proc_effects",
  "c_f_exp_da", "c_binary_expr_da", "c_duration_constraint", "c_d_op",
  "c_d_value", "c_duration_constraints", "c_neg_simple_effect",
  "c_pos_simple_effect", "c_init_neg_simple_effect",
  "c_init_pos_simple_effect", "c_forall_effect", "c_cond_effect",
  "c_assignment", "c_f_exp", "c_f_exp_t", "c_number", "c_f_head",
  "c_ground_f_head", "c_comparison_op", "c_pre_goal_descriptor",
  "c_pref_con_goal", "c_pref_goal", "c_pref_con_goal_list",
  "c_pref_goal_descriptor", "c_constraint_goal_list", "c_constraint_goal",
  "c_goal_descriptor", "c_pre_goal_descriptor_list", "c_goal_list",
  "c_forall", "c_exists", "c_proposition", "c_derived_proposition",
  "c_init_proposition", "c_predicates", "c_functions_def",
  "c_constraints_def", "c_constraints_probdef", "c_structure_defs",
  "c_structure_def", "c_class_def", "c_rule_head", "c_derivation_rule",
  "c_action_def", "c_event_def", "c_process_def", "c_durative_action_def",
  "c_da_def_body", "c_da_gd", "c_da_gds", "c_timed_gd", "c_args_head",
  "c_require_key", "c_domain_constants", "c_type_names", "c_problem",
  "c_problem_body", "c_objects", "c_initial_state", "c_goals",
  "c_goal_spec", "c_metric_spec", "c_length_spec", "c_optimization",
  "c_ground_f_exp", "c_binary_ground_f_exp", "c_binary_ground_f_pexps",
  "c_binary_ground_f_mexps", "c_plan", "c_step_t_d", "c_step_d", "c_step",
  "c_float", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   116,   117,   117,   117,   118,   118,   119,   119,   119,
     119,   119,   119,   119,   119,   120,   121,   122,   123,   124,
     124,   125,   125,   126,   126,   127,   127,   128,   128,   129,
     130,   130,   131,   132,   132,   133,   133,   134,   134,   135,
     136,   136,   136,   137,   137,   138,   138,   138,   139,   139,
     140,   140,   141,   141,   141,   142,   142,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   150,   151,   151,   152,
     152,   152,   152,   152,   153,   154,   154,   154,   154,   155,
     155,   155,   155,   155,   156,   156,   157,   157,   157,   158,
     158,   158,   158,   159,   159,   160,   160,   160,   160,   160,
     160,   161,   161,   162,   162,   162,   162,   162,   163,   163,
     163,   164,   164,   164,   164,   165,   165,   166,   166,   167,
     167,   167,   168,   168,   168,   168,   169,   169,   169,   169,
     169,   170,   170,   170,   171,   171,   172,   172,   172,   172,
     173,   173,   173,   173,   174,   174,   174,   174,   175,   175,
     175,   176,   177,   177,   178,   179,   180,   181,   182,   183,
     184,   184,   184,   184,   184,   185,   185,   185,   185,   185,
     185,   185,   186,   186,   186,   187,   187,   188,   188,   188,
     188,   189,   189,   189,   190,   190,   190,   190,   190,   191,
     191,   191,   191,   191,   192,   192,   192,   192,   192,   193,
     193,   193,   193,   194,   194,   195,   195,   195,   196,   196,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   197,
     197,   197,   198,   198,   198,   198,   198,   198,   198,   198,
     199,   199,   200,   200,   201,   202,   203,   204,   205,   206,
     206,   207,   207,   208,   208,   209,   209,   210,   210,   211,
     211,   211,   211,   211,   211,   212,   213,   214,   215,   215,
     216,   216,   217,   217,   218,   218,   219,   219,   219,   220,
     220,   221,   221,   222,   222,   222,   222,   222,   223,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   225,   226,   227,   227,   228,   228,   228,
     228,   228,   228,   228,   228,   229,   230,   231,   232,   233,
     233,   234,   234,   234,   235,   235,   236,   236,   236,   236,
     236,   236,   237,   237,   237,   237,   238,   238,   239,   239,
     240,   240,   240,   240,   241,   241,   242,   242,   243,   244,
     244
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     5,     4,     2,     2,     2,
       2,     2,     2,     2,     1,     4,     1,     1,     4,     2,
       0,     4,     4,     2,     0,     2,     1,     4,     3,     1,
       1,     1,     1,     2,     0,     5,     3,     2,     0,     1,
       4,     4,     1,     3,     0,     4,     4,     1,     2,     0,
       2,     0,     4,     4,     1,     2,     3,     0,     1,     1,
       1,     1,     4,     1,     1,     2,     0,     2,     0,     6,
       2,     2,     2,     0,     4,     2,     2,     2,     0,     1,
       1,     1,     1,     1,     4,     1,     1,     1,     1,     2,
       2,     2,     0,     4,     4,     4,     7,     5,     5,     1,
       1,     2,     0,     4,     4,     5,     5,     3,     5,     5,
       3,     4,     7,     5,     1,     2,     0,     4,     1,     1,
       1,     1,     2,     2,     2,     0,     5,     5,     5,     5,
       5,     5,     5,     4,     2,     0,     1,     2,     1,     1,
       5,     5,     5,     5,     4,     6,     9,     9,     1,     1,
       1,     1,     2,     0,     4,     1,     4,     1,     7,     5,
       5,     5,     5,     5,     5,     4,     5,     5,     5,     5,
       1,     1,     5,     5,     1,     1,     1,     4,     4,     1,
       6,     4,     4,     1,     1,     1,     1,     1,     1,     1,
       4,     7,     3,     2,     4,     5,     4,     7,     1,     4,
       5,     4,     7,     2,     1,     4,     5,     1,     2,     1,
       4,     7,     4,     4,     4,     5,     4,     5,     5,     6,
       6,     5,     1,     4,     4,     4,     5,     7,     7,     5,
       2,     1,     2,     1,     1,     1,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     2,     1,     1,
       1,     1,     1,     1,     1,     5,     1,     5,    12,     4,
      12,     4,    12,     4,    11,     4,     3,     3,     0,     1,
       4,     2,     0,     4,     4,     4,     5,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,    12,     5,     2,     2,     2,
       2,     2,     2,     2,     0,     4,     4,     1,     4,     5,
       4,     7,     5,     5,     1,     1,     3,     1,     1,     1,
       4,     3,     3,     3,     3,     3,     1,     2,     1,     2,
       2,     3,     3,     0,     3,     1,     4,     1,     4,     1,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     343,     0,     0,   350,   349,     0,     2,     3,     4,   343,
     345,   347,     0,     0,    49,   343,   343,     1,     0,   340,
       0,     0,     0,     0,    60,     0,    49,   342,   341,     0,
     344,     0,     0,     6,     0,     0,     0,     0,     0,     0,
       0,    14,   248,   254,   253,   249,   250,   251,   252,     0,
       0,   348,    48,   346,     0,   306,     0,    20,     0,     0,
       0,     0,     0,     0,     0,   256,    51,     0,     0,    66,
       0,     5,    13,     7,    10,    11,    12,     0,   247,     9,
       8,    15,     0,    16,    20,     0,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,    47,    51,     0,     0,     0,    26,     0,
       0,     0,    54,     0,     0,     0,    19,    18,     0,     0,
      22,    21,   293,   279,   280,   294,   282,   281,   283,   284,
     285,   295,   286,   287,   291,   292,   290,   289,   288,   296,
     297,   298,   299,   300,   301,   302,    23,   244,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,   259,   278,     0,   263,     0,   261,     0,   265,
       0,   303,     0,    50,   240,     0,    29,    44,   239,    25,
     242,     0,   241,    33,   304,    63,     0,    65,    30,    31,
      44,     0,     0,   222,     0,   255,     0,     0,   209,     0,
       0,   175,   176,     0,     0,     0,     0,     0,     0,     0,
      44,    44,    44,    44,    44,     0,    64,    51,    51,    28,
       0,     0,    42,     0,    39,    44,    66,    66,     0,     0,
       0,   235,     0,     0,   188,   184,   185,   186,   187,    57,
       0,     0,     0,   257,     0,   212,   210,   208,   213,   214,
       0,   216,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,    46,    45,    58,    44,    27,     0,    36,
       0,    53,    52,   237,   233,     0,     0,     0,     0,     0,
       0,   179,     0,   170,   171,    44,    44,   314,   215,   217,
     218,     0,     0,   221,     0,     0,     0,     0,     0,     0,
      43,    44,    44,    38,   224,   232,   225,     0,   223,   236,
       0,    55,    57,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   314,   314,     0,   314,   314,   314,   314,
     314,   219,   220,     0,     0,     0,     0,     0,    62,    67,
      41,    40,     0,    35,   226,    59,    56,     0,     0,     0,
       0,     0,     0,     0,   229,     0,     0,     0,    73,    51,
     317,     0,     0,     0,   307,   311,   305,   308,   309,   310,
     312,   313,   211,     0,     0,   189,   207,     0,     0,     0,
     268,    37,   178,   177,     0,   165,     0,     0,     0,    57,
       0,     0,     0,     0,     0,   198,     0,     0,     0,     0,
       0,   324,   325,     0,     0,   193,     0,     0,     0,     0,
       0,     0,     0,     0,   153,   150,   149,   148,     0,     0,
     166,   167,   169,   168,     0,   227,   228,   246,     0,     0,
       0,   245,     0,   316,    72,    71,    70,   157,   315,     0,
       0,   320,     0,   329,   183,   328,   327,     0,   318,     0,
       0,   192,   231,   207,     0,    44,     0,     0,    79,    81,
      80,    83,    82,   155,     0,     0,     0,     0,     0,     0,
       0,   264,     0,     0,   180,     0,     0,   204,     0,   198,
      44,     0,     0,    32,    73,    57,   322,     0,   323,     0,
       0,    57,    57,     0,     0,     0,     0,     0,   319,     0,
     205,   190,   230,     0,     0,     0,     0,     0,   258,   135,
       0,     0,   262,   260,     0,     0,   144,   152,     0,     0,
     267,   269,     0,   266,    99,   100,     0,   194,   196,   203,
       0,     0,     0,     0,     0,     0,     0,     0,   331,     0,
       0,     0,     0,     0,     0,     0,   326,   206,     0,     0,
       0,     0,    78,    85,    86,    87,    78,    78,    88,     0,
       0,    78,    44,     0,     0,     0,     0,     0,     0,   151,
       0,     0,     0,     0,   272,     0,     0,     0,   102,     0,
       0,     0,     0,     0,     0,     0,     0,   195,     0,   156,
       0,    74,   238,   321,   330,   182,   181,   336,   332,   333,
     335,   338,   334,     0,     0,    94,    92,     0,     0,    93,
      75,    77,    76,   154,     0,     0,   133,   134,     0,   174,
       0,     0,     0,     0,   145,     0,     0,     0,     0,     0,
       0,     0,   107,     0,     0,   118,   121,   119,   120,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    44,     0,
       0,    69,   337,   339,   191,     0,     0,     0,   159,     0,
       0,   131,   132,     0,     0,     0,   277,   273,   274,   275,
     270,   271,   125,     0,     0,     0,     0,     0,   103,   104,
      95,   101,     0,     0,   114,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,     0,
      84,    89,    90,    91,     0,     0,     0,     0,     0,   276,
       0,     0,     0,     0,     0,     0,    97,     0,   116,     0,
       0,     0,     0,    98,     0,   161,   105,   162,   106,   163,
     164,   160,     0,     0,     0,     0,    44,   158,     0,     0,
     146,   147,     0,   117,   124,   122,   123,     0,     0,     0,
     136,   138,   139,     0,     0,     0,     0,   110,     0,     0,
       0,     0,    44,     0,     0,     0,   199,   201,     0,   172,
     173,     0,     0,     0,     0,   137,   127,   128,   129,   130,
     126,   111,   115,     0,     0,     0,     0,    96,   200,     0,
       0,     0,     0,     0,   113,   108,   109,     0,     0,     0,
       0,     0,     0,     0,   202,   140,   141,   143,   142,   112
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,    35,    23,    84,   318,    36,    85,    37,
      89,   107,   108,   177,   239,   485,   110,   183,   343,   225,
     221,   222,   102,    25,   103,   111,   279,   266,   346,   311,
     104,   217,   187,   218,   112,   299,   396,   434,   551,   457,
     552,   553,   655,   458,   523,   640,   524,   684,   685,   758,
     634,   635,   710,   636,   465,   563,   749,   750,   380,   418,
     568,   469,   554,   555,   435,   436,   556,   557,   558,   569,
     620,   283,   284,   446,   240,   374,   477,   650,   478,   375,
     197,   333,   376,   454,   275,   507,   242,   193,   114,   437,
      38,    39,    40,   324,    41,    42,    43,    70,    44,    45,
      46,    47,    48,   419,   520,   631,   521,   164,   146,    49,
      50,     7,   325,   326,   327,   363,   328,   329,   330,   403,
     597,   497,   598,   602,     8,     9,    10,    11,    12
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -688
static const yytype_int16 yypact[] =
{
      48,    63,   121,  -688,  -688,    38,  -688,  -688,  -688,   176,
    -688,    62,   -20,    80,   -21,   176,   176,  -688,   -14,  -688,
     131,   251,    11,    57,  -688,   231,   -21,  -688,  -688,   261,
    -688,   177,     8,  -688,   720,   268,   286,   286,   286,   286,
     286,   290,  -688,  -688,  -688,  -688,  -688,  -688,  -688,   286,
     286,  -688,  -688,  -688,   294,  -688,   299,   203,   206,   495,
     274,    20,    25,    56,    79,  -688,   209,   329,   369,  -688,
     320,  -688,  -688,  -688,  -688,  -688,  -688,   702,  -688,  -688,
    -688,  -688,   324,  -688,   203,   330,  -688,   336,   341,   628,
     363,   624,   376,   398,   366,   409,   366,   412,   366,   416,
     366,  -688,   474,   354,   209,   484,   104,   486,   504,   494,
     244,   527,   -37,   -25,   532,   469,  -688,  -688,   497,   539,
    -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,  -688,  -688,  -688,  -688,  -688,   532,   542,
    -688,   532,   532,   153,   532,   532,   532,   153,   153,   153,
     570,  -688,  -688,  -688,   572,  -688,   576,  -688,   579,  -688,
     592,  -688,    19,  -688,  -688,   599,  -688,   514,  -688,  -688,
    -688,   108,  -688,  -688,  -688,  -688,    19,  -688,  -688,  -688,
     514,   508,   604,  -688,   507,  -688,   611,   371,  -688,   614,
     623,  -688,  -688,   532,   633,   532,   532,   532,   153,   532,
     514,   514,   514,   514,   514,   567,  -688,   209,   209,  -688,
     560,   668,   556,   670,  -688,   514,  -688,  -688,   673,   532,
     532,  -688,   532,   532,   135,  -688,  -688,  -688,  -688,  -688,
      75,   679,   681,  -688,   683,  -688,  -688,  -688,  -688,  -688,
     689,  -688,   696,   727,   532,   532,   728,   731,   739,   742,
     748,   749,  -688,  -688,  -688,  -688,   514,  -688,    19,  -688,
     750,  -688,  -688,  -688,  -688,   381,   396,   532,   767,   136,
     517,  -688,    75,  -688,  -688,   514,   514,   769,  -688,  -688,
    -688,   770,   773,  -688,   542,   740,   741,   747,   718,    86,
    -688,   514,   514,   676,  -688,  -688,  -688,   790,  -688,  -688,
     690,  -688,   787,  -688,    75,    75,    75,    75,   789,   794,
     795,   796,   462,   769,   769,   797,   769,   769,   769,   769,
     769,  -688,  -688,   798,   800,   532,   532,   801,  -688,  -688,
    -688,  -688,   711,  -688,  -688,  -688,  -688,   199,   207,    75,
      59,    75,    75,   699,  -688,   532,   532,   353,  -688,   209,
    -688,   332,   195,   800,  -688,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,   298,   756,  -688,  -688,   757,   758,   485,
    -688,  -688,  -688,  -688,   806,  -688,   807,   808,   809,  -688,
     810,   811,   812,   355,   813,  -688,   430,   814,   712,   713,
     815,  -688,  -688,    46,   818,  -688,    42,   439,   820,   821,
     822,   821,   823,   824,  -688,  -688,  -688,  -688,   736,    37,
    -688,  -688,  -688,  -688,   215,  -688,  -688,  -688,   109,   826,
     827,  -688,   182,  -688,  -688,  -688,  -688,  -688,  -688,    82,
     828,  -688,   498,  -688,  -688,  -688,  -688,   829,  -688,   532,
     830,  -688,  -688,  -688,   444,   514,   373,   831,  -688,  -688,
    -688,  -688,  -688,  -688,   166,   832,   833,   180,   180,   459,
     759,  -688,   835,   836,  -688,   542,   837,  -688,   468,  -688,
     514,   839,    34,  -688,  -688,  -688,  -688,   723,  -688,   840,
     737,  -688,  -688,    46,    46,    46,    46,   841,  -688,   842,
    -688,  -688,  -688,   843,   418,   845,   532,   846,  -688,  -688,
      34,    34,  -688,  -688,   760,   761,  -688,  -688,    75,   307,
    -688,  -688,   408,  -688,  -688,  -688,   847,  -688,  -688,  -688,
     850,   744,   851,   370,   153,   477,   230,   852,  -688,   853,
     232,   241,    46,    46,    46,    46,  -688,  -688,   800,   854,
     605,   855,   857,  -688,  -688,  -688,   857,   857,  -688,   -25,
     858,   857,   514,   483,    58,    58,   780,   782,   860,  -688,
     110,   532,   532,   532,  -688,   861,   863,   863,  -688,   835,
     532,    34,    34,    34,    34,    34,   864,  -688,   865,  -688,
     866,  -688,  -688,  -688,  -688,  -688,  -688,    46,  -688,  -688,
    -688,    46,  -688,   867,   810,  -688,  -688,    34,    34,  -688,
    -688,  -688,  -688,  -688,   868,   869,  -688,  -688,   755,  -688,
     870,   871,    75,    75,  -688,   292,   873,   874,   875,   876,
     877,   502,  -688,   612,   878,  -688,  -688,  -688,  -688,   879,
     561,   836,   881,    52,    52,    75,    75,    75,   514,   489,
     882,  -688,  -688,  -688,  -688,   574,    75,    75,  -688,   821,
      64,  -688,  -688,   883,   884,   885,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,    34,    34,    34,    34,    34,  -688,  -688,
    -688,  -688,   886,   269,  -688,   887,   669,   888,   889,   890,
     891,   892,   893,   894,   895,   116,   826,   897,  -688,   630,
    -688,  -688,  -688,  -688,   898,    75,   805,   899,   900,  -688,
     590,    99,    99,    99,    99,    99,  -688,   901,  -688,   532,
      34,    34,   903,  -688,    64,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,   836,   542,   904,   613,   514,  -688,   905,   906,
    -688,  -688,   664,  -688,  -688,  -688,  -688,   678,   834,   907,
    -688,  -688,  -688,   908,   909,   910,   912,  -688,   622,   881,
      58,    58,   514,    67,   913,   914,  -688,  -688,   915,  -688,
    -688,    99,    99,    99,    99,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,   916,   917,   918,   919,  -688,  -688,   865,
      99,    99,    99,    99,  -688,  -688,  -688,   881,   920,   921,
     922,   923,   924,   925,  -688,  -688,  -688,  -688,  -688,  -688
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -688,  -688,  -688,   729,  -688,  -688,   819,  -688,   817,   378,
    -688,   799,  -688,  -688,   825,  -688,  -688,  -688,  -688,  -688,
    -179,   665,  -189,   911,   838,   453,  -259,  -688,  -688,    51,
    -688,  -139,  -688,  -172,  -688,  -688,   446,  -688,  -151,  -407,
    -688,  -688,  -688,  -688,  -403,  -688,  -688,  -688,  -687,  -688,
     356,  -688,  -688,   222,   372,  -688,  -289,  -688,   465,   242,
     122,  -688,  -399,  -396,  -688,  -688,  -393,  -392,  -446,  -216,
    -540,  -152,  -395,  -688,  -688,  -343,  -354,   147,   243,  -688,
    -688,   -60,  -112,  -688,   710,   -83,  -688,  -359,  -688,   460,
    -688,   856,  -688,  -688,  -688,   926,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,  -688,  -549,  -688,  -339,   208,  -688,  -688,
    -688,  -688,   226,  -688,  -688,  -688,  -688,  -688,  -688,  -688,
    -344,  -688,   347,   344,   249,  -688,   927,  -688,   929
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -79
static const yytype_int16 yytable[] =
{
      92,   203,   192,   394,   466,   207,   208,   209,   160,    55,
     459,   228,   459,   460,   227,   460,   461,   462,   461,   462,
     404,    93,   215,    31,   282,   621,    95,   525,   263,   264,
     641,   257,   258,   259,   260,   261,   196,   533,    17,   199,
     200,   471,   204,   205,   206,   191,   270,   226,   188,   442,
     463,     1,   463,   347,   348,   686,   255,    97,    33,   447,
      34,   618,   280,   385,   452,    26,   319,   280,    20,   185,
     280,   782,   783,    21,    13,   186,     2,    26,   280,    32,
      99,   189,   671,    22,   472,    24,   486,   534,   473,   198,
     338,   250,    14,   252,   253,   254,   302,   256,   349,   350,
     351,   352,   747,   688,   690,   175,   320,   321,   241,   223,
     803,   502,    91,   625,    56,   564,   565,   274,   274,    91,
     277,   278,   340,   341,   529,   216,    94,   339,   443,   301,
     424,    96,   619,   384,   386,   387,   388,   247,   619,   -30,
     309,   281,   291,   292,   705,   463,   560,   739,   449,   542,
     543,   544,   545,   444,   201,   202,     3,     4,   487,   281,
     201,   202,    98,   305,   305,   307,   281,   201,   202,    14,
     397,   281,   201,   202,   281,   201,   202,   637,   637,    18,
     638,   638,   281,   201,   202,   100,   643,   644,   645,   646,
     647,   748,   216,   463,   525,   525,   400,   463,   463,   599,
     600,   601,   463,   382,     2,   603,   281,   201,   202,   703,
     176,   383,   656,   657,   224,   475,   626,   463,   463,   474,
     784,   785,   733,   377,   378,   509,   536,   -30,   310,    15,
      16,   627,   540,   541,   592,    51,   595,   681,   682,     3,
       4,   -30,    24,   390,   391,   596,   481,   181,   182,   510,
     511,   445,   704,   415,    18,   482,   701,   601,    19,   702,
     459,   201,   202,   460,    27,    28,   461,   462,    53,   416,
     717,   417,    71,   401,   402,    90,   503,    91,   711,   712,
     713,   714,   715,    54,     3,     4,   525,   665,   483,    34,
     408,   310,   484,    77,   450,   453,   463,   395,    81,   310,
     463,   530,   405,    82,   166,    24,   168,   310,   170,    83,
     430,   745,    86,    24,   746,   101,   752,   752,   752,   752,
     752,    24,   310,   113,   310,   760,   761,   115,   718,   764,
     105,   150,   106,   310,   117,   719,    24,   499,    24,   118,
     570,   445,   445,   445,   445,   120,   406,    24,   571,   572,
     573,   463,   720,   721,   392,   570,   393,   407,   230,   231,
     150,   232,   233,   571,   572,   573,   574,   147,   476,   479,
     109,   234,   -34,   -34,    91,   246,   752,   752,   752,   752,
     161,   529,   590,   615,   191,   304,   235,   236,   237,   238,
     445,   445,   445,   445,   561,   752,   752,   752,   752,   191,
     306,   610,   162,   428,   189,   611,   612,   398,   399,   575,
     614,   163,   148,   165,   429,   526,   167,   150,   395,   549,
     169,   550,   -78,   753,   754,   755,   756,   687,   689,   691,
     692,   693,   504,   432,   433,   150,   604,   505,   506,   586,
     687,   689,   373,   451,   706,   445,   188,   373,   501,   445,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   628,
     629,   630,   379,   516,   576,   577,   172,   578,   642,   694,
     150,   393,   528,   579,   580,    59,   312,   313,   171,   189,
     432,   591,   790,   791,   792,   793,   464,   616,   174,   738,
     178,   581,   582,   583,   584,   585,    88,   357,   180,   -24,
     -24,   799,   800,   801,   802,   519,   670,   106,   763,   -24,
     -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,
     -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,
     -24,   184,   358,   359,   360,   191,   361,   695,   194,   362,
      68,   412,   413,   195,   414,    91,   148,   388,   696,   364,
     365,   150,   367,   368,   369,   370,   371,   768,   415,   751,
     751,   751,   751,   751,   522,   680,   697,   229,   230,   231,
     150,   232,   233,   210,   416,   211,   417,   699,   700,   212,
     489,   234,   213,   786,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   742,   743,   214,   235,   236,   237,   238,
     722,   -24,   490,   219,   491,   492,   220,   759,   243,   493,
     494,   495,   496,   244,   189,   245,   393,   767,   248,   751,
     751,   751,   751,   312,   313,   683,   781,   249,   314,   315,
     316,   317,   121,   122,   262,   734,   479,   251,   751,   751,
     751,   751,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   606,   323,   265,   150,   268,   505,
     506,   672,   267,   765,   269,   395,   505,   273,   188,   271,
     272,   148,   285,   149,   286,   188,   150,   287,   607,   608,
     583,   584,   585,   288,   505,   673,   674,   675,   676,   677,
     289,   323,   323,   188,   323,   323,   323,   323,   323,   514,
     515,   189,    58,   607,   608,   583,   584,   585,   189,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   505,    57,
      58,   290,   293,    59,   145,   294,   189,   188,    61,    62,
      63,    64,    65,   295,   663,   664,   296,   673,   674,   675,
     676,   677,   297,   298,   303,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    72,    73,    74,    75,    76,
     189,   308,   322,   337,   331,   312,   313,   332,    79,    80,
     314,   315,   316,   724,   312,   313,   334,   335,   342,   771,
     772,   773,   774,   336,   344,   -17,   345,   353,   354,   355,
     356,   366,   372,   373,   379,   381,   389,   409,   410,   411,
     420,   421,   422,   423,   425,   426,   427,   431,   438,   441,
     439,   440,   448,   455,   456,   464,   467,   468,   470,   393,
     480,   537,   488,   498,   500,   508,   512,   513,   519,   522,
     518,   527,   531,   539,   538,   546,   547,   548,   559,   562,
     483,   587,   566,   567,   588,   589,   593,   594,   605,   609,
     550,   622,   613,   623,   624,   632,   633,   648,   649,   660,
     651,   654,   658,   659,   661,   662,   625,    87,   666,   667,
     668,   669,   678,   679,   683,   739,   698,   707,   708,   709,
     716,   723,   725,   726,   727,   728,   729,   730,   731,   732,
     736,   116,   737,   740,   741,   757,   762,   179,   766,   769,
     770,   776,   777,   778,   779,   775,   780,   787,   788,   789,
     794,   795,   796,   797,   804,   805,   806,   807,   808,   809,
     535,   300,   744,   639,   517,   617,   798,    52,   190,   735,
     276,   532,   173,   119,   652,   653,     0,     0,    30,    29,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    78
};

static const yytype_int16 yycheck[] =
{
      60,   153,   114,   357,   411,   157,   158,   159,    91,     1,
     409,   190,   411,   409,   186,   411,   409,   409,   411,   411,
     363,     1,     3,    12,   240,   565,     1,   473,   217,   218,
     579,   210,   211,   212,   213,   214,   148,     3,     0,   151,
     152,     4,   154,   155,   156,     3,   225,   186,    73,     3,
     409,     3,   411,   312,   313,     3,   208,     1,     1,   403,
       3,     3,     3,     4,   407,    14,   282,     3,     6,   106,
       3,   758,   759,    93,    11,   112,    28,    26,     3,    68,
       1,   106,   631,     3,    47,   106,     4,   482,    51,   149,
       4,   203,   106,   205,   206,   207,   268,   209,   314,   315,
     316,   317,     3,   643,   644,     1,   285,   286,   191,     1,
     797,   454,     3,     3,   106,   510,   511,   229,   230,     3,
     232,   233,   301,   302,   478,   106,   106,   299,    82,   268,
     389,   106,    80,   349,   350,   351,   352,   197,    80,     4,
       4,   107,   254,   255,    80,   504,   505,    80,   106,   493,
     494,   495,   496,   107,   108,   109,   108,   109,    76,   107,
     108,   109,   106,   275,   276,   277,   107,   108,   109,   106,
     359,   107,   108,   109,   107,   108,   109,   576,   577,     3,
     576,   577,   107,   108,   109,   106,   581,   582,   583,   584,
     585,    92,   106,   552,   640,   641,     1,   556,   557,   543,
     544,   545,   561,     4,    28,   548,   107,   108,   109,   655,
     106,     4,   607,   608,   106,   106,   106,   576,   577,     4,
     760,   761,   106,   335,   336,    59,   485,    92,    92,   108,
     109,   570,   491,   492,     4,     4,     4,   640,   641,   108,
     109,   106,   106,   355,   356,     4,    64,     3,     4,    83,
      84,   403,   659,    73,     3,    73,   655,   601,     9,   655,
     659,   108,   109,   659,    15,    16,   659,   659,     7,    89,
       1,    91,     4,    78,    79,     1,   455,     3,   673,   674,
     675,   676,   677,   106,   108,   109,   732,   626,   106,     3,
     373,    92,   110,     3,   406,   407,   655,   357,     4,    92,
     659,   480,     4,     4,    96,   106,    98,    92,   100,   106,
     393,   710,   106,   106,   710,   106,   711,   712,   713,   714,
     715,   106,    92,     3,    92,   720,   721,     3,    59,   732,
       1,    62,     3,    92,     4,    66,   106,   449,   106,     3,
      48,   493,   494,   495,   496,     4,    48,   106,    56,    57,
      58,   710,    83,    84,     1,    48,     3,    59,    60,    61,
      62,    63,    64,    56,    57,    58,    59,     4,   428,   429,
       1,    73,     3,     4,     3,     4,   771,   772,   773,   774,
       4,   735,   534,   562,     3,     4,    88,    89,    90,    91,
     542,   543,   544,   545,   506,   790,   791,   792,   793,     3,
       4,   552,     4,    48,   106,   556,   557,    75,    76,     1,
     561,    45,    57,     4,    59,   475,     4,    62,   478,     1,
       4,     3,     4,   712,   713,   714,   715,   643,   644,   645,
     646,   647,    59,     3,     4,    62,   548,    64,    65,   522,
     656,   657,     3,     4,   660,   597,    73,     3,     4,   601,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   571,
     572,   573,     3,     4,    56,    57,   112,    59,   580,   648,
      62,     3,     4,    65,    66,    13,   106,   107,     4,   106,
       3,     4,   771,   772,   773,   774,     3,     4,     4,   705,
       4,    83,    84,    85,    86,    87,     1,    35,     4,     4,
       5,   790,   791,   792,   793,     3,     4,     3,   724,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,     4,    70,    71,    72,     3,    74,    48,    69,    77,
      43,    56,    57,     4,    59,     3,    57,   763,    59,   323,
     324,    62,   326,   327,   328,   329,   330,   736,    73,   711,
     712,   713,   714,   715,     3,     4,   649,    59,    60,    61,
      62,    63,    64,     3,    89,     3,    91,     3,     4,     3,
      82,    73,     3,   762,    95,    96,    97,    98,    99,   100,
     101,   102,   103,     3,     4,     3,    88,    89,    90,    91,
     683,   106,   104,     4,   106,   107,    92,   719,     4,   111,
     112,   113,   114,   106,   106,     4,     3,     4,     4,   771,
     772,   773,   774,   106,   107,     3,     4,     4,   111,   112,
     113,   114,     4,     5,    67,   695,   696,     4,   790,   791,
     792,   793,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    59,   287,   106,    62,   112,    64,
      65,    59,     4,   733,     4,   735,    64,     4,    73,   226,
     227,    57,     3,    59,     3,    73,    62,     4,    83,    84,
      85,    86,    87,     4,    64,    83,    84,    85,    86,    87,
       4,   323,   324,    73,   326,   327,   328,   329,   330,   467,
     468,   106,    10,    83,    84,    85,    86,    87,   106,    95,
      96,    97,    98,    99,   100,   101,   102,   103,    64,     9,
      10,     4,     4,    13,   106,     4,   106,    73,    36,    37,
      38,    39,    40,     4,   622,   623,     4,    83,    84,    85,
      86,    87,     4,     4,     4,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    36,    37,    38,    39,    40,
     106,     4,     3,    55,     4,   106,   107,     4,    49,    50,
     111,   112,   113,   114,   106,   107,    46,    46,   112,   111,
     112,   113,   114,    46,     4,     8,   106,     8,     4,     4,
       4,     4,     4,     3,     3,    94,   107,    51,    51,    51,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
     108,   108,     4,     3,     3,     3,     3,     3,    92,     3,
       3,   108,     4,     4,     4,     4,     4,     4,     3,     3,
      81,     4,     3,   106,     4,     4,     4,     4,     3,     3,
     106,     4,    92,    92,     4,     4,     4,     4,     4,     4,
       3,    81,     4,    81,     4,     4,     3,     3,     3,   114,
       4,     4,     4,     4,     4,     4,     3,    58,     4,     4,
       4,     4,     4,     4,     3,    80,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       3,    84,     4,     4,     4,     4,     3,   108,     4,     4,
       4,     4,     4,     4,     4,    81,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
     484,   266,   710,   577,   469,   563,   789,    26,   113,   696,
     230,   481,   104,    87,   597,   601,    -1,    -1,    21,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    41
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    28,   108,   109,   117,   118,   227,   240,   241,
     242,   243,   244,    11,   106,   108,   109,     0,     3,   240,
       6,    93,     3,   120,   106,   139,   145,   240,   240,   244,
     242,    12,    68,     1,     3,   119,   123,   125,   206,   207,
     208,   210,   211,   212,   214,   215,   216,   217,   218,   225,
     226,     4,   139,     7,   106,     1,   106,     9,    10,    13,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
     213,     4,   119,   119,   119,   119,   119,     3,   211,   119,
     119,     4,     4,   106,   121,   124,   106,   122,     1,   126,
       1,     3,   197,     1,   106,     1,   106,     1,   106,     1,
     106,   106,   138,   140,   146,     1,     3,   127,   128,     1,
     132,   141,   150,     3,   204,     3,   124,     4,     3,   207,
       4,     4,     5,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,   106,   224,     4,    57,    59,
      62,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     201,     4,     4,    45,   223,     4,   223,     4,   223,     4,
     223,     4,   112,   140,     4,     1,   106,   129,     4,   127,
       4,     3,     4,   133,     4,   106,   112,   148,    73,   106,
     130,     3,   198,   203,    69,     4,   198,   196,   197,   198,
     198,   108,   109,   187,   198,   198,   198,   187,   187,   187,
       3,     3,     3,     3,     3,     3,   106,   147,   149,     4,
      92,   136,   137,     1,   106,   135,   147,   149,   136,    59,
      60,    61,    63,    64,    73,    88,    89,    90,    91,   130,
     190,   201,   202,     4,   106,     4,     4,   197,     4,     4,
     198,     4,   198,   198,   198,   187,   198,   136,   136,   136,
     136,   136,    67,   138,   138,   106,   143,     4,   112,     4,
     136,   141,   141,     4,   198,   200,   200,   198,   198,   142,
       3,   107,   185,   187,   188,     3,     3,     4,     4,     4,
       4,   198,   198,     4,     4,     4,     4,     4,     4,   151,
     137,   147,   149,     4,     4,   198,     4,   198,     4,     4,
      92,   145,   106,   107,   111,   112,   113,   114,   122,   185,
     136,   136,     3,   125,   209,   228,   229,   230,   232,   233,
     234,     4,     4,   197,    46,    46,    46,    55,     4,   149,
     136,   136,   112,   134,     4,   106,   144,   142,   142,   185,
     185,   185,   185,     8,     4,     4,     4,    35,    70,    71,
      72,    74,    77,   231,   228,   228,     4,   228,   228,   228,
     228,   228,     4,     3,   191,   195,   198,   198,   198,     3,
     174,    94,     4,     4,   185,     4,   185,   185,   185,   107,
     198,   198,     1,     3,   192,   197,   152,   138,    75,    76,
       1,    78,    79,   235,   191,     4,    48,    59,   201,    51,
      51,    51,    56,    57,    59,    73,    89,    91,   175,   219,
       4,     4,     4,     4,   142,     4,     4,     4,    48,    59,
     201,     4,     3,     4,   153,   180,   181,   205,     4,   108,
     108,     4,     3,    82,   107,   187,   189,   236,     4,   106,
     198,     4,   191,   198,   199,     3,     3,   155,   159,   178,
     179,   182,   183,   203,     3,   170,   155,     3,     3,   177,
      92,     4,    47,    51,     4,   106,   197,   192,   194,   197,
       3,    64,    73,   106,   110,   131,     4,    76,     4,    82,
     104,   106,   107,   111,   112,   113,   114,   237,     4,   198,
       4,     4,   191,   136,    59,    64,    65,   201,     4,    59,
      83,    84,     4,     4,   175,   175,     4,   174,    81,     3,
     220,   222,     3,   160,   162,   184,   197,     4,     4,   192,
     136,     3,   205,     3,   188,   152,   142,   108,     4,   106,
     142,   142,   236,   236,   236,   236,     4,     4,     4,     1,
       3,   154,   156,   157,   178,   179,   182,   183,   184,     3,
     203,   198,     3,   171,   188,   188,    92,    92,   176,   185,
      48,    56,    57,    58,    59,     1,    56,    57,    59,    65,
      66,    83,    84,    85,    86,    87,   201,     4,     4,     4,
     187,     4,     4,     4,     4,     4,     4,   236,   238,   236,
     236,   236,   239,   191,   198,     4,    59,    83,    84,     4,
     154,   154,   154,     4,   154,   136,     4,   170,     3,    80,
     186,   186,    81,    81,     4,     3,   106,   222,   198,   198,
     198,   221,     4,     3,   166,   167,   169,   178,   179,   166,
     161,   220,   198,   188,   188,   188,   188,   188,     3,     3,
     193,     4,   238,   239,     4,   158,   188,   188,     4,     4,
     114,     4,     4,   176,   176,   222,     4,     4,     4,     4,
       4,   220,    59,    83,    84,    85,    86,    87,     4,     4,
       4,   160,   160,     3,   163,   164,     3,   185,   186,   185,
     186,   185,   185,   185,   136,    48,    59,   201,     4,     3,
       4,   178,   179,   184,   155,    80,   185,     4,     4,     4,
     168,   188,   188,   188,   188,   188,     4,     1,    59,    66,
      83,    84,   201,     4,   114,     4,     4,     4,     4,     4,
       4,     4,     4,   106,   197,   194,     3,     4,   185,    80,
       4,     4,     3,     4,   169,   178,   179,     3,    92,   172,
     173,   187,   188,   172,   172,   172,   172,     4,   165,   198,
     188,   188,     3,   185,   160,   197,     4,     4,   136,     4,
       4,   111,   112,   113,   114,    81,     4,     4,     4,     4,
       4,     4,   164,   164,   186,   186,   136,     4,     4,     4,
     172,   172,   172,   172,     4,     4,     4,     4,   193,   172,
     172,   172,   172,   164,     4,     4,     4,     4,     4,     4
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1464 of yacc.c  */
#line 260 "src/pddl+.yacc"
    {top_thing= (yyvsp[(1) - (1)].t_domain); current_analysis->the_domain= (yyvsp[(1) - (1)].t_domain);;}
    break;

  case 3:

/* Line 1464 of yacc.c  */
#line 261 "src/pddl+.yacc"
    {top_thing= (yyvsp[(1) - (1)].t_problem); current_analysis->the_problem= (yyvsp[(1) - (1)].t_problem);;}
    break;

  case 4:

/* Line 1464 of yacc.c  */
#line 262 "src/pddl+.yacc"
    {top_thing= (yyvsp[(1) - (1)].t_plan); ;}
    break;

  case 5:

/* Line 1464 of yacc.c  */
#line 267 "src/pddl+.yacc"
    {(yyval.t_domain)= (yyvsp[(4) - (5)].t_domain); (yyval.t_domain)->name= (yyvsp[(3) - (5)].cp);delete [] (yyvsp[(3) - (5)].cp);
	if (types_used && !types_defined) {
		yyerrok; log_error(E_FATAL,"Syntax error in domain - no :types section, but types used in definitions.");
	}
	;}
    break;

  case 6:

/* Line 1464 of yacc.c  */
#line 273 "src/pddl+.yacc"
    {yyerrok; (yyval.t_domain)=static_cast<domain*>(NULL);
       	log_error(E_FATAL,"Syntax error in domain"); ;}
    break;

  case 7:

/* Line 1464 of yacc.c  */
#line 279 "src/pddl+.yacc"
    {(yyval.t_domain)= (yyvsp[(2) - (2)].t_domain); (yyval.t_domain)->req= (yyvsp[(1) - (2)].t_pddl_req_flag);;}
    break;

  case 8:

/* Line 1464 of yacc.c  */
#line 280 "src/pddl+.yacc"
    {types_defined = true; (yyval.t_domain)= (yyvsp[(2) - (2)].t_domain); (yyval.t_domain)->types= (yyvsp[(1) - (2)].t_type_list);;}
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 281 "src/pddl+.yacc"
    {(yyval.t_domain)= (yyvsp[(2) - (2)].t_domain); (yyval.t_domain)->constants= (yyvsp[(1) - (2)].t_const_symbol_list);;}
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 282 "src/pddl+.yacc"
    {(yyval.t_domain)= (yyvsp[(2) - (2)].t_domain);
                                       (yyval.t_domain)->predicates= (yyvsp[(1) - (2)].t_pred_decl_list); ;}
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 284 "src/pddl+.yacc"
    {(yyval.t_domain)= (yyvsp[(2) - (2)].t_domain);
                                       (yyval.t_domain)->functions= (yyvsp[(1) - (2)].t_func_decl_list); ;}
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 286 "src/pddl+.yacc"
    {(yyval.t_domain)= (yyvsp[(2) - (2)].t_domain);
   				       (yyval.t_domain)->constraints = (yyvsp[(1) - (2)].t_con_goal);;}
    break;

  case 13:

/* Line 1464 of yacc.c  */
#line 288 "src/pddl+.yacc"
    {(yyval.t_domain) = (yyvsp[(2) - (2)].t_domain);;}
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 289 "src/pddl+.yacc"
    {(yyval.t_domain)= new domain((yyvsp[(1) - (1)].t_structure_store)); ;}
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 292 "src/pddl+.yacc"
    {(yyval.cp)=(yyvsp[(3) - (4)].cp);;}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 296 "src/pddl+.yacc"
    { (yyval.t_class)=current_analysis->classes_tab.new_symbol_put((yyvsp[(1) - (1)].cp));
       delete [] (yyvsp[(1) - (1)].cp); ;}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 299 "src/pddl+.yacc"
    { (yyval.t_class) = current_analysis->classes_tab.symbol_get((yyvsp[(1) - (1)].cp)); delete [] (yyvsp[(1) - (1)].cp);;}
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 301 "src/pddl+.yacc"
    {(yyval.t_dummy) = 0;;}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 303 "src/pddl+.yacc"
    {(yyval.t_dummy) = 0;;}
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 304 "src/pddl+.yacc"
    {(yyval.t_dummy) = 0;;}
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 309 "src/pddl+.yacc"
    {
	// Stash in analysis object --- we need to refer to it during parse
	//   but domain object is not created yet,
	current_analysis->req |= (yyvsp[(3) - (4)].t_pddl_req_flag);
	(yyval.t_pddl_req_flag)=(yyvsp[(3) - (4)].t_pddl_req_flag);
    ;}
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 316 "src/pddl+.yacc"
    {yyerrok;
       log_error(E_FATAL,"Syntax error in requirements declaration.");
       (yyval.t_pddl_req_flag)= 0; ;}
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 322 "src/pddl+.yacc"
    { (yyval.t_pddl_req_flag)= (yyvsp[(1) - (2)].t_pddl_req_flag) | (yyvsp[(2) - (2)].t_pddl_req_flag); ;}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 323 "src/pddl+.yacc"
    { (yyval.t_pddl_req_flag)= 0; ;}
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 329 "src/pddl+.yacc"
    {(yyval.t_pred_decl_list)=(yyvsp[(2) - (2)].t_pred_decl_list); (yyval.t_pred_decl_list)->push_front((yyvsp[(1) - (2)].t_pred_decl));;}
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 331 "src/pddl+.yacc"
    {  (yyval.t_pred_decl_list)=new pred_decl_list;
           (yyval.t_pred_decl_list)->push_front((yyvsp[(1) - (1)].t_pred_decl)); ;}
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 336 "src/pddl+.yacc"
    {(yyval.t_pred_decl)= new pred_decl((yyvsp[(2) - (4)].t_pred_symbol),(yyvsp[(3) - (4)].t_var_symbol_list),current_analysis->var_tab_stack.pop());;}
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 338 "src/pddl+.yacc"
    {yyerrok;
        // hope someone makes this error someday
        log_error(E_FATAL,"Syntax error in predicate declaration.");
	(yyval.t_pred_decl)= static_cast<pred_decl*>(NULL); ;}
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 346 "src/pddl+.yacc"
    { (yyval.t_pred_symbol)=current_analysis->pred_tab.new_symbol_put((yyvsp[(1) - (1)].cp));
           current_analysis->var_tab_stack.push(
           				current_analysis->buildPredTab());
           delete [] (yyvsp[(1) - (1)].cp); ;}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 353 "src/pddl+.yacc"
    { (yyval.t_pred_symbol)=current_analysis->pred_tab.symbol_ref("=");
	      requires(E_EQUALITY); ;}
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 355 "src/pddl+.yacc"
    { (yyval.t_pred_symbol)=current_analysis->pred_tab.symbol_get((yyvsp[(1) - (1)].cp)); delete [] (yyvsp[(1) - (1)].cp); ;}
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 363 "src/pddl+.yacc"
    { (yyval.t_pred_symbol)=current_analysis->pred_tab.symbol_get((yyvsp[(1) - (1)].cp)); delete [] (yyvsp[(1) - (1)].cp);;}
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 369 "src/pddl+.yacc"
    {(yyval.t_func_decl_list)=(yyvsp[(1) - (2)].t_func_decl_list); (yyval.t_func_decl_list)->push_back((yyvsp[(2) - (2)].t_func_decl));;}
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 370 "src/pddl+.yacc"
    { (yyval.t_func_decl_list)=new func_decl_list; ;}
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 375 "src/pddl+.yacc"
    {(yyval.t_func_decl)= new func_decl((yyvsp[(2) - (5)].t_func_symbol),(yyvsp[(3) - (5)].t_var_symbol_list),current_analysis->var_tab_stack.pop());;}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 377 "src/pddl+.yacc"
    {yyerrok;
	 log_error(E_FATAL,"Syntax error in functor declaration.");
	 (yyval.t_func_decl)= (int) NULL; ;}
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 383 "src/pddl+.yacc"
    {(yyval.t_dummy) = (int) NULL;;}
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 383 "src/pddl+.yacc"
    {(yyval.t_dummy)= (int) NULL;;}
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 387 "src/pddl+.yacc"
    { (yyval.t_func_symbol)=current_analysis->func_tab.new_symbol_put((yyvsp[(1) - (1)].cp));
           current_analysis->var_tab_stack.push(
           		current_analysis->buildFuncTab());
           delete [] (yyvsp[(1) - (1)].cp); ;}
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 400 "src/pddl+.yacc"
    {
      (yyval.t_var_symbol_list)= (yyvsp[(1) - (4)].t_var_symbol_list);
      (yyval.t_var_symbol_list)->set_types((yyvsp[(3) - (4)].t_type));           /* Set types for variables */
      (yyval.t_var_symbol_list)->splice((yyval.t_var_symbol_list)->end(),*(yyvsp[(4) - (4)].t_var_symbol_list));   /* Join lists */
      delete (yyvsp[(4) - (4)].t_var_symbol_list);                   /* Delete (now empty) list */
      requires(E_TYPING);
      types_used = true;
   ;}
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 409 "src/pddl+.yacc"
    {
      (yyval.t_var_symbol_list)= (yyvsp[(1) - (4)].t_var_symbol_list);
      (yyval.t_var_symbol_list)->set_either_types((yyvsp[(3) - (4)].t_type_list));    /* Set types for variables */
      (yyval.t_var_symbol_list)->splice((yyval.t_var_symbol_list)->end(),*(yyvsp[(4) - (4)].t_var_symbol_list));   /* Join lists */
      delete (yyvsp[(4) - (4)].t_var_symbol_list);                   /* Delete (now empty) list */
      requires(E_TYPING);
      types_used = true;
   ;}
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 418 "src/pddl+.yacc"
    {
       (yyval.t_var_symbol_list)= (yyvsp[(1) - (1)].t_var_symbol_list);
   ;}
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 430 "src/pddl+.yacc"
    {(yyval.t_var_symbol_list)=(yyvsp[(3) - (3)].t_var_symbol_list); (yyvsp[(3) - (3)].t_var_symbol_list)->push_front((yyvsp[(2) - (3)].t_var_symbol)); ;}
    break;

  case 44:

/* Line 1464 of yacc.c  */
#line 431 "src/pddl+.yacc"
    {(yyval.t_var_symbol_list)= new var_symbol_list; ;}
    break;

  case 45:

/* Line 1464 of yacc.c  */
#line 438 "src/pddl+.yacc"
    {
      (yyval.t_const_symbol_list)= (yyvsp[(1) - (4)].t_const_symbol_list);
      (yyvsp[(1) - (4)].t_const_symbol_list)->set_types((yyvsp[(3) - (4)].t_type));           /* Set types for constants */
      (yyvsp[(1) - (4)].t_const_symbol_list)->splice((yyvsp[(1) - (4)].t_const_symbol_list)->end(),*(yyvsp[(4) - (4)].t_const_symbol_list)); /* Join lists */
      delete (yyvsp[(4) - (4)].t_const_symbol_list);                   /* Delete (now empty) list */
      requires(E_TYPING);
      types_used = true;
   ;}
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 447 "src/pddl+.yacc"
    {
      (yyval.t_const_symbol_list)= (yyvsp[(1) - (4)].t_const_symbol_list);
      (yyvsp[(1) - (4)].t_const_symbol_list)->set_either_types((yyvsp[(3) - (4)].t_type_list));
      (yyvsp[(1) - (4)].t_const_symbol_list)->splice((yyvsp[(1) - (4)].t_const_symbol_list)->end(),*(yyvsp[(4) - (4)].t_const_symbol_list));
      delete (yyvsp[(4) - (4)].t_const_symbol_list);
      requires(E_TYPING);
      types_used = true;
   ;}
    break;

  case 47:

/* Line 1464 of yacc.c  */
#line 456 "src/pddl+.yacc"
    {(yyval.t_const_symbol_list)= (yyvsp[(1) - (1)].t_const_symbol_list);;}
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 461 "src/pddl+.yacc"
    {(yyval.t_const_symbol_list)=(yyvsp[(2) - (2)].t_const_symbol_list); (yyvsp[(2) - (2)].t_const_symbol_list)->push_front((yyvsp[(1) - (2)].t_const_symbol));;}
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 462 "src/pddl+.yacc"
    {(yyval.t_const_symbol_list)=new const_symbol_list;;}
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 466 "src/pddl+.yacc"
    {(yyval.t_const_symbol_list)=(yyvsp[(2) - (2)].t_const_symbol_list); (yyvsp[(2) - (2)].t_const_symbol_list)->push_front((yyvsp[(1) - (2)].t_const_symbol));;}
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 467 "src/pddl+.yacc"
    {(yyval.t_const_symbol_list)=new const_symbol_list;;}
    break;

  case 52:

/* Line 1464 of yacc.c  */
#line 476 "src/pddl+.yacc"
    {
       (yyval.t_type_list)= (yyvsp[(1) - (4)].t_type_list);
       (yyval.t_type_list)->set_types((yyvsp[(3) - (4)].t_type));           /* Set types for constants */
       (yyval.t_type_list)->splice((yyval.t_type_list)->end(),*(yyvsp[(4) - (4)].t_type_list)); /* Join lists */
       delete (yyvsp[(4) - (4)].t_type_list);                   /* Delete (now empty) list */
   ;}
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 483 "src/pddl+.yacc"
    {
   // This parse needs to be excluded, we think (DPL&MF: 6/9/01)
       (yyval.t_type_list)= (yyvsp[(1) - (4)].t_type_list);
       (yyval.t_type_list)->set_either_types((yyvsp[(3) - (4)].t_type_list));
       (yyval.t_type_list)->splice((yyvsp[(1) - (4)].t_type_list)->end(),*(yyvsp[(4) - (4)].t_type_list));
       delete (yyvsp[(4) - (4)].t_type_list);
   ;}
    break;

  case 54:

/* Line 1464 of yacc.c  */
#line 492 "src/pddl+.yacc"
    { (yyval.t_type_list)= (yyvsp[(1) - (1)].t_type_list); ;}
    break;

  case 55:

/* Line 1464 of yacc.c  */
#line 498 "src/pddl+.yacc"
    {(yyval.t_parameter_symbol_list)=(yyvsp[(1) - (2)].t_parameter_symbol_list); (yyval.t_parameter_symbol_list)->push_back((yyvsp[(2) - (2)].t_const_symbol)); ;}
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 500 "src/pddl+.yacc"
    {(yyval.t_parameter_symbol_list)=(yyvsp[(1) - (3)].t_parameter_symbol_list); (yyval.t_parameter_symbol_list)->push_back((yyvsp[(3) - (3)].t_var_symbol)); ;}
    break;

  case 57:

/* Line 1464 of yacc.c  */
#line 501 "src/pddl+.yacc"
    {(yyval.t_parameter_symbol_list)= new parameter_symbol_list;;}
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 508 "src/pddl+.yacc"
    { (yyval.t_var_symbol)= current_analysis->var_tab_stack.top()->symbol_put((yyvsp[(1) - (1)].cp)); delete [] (yyvsp[(1) - (1)].cp); ;}
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 514 "src/pddl+.yacc"
    { (yyval.t_var_symbol)= current_analysis->var_tab_stack.symbol_get((yyvsp[(1) - (1)].cp)); delete [] (yyvsp[(1) - (1)].cp); ;}
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 518 "src/pddl+.yacc"
    { (yyval.t_const_symbol)= current_analysis->const_tab.symbol_get((yyvsp[(1) - (1)].cp)); delete [] (yyvsp[(1) - (1)].cp); ;}
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 522 "src/pddl+.yacc"
    { (yyval.t_const_symbol)= current_analysis->const_tab.new_symbol_put((yyvsp[(1) - (1)].cp)); delete [] (yyvsp[(1) - (1)].cp);;}
    break;

  case 62:

/* Line 1464 of yacc.c  */
#line 527 "src/pddl+.yacc"
    { (yyval.t_type_list)= (yyvsp[(3) - (4)].t_type_list); ;}
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 532 "src/pddl+.yacc"
    { (yyval.t_type)= current_analysis->pddl_type_tab.symbol_ref((yyvsp[(1) - (1)].cp)); delete [] (yyvsp[(1) - (1)].cp);;}
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 539 "src/pddl+.yacc"
    { (yyval.t_type)= current_analysis->pddl_type_tab.symbol_ref((yyvsp[(1) - (1)].cp)); delete [] (yyvsp[(1) - (1)].cp);;}
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 544 "src/pddl+.yacc"
    {(yyval.t_type_list)= (yyvsp[(1) - (2)].t_type_list); (yyval.t_type_list)->push_back((yyvsp[(2) - (2)].t_type));;}
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 545 "src/pddl+.yacc"
    {(yyval.t_type_list)= new pddl_type_list;;}
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 550 "src/pddl+.yacc"
    {(yyval.t_type_list)= (yyvsp[(1) - (2)].t_type_list); (yyval.t_type_list)->push_back((yyvsp[(2) - (2)].t_type));;}
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 551 "src/pddl+.yacc"
    {(yyval.t_type_list)= new pddl_type_list;;}
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 556 "src/pddl+.yacc"
    { (yyval.t_effect_lists)=(yyvsp[(1) - (6)].t_effect_lists);
	  (yyval.t_effect_lists)->assign_effects.push_back(new assignment((yyvsp[(4) - (6)].t_func_term),E_ASSIGN,(yyvsp[(5) - (6)].t_num_expression)));
          if((yyvsp[(4) - (6)].t_func_term)->getFunction()->getName()=="total-cost")
          {
          	requires(E_ACTIONCOSTS);
          	// Should also check that $5 is 0...
		  }
          else
          {
          	requires(E_NFLUENTS);
          }
	;}
    break;

  case 70:

/* Line 1464 of yacc.c  */
#line 569 "src/pddl+.yacc"
    { (yyval.t_effect_lists)=(yyvsp[(1) - (2)].t_effect_lists); (yyval.t_effect_lists)->add_effects.push_back((yyvsp[(2) - (2)].t_simple_effect)); ;}
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 571 "src/pddl+.yacc"
    { (yyval.t_effect_lists)=(yyvsp[(1) - (2)].t_effect_lists); (yyval.t_effect_lists)->del_effects.push_back((yyvsp[(2) - (2)].t_simple_effect)); ;}
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 573 "src/pddl+.yacc"
    { (yyval.t_effect_lists)=(yyvsp[(1) - (2)].t_effect_lists); (yyval.t_effect_lists)->timed_effects.push_back((yyvsp[(2) - (2)].t_timed_effect)); ;}
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 575 "src/pddl+.yacc"
    { (yyval.t_effect_lists)= new effect_lists;;}
    break;

  case 74:

/* Line 1464 of yacc.c  */
#line 580 "src/pddl+.yacc"
    { requires(E_TIMED_INITIAL_LITERALS);
   		(yyval.t_timed_effect)=new timed_initial_literal((yyvsp[(3) - (4)].t_effect_lists),(yyvsp[(2) - (4)].fval));;}
    break;

  case 75:

/* Line 1464 of yacc.c  */
#line 585 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=(yyvsp[(2) - (2)].t_effect_lists); (yyval.t_effect_lists)->append_effects((yyvsp[(1) - (2)].t_effect_lists)); delete (yyvsp[(1) - (2)].t_effect_lists);;}
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 586 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=(yyvsp[(2) - (2)].t_effect_lists); (yyval.t_effect_lists)->cond_effects.push_front((yyvsp[(1) - (2)].t_cond_effect));
                                      requires(E_COND_EFFS);;}
    break;

  case 77:

/* Line 1464 of yacc.c  */
#line 588 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=(yyvsp[(2) - (2)].t_effect_lists); (yyval.t_effect_lists)->forall_effects.push_front((yyvsp[(1) - (2)].t_forall_effect));
                                      requires(E_COND_EFFS);;}
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 590 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=new effect_lists(); ;}
    break;

  case 79:

/* Line 1464 of yacc.c  */
#line 599 "src/pddl+.yacc"
    {(yyval.t_effect_lists)= (yyvsp[(1) - (1)].t_effect_lists);;}
    break;

  case 80:

/* Line 1464 of yacc.c  */
#line 600 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->add_effects.push_front((yyvsp[(1) - (1)].t_simple_effect));;}
    break;

  case 81:

/* Line 1464 of yacc.c  */
#line 601 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->del_effects.push_front((yyvsp[(1) - (1)].t_simple_effect));;}
    break;

  case 82:

/* Line 1464 of yacc.c  */
#line 602 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->cond_effects.push_front((yyvsp[(1) - (1)].t_cond_effect));;}
    break;

  case 83:

/* Line 1464 of yacc.c  */
#line 603 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->forall_effects.push_front((yyvsp[(1) - (1)].t_forall_effect));;}
    break;

  case 84:

/* Line 1464 of yacc.c  */
#line 607 "src/pddl+.yacc"
    {(yyval.t_effect_lists)= (yyvsp[(3) - (4)].t_effect_lists);;}
    break;

  case 85:

/* Line 1464 of yacc.c  */
#line 608 "src/pddl+.yacc"
    {(yyval.t_effect_lists)= (yyvsp[(1) - (1)].t_effect_lists);;}
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 613 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->del_effects.push_front((yyvsp[(1) - (1)].t_simple_effect));;}
    break;

  case 87:

/* Line 1464 of yacc.c  */
#line 615 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->add_effects.push_front((yyvsp[(1) - (1)].t_simple_effect));;}
    break;

  case 88:

/* Line 1464 of yacc.c  */
#line 617 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->assign_effects.push_front((yyvsp[(1) - (1)].t_assignment));
         requires(E_NFLUENTS);;}
    break;

  case 89:

/* Line 1464 of yacc.c  */
#line 623 "src/pddl+.yacc"
    {(yyval.t_effect_lists)= (yyvsp[(1) - (2)].t_effect_lists); (yyval.t_effect_lists)->del_effects.push_back((yyvsp[(2) - (2)].t_simple_effect));;}
    break;

  case 90:

/* Line 1464 of yacc.c  */
#line 624 "src/pddl+.yacc"
    {(yyval.t_effect_lists)= (yyvsp[(1) - (2)].t_effect_lists); (yyval.t_effect_lists)->add_effects.push_back((yyvsp[(2) - (2)].t_simple_effect));;}
    break;

  case 91:

/* Line 1464 of yacc.c  */
#line 625 "src/pddl+.yacc"
    {(yyval.t_effect_lists)= (yyvsp[(1) - (2)].t_effect_lists); (yyval.t_effect_lists)->assign_effects.push_back((yyvsp[(2) - (2)].t_assignment));
                                     requires(E_NFLUENTS); ;}
    break;

  case 92:

/* Line 1464 of yacc.c  */
#line 627 "src/pddl+.yacc"
    { (yyval.t_effect_lists)= new effect_lists; ;}
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 632 "src/pddl+.yacc"
    { (yyval.t_effect_lists)=(yyvsp[(3) - (4)].t_effect_lists); ;}
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 634 "src/pddl+.yacc"
    {yyerrok; (yyval.t_effect_lists)=NULL;
	 log_error(E_FATAL,"Syntax error in (and ...)");
	;}
    break;

  case 95:

/* Line 1464 of yacc.c  */
#line 642 "src/pddl+.yacc"
    { (yyval.t_effect_lists)=(yyvsp[(3) - (4)].t_effect_lists); ;}
    break;

  case 96:

/* Line 1464 of yacc.c  */
#line 647 "src/pddl+.yacc"
    { (yyval.t_effect_lists)= new effect_lists;
          (yyval.t_effect_lists)->forall_effects.push_back(
	       new forall_effect((yyvsp[(6) - (7)].t_effect_lists), (yyvsp[(4) - (7)].t_var_symbol_list), current_analysis->var_tab_stack.pop()));
          requires(E_COND_EFFS);;}
    break;

  case 97:

/* Line 1464 of yacc.c  */
#line 652 "src/pddl+.yacc"
    { (yyval.t_effect_lists)= new effect_lists;
	  (yyval.t_effect_lists)->cond_effects.push_back(
	       new cond_effect((yyvsp[(3) - (5)].t_goal),(yyvsp[(4) - (5)].t_effect_lists)));
          requires(E_COND_EFFS); ;}
    break;

  case 98:

/* Line 1464 of yacc.c  */
#line 657 "src/pddl+.yacc"
    { (yyval.t_effect_lists)= new effect_lists;
	  (yyval.t_effect_lists)->cond_assign_effects.push_back(
	       new cond_effect((yyvsp[(3) - (5)].t_goal),(yyvsp[(4) - (5)].t_effect_lists)));
          requires(E_COND_EFFS); ;}
    break;

  case 99:

/* Line 1464 of yacc.c  */
#line 662 "src/pddl+.yacc"
    { (yyval.t_effect_lists)=new effect_lists;
          (yyval.t_effect_lists)->timed_effects.push_back((yyvsp[(1) - (1)].t_timed_effect)); ;}
    break;

  case 100:

/* Line 1464 of yacc.c  */
#line 665 "src/pddl+.yacc"
    { (yyval.t_effect_lists)= new effect_lists;
	  (yyval.t_effect_lists)->assign_effects.push_front((yyvsp[(1) - (1)].t_assignment));
          requires(E_NFLUENTS); ;}
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 671 "src/pddl+.yacc"
    { (yyval.t_effect_lists)=(yyvsp[(1) - (2)].t_effect_lists); (yyvsp[(1) - (2)].t_effect_lists)->append_effects((yyvsp[(2) - (2)].t_effect_lists)); delete (yyvsp[(2) - (2)].t_effect_lists); ;}
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 672 "src/pddl+.yacc"
    { (yyval.t_effect_lists)= new effect_lists; ;}
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 677 "src/pddl+.yacc"
    {(yyval.t_timed_effect)=new timed_effect((yyvsp[(3) - (4)].t_effect_lists),E_AT_START);;}
    break;

  case 104:

/* Line 1464 of yacc.c  */
#line 679 "src/pddl+.yacc"
    {(yyval.t_timed_effect)=new timed_effect((yyvsp[(3) - (4)].t_effect_lists),E_AT_END);;}
    break;

  case 105:

/* Line 1464 of yacc.c  */
#line 681 "src/pddl+.yacc"
    {(yyval.t_timed_effect)=new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_timed_effect)->effs->assign_effects.push_front(
	     new assignment((yyvsp[(3) - (5)].t_func_term),E_INCREASE,(yyvsp[(4) - (5)].t_expression))); ;}
    break;

  case 106:

/* Line 1464 of yacc.c  */
#line 685 "src/pddl+.yacc"
    {(yyval.t_timed_effect)=new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_timed_effect)->effs->assign_effects.push_front(
	     new assignment((yyvsp[(3) - (5)].t_func_term),E_DECREASE,(yyvsp[(4) - (5)].t_expression))); ;}
    break;

  case 107:

/* Line 1464 of yacc.c  */
#line 689 "src/pddl+.yacc"
    {yyerrok; (yyval.t_timed_effect)=NULL;
	log_error(E_FATAL,"Syntax error in timed effect"); ;}
    break;

  case 108:

/* Line 1464 of yacc.c  */
#line 695 "src/pddl+.yacc"
    {(yyval.t_timed_effect)=new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_timed_effect)->effs->assign_effects.push_front(
	     new assignment((yyvsp[(3) - (5)].t_func_term),E_INCREASE,(yyvsp[(4) - (5)].t_expression))); ;}
    break;

  case 109:

/* Line 1464 of yacc.c  */
#line 699 "src/pddl+.yacc"
    {(yyval.t_timed_effect)=new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_timed_effect)->effs->assign_effects.push_front(
	     new assignment((yyvsp[(3) - (5)].t_func_term),E_DECREASE,(yyvsp[(4) - (5)].t_expression))); ;}
    break;

  case 110:

/* Line 1464 of yacc.c  */
#line 703 "src/pddl+.yacc"
    {yyerrok; (yyval.t_timed_effect)=NULL;
	log_error(E_FATAL,"Syntax error in conditional continuous effect"); ;}
    break;

  case 111:

/* Line 1464 of yacc.c  */
#line 709 "src/pddl+.yacc"
    { (yyval.t_effect_lists)=(yyvsp[(3) - (4)].t_effect_lists); ;}
    break;

  case 112:

/* Line 1464 of yacc.c  */
#line 714 "src/pddl+.yacc"
    { (yyval.t_effect_lists)= new effect_lists;
          (yyval.t_effect_lists)->forall_effects.push_back(
	       new forall_effect((yyvsp[(6) - (7)].t_effect_lists), (yyvsp[(4) - (7)].t_var_symbol_list), current_analysis->var_tab_stack.pop()));
          requires(E_COND_EFFS);;}
    break;

  case 113:

/* Line 1464 of yacc.c  */
#line 719 "src/pddl+.yacc"
    { (yyval.t_effect_lists)= new effect_lists;
	  (yyval.t_effect_lists)->cond_assign_effects.push_back(
	       new cond_effect((yyvsp[(3) - (5)].t_goal),(yyvsp[(4) - (5)].t_effect_lists)));
          requires(E_COND_EFFS); ;}
    break;

  case 114:

/* Line 1464 of yacc.c  */
#line 724 "src/pddl+.yacc"
    { (yyval.t_effect_lists)=new effect_lists;
          (yyval.t_effect_lists)->timed_effects.push_back((yyvsp[(1) - (1)].t_timed_effect)); ;}
    break;

  case 115:

/* Line 1464 of yacc.c  */
#line 729 "src/pddl+.yacc"
    { (yyval.t_effect_lists)=(yyvsp[(1) - (2)].t_effect_lists); (yyvsp[(1) - (2)].t_effect_lists)->append_effects((yyvsp[(2) - (2)].t_effect_lists)); delete (yyvsp[(2) - (2)].t_effect_lists); ;}
    break;

  case 116:

/* Line 1464 of yacc.c  */
#line 730 "src/pddl+.yacc"
    { (yyval.t_effect_lists)= new effect_lists; ;}
    break;

  case 117:

/* Line 1464 of yacc.c  */
#line 734 "src/pddl+.yacc"
    {(yyval.t_effect_lists)= (yyvsp[(3) - (4)].t_effect_lists);;}
    break;

  case 118:

/* Line 1464 of yacc.c  */
#line 735 "src/pddl+.yacc"
    {(yyval.t_effect_lists)= (yyvsp[(1) - (1)].t_effect_lists);;}
    break;

  case 119:

/* Line 1464 of yacc.c  */
#line 740 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->del_effects.push_front((yyvsp[(1) - (1)].t_simple_effect));;}
    break;

  case 120:

/* Line 1464 of yacc.c  */
#line 742 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->add_effects.push_front((yyvsp[(1) - (1)].t_simple_effect));;}
    break;

  case 121:

/* Line 1464 of yacc.c  */
#line 744 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->assign_effects.push_front((yyvsp[(1) - (1)].t_assignment));
         requires(E_NFLUENTS);;}
    break;

  case 122:

/* Line 1464 of yacc.c  */
#line 750 "src/pddl+.yacc"
    {(yyval.t_effect_lists)= (yyvsp[(1) - (2)].t_effect_lists); (yyval.t_effect_lists)->del_effects.push_back((yyvsp[(2) - (2)].t_simple_effect));;}
    break;

  case 123:

/* Line 1464 of yacc.c  */
#line 751 "src/pddl+.yacc"
    {(yyval.t_effect_lists)= (yyvsp[(1) - (2)].t_effect_lists); (yyval.t_effect_lists)->add_effects.push_back((yyvsp[(2) - (2)].t_simple_effect));;}
    break;

  case 124:

/* Line 1464 of yacc.c  */
#line 752 "src/pddl+.yacc"
    {(yyval.t_effect_lists)= (yyvsp[(1) - (2)].t_effect_lists); (yyval.t_effect_lists)->assign_effects.push_back((yyvsp[(2) - (2)].t_assignment));
                                     requires(E_NFLUENTS); ;}
    break;

  case 125:

/* Line 1464 of yacc.c  */
#line 754 "src/pddl+.yacc"
    { (yyval.t_effect_lists)= new effect_lists; ;}
    break;

  case 126:

/* Line 1464 of yacc.c  */
#line 760 "src/pddl+.yacc"
    { (yyval.t_assignment)= new assignment((yyvsp[(3) - (5)].t_func_term),E_ASSIGN,(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 127:

/* Line 1464 of yacc.c  */
#line 762 "src/pddl+.yacc"
    { (yyval.t_assignment)= new assignment((yyvsp[(3) - (5)].t_func_term),E_INCREASE,(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 128:

/* Line 1464 of yacc.c  */
#line 764 "src/pddl+.yacc"
    { (yyval.t_assignment)= new assignment((yyvsp[(3) - (5)].t_func_term),E_DECREASE,(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 129:

/* Line 1464 of yacc.c  */
#line 766 "src/pddl+.yacc"
    { (yyval.t_assignment)= new assignment((yyvsp[(3) - (5)].t_func_term),E_SCALE_UP,(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 130:

/* Line 1464 of yacc.c  */
#line 768 "src/pddl+.yacc"
    { (yyval.t_assignment)= new assignment((yyvsp[(3) - (5)].t_func_term),E_SCALE_DOWN,(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 131:

/* Line 1464 of yacc.c  */
#line 773 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=new effect_lists;
         timed_effect * te = new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_effect_lists)->timed_effects.push_front(te);
         te->effs->assign_effects.push_front(
	     new assignment((yyvsp[(3) - (5)].t_func_term),E_INCREASE,(yyvsp[(4) - (5)].t_expression))); ;}
    break;

  case 132:

/* Line 1464 of yacc.c  */
#line 779 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=new effect_lists;
         timed_effect * te = new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_effect_lists)->timed_effects.push_front(te);
         te->effs->assign_effects.push_front(
	     new assignment((yyvsp[(3) - (5)].t_func_term),E_DECREASE,(yyvsp[(4) - (5)].t_expression))); ;}
    break;

  case 133:

/* Line 1464 of yacc.c  */
#line 785 "src/pddl+.yacc"
    {(yyval.t_effect_lists) = (yyvsp[(3) - (4)].t_effect_lists);;}
    break;

  case 134:

/* Line 1464 of yacc.c  */
#line 789 "src/pddl+.yacc"
    { (yyval.t_effect_lists)=(yyvsp[(1) - (2)].t_effect_lists); (yyvsp[(1) - (2)].t_effect_lists)->append_effects((yyvsp[(2) - (2)].t_effect_lists)); delete (yyvsp[(2) - (2)].t_effect_lists); ;}
    break;

  case 135:

/* Line 1464 of yacc.c  */
#line 790 "src/pddl+.yacc"
    { (yyval.t_effect_lists)= new effect_lists; ;}
    break;

  case 136:

/* Line 1464 of yacc.c  */
#line 794 "src/pddl+.yacc"
    {(yyval.t_expression)= (yyvsp[(1) - (1)].t_expression);;}
    break;

  case 137:

/* Line 1464 of yacc.c  */
#line 795 "src/pddl+.yacc"
    {(yyval.t_expression)= new special_val_expr(E_DURATION_VAR);
                    requires( E_DURATION_INEQUALITIES );;}
    break;

  case 138:

/* Line 1464 of yacc.c  */
#line 797 "src/pddl+.yacc"
    { (yyval.t_expression)=(yyvsp[(1) - (1)].t_num_expression); ;}
    break;

  case 139:

/* Line 1464 of yacc.c  */
#line 798 "src/pddl+.yacc"
    { (yyval.t_expression)= (yyvsp[(1) - (1)].t_func_term); ;}
    break;

  case 140:

/* Line 1464 of yacc.c  */
#line 803 "src/pddl+.yacc"
    { (yyval.t_expression)= new plus_expression((yyvsp[(3) - (5)].t_expression),(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 141:

/* Line 1464 of yacc.c  */
#line 805 "src/pddl+.yacc"
    { (yyval.t_expression)= new minus_expression((yyvsp[(3) - (5)].t_expression),(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 142:

/* Line 1464 of yacc.c  */
#line 807 "src/pddl+.yacc"
    { (yyval.t_expression)= new mul_expression((yyvsp[(3) - (5)].t_expression),(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 143:

/* Line 1464 of yacc.c  */
#line 809 "src/pddl+.yacc"
    { (yyval.t_expression)= new div_expression((yyvsp[(3) - (5)].t_expression),(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 144:

/* Line 1464 of yacc.c  */
#line 814 "src/pddl+.yacc"
    { (yyval.t_goal)= new conj_goal((yyvsp[(3) - (4)].t_goal_list)); ;}
    break;

  case 145:

/* Line 1464 of yacc.c  */
#line 816 "src/pddl+.yacc"
    { (yyval.t_goal)= new timed_goal(new comparison((yyvsp[(2) - (6)].t_comparison_op),
        			new special_val_expr(E_DURATION_VAR),(yyvsp[(5) - (6)].t_expression)),E_AT_START); ;}
    break;

  case 146:

/* Line 1464 of yacc.c  */
#line 819 "src/pddl+.yacc"
    { (yyval.t_goal) = new timed_goal(new comparison((yyvsp[(4) - (9)].t_comparison_op),
					new special_val_expr(E_DURATION_VAR),(yyvsp[(7) - (9)].t_expression)),E_AT_START);;}
    break;

  case 147:

/* Line 1464 of yacc.c  */
#line 822 "src/pddl+.yacc"
    { (yyval.t_goal) = new timed_goal(new comparison((yyvsp[(4) - (9)].t_comparison_op),
					new special_val_expr(E_DURATION_VAR),(yyvsp[(7) - (9)].t_expression)),E_AT_END);;}
    break;

  case 148:

/* Line 1464 of yacc.c  */
#line 827 "src/pddl+.yacc"
    {(yyval.t_comparison_op)= E_LESSEQ; requires(E_DURATION_INEQUALITIES);;}
    break;

  case 149:

/* Line 1464 of yacc.c  */
#line 828 "src/pddl+.yacc"
    {(yyval.t_comparison_op)= E_GREATEQ; requires(E_DURATION_INEQUALITIES);;}
    break;

  case 150:

/* Line 1464 of yacc.c  */
#line 829 "src/pddl+.yacc"
    {(yyval.t_comparison_op)= E_EQUALS; ;}
    break;

  case 151:

/* Line 1464 of yacc.c  */
#line 837 "src/pddl+.yacc"
    {(yyval.t_expression)= (yyvsp[(1) - (1)].t_expression); ;}
    break;

  case 152:

/* Line 1464 of yacc.c  */
#line 842 "src/pddl+.yacc"
    { (yyval.t_goal_list)=(yyvsp[(1) - (2)].t_goal_list); (yyval.t_goal_list)->push_back((yyvsp[(2) - (2)].t_goal)); ;}
    break;

  case 153:

/* Line 1464 of yacc.c  */
#line 844 "src/pddl+.yacc"
    { (yyval.t_goal_list)= new goal_list; ;}
    break;

  case 154:

/* Line 1464 of yacc.c  */
#line 849 "src/pddl+.yacc"
    { (yyval.t_simple_effect)= new simple_effect((yyvsp[(3) - (4)].t_proposition)); ;}
    break;

  case 155:

/* Line 1464 of yacc.c  */
#line 854 "src/pddl+.yacc"
    { (yyval.t_simple_effect)= new simple_effect((yyvsp[(1) - (1)].t_proposition)); ;}
    break;

  case 156:

/* Line 1464 of yacc.c  */
#line 861 "src/pddl+.yacc"
    { (yyval.t_simple_effect)= new simple_effect((yyvsp[(3) - (4)].t_proposition)); ;}
    break;

  case 157:

/* Line 1464 of yacc.c  */
#line 866 "src/pddl+.yacc"
    { (yyval.t_simple_effect)= new simple_effect((yyvsp[(1) - (1)].t_proposition)); ;}
    break;

  case 158:

/* Line 1464 of yacc.c  */
#line 871 "src/pddl+.yacc"
    { (yyval.t_forall_effect)= new forall_effect((yyvsp[(6) - (7)].t_effect_lists), (yyvsp[(4) - (7)].t_var_symbol_list), current_analysis->var_tab_stack.pop());;}
    break;

  case 159:

/* Line 1464 of yacc.c  */
#line 876 "src/pddl+.yacc"
    { (yyval.t_cond_effect)= new cond_effect((yyvsp[(3) - (5)].t_goal),(yyvsp[(4) - (5)].t_effect_lists)); ;}
    break;

  case 160:

/* Line 1464 of yacc.c  */
#line 881 "src/pddl+.yacc"
    { (yyval.t_assignment)= new assignment((yyvsp[(3) - (5)].t_func_term),E_ASSIGN,(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 161:

/* Line 1464 of yacc.c  */
#line 883 "src/pddl+.yacc"
    { (yyval.t_assignment)= new assignment((yyvsp[(3) - (5)].t_func_term),E_INCREASE,(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 162:

/* Line 1464 of yacc.c  */
#line 885 "src/pddl+.yacc"
    { (yyval.t_assignment)= new assignment((yyvsp[(3) - (5)].t_func_term),E_DECREASE,(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 163:

/* Line 1464 of yacc.c  */
#line 887 "src/pddl+.yacc"
    { (yyval.t_assignment)= new assignment((yyvsp[(3) - (5)].t_func_term),E_SCALE_UP,(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 164:

/* Line 1464 of yacc.c  */
#line 889 "src/pddl+.yacc"
    { (yyval.t_assignment)= new assignment((yyvsp[(3) - (5)].t_func_term),E_SCALE_DOWN,(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 165:

/* Line 1464 of yacc.c  */
#line 894 "src/pddl+.yacc"
    { (yyval.t_expression)= new uminus_expression((yyvsp[(3) - (4)].t_expression)); requires(E_NFLUENTS); ;}
    break;

  case 166:

/* Line 1464 of yacc.c  */
#line 896 "src/pddl+.yacc"
    { (yyval.t_expression)= new plus_expression((yyvsp[(3) - (5)].t_expression),(yyvsp[(4) - (5)].t_expression)); requires(E_NFLUENTS); ;}
    break;

  case 167:

/* Line 1464 of yacc.c  */
#line 898 "src/pddl+.yacc"
    { (yyval.t_expression)= new minus_expression((yyvsp[(3) - (5)].t_expression),(yyvsp[(4) - (5)].t_expression)); requires(E_NFLUENTS); ;}
    break;

  case 168:

/* Line 1464 of yacc.c  */
#line 900 "src/pddl+.yacc"
    { (yyval.t_expression)= new mul_expression((yyvsp[(3) - (5)].t_expression),(yyvsp[(4) - (5)].t_expression)); requires(E_NFLUENTS); ;}
    break;

  case 169:

/* Line 1464 of yacc.c  */
#line 902 "src/pddl+.yacc"
    { (yyval.t_expression)= new div_expression((yyvsp[(3) - (5)].t_expression),(yyvsp[(4) - (5)].t_expression)); requires(E_NFLUENTS); ;}
    break;

  case 170:

/* Line 1464 of yacc.c  */
#line 903 "src/pddl+.yacc"
    { (yyval.t_expression)=(yyvsp[(1) - (1)].t_num_expression); ;}
    break;

  case 171:

/* Line 1464 of yacc.c  */
#line 904 "src/pddl+.yacc"
    { (yyval.t_expression)= (yyvsp[(1) - (1)].t_func_term); requires(E_NFLUENTS); ;}
    break;

  case 172:

/* Line 1464 of yacc.c  */
#line 909 "src/pddl+.yacc"
    { (yyval.t_expression)= new mul_expression(new special_val_expr(E_HASHT),(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 173:

/* Line 1464 of yacc.c  */
#line 911 "src/pddl+.yacc"
    { (yyval.t_expression)= new mul_expression((yyvsp[(3) - (5)].t_expression), new special_val_expr(E_HASHT)); ;}
    break;

  case 174:

/* Line 1464 of yacc.c  */
#line 913 "src/pddl+.yacc"
    { (yyval.t_expression)= new special_val_expr(E_HASHT); ;}
    break;

  case 175:

/* Line 1464 of yacc.c  */
#line 918 "src/pddl+.yacc"
    { (yyval.t_num_expression)=new int_expression((yyvsp[(1) - (1)].ival));   ;}
    break;

  case 176:

/* Line 1464 of yacc.c  */
#line 919 "src/pddl+.yacc"
    { (yyval.t_num_expression)=new float_expression((yyvsp[(1) - (1)].fval)); ;}
    break;

  case 177:

/* Line 1464 of yacc.c  */
#line 923 "src/pddl+.yacc"
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[(2) - (4)].cp)), (yyvsp[(3) - (4)].t_parameter_symbol_list)); delete [] (yyvsp[(2) - (4)].cp); ;}
    break;

  case 178:

/* Line 1464 of yacc.c  */
#line 926 "src/pddl+.yacc"
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[(2) - (4)].cp)), (yyvsp[(3) - (4)].t_parameter_symbol_list)); delete [] (yyvsp[(2) - (4)].cp); ;}
    break;

  case 179:

/* Line 1464 of yacc.c  */
#line 928 "src/pddl+.yacc"
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[(1) - (1)].cp)),
                            new parameter_symbol_list); delete [] (yyvsp[(1) - (1)].cp);;}
    break;

  case 180:

/* Line 1464 of yacc.c  */
#line 931 "src/pddl+.yacc"
    { (yyval.t_func_term) = new class_func_term( (yyvsp[(2) - (6)].t_class), current_analysis->func_tab.symbol_get((yyvsp[(4) - (6)].cp)), (yyvsp[(5) - (6)].t_parameter_symbol_list)); delete [] (yyvsp[(4) - (6)].cp);;}
    break;

  case 181:

/* Line 1464 of yacc.c  */
#line 948 "src/pddl+.yacc"
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[(2) - (4)].cp)), (yyvsp[(3) - (4)].t_parameter_symbol_list)); delete [] (yyvsp[(2) - (4)].cp); ;}
    break;

  case 182:

/* Line 1464 of yacc.c  */
#line 950 "src/pddl+.yacc"
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[(2) - (4)].cp)), (yyvsp[(3) - (4)].t_parameter_symbol_list)); delete [] (yyvsp[(2) - (4)].cp); ;}
    break;

  case 183:

/* Line 1464 of yacc.c  */
#line 952 "src/pddl+.yacc"
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[(1) - (1)].cp)),
                            new parameter_symbol_list); delete [] (yyvsp[(1) - (1)].cp);;}
    break;

  case 184:

/* Line 1464 of yacc.c  */
#line 957 "src/pddl+.yacc"
    { (yyval.t_comparison_op)= E_GREATER; ;}
    break;

  case 185:

/* Line 1464 of yacc.c  */
#line 958 "src/pddl+.yacc"
    { (yyval.t_comparison_op)= E_GREATEQ; ;}
    break;

  case 186:

/* Line 1464 of yacc.c  */
#line 959 "src/pddl+.yacc"
    { (yyval.t_comparison_op)= E_LESS; ;}
    break;

  case 187:

/* Line 1464 of yacc.c  */
#line 960 "src/pddl+.yacc"
    { (yyval.t_comparison_op)= E_LESSEQ; ;}
    break;

  case 188:

/* Line 1464 of yacc.c  */
#line 961 "src/pddl+.yacc"
    { (yyval.t_comparison_op)= E_EQUALS; ;}
    break;

  case 189:

/* Line 1464 of yacc.c  */
#line 974 "src/pddl+.yacc"
    {(yyval.t_goal)= (yyvsp[(1) - (1)].t_goal);;}
    break;

  case 190:

/* Line 1464 of yacc.c  */
#line 981 "src/pddl+.yacc"
    {(yyval.t_goal) = new conj_goal((yyvsp[(3) - (4)].t_goal_list));;}
    break;

  case 191:

/* Line 1464 of yacc.c  */
#line 984 "src/pddl+.yacc"
    {(yyval.t_goal)= new qfied_goal(E_FORALL,(yyvsp[(4) - (7)].t_var_symbol_list),(yyvsp[(6) - (7)].t_goal),current_analysis->var_tab_stack.pop());
        requires(E_UNIV_PRECS);;}
    break;

  case 192:

/* Line 1464 of yacc.c  */
#line 986 "src/pddl+.yacc"
    {(yyval.t_goal) = new conj_goal(new goal_list);;}
    break;

  case 193:

/* Line 1464 of yacc.c  */
#line 987 "src/pddl+.yacc"
    {(yyval.t_goal) = new conj_goal(new goal_list);;}
    break;

  case 194:

/* Line 1464 of yacc.c  */
#line 992 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new preference((yyvsp[(3) - (4)].t_con_goal));requires(E_PREFERENCES);;}
    break;

  case 195:

/* Line 1464 of yacc.c  */
#line 994 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new preference((yyvsp[(3) - (5)].cp),(yyvsp[(4) - (5)].t_con_goal));requires(E_PREFERENCES);;}
    break;

  case 196:

/* Line 1464 of yacc.c  */
#line 996 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new conj_goal((yyvsp[(3) - (4)].t_goal_list));;}
    break;

  case 197:

/* Line 1464 of yacc.c  */
#line 999 "src/pddl+.yacc"
    {(yyval.t_con_goal)= new qfied_goal(E_FORALL,(yyvsp[(4) - (7)].t_var_symbol_list),(yyvsp[(6) - (7)].t_con_goal),current_analysis->var_tab_stack.pop());
                requires(E_UNIV_PRECS);;}
    break;

  case 198:

/* Line 1464 of yacc.c  */
#line 1002 "src/pddl+.yacc"
    {(yyval.t_con_goal) = (yyvsp[(1) - (1)].t_con_goal);;}
    break;

  case 199:

/* Line 1464 of yacc.c  */
#line 1007 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new preference((yyvsp[(3) - (4)].t_con_goal));requires(E_PREFERENCES);;}
    break;

  case 200:

/* Line 1464 of yacc.c  */
#line 1009 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new preference((yyvsp[(3) - (5)].cp),(yyvsp[(4) - (5)].t_con_goal));requires(E_PREFERENCES);;}
    break;

  case 201:

/* Line 1464 of yacc.c  */
#line 1011 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new conj_goal((yyvsp[(3) - (4)].t_goal_list));;}
    break;

  case 202:

/* Line 1464 of yacc.c  */
#line 1014 "src/pddl+.yacc"
    {(yyval.t_con_goal)= new qfied_goal(E_FORALL,(yyvsp[(4) - (7)].t_var_symbol_list),(yyvsp[(6) - (7)].t_con_goal),current_analysis->var_tab_stack.pop());
                requires(E_UNIV_PRECS);;}
    break;

  case 203:

/* Line 1464 of yacc.c  */
#line 1020 "src/pddl+.yacc"
    {(yyval.t_goal_list)=(yyvsp[(1) - (2)].t_goal_list); (yyvsp[(1) - (2)].t_goal_list)->push_back((yyvsp[(2) - (2)].t_con_goal));;}
    break;

  case 204:

/* Line 1464 of yacc.c  */
#line 1022 "src/pddl+.yacc"
    {(yyval.t_goal_list)= new goal_list; (yyval.t_goal_list)->push_back((yyvsp[(1) - (1)].t_con_goal));;}
    break;

  case 205:

/* Line 1464 of yacc.c  */
#line 1027 "src/pddl+.yacc"
    {(yyval.t_goal)= new preference((yyvsp[(3) - (4)].t_goal)); requires(E_PREFERENCES);;}
    break;

  case 206:

/* Line 1464 of yacc.c  */
#line 1029 "src/pddl+.yacc"
    {(yyval.t_goal)= new preference((yyvsp[(3) - (5)].cp),(yyvsp[(4) - (5)].t_goal)); requires(E_PREFERENCES);;}
    break;

  case 207:

/* Line 1464 of yacc.c  */
#line 1033 "src/pddl+.yacc"
    {(yyval.t_goal)=(yyvsp[(1) - (1)].t_goal);;}
    break;

  case 208:

/* Line 1464 of yacc.c  */
#line 1038 "src/pddl+.yacc"
    {(yyval.t_goal_list) = (yyvsp[(1) - (2)].t_goal_list); (yyval.t_goal_list)->push_back((yyvsp[(2) - (2)].t_con_goal));;}
    break;

  case 209:

/* Line 1464 of yacc.c  */
#line 1040 "src/pddl+.yacc"
    {(yyval.t_goal_list) = new goal_list; (yyval.t_goal_list)->push_back((yyvsp[(1) - (1)].t_con_goal));;}
    break;

  case 210:

/* Line 1464 of yacc.c  */
#line 1045 "src/pddl+.yacc"
    {(yyval.t_con_goal)= new conj_goal((yyvsp[(3) - (4)].t_goal_list));;}
    break;

  case 211:

/* Line 1464 of yacc.c  */
#line 1047 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new qfied_goal(E_FORALL,(yyvsp[(4) - (7)].t_var_symbol_list),(yyvsp[(6) - (7)].t_con_goal),current_analysis->var_tab_stack.pop());
        requires(E_UNIV_PRECS);;}
    break;

  case 212:

/* Line 1464 of yacc.c  */
#line 1050 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new constraint_goal(E_ATEND,(yyvsp[(3) - (4)].t_goal));;}
    break;

  case 213:

/* Line 1464 of yacc.c  */
#line 1052 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new constraint_goal(E_ALWAYS,(yyvsp[(3) - (4)].t_goal));;}
    break;

  case 214:

/* Line 1464 of yacc.c  */
#line 1054 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new constraint_goal(E_SOMETIME,(yyvsp[(3) - (4)].t_goal));;}
    break;

  case 215:

/* Line 1464 of yacc.c  */
#line 1056 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new constraint_goal(E_WITHIN,(yyvsp[(4) - (5)].t_goal),NULL,(yyvsp[(3) - (5)].t_num_expression)->double_value(),0.0);delete (yyvsp[(3) - (5)].t_num_expression);;}
    break;

  case 216:

/* Line 1464 of yacc.c  */
#line 1058 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new constraint_goal(E_ATMOSTONCE,(yyvsp[(3) - (4)].t_goal));;}
    break;

  case 217:

/* Line 1464 of yacc.c  */
#line 1060 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new constraint_goal(E_SOMETIMEAFTER,(yyvsp[(4) - (5)].t_goal),(yyvsp[(3) - (5)].t_goal));;}
    break;

  case 218:

/* Line 1464 of yacc.c  */
#line 1062 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new constraint_goal(E_SOMETIMEBEFORE,(yyvsp[(4) - (5)].t_goal),(yyvsp[(3) - (5)].t_goal));;}
    break;

  case 219:

/* Line 1464 of yacc.c  */
#line 1064 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new constraint_goal(E_ALWAYSWITHIN,(yyvsp[(5) - (6)].t_goal),(yyvsp[(4) - (6)].t_goal),(yyvsp[(3) - (6)].t_num_expression)->double_value(),0.0);delete (yyvsp[(3) - (6)].t_num_expression);;}
    break;

  case 220:

/* Line 1464 of yacc.c  */
#line 1066 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new constraint_goal(E_HOLDDURING,(yyvsp[(5) - (6)].t_goal),NULL,(yyvsp[(4) - (6)].t_num_expression)->double_value(),(yyvsp[(3) - (6)].t_num_expression)->double_value());delete (yyvsp[(3) - (6)].t_num_expression);delete (yyvsp[(4) - (6)].t_num_expression);;}
    break;

  case 221:

/* Line 1464 of yacc.c  */
#line 1068 "src/pddl+.yacc"
    {(yyval.t_con_goal) = new constraint_goal(E_HOLDAFTER,(yyvsp[(4) - (5)].t_goal),NULL,0.0,(yyvsp[(3) - (5)].t_num_expression)->double_value());delete (yyvsp[(3) - (5)].t_num_expression);;}
    break;

  case 222:

/* Line 1464 of yacc.c  */
#line 1073 "src/pddl+.yacc"
    {(yyval.t_goal)= new simple_goal((yyvsp[(1) - (1)].t_proposition),E_POS);;}
    break;

  case 223:

/* Line 1464 of yacc.c  */
#line 1075 "src/pddl+.yacc"
    {(yyval.t_goal)= new neg_goal((yyvsp[(3) - (4)].t_goal));simple_goal * s = dynamic_cast<simple_goal *>((yyvsp[(3) - (4)].t_goal));
       if(s && s->getProp()->head->getName()=="=") {requires(E_EQUALITY);}
       else{requires(E_NEGATIVE_PRECONDITIONS);};;}
    break;

  case 224:

/* Line 1464 of yacc.c  */
#line 1079 "src/pddl+.yacc"
    {(yyval.t_goal)= new conj_goal((yyvsp[(3) - (4)].t_goal_list));;}
    break;

  case 225:

/* Line 1464 of yacc.c  */
#line 1081 "src/pddl+.yacc"
    {(yyval.t_goal)= new disj_goal((yyvsp[(3) - (4)].t_goal_list));
        requires(E_DISJUNCTIVE_PRECONDS);;}
    break;

  case 226:

/* Line 1464 of yacc.c  */
#line 1084 "src/pddl+.yacc"
    {(yyval.t_goal)= new imply_goal((yyvsp[(3) - (5)].t_goal),(yyvsp[(4) - (5)].t_goal));
        requires(E_DISJUNCTIVE_PRECONDS);;}
    break;

  case 227:

/* Line 1464 of yacc.c  */
#line 1088 "src/pddl+.yacc"
    {(yyval.t_goal)= new qfied_goal((yyvsp[(2) - (7)].t_quantifier),(yyvsp[(4) - (7)].t_var_symbol_list),(yyvsp[(6) - (7)].t_goal),current_analysis->var_tab_stack.pop());;}
    break;

  case 228:

/* Line 1464 of yacc.c  */
#line 1091 "src/pddl+.yacc"
    {(yyval.t_goal)= new qfied_goal((yyvsp[(2) - (7)].t_quantifier),(yyvsp[(4) - (7)].t_var_symbol_list),(yyvsp[(6) - (7)].t_goal),current_analysis->var_tab_stack.pop());;}
    break;

  case 229:

/* Line 1464 of yacc.c  */
#line 1093 "src/pddl+.yacc"
    {(yyval.t_goal)= new comparison((yyvsp[(2) - (5)].t_comparison_op),(yyvsp[(3) - (5)].t_expression),(yyvsp[(4) - (5)].t_expression));
        requires(E_NFLUENTS);;}
    break;

  case 230:

/* Line 1464 of yacc.c  */
#line 1099 "src/pddl+.yacc"
    {(yyval.t_goal_list)=(yyvsp[(1) - (2)].t_goal_list); (yyvsp[(1) - (2)].t_goal_list)->push_back((yyvsp[(2) - (2)].t_goal));;}
    break;

  case 231:

/* Line 1464 of yacc.c  */
#line 1101 "src/pddl+.yacc"
    {(yyval.t_goal_list)= new goal_list; (yyval.t_goal_list)->push_back((yyvsp[(1) - (1)].t_goal));;}
    break;

  case 232:

/* Line 1464 of yacc.c  */
#line 1106 "src/pddl+.yacc"
    {(yyval.t_goal_list)=(yyvsp[(1) - (2)].t_goal_list); (yyvsp[(1) - (2)].t_goal_list)->push_back((yyvsp[(2) - (2)].t_goal));;}
    break;

  case 233:

/* Line 1464 of yacc.c  */
#line 1108 "src/pddl+.yacc"
    {(yyval.t_goal_list)= new goal_list; (yyval.t_goal_list)->push_back((yyvsp[(1) - (1)].t_goal));;}
    break;

  case 234:

/* Line 1464 of yacc.c  */
#line 1118 "src/pddl+.yacc"
    {(yyval.t_quantifier)=E_FORALL;
        current_analysis->var_tab_stack.push(
        		current_analysis->buildForallTab());;}
    break;

  case 235:

/* Line 1464 of yacc.c  */
#line 1125 "src/pddl+.yacc"
    {(yyval.t_quantifier)=E_EXISTS;
        current_analysis->var_tab_stack.push(
        	current_analysis->buildExistsTab());;}
    break;

  case 236:

/* Line 1464 of yacc.c  */
#line 1132 "src/pddl+.yacc"
    {(yyval.t_proposition)=new proposition((yyvsp[(2) - (4)].t_pred_symbol),(yyvsp[(3) - (4)].t_parameter_symbol_list));;}
    break;

  case 237:

/* Line 1464 of yacc.c  */
#line 1137 "src/pddl+.yacc"
    {(yyval.t_proposition) = new proposition((yyvsp[(2) - (4)].t_pred_symbol),(yyvsp[(3) - (4)].t_var_symbol_list));;}
    break;

  case 238:

/* Line 1464 of yacc.c  */
#line 1142 "src/pddl+.yacc"
    {(yyval.t_proposition)=new proposition((yyvsp[(2) - (4)].t_pred_symbol),(yyvsp[(3) - (4)].t_parameter_symbol_list));;}
    break;

  case 239:

/* Line 1464 of yacc.c  */
#line 1147 "src/pddl+.yacc"
    {(yyval.t_pred_decl_list)= (yyvsp[(3) - (4)].t_pred_decl_list);;}
    break;

  case 240:

/* Line 1464 of yacc.c  */
#line 1149 "src/pddl+.yacc"
    {yyerrok; (yyval.t_pred_decl_list)=NULL;
	 log_error(E_FATAL,"Syntax error in (:predicates ...)");
	;}
    break;

  case 241:

/* Line 1464 of yacc.c  */
#line 1156 "src/pddl+.yacc"
    {(yyval.t_func_decl_list)= (yyvsp[(3) - (4)].t_func_decl_list);;}
    break;

  case 242:

/* Line 1464 of yacc.c  */
#line 1158 "src/pddl+.yacc"
    {yyerrok; (yyval.t_func_decl_list)=NULL;
	 log_error(E_FATAL,"Syntax error in (:functions ...)");
	;}
    break;

  case 243:

/* Line 1464 of yacc.c  */
#line 1165 "src/pddl+.yacc"
    {(yyval.t_con_goal) = (yyvsp[(3) - (4)].t_con_goal);;}
    break;

  case 244:

/* Line 1464 of yacc.c  */
#line 1167 "src/pddl+.yacc"
    {yyerrok; (yyval.t_con_goal)=NULL;
      log_error(E_FATAL,"Syntax error in (:constraints ...)");
      ;}
    break;

  case 245:

/* Line 1464 of yacc.c  */
#line 1174 "src/pddl+.yacc"
    {(yyval.t_con_goal) = (yyvsp[(3) - (4)].t_con_goal);;}
    break;

  case 246:

/* Line 1464 of yacc.c  */
#line 1176 "src/pddl+.yacc"
    {yyerrok; (yyval.t_con_goal)=NULL;
      log_error(E_FATAL,"Syntax error in (:constraints ...)");
      ;}
    break;

  case 247:

/* Line 1464 of yacc.c  */
#line 1182 "src/pddl+.yacc"
    { (yyval.t_structure_store)=(yyvsp[(1) - (2)].t_structure_store); (yyval.t_structure_store)->push_back((yyvsp[(2) - (2)].t_structure_def)); ;}
    break;

  case 248:

/* Line 1464 of yacc.c  */
#line 1183 "src/pddl+.yacc"
    { (yyval.t_structure_store)= new structure_store; (yyval.t_structure_store)->push_back((yyvsp[(1) - (1)].t_structure_def)); ;}
    break;

  case 249:

/* Line 1464 of yacc.c  */
#line 1187 "src/pddl+.yacc"
    { (yyval.t_structure_def)= (yyvsp[(1) - (1)].t_action_def); ;}
    break;

  case 250:

/* Line 1464 of yacc.c  */
#line 1188 "src/pddl+.yacc"
    { (yyval.t_structure_def)= (yyvsp[(1) - (1)].t_event_def); requires(E_TIME); ;}
    break;

  case 251:

/* Line 1464 of yacc.c  */
#line 1189 "src/pddl+.yacc"
    { (yyval.t_structure_def)= (yyvsp[(1) - (1)].t_process_def); requires(E_TIME); ;}
    break;

  case 252:

/* Line 1464 of yacc.c  */
#line 1190 "src/pddl+.yacc"
    { (yyval.t_structure_def)= (yyvsp[(1) - (1)].t_durative_action_def); requires(E_DURATIVE_ACTIONS); ;}
    break;

  case 253:

/* Line 1464 of yacc.c  */
#line 1191 "src/pddl+.yacc"
    { (yyval.t_structure_def)= (yyvsp[(1) - (1)].t_derivation_rule); requires(E_DERIVED_PREDICATES);;}
    break;

  case 254:

/* Line 1464 of yacc.c  */
#line 1192 "src/pddl+.yacc"
    { (yyval.t_structure_def) = (yyvsp[(1) - (1)].t_class_def); requires(E_MODULES);;}
    break;

  case 255:

/* Line 1464 of yacc.c  */
#line 1200 "src/pddl+.yacc"
    {(yyval.t_class_def) = new class_def((yyvsp[(3) - (5)].t_class),(yyvsp[(4) - (5)].t_func_decl_list));;}
    break;

  case 256:

/* Line 1464 of yacc.c  */
#line 1204 "src/pddl+.yacc"
    {(yyval.t_dummy)= 0;
    	current_analysis->var_tab_stack.push(
    					current_analysis->buildRuleTab());;}
    break;

  case 257:

/* Line 1464 of yacc.c  */
#line 1215 "src/pddl+.yacc"
    {(yyval.t_derivation_rule) = new derivation_rule((yyvsp[(3) - (5)].t_proposition),(yyvsp[(4) - (5)].t_goal),current_analysis->var_tab_stack.pop());;}
    break;

  case 258:

/* Line 1464 of yacc.c  */
#line 1227 "src/pddl+.yacc"
    { (yyval.t_action_def)= current_analysis->buildAction(current_analysis->op_tab.new_symbol_put((yyvsp[(3) - (12)].cp)),
			(yyvsp[(6) - (12)].t_var_symbol_list),(yyvsp[(9) - (12)].t_goal),(yyvsp[(11) - (12)].t_effect_lists),
			current_analysis->var_tab_stack.pop()); delete [] (yyvsp[(3) - (12)].cp); ;}
    break;

  case 259:

/* Line 1464 of yacc.c  */
#line 1231 "src/pddl+.yacc"
    {yyerrok;
	 log_error(E_FATAL,"Syntax error in action declaration.");
	 (yyval.t_action_def)= NULL; ;}
    break;

  case 260:

/* Line 1464 of yacc.c  */
#line 1244 "src/pddl+.yacc"
    {(yyval.t_event_def)= current_analysis->buildEvent(current_analysis->op_tab.new_symbol_put((yyvsp[(3) - (12)].cp)),
		   (yyvsp[(6) - (12)].t_var_symbol_list),(yyvsp[(9) - (12)].t_goal),(yyvsp[(11) - (12)].t_effect_lists),
		   current_analysis->var_tab_stack.pop()); delete [] (yyvsp[(3) - (12)].cp);;}
    break;

  case 261:

/* Line 1464 of yacc.c  */
#line 1249 "src/pddl+.yacc"
    {yyerrok;
	 log_error(E_FATAL,"Syntax error in event declaration.");
	 (yyval.t_event_def)= NULL; ;}
    break;

  case 262:

/* Line 1464 of yacc.c  */
#line 1261 "src/pddl+.yacc"
    {(yyval.t_process_def)= current_analysis->buildProcess(current_analysis->op_tab.new_symbol_put((yyvsp[(3) - (12)].cp)),
		     (yyvsp[(6) - (12)].t_var_symbol_list),(yyvsp[(9) - (12)].t_goal),(yyvsp[(11) - (12)].t_effect_lists),
                     current_analysis->var_tab_stack.pop()); delete [] (yyvsp[(3) - (12)].cp);;}
    break;

  case 263:

/* Line 1464 of yacc.c  */
#line 1265 "src/pddl+.yacc"
    {yyerrok;
	 log_error(E_FATAL,"Syntax error in process declaration.");
	 (yyval.t_process_def)= NULL; ;}
    break;

  case 264:

/* Line 1464 of yacc.c  */
#line 1277 "src/pddl+.yacc"
    { (yyval.t_durative_action_def)= (yyvsp[(10) - (11)].t_durative_action_def);
      (yyval.t_durative_action_def)->name= current_analysis->op_tab.new_symbol_put((yyvsp[(3) - (11)].cp));
      (yyval.t_durative_action_def)->symtab= current_analysis->var_tab_stack.pop();
      (yyval.t_durative_action_def)->parameters= (yyvsp[(6) - (11)].t_var_symbol_list);
      (yyval.t_durative_action_def)->dur_constraint= (yyvsp[(9) - (11)].t_goal);
      delete [] (yyvsp[(3) - (11)].cp);
    ;}
    break;

  case 265:

/* Line 1464 of yacc.c  */
#line 1286 "src/pddl+.yacc"
    {yyerrok;
	 log_error(E_FATAL,"Syntax error in durative-action declaration.");
	 (yyval.t_durative_action_def)= NULL; ;}
    break;

  case 266:

/* Line 1464 of yacc.c  */
#line 1293 "src/pddl+.yacc"
    {(yyval.t_durative_action_def)=(yyvsp[(1) - (3)].t_durative_action_def); (yyval.t_durative_action_def)->effects=(yyvsp[(3) - (3)].t_effect_lists);;}
    break;

  case 267:

/* Line 1464 of yacc.c  */
#line 1295 "src/pddl+.yacc"
    {(yyval.t_durative_action_def)=(yyvsp[(1) - (3)].t_durative_action_def); (yyval.t_durative_action_def)->precondition=(yyvsp[(3) - (3)].t_goal);;}
    break;

  case 268:

/* Line 1464 of yacc.c  */
#line 1296 "src/pddl+.yacc"
    {(yyval.t_durative_action_def)= current_analysis->buildDurativeAction();;}
    break;

  case 269:

/* Line 1464 of yacc.c  */
#line 1301 "src/pddl+.yacc"
    { (yyval.t_goal)=(yyvsp[(1) - (1)].t_goal); ;}
    break;

  case 270:

/* Line 1464 of yacc.c  */
#line 1303 "src/pddl+.yacc"
    { (yyval.t_goal)= new conj_goal((yyvsp[(3) - (4)].t_goal_list)); ;}
    break;

  case 271:

/* Line 1464 of yacc.c  */
#line 1308 "src/pddl+.yacc"
    { (yyval.t_goal_list)=(yyvsp[(1) - (2)].t_goal_list); (yyval.t_goal_list)->push_back((yyvsp[(2) - (2)].t_goal)); ;}
    break;

  case 272:

/* Line 1464 of yacc.c  */
#line 1310 "src/pddl+.yacc"
    { (yyval.t_goal_list)= new goal_list; ;}
    break;

  case 273:

/* Line 1464 of yacc.c  */
#line 1315 "src/pddl+.yacc"
    {(yyval.t_goal)= new timed_goal((yyvsp[(3) - (4)].t_goal),E_AT_START);;}
    break;

  case 274:

/* Line 1464 of yacc.c  */
#line 1317 "src/pddl+.yacc"
    {(yyval.t_goal)= new timed_goal((yyvsp[(3) - (4)].t_goal),E_AT_END);;}
    break;

  case 275:

/* Line 1464 of yacc.c  */
#line 1319 "src/pddl+.yacc"
    {(yyval.t_goal)= new timed_goal((yyvsp[(3) - (4)].t_goal),E_OVER_ALL);;}
    break;

  case 276:

/* Line 1464 of yacc.c  */
#line 1321 "src/pddl+.yacc"
    {timed_goal * tg = dynamic_cast<timed_goal *>((yyvsp[(4) - (5)].t_goal));
		(yyval.t_goal) = new timed_goal(new preference((yyvsp[(3) - (5)].cp),tg->clearGoal()),tg->getTime());
			delete tg;
			requires(E_PREFERENCES);;}
    break;

  case 277:

/* Line 1464 of yacc.c  */
#line 1326 "src/pddl+.yacc"
    {(yyval.t_goal) = new preference((yyvsp[(3) - (4)].t_goal));requires(E_PREFERENCES);;}
    break;

  case 278:

/* Line 1464 of yacc.c  */
#line 1330 "src/pddl+.yacc"
    {(yyval.t_dummy)= 0; current_analysis->var_tab_stack.push(
    				current_analysis->buildOpTab());;}
    break;

  case 279:

/* Line 1464 of yacc.c  */
#line 1335 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_EQUALITY;;}
    break;

  case 280:

/* Line 1464 of yacc.c  */
#line 1336 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_STRIPS;;}
    break;

  case 281:

/* Line 1464 of yacc.c  */
#line 1338 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_TYPING;;}
    break;

  case 282:

/* Line 1464 of yacc.c  */
#line 1340 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_NEGATIVE_PRECONDITIONS;;}
    break;

  case 283:

/* Line 1464 of yacc.c  */
#line 1342 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_DISJUNCTIVE_PRECONDS;;}
    break;

  case 284:

/* Line 1464 of yacc.c  */
#line 1343 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_EXT_PRECS;;}
    break;

  case 285:

/* Line 1464 of yacc.c  */
#line 1344 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_UNIV_PRECS;;}
    break;

  case 286:

/* Line 1464 of yacc.c  */
#line 1345 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_COND_EFFS;;}
    break;

  case 287:

/* Line 1464 of yacc.c  */
#line 1346 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_NFLUENTS | E_OFLUENTS;;}
    break;

  case 288:

/* Line 1464 of yacc.c  */
#line 1348 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_DURATIVE_ACTIONS;;}
    break;

  case 289:

/* Line 1464 of yacc.c  */
#line 1349 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_TIME |
                      E_NFLUENTS |
                      E_DURATIVE_ACTIONS; ;}
    break;

  case 290:

/* Line 1464 of yacc.c  */
#line 1352 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)=E_ACTIONCOSTS | E_NFLUENTS;;}
    break;

  case 291:

/* Line 1464 of yacc.c  */
#line 1355 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)=E_OFLUENTS;;}
    break;

  case 292:

/* Line 1464 of yacc.c  */
#line 1356 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)=E_NFLUENTS;;}
    break;

  case 293:

/* Line 1464 of yacc.c  */
#line 1357 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)=E_MODULES;;}
    break;

  case 294:

/* Line 1464 of yacc.c  */
#line 1359 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_STRIPS |
		      E_TYPING |
		      E_NEGATIVE_PRECONDITIONS |
		      E_DISJUNCTIVE_PRECONDS |
		      E_EQUALITY |
		      E_EXT_PRECS |
		      E_UNIV_PRECS |
		      E_COND_EFFS;;}
    break;

  case 295:

/* Line 1464 of yacc.c  */
#line 1368 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_EXT_PRECS |
		      E_UNIV_PRECS;;}
    break;

  case 296:

/* Line 1464 of yacc.c  */
#line 1372 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_DURATION_INEQUALITIES;;}
    break;

  case 297:

/* Line 1464 of yacc.c  */
#line 1375 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag)= E_CONTINUOUS_EFFECTS;;}
    break;

  case 298:

/* Line 1464 of yacc.c  */
#line 1377 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag) = E_DERIVED_PREDICATES;;}
    break;

  case 299:

/* Line 1464 of yacc.c  */
#line 1379 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag) = E_TIMED_INITIAL_LITERALS;;}
    break;

  case 300:

/* Line 1464 of yacc.c  */
#line 1381 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag) = E_PREFERENCES;;}
    break;

  case 301:

/* Line 1464 of yacc.c  */
#line 1383 "src/pddl+.yacc"
    {(yyval.t_pddl_req_flag) = E_CONSTRAINTS;;}
    break;

  case 302:

/* Line 1464 of yacc.c  */
#line 1385 "src/pddl+.yacc"
    {log_error(E_WARNING,"Unrecognised requirements declaration ");
       (yyval.t_pddl_req_flag)= 0; delete [] (yyvsp[(1) - (1)].cp);;}
    break;

  case 303:

/* Line 1464 of yacc.c  */
#line 1391 "src/pddl+.yacc"
    {(yyval.t_const_symbol_list)=(yyvsp[(3) - (4)].t_const_symbol_list);;}
    break;

  case 304:

/* Line 1464 of yacc.c  */
#line 1395 "src/pddl+.yacc"
    {(yyval.t_type_list)=(yyvsp[(3) - (4)].t_type_list); requires(E_TYPING);;}
    break;

  case 305:

/* Line 1464 of yacc.c  */
#line 1405 "src/pddl+.yacc"
    {(yyval.t_problem)=(yyvsp[(11) - (12)].t_problem); (yyval.t_problem)->name = (yyvsp[(5) - (12)].cp); (yyval.t_problem)->domain_name = (yyvsp[(9) - (12)].cp);
		if (types_used && !types_defined) {
			yyerrok; log_error(E_FATAL,"Syntax error in problem file - types used, but no :types section in domain file.");
		}

	;}
    break;

  case 306:

/* Line 1464 of yacc.c  */
#line 1412 "src/pddl+.yacc"
    {yyerrok; (yyval.t_problem)=NULL;
       	log_error(E_FATAL,"Syntax error in problem definition."); ;}
    break;

  case 307:

/* Line 1464 of yacc.c  */
#line 1418 "src/pddl+.yacc"
    {(yyval.t_problem)=(yyvsp[(2) - (2)].t_problem); (yyval.t_problem)->req= (yyvsp[(1) - (2)].t_pddl_req_flag);;}
    break;

  case 308:

/* Line 1464 of yacc.c  */
#line 1419 "src/pddl+.yacc"
    {(yyval.t_problem)=(yyvsp[(2) - (2)].t_problem); (yyval.t_problem)->objects= (yyvsp[(1) - (2)].t_const_symbol_list);;}
    break;

  case 309:

/* Line 1464 of yacc.c  */
#line 1420 "src/pddl+.yacc"
    {(yyval.t_problem)=(yyvsp[(2) - (2)].t_problem); (yyval.t_problem)->initial_state= (yyvsp[(1) - (2)].t_effect_lists);;}
    break;

  case 310:

/* Line 1464 of yacc.c  */
#line 1421 "src/pddl+.yacc"
    {(yyval.t_problem)=(yyvsp[(2) - (2)].t_problem); (yyval.t_problem)->the_goal= (yyvsp[(1) - (2)].t_goal);;}
    break;

  case 311:

/* Line 1464 of yacc.c  */
#line 1423 "src/pddl+.yacc"
    {(yyval.t_problem)=(yyvsp[(2) - (2)].t_problem); (yyval.t_problem)->constraints = (yyvsp[(1) - (2)].t_con_goal);;}
    break;

  case 312:

/* Line 1464 of yacc.c  */
#line 1424 "src/pddl+.yacc"
    {(yyval.t_problem)=(yyvsp[(2) - (2)].t_problem); if((yyval.t_problem)->metric == 0) {(yyval.t_problem)->metric= (yyvsp[(1) - (2)].t_metric);}
											else {(yyval.t_problem)->metric->add((yyvsp[(1) - (2)].t_metric));};}
    break;

  case 313:

/* Line 1464 of yacc.c  */
#line 1426 "src/pddl+.yacc"
    {(yyval.t_problem)=(yyvsp[(2) - (2)].t_problem); (yyval.t_problem)->length= (yyvsp[(1) - (2)].t_length_spec);;}
    break;

  case 314:

/* Line 1464 of yacc.c  */
#line 1427 "src/pddl+.yacc"
    {(yyval.t_problem)=new problem;;}
    break;

  case 315:

/* Line 1464 of yacc.c  */
#line 1430 "src/pddl+.yacc"
    {(yyval.t_const_symbol_list)=(yyvsp[(3) - (4)].t_const_symbol_list);;}
    break;

  case 316:

/* Line 1464 of yacc.c  */
#line 1433 "src/pddl+.yacc"
    {(yyval.t_effect_lists)=(yyvsp[(3) - (4)].t_effect_lists);;}
    break;

  case 317:

/* Line 1464 of yacc.c  */
#line 1436 "src/pddl+.yacc"
    {(yyval.vtab) = current_analysis->buildOpTab();;}
    break;

  case 318:

/* Line 1464 of yacc.c  */
#line 1439 "src/pddl+.yacc"
    {(yyval.t_goal)=(yyvsp[(3) - (4)].t_goal);delete (yyvsp[(2) - (4)].vtab);;}
    break;

  case 319:

/* Line 1464 of yacc.c  */
#line 1444 "src/pddl+.yacc"
    { (yyval.t_metric)= new metric_spec((yyvsp[(3) - (5)].t_optimization),(yyvsp[(4) - (5)].t_expression)); ;}
    break;

  case 320:

/* Line 1464 of yacc.c  */
#line 1446 "src/pddl+.yacc"
    {yyerrok;
        log_error(E_FATAL,"Syntax error in metric declaration.");
        (yyval.t_metric)= NULL; ;}
    break;

  case 321:

/* Line 1464 of yacc.c  */
#line 1453 "src/pddl+.yacc"
    {(yyval.t_length_spec)= new length_spec(E_BOTH,(yyvsp[(4) - (7)].ival),(yyvsp[(6) - (7)].ival));;}
    break;

  case 322:

/* Line 1464 of yacc.c  */
#line 1456 "src/pddl+.yacc"
    {(yyval.t_length_spec) = new length_spec(E_SERIAL,(yyvsp[(4) - (5)].ival));;}
    break;

  case 323:

/* Line 1464 of yacc.c  */
#line 1460 "src/pddl+.yacc"
    {(yyval.t_length_spec) = new length_spec(E_PARALLEL,(yyvsp[(4) - (5)].ival));;}
    break;

  case 324:

/* Line 1464 of yacc.c  */
#line 1466 "src/pddl+.yacc"
    {(yyval.t_optimization)= E_MINIMIZE;;}
    break;

  case 325:

/* Line 1464 of yacc.c  */
#line 1467 "src/pddl+.yacc"
    {(yyval.t_optimization)= E_MAXIMIZE;;}
    break;

  case 326:

/* Line 1464 of yacc.c  */
#line 1472 "src/pddl+.yacc"
    {(yyval.t_expression)= (yyvsp[(2) - (3)].t_expression);;}
    break;

  case 327:

/* Line 1464 of yacc.c  */
#line 1473 "src/pddl+.yacc"
    {(yyval.t_expression)= (yyvsp[(1) - (1)].t_func_term);;}
    break;

  case 328:

/* Line 1464 of yacc.c  */
#line 1474 "src/pddl+.yacc"
    {(yyval.t_expression)= (yyvsp[(1) - (1)].t_num_expression);;}
    break;

  case 329:

/* Line 1464 of yacc.c  */
#line 1475 "src/pddl+.yacc"
    { (yyval.t_expression)= new special_val_expr(E_TOTAL_TIME); ;}
    break;

  case 330:

/* Line 1464 of yacc.c  */
#line 1477 "src/pddl+.yacc"
    {(yyval.t_expression) = new violation_term((yyvsp[(3) - (4)].cp));;}
    break;

  case 331:

/* Line 1464 of yacc.c  */
#line 1478 "src/pddl+.yacc"
    { (yyval.t_expression)= new special_val_expr(E_TOTAL_TIME); ;}
    break;

  case 332:

/* Line 1464 of yacc.c  */
#line 1482 "src/pddl+.yacc"
    { (yyval.t_expression)= new plus_expression((yyvsp[(2) - (3)].t_expression),(yyvsp[(3) - (3)].t_expression)); ;}
    break;

  case 333:

/* Line 1464 of yacc.c  */
#line 1483 "src/pddl+.yacc"
    { (yyval.t_expression)= new minus_expression((yyvsp[(2) - (3)].t_expression),(yyvsp[(3) - (3)].t_expression)); ;}
    break;

  case 334:

/* Line 1464 of yacc.c  */
#line 1484 "src/pddl+.yacc"
    { (yyval.t_expression)= new mul_expression((yyvsp[(2) - (3)].t_expression),(yyvsp[(3) - (3)].t_expression)); ;}
    break;

  case 335:

/* Line 1464 of yacc.c  */
#line 1485 "src/pddl+.yacc"
    { (yyval.t_expression)= new div_expression((yyvsp[(2) - (3)].t_expression),(yyvsp[(3) - (3)].t_expression)); ;}
    break;

  case 336:

/* Line 1464 of yacc.c  */
#line 1489 "src/pddl+.yacc"
    {(yyval.t_expression) = (yyvsp[(1) - (1)].t_expression);;}
    break;

  case 337:

/* Line 1464 of yacc.c  */
#line 1491 "src/pddl+.yacc"
    {(yyval.t_expression) = new plus_expression((yyvsp[(1) - (2)].t_expression),(yyvsp[(2) - (2)].t_expression));;}
    break;

  case 338:

/* Line 1464 of yacc.c  */
#line 1495 "src/pddl+.yacc"
    {(yyval.t_expression) = (yyvsp[(1) - (1)].t_expression);;}
    break;

  case 339:

/* Line 1464 of yacc.c  */
#line 1497 "src/pddl+.yacc"
    {(yyval.t_expression) = new mul_expression((yyvsp[(1) - (2)].t_expression),(yyvsp[(2) - (2)].t_expression));;}
    break;

  case 340:

/* Line 1464 of yacc.c  */
#line 1503 "src/pddl+.yacc"
    {(yyval.t_plan)= (yyvsp[(2) - (2)].t_plan);
         (yyval.t_plan)->push_front((yyvsp[(1) - (2)].t_step)); ;}
    break;

  case 341:

/* Line 1464 of yacc.c  */
#line 1506 "src/pddl+.yacc"
    {(yyval.t_plan) = (yyvsp[(3) - (3)].t_plan);(yyval.t_plan)->insertTime((yyvsp[(2) - (3)].fval));;}
    break;

  case 342:

/* Line 1464 of yacc.c  */
#line 1508 "src/pddl+.yacc"
    {(yyval.t_plan) = (yyvsp[(3) - (3)].t_plan);(yyval.t_plan)->insertTime((yyvsp[(2) - (3)].ival));;}
    break;

  case 343:

/* Line 1464 of yacc.c  */
#line 1510 "src/pddl+.yacc"
    {(yyval.t_plan)= new plan;;}
    break;

  case 344:

/* Line 1464 of yacc.c  */
#line 1515 "src/pddl+.yacc"
    {(yyval.t_step)=(yyvsp[(3) - (3)].t_step);
         (yyval.t_step)->start_time_given=1;
         (yyval.t_step)->start_time=(yyvsp[(1) - (3)].fval);;}
    break;

  case 345:

/* Line 1464 of yacc.c  */
#line 1519 "src/pddl+.yacc"
    {(yyval.t_step)=(yyvsp[(1) - (1)].t_step);
	 (yyval.t_step)->start_time_given=0;;}
    break;

  case 346:

/* Line 1464 of yacc.c  */
#line 1525 "src/pddl+.yacc"
    {(yyval.t_step)= (yyvsp[(1) - (4)].t_step);
	 (yyval.t_step)->duration_given=1;
         (yyval.t_step)->duration= (yyvsp[(3) - (4)].fval);;}
    break;

  case 347:

/* Line 1464 of yacc.c  */
#line 1529 "src/pddl+.yacc"
    {(yyval.t_step)= (yyvsp[(1) - (1)].t_step);
         (yyval.t_step)->duration_given=0;;}
    break;

  case 348:

/* Line 1464 of yacc.c  */
#line 1535 "src/pddl+.yacc"
    {(yyval.t_step)= new plan_step(
              current_analysis->op_tab.symbol_get((yyvsp[(2) - (4)].cp)),
	      (yyvsp[(3) - (4)].t_const_symbol_list)); delete [] (yyvsp[(2) - (4)].cp);
      ;}
    break;

  case 349:

/* Line 1464 of yacc.c  */
#line 1542 "src/pddl+.yacc"
    {(yyval.fval)= (yyvsp[(1) - (1)].fval);;}
    break;

  case 350:

/* Line 1464 of yacc.c  */
#line 1543 "src/pddl+.yacc"
    {(yyval.fval)= (float) (yyvsp[(1) - (1)].ival);;}
    break;



/* Line 1464 of yacc.c  */
#line 5022 "src/pddl+.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1684 of yacc.c  */
#line 1546 "src/pddl+.yacc"


#include <cstdio>
#include <iostream>
int line_no= 1;
using std::istream;
#include "lex.yy.cc"

namespace VAL {
extern yyFlexLexer* yfl;
};


int yyerror(char * s)
{
    return 0;
}

int yylex()
{
    return yfl->yylex();
}

