/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 10 "pddl+.y" /* yacc.c:339  */

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


#line 128 "pddl+.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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
    AFTER = 360,
    BOGUS = 361,
    CONTROL = 362,
    SUPPLYDEMAND = 363,
    SUPPLYDEMAND_REQ = 364,
    NAMETAG = 365,
    NAME = 366,
    FUNCTION_SYMBOL = 367,
    INTVAL = 368,
    FLOATVAL = 369,
    AT_TIME = 370,
    HYPHEN = 371,
    PLUS = 372,
    MUL = 373,
    DIV = 374,
    UMINUS = 375
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 72 "pddl+.y" /* yacc.c:355  */

    parse_category* t_parse_category;

    effect_lists* t_effect_lists;
    effect* t_effect;
    simple_effect* t_simple_effect;
    cond_effect*   t_cond_effect;
    forall_effect* t_forall_effect;
    timed_effect* t_timed_effect;
	supplied_effect* t_supplied_effect;

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


#line 363 "pddl+.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 380 "pddl+.cpp" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  17
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1001

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  121
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  134
/* YYNRULES -- Number of rules.  */
#define YYNRULES  366
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  846

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   375

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
     115,   116,   117,   118,   119,   120
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   255,   255,   256,   257,   261,   268,   276,   277,   278,
     279,   281,   283,   285,   286,   289,   293,   296,   298,   300,
     301,   305,   312,   319,   320,   325,   327,   332,   334,   342,
     350,   352,   360,   365,   367,   371,   373,   380,   380,   383,
     396,   405,   414,   421,   427,   434,   436,   442,   451,   461,
     466,   467,   471,   472,   480,   487,   496,   502,   504,   506,
     513,   519,   520,   524,   528,   532,   537,   544,   549,   551,
     555,   557,   561,   574,   576,   578,   580,   583,   587,   593,
     595,   601,   607,   608,   610,   612,   621,   622,   623,   624,
     625,   629,   630,   634,   636,   638,   645,   646,   647,   649,
     653,   655,   663,   665,   673,   678,   683,   686,   693,   694,
     699,   701,   703,   707,   711,   717,   721,   725,   731,   733,
     741,   746,   752,   753,   757,   758,   762,   764,   766,   773,
     774,   775,   777,   782,   784,   786,   788,   790,   795,   801,
     807,   812,   813,   817,   818,   820,   821,   822,   826,   828,
     830,   832,   837,   839,   842,   845,   851,   852,   853,   861,
     865,   868,   872,   877,   884,   889,   894,   899,   904,   906,
     908,   910,   912,   917,   919,   921,   923,   925,   927,   928,
     929,   933,   935,   937,   943,   944,   947,   950,   952,   955,
     972,   974,   976,   982,   983,   984,   985,   986,   998,  1005,
    1007,  1011,  1012,  1016,  1018,  1020,  1022,  1026,  1031,  1033,
    1035,  1037,  1044,  1046,  1051,  1053,  1057,  1062,  1064,  1069,
    1071,  1074,  1076,  1078,  1080,  1082,  1084,  1086,  1088,  1090,
    1092,  1094,  1096,  1103,  1105,  1110,  1112,  1116,  1118,  1121,
    1124,  1127,  1130,  1136,  1138,  1143,  1145,  1155,  1162,  1169,
    1174,  1179,  1184,  1186,  1193,  1195,  1202,  1204,  1211,  1213,
    1220,  1221,  1225,  1226,  1227,  1228,  1229,  1230,  1233,  1242,
    1248,  1257,  1268,  1275,  1286,  1292,  1302,  1308,  1310,  1313,
    1331,  1338,  1340,  1342,  1346,  1348,  1353,  1356,  1360,  1362,
    1364,  1366,  1371,  1376,  1381,  1382,  1384,  1385,  1387,  1389,
    1390,  1391,  1392,  1393,  1395,  1398,  1401,  1402,  1403,  1405,
    1414,  1417,  1420,  1422,  1424,  1426,  1428,  1430,  1431,  1437,
    1441,  1446,  1458,  1466,  1467,  1468,  1469,  1470,  1472,  1474,
    1475,  1478,  1481,  1484,  1487,  1491,  1493,  1500,  1503,  1507,
    1514,  1515,  1520,  1521,  1522,  1523,  1524,  1526,  1530,  1531,
    1532,  1533,  1537,  1538,  1543,  1544,  1550,  1553,  1555,  1558,
    1562,  1566,  1572,  1576,  1582,  1590,  1591
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
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
  "ALWAYSWITHIN", "HOLDDURING", "HOLDAFTER", "ISVIOLATED", "AFTER",
  "BOGUS", "CONTROL", "SUPPLYDEMAND", "SUPPLYDEMAND_REQ", "NAMETAG",
  "NAME", "FUNCTION_SYMBOL", "INTVAL", "FLOATVAL", "AT_TIME", "HYPHEN",
  "PLUS", "MUL", "DIV", "UMINUS", "$accept", "mystartsymbol", "c_domain",
  "c_preamble", "c_domain_name", "c_new_class", "c_class", "c_classes",
  "c_class_seq", "c_domain_require_def", "c_reqs", "c_pred_decls",
  "c_pred_decl", "c_new_pred_symbol", "c_pred_symbol",
  "c_init_pred_symbol", "c_func_decls", "c_func_decl", "c_ntype",
  "c_new_func_symbol", "c_typed_var_list", "c_control_params_list",
  "c_var_symbol_list", "c_typed_consts", "c_const_symbols",
  "c_new_const_symbols", "c_typed_types", "c_parameter_symbols",
  "c_declaration_var_symbol", "c_var_symbol", "c_const_symbol",
  "c_new_const_symbol", "c_either_type", "c_new_primitive_type",
  "c_primitive_type", "c_new_primitive_types", "c_primitive_types",
  "c_init_els", "c_supply_demand", "c_demand", "c_timed_initial_literal",
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
  "c_name_or_goal_descriptor", "c_goal_descriptor",
  "c_pre_goal_descriptor_list", "c_goal_list", "c_forall", "c_exists",
  "c_proposition", "c_derived_proposition", "c_init_proposition",
  "c_predicates", "c_functions_def", "c_constraints_def",
  "c_constraints_probdef", "c_structure_defs", "c_structure_def",
  "c_class_def", "c_rule_head", "c_derivation_rule", "c_action_def",
  "c_event_def", "c_process_def", "c_control", "c_durative_action_def",
  "c_da_def_body", "c_da_gd", "c_da_gds", "c_timed_gd", "c_args_head",
  "c_require_key", "c_domain_constants", "c_type_names", "c_problem",
  "c_problem_body", "c_objects", "c_initial_state", "c_goals",
  "c_goal_spec", "c_metric_spec", "c_length_spec", "c_optimization",
  "c_ground_f_exp", "c_binary_ground_f_exp", "c_binary_ground_f_pexps",
  "c_binary_ground_f_mexps", "c_plan", "c_step_t_d", "c_step_d", "c_step",
  "c_float", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
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
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375
};
# endif

#define YYPACT_NINF -603

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-603)))

#define YYTABLE_NINF -86

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      19,    23,   159,  -603,  -603,    16,  -603,  -603,  -603,    54,
    -603,    56,   -34,    67,   -20,    54,    54,  -603,     0,  -603,
     311,   178,    41,   109,  -603,   204,   -20,  -603,  -603,   187,
    -603,   123,    12,  -603,   805,   244,   264,   264,   264,   264,
     264,   277,  -603,  -603,  -603,  -603,  -603,  -603,  -603,   264,
     264,  -603,  -603,  -603,   280,  -603,   284,   181,   184,   550,
     282,    32,    35,    53,    82,  -603,   196,   295,   451,  -603,
     315,  -603,  -603,  -603,  -603,  -603,  -603,   491,  -603,  -603,
    -603,  -603,   325,  -603,   181,   335,  -603,   347,   356,   668,
     364,   447,   367,   372,   338,   387,   338,   390,   338,   393,
     338,  -603,   413,   310,   196,   428,    87,   432,   443,   446,
     438,   456,   115,    11,   445,   409,  -603,  -603,   437,   499,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,   445,
     502,  -603,   445,   445,   360,   445,   445,   445,   360,   360,
     360,    77,   397,   508,  -603,  -603,  -603,   511,  -603,   529,
    -603,   531,  -603,   538,  -603,    90,  -603,  -603,   549,  -603,
     464,  -603,  -603,  -603,    88,  -603,  -603,  -603,  -603,    90,
    -603,  -603,  -603,   464,   530,   554,  -603,   449,  -603,   611,
     487,  -603,   620,   624,  -603,  -603,   445,   628,   445,   445,
     445,   360,   445,  -603,    77,  -603,   445,   464,   464,   464,
     464,   464,   573,  -603,   196,   196,  -603,   541,   650,   544,
     658,  -603,   464,  -603,  -603,   662,   445,   445,  -603,   445,
     445,   110,  -603,  -603,  -603,  -603,  -603,    24,   675,   677,
    -603,   707,  -603,  -603,  -603,  -603,  -603,   710,  -603,   712,
     715,   445,   445,   718,   719,   720,   723,   724,   725,   727,
     729,  -603,  -603,  -603,  -603,   464,  -603,    90,  -603,   730,
    -603,  -603,  -603,  -603,   495,   514,   445,   740,   213,   630,
     614,  -603,    24,  -603,  -603,   464,   464,   717,  -603,  -603,
    -603,   741,   746,  -603,  -603,  -603,   502,   697,   714,   735,
     655,   153,  -603,   464,   464,   649,  -603,  -603,  -603,   759,
    -603,  -603,   -26,  -603,   774,  -603,    24,    24,    24,    24,
     776,  -603,   763,   782,   784,   357,   717,   717,   786,   717,
     717,   717,   717,   717,  -603,  -603,   788,   790,   445,   445,
     792,   742,  -603,  -603,  -603,  -603,   702,  -603,  -603,  -603,
    -603,  -603,   214,   216,    70,    24,    24,    24,   686,  -603,
     445,   445,   337,  -603,   196,  -603,   444,   252,   790,  -603,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,   273,   760,
    -603,  -603,   761,   762,   112,   807,  -603,  -603,  -603,  -603,
     815,   816,   821,   822,  -603,   824,   825,   834,   656,   835,
    -603,   519,   848,   749,   750,   849,  -603,  -603,    57,   850,
    -603,    92,   533,   858,   865,   866,   865,   867,   754,   388,
    -603,  -603,  -603,  -603,  -603,   231,  -603,  -603,  -603,    98,
     869,   870,  -603,   308,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,    31,   871,  -603,   705,  -603,  -603,  -603,  -603,   872,
    -603,   445,   873,  -603,  -603,  -603,   536,   464,   448,   874,
    -603,  -603,  -603,  -603,  -603,  -603,   168,   875,   876,  -603,
     780,   878,   879,  -603,  -603,  -603,  -603,   791,   318,  -603,
     502,   880,  -603,   559,  -603,   464,   882,    49,   775,  -603,
    -603,  -603,  -603,   777,  -603,   883,   778,  -603,  -603,    57,
      57,    57,    57,   884,  -603,   887,  -603,  -603,  -603,   888,
     467,   890,   445,   891,  -603,  -603,    49,    49,  -603,  -603,
     112,   190,   190,   583,   814,  -603,   893,   894,   895,  -603,
    -603,  -603,   896,   787,   897,   490,   360,   445,   609,   234,
     898,  -603,   899,   237,   274,    57,    57,    57,    57,  -603,
    -603,   790,   900,   552,   901,   903,  -603,  -603,  -603,   903,
     903,  -603,    11,   904,   903,   464,   619,    78,    78,  -603,
     817,   818,  -603,  -603,    24,   350,  -603,  -603,   410,  -603,
    -603,  -603,  -603,   908,  -603,   909,    72,  -603,  -603,  -603,
    -603,  -603,  -603,  -603,    57,  -603,    57,  -603,  -603,   910,
     824,  -603,  -603,    49,    49,    49,    49,    49,  -603,  -603,
    -603,  -603,  -603,   911,   912,  -603,  -603,   789,  -603,   913,
     914,   831,   838,   916,  -603,   102,   445,   445,   445,  -603,
     917,   919,   919,  -603,   893,   445,    49,    49,   920,   673,
     921,  -603,   245,  -603,   903,   360,  -603,  -603,  -603,   623,
      24,    24,    24,    24,    24,  -603,   865,     7,  -603,  -603,
      24,    24,  -603,   923,   924,   925,   926,   927,   627,  -603,
     653,   928,  -603,  -603,  -603,  -603,   929,   639,   894,   931,
      83,    83,   464,   105,   869,   932,  -603,   933,   903,   716,
    -603,  -603,  -603,  -603,   934,   935,   936,   937,   938,   939,
      24,   844,   940,   941,   942,  -603,  -603,  -603,  -603,  -603,
    -603,  -603,    49,    49,    49,    49,    49,  -603,  -603,  -603,
    -603,   943,   316,  -603,   944,   739,   945,   946,   947,   502,
     948,   642,   464,  -603,   949,  -603,  -603,  -603,  -603,  -603,
    -603,   950,   951,   952,   953,  -603,   644,   177,   177,   177,
     177,   177,  -603,   954,  -603,   445,    49,    49,   956,  -603,
       7,  -603,  -603,   894,   957,  -603,  -603,   958,  -603,  -603,
    -603,  -603,  -603,   721,  -603,  -603,  -603,  -603,   748,   -50,
     959,  -603,  -603,  -603,   960,   961,   962,   963,  -603,   647,
     931,    78,    78,   464,   202,   964,  -603,   908,   177,   177,
     177,   177,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,
    -603,   965,   966,   967,   968,  -603,   969,   177,   177,   177,
     177,  -603,  -603,  -603,   931,  -603,   970,   971,   972,   973,
     974,  -603,  -603,  -603,  -603,  -603
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     359,     0,     0,   366,   365,     0,     2,     3,     4,   359,
     361,   363,     0,     0,    51,   359,   359,     1,     0,   356,
       0,     0,     0,     0,    63,     0,    51,   358,   357,     0,
     360,     0,     0,     6,     0,     0,     0,     0,     0,     0,
       0,    14,   261,   267,   266,   262,   263,   264,   265,     0,
       0,   364,    50,   362,     0,   322,     0,    20,     0,     0,
       0,     0,     0,     0,     0,   269,    53,     0,     0,    69,
       0,     5,    13,     7,    10,    11,    12,     0,   260,     9,
       8,    15,     0,    16,    20,     0,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,    49,    53,     0,     0,     0,    26,     0,
       0,     0,    56,     0,     0,     0,    19,    18,     0,     0,
      22,    21,   308,   294,   295,   309,   297,   296,   298,   299,
     300,   310,   301,   302,   306,   307,   305,   304,   303,   311,
     312,   313,   314,   315,   316,   317,   318,    23,   257,     0,
       0,   247,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   256,   272,   293,     0,   276,     0,
     274,     0,   280,     0,   319,     0,    52,   253,     0,    29,
      46,   252,    25,   255,     0,   254,    33,   320,    66,     0,
      68,    30,    31,    46,     0,     0,   235,     0,   268,     0,
       0,   218,     0,     0,   184,   185,     0,     0,     0,     0,
       0,     0,     0,   234,     0,   233,     0,    46,    46,    46,
      46,    46,     0,    67,    53,    53,    28,     0,     0,    42,
       0,    39,    46,    69,    69,     0,     0,     0,   248,     0,
       0,   197,   193,   194,   195,   196,    59,     0,     0,     0,
     270,     0,   221,   219,   217,   222,   223,     0,   225,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    71,    48,    47,    60,    46,    27,     0,    36,     0,
      55,    54,   250,   246,     0,     0,     0,     0,     0,     0,
       0,   188,     0,   178,   180,    46,    46,   330,   224,   226,
     227,     0,     0,   230,   231,   232,     0,     0,     0,     0,
     278,     0,    45,    46,    46,    38,   237,   245,   238,     0,
     236,   249,     0,    57,    59,    59,     0,     0,     0,     0,
       0,   179,     0,     0,     0,     0,   330,   330,     0,   330,
     330,   330,   330,   330,   228,   229,     0,     0,     0,     0,
       0,     0,    65,    70,    41,    40,     0,    35,   239,    62,
      61,    58,     0,     0,     0,     0,     0,     0,     0,   242,
       0,     0,     0,    77,    53,   333,     0,     0,     0,   323,
     327,   321,   324,   325,   326,   328,   329,   220,     0,     0,
     198,   216,     0,     0,    44,     0,    37,   187,   186,   173,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
     207,     0,     0,     0,     0,     0,   340,   341,     0,     0,
     202,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     283,   175,   174,   176,   177,     0,   240,   241,   259,     0,
       0,     0,   258,     0,   332,    76,    75,    74,    73,   165,
     331,     0,     0,   336,     0,   345,   192,   344,   343,     0,
     334,     0,     0,   201,   244,   216,     0,    46,     0,     0,
      86,    88,    87,    90,    89,   163,     0,     0,     0,   277,
       0,     0,     0,   161,   158,   157,   156,     0,     0,   189,
       0,     0,   213,     0,   207,    46,     0,     0,     0,    32,
      77,    59,   338,     0,   339,     0,     0,    59,    59,     0,
       0,     0,     0,     0,   335,     0,   214,   199,   243,     0,
       0,     0,     0,     0,   271,   142,     0,     0,   275,   273,
      44,     0,     0,     0,     0,   279,     0,     0,     0,   203,
     205,   212,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   347,     0,     0,     0,     0,     0,     0,     0,   342,
     215,     0,     0,     0,     0,    85,    92,    93,    94,    85,
      85,    95,     0,     0,    85,    46,     0,     0,     0,    43,
       0,     0,   152,   160,     0,     0,   282,   284,     0,   281,
     106,   107,   204,     0,   164,     0,     0,    81,   251,   337,
     346,   191,   190,   349,   352,   348,   354,   350,   351,     0,
       0,   101,    99,     0,     0,     0,     0,     0,   100,    82,
      84,    83,   162,     0,     0,   140,   141,     0,   183,     0,
       0,     0,     0,     0,   159,     0,     0,     0,     0,   287,
       0,     0,     0,   109,     0,     0,     0,     0,     0,     0,
       0,    72,     0,    78,    85,     0,   353,   355,   200,     0,
       0,     0,     0,     0,     0,   167,     0,     0,   138,   139,
       0,     0,   153,     0,     0,     0,     0,     0,     0,   114,
       0,     0,   125,   128,   126,   127,     0,     0,     0,     0,
       0,     0,    46,     0,     0,     0,   206,     0,    85,     0,
      91,    96,    97,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,   288,   289,   290,   285,
     286,   132,     0,     0,     0,     0,     0,   110,   111,   102,
     108,     0,     0,   121,     0,     0,     0,     0,     0,     0,
       0,     0,    46,    80,     0,   169,   170,   171,   172,   168,
     166,     0,     0,     0,     0,   291,     0,     0,     0,     0,
       0,     0,   104,     0,   123,     0,     0,     0,     0,   105,
       0,   112,   113,     0,     0,   208,   210,     0,    79,   181,
     182,   154,   155,     0,   124,   131,   129,   130,     0,     0,
       0,   143,   145,   146,     0,     0,     0,     0,   117,     0,
       0,     0,     0,    46,     0,     0,   209,     0,     0,     0,
       0,     0,   144,   147,   134,   135,   136,   137,   133,   118,
     122,     0,     0,     0,     0,   103,     0,     0,     0,     0,
       0,   120,   115,   116,     0,   211,     0,     0,     0,     0,
       0,   149,   148,   150,   151,   119
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -603,  -603,  -603,   560,  -603,  -603,   902,  -603,   843,   494,
    -603,   828,  -603,  -603,   868,  -603,  -603,  -603,  -603,  -603,
    -154,   450,  -264,  -196,   975,   881,   472,  -301,  -603,  -603,
      42,  -603,  -159,  -603,  -164,  -603,  -603,   479,  -603,  -603,
    -603,  -519,  -417,  -603,  -603,  -603,  -603,  -476,  -603,  -603,
    -603,  -560,  -603,   340,  -603,  -603,   227,   408,  -603,  -346,
    -603,   453,   176,    39,  -603,  -412,  -409,  -603,  -603,  -406,
    -405,  -511,  -175,  -546,  -153,  -371,  -603,  -603,  -374,  -364,
     180,   294,  -603,  -603,   -60,   779,  -112,  -603,   752,   -88,
    -603,  -355,  -603,   496,  -603,   907,  -603,  -603,  -603,   955,
    -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -603,  -602,
    -603,  -558,   322,  -603,  -603,  -603,  -603,   328,  -603,  -603,
    -603,  -603,  -603,  -603,  -603,  -287,  -603,   386,   385,   358,
    -603,   976,  -603,   978
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,    35,    23,    84,   330,    36,    85,    37,
      89,   107,   108,   180,   246,   501,   110,   186,   357,   232,
     228,   427,   229,   102,    25,   103,   111,   288,   275,   361,
     323,   104,   224,   190,   225,   112,   311,   411,   445,   653,
     446,   564,   469,   565,   566,   659,   470,   589,   687,   590,
     733,   734,   799,   681,   682,   756,   683,   477,   576,   790,
     791,   430,   487,   633,   533,   567,   568,   447,   448,   569,
     570,   571,   634,   629,   293,   294,   458,   247,   389,   492,
     650,   493,   390,   200,   346,   214,   391,   466,   284,   523,
     249,   196,   114,   449,    38,    39,    40,   337,    41,    42,
      43,    70,    44,    45,    46,    47,   351,    48,   488,   586,
     678,   587,   167,   147,    49,    50,     7,   338,   339,   340,
     378,   341,   342,   343,   418,   604,   513,   605,   607,     8,
       9,    10,    11,    12
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      92,   206,   195,   163,   419,   210,   211,   212,   409,   478,
     289,   312,   471,    55,   471,   472,    17,   472,   473,   474,
     473,   474,     1,   362,   363,   234,   591,   289,   272,   273,
     233,   812,   630,    93,    13,   502,    95,   199,   655,   235,
     202,   203,   688,   207,   208,   209,   619,     2,   464,   215,
     620,   621,   545,    31,    97,   623,    26,    18,   262,    21,
     454,   813,    20,   266,   267,   268,   269,   270,    26,   475,
      22,   475,   292,   289,   399,   652,   720,   674,   279,   359,
     194,   627,     2,    99,   191,   360,   735,   710,   178,   230,
     201,    24,   518,   222,   257,   194,   259,   260,   261,   290,
     263,    91,   215,   435,   265,   652,   248,   503,    91,    32,
      33,    14,    34,   314,   -30,   714,   290,   332,   313,   291,
     204,   205,   192,    56,   283,   283,   546,   286,   287,   541,
     428,   459,     3,     4,    14,   697,   291,   204,   205,   455,
     254,   333,   334,    94,   736,   737,    96,   353,   703,   301,
     302,   364,   365,   366,   367,   577,   578,   352,   628,   354,
     355,   291,   290,   628,    98,   475,   573,     3,     4,   456,
     204,   205,   317,   317,   319,   290,   591,   591,   412,   744,
     788,    18,   291,   204,   205,   204,   205,   609,   213,   400,
     401,   402,   403,   100,    53,   291,   204,   205,   179,   231,
     549,   223,   -30,   461,   227,   289,   553,   554,    51,   490,
     475,   730,   731,   673,   475,   475,   739,   321,   397,   475,
     398,   -30,   555,   556,   557,   558,   188,   525,   -46,   684,
     684,   189,   685,   685,    54,   489,   392,   393,   598,   820,
     821,   601,   660,   661,   662,   663,   664,   701,    71,   709,
     702,   526,   527,   415,   471,   822,   823,   472,   405,   406,
     473,   474,   591,   484,   223,   457,   428,    34,   603,   789,
     606,   608,    15,    16,   840,   690,   691,   420,   602,   485,
      77,   486,   752,    90,    81,    91,   475,   475,    82,   291,
     204,   205,    83,   635,   290,    86,   105,   805,   106,   475,
     423,   636,   637,   638,   475,   322,   322,   101,   322,   462,
     465,   475,   410,   519,   291,   204,   205,   763,   113,   606,
     441,   421,   535,   322,    24,    24,   322,    24,   115,   322,
     416,   417,   422,   237,   238,   151,   239,   240,   407,   117,
     408,   542,    24,   475,   786,    24,   241,   787,    24,   515,
     118,   757,   758,   759,   760,   761,   457,   457,   457,   457,
     120,   242,   243,   244,   245,   536,   322,    19,   148,   537,
      59,   164,   496,    27,    28,   764,   165,   541,   151,   491,
     494,   497,   765,   166,   192,    24,   793,   793,   793,   793,
     793,   168,   372,   595,   170,   801,   802,   172,   635,   766,
     767,   475,   457,   457,   457,   457,   636,   637,   638,   639,
     574,   640,   794,   795,   796,   797,   498,   174,   169,   499,
     171,   624,   173,   500,     3,     4,   175,   373,   374,   375,
     538,   376,   177,   410,   377,   596,   181,   793,   793,   793,
     793,   184,   185,   654,   481,   482,   106,   483,   194,   610,
     183,   457,   109,   457,   -34,   -34,   793,   793,   793,   793,
     187,   484,   827,   828,   829,   830,   641,   642,   562,   643,
     563,   -85,   151,   204,   205,   644,   645,   485,   197,   486,
      68,   836,   837,   838,   839,   704,   705,   706,   707,   708,
      91,   253,   711,   646,   647,   615,   616,   617,   194,   316,
     648,    58,   698,   198,   149,    91,   150,   520,   216,   151,
     151,   217,   521,   522,   218,   704,   705,   194,   318,   413,
     414,   191,   443,   444,   675,   676,   677,    61,    62,    63,
      64,    65,   219,   689,   220,   751,   388,   463,   738,   388,
     517,   221,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    88,   161,   226,   -24,   -24,   227,   162,   250,   192,
     251,   695,   408,   540,   -24,   -24,   -24,   -24,   -24,   -24,
     -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,   -24,
     -24,   -24,   -24,   -24,   -24,   -24,   429,   582,   777,   236,
     237,   238,   151,   239,   240,   804,    72,    73,    74,    75,
      76,   324,   325,   241,   792,   792,   792,   792,   792,    79,
      80,   612,   443,   597,   151,   252,   521,   522,   242,   243,
     244,   245,   476,   625,   255,   191,   699,   700,   256,   402,
     585,   719,   258,   740,   494,   613,   614,   615,   616,   617,
     271,   192,   588,   729,   768,   408,   776,   783,   784,   824,
     732,   819,   274,   800,   276,   792,   792,   792,   792,   -24,
     277,   -24,   278,   192,   379,   380,   282,   382,   383,   384,
     385,   386,   121,   122,   792,   792,   792,   792,   295,   774,
     296,   410,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   439,   280,   281,   580,   581,   712,
     713,   297,   721,   149,   298,   440,   299,   521,   151,   300,
     335,   693,   303,   304,   305,   331,   191,   306,   307,   308,
     149,   309,   694,   310,   315,   151,   722,   723,   724,   725,
     726,   324,   325,   347,   320,   344,   326,   327,   328,   329,
     345,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     348,   161,   350,   358,   192,   356,   162,   369,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   145,   161,   146,
     521,   349,   -17,   162,   368,   521,   370,   505,   371,   191,
     381,   336,   387,   388,   191,   394,   396,   395,   404,   613,
     614,   615,   616,   617,   722,   723,   724,   725,   726,   506,
     429,   424,   425,   426,    57,    58,   507,   508,    59,   431,
     432,   509,   510,   511,   512,   433,   434,   192,   436,   437,
     336,   336,   192,   336,   336,   336,   336,   336,   438,   442,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
     324,   325,   450,   453,   460,   326,   327,   770,   329,   324,
     325,   467,   451,   452,   808,   809,   810,   811,   468,   476,
     480,   479,   408,   495,   530,   504,   514,   516,   524,   528,
     529,   531,   532,   534,   539,   543,   547,   551,   559,   552,
     550,   560,   561,   572,   575,   584,   585,   588,   499,   592,
     593,   594,   599,   600,   611,   618,   563,   667,   622,   631,
     632,   649,   670,   651,   658,   665,   666,   668,   669,   671,
     672,   679,   680,   692,   752,   696,   652,   116,   715,   716,
     717,   718,   727,   728,   732,   742,   182,   743,   745,   746,
     747,   748,   749,   750,   753,   754,   755,   762,   769,   771,
     772,   773,   775,   778,   779,   780,   781,   782,   798,   803,
      87,   806,   807,   814,   815,   816,   817,   818,   825,   831,
     832,   833,   834,   835,   841,   842,   843,   844,   845,   548,
     579,   193,   686,   785,   626,   176,   583,   826,   741,   285,
     656,   657,   544,   264,   119,     0,    78,    30,    29,     0,
       0,    52
};

static const yytype_int16 yycheck[] =
{
      60,   154,   114,    91,   378,   158,   159,   160,   372,   426,
       3,   275,   424,     1,   426,   424,     0,   426,   424,   424,
     426,   426,     3,   324,   325,   189,   537,     3,   224,   225,
     189,    81,   578,     1,    11,     4,     1,   149,   596,   193,
     152,   153,   644,   155,   156,   157,   565,    28,   422,   161,
     569,   570,     3,    12,     1,   574,    14,     3,   211,    93,
       3,   111,     6,   217,   218,   219,   220,   221,    26,   424,
       3,   426,   247,     3,     4,     3,   678,   635,   232,   105,
       3,     3,    28,     1,    73,   111,     3,    80,     1,     1,
     150,   111,   466,     3,   206,     3,   208,   209,   210,    92,
     212,     3,   214,   404,   216,     3,   194,    76,     3,    68,
       1,   111,     3,   277,     4,   673,    92,   292,   277,   112,
     113,   114,   111,   111,   236,   237,   497,   239,   240,   493,
     394,   418,   113,   114,   111,   654,   112,   113,   114,    82,
     200,   295,   296,   111,   690,   691,   111,   311,   659,   261,
     262,   326,   327,   328,   329,   526,   527,     4,    80,   313,
     314,   112,    92,    80,   111,   520,   521,   113,   114,   112,
     113,   114,   284,   285,   286,    92,   687,   688,   374,   698,
       3,     3,   112,   113,   114,   113,   114,   561,   111,   364,
     365,   366,   367,   111,     7,   112,   113,   114,   111,   111,
     501,   111,    92,   111,    92,     3,   507,   508,     4,   111,
     565,   687,   688,   111,   569,   570,   111,     4,     4,   574,
       4,   111,   509,   510,   511,   512,   111,    59,   116,   641,
     642,   116,   641,   642,   111,     4,   348,   349,     4,   799,
     800,     4,   613,   614,   615,   616,   617,   659,     4,   666,
     659,    83,    84,     1,   666,   801,   802,   666,   370,   371,
     666,   666,   773,    73,   111,   418,   530,     3,   555,    92,
     557,   558,   113,   114,   834,   646,   647,     4,     4,    89,
       3,    91,    80,     1,     4,     3,   641,   642,     4,   112,
     113,   114,   111,    48,    92,   111,     1,   773,     3,   654,
     388,    56,    57,    58,   659,    92,    92,   111,    92,   421,
     422,   666,   372,   467,   112,   113,   114,     1,     3,   606,
     408,    48,     4,    92,   111,   111,    92,   111,     3,    92,
      78,    79,    59,    60,    61,    62,    63,    64,     1,     4,
       3,   495,   111,   698,   756,   111,    73,   756,   111,   461,
       3,   722,   723,   724,   725,   726,   509,   510,   511,   512,
       4,    88,    89,    90,    91,    47,    92,     9,     4,    51,
      13,     4,    64,    15,    16,    59,     4,   741,    62,   439,
     440,    73,    66,    45,   111,   111,   757,   758,   759,   760,
     761,     4,    35,   546,     4,   766,   767,     4,    48,    83,
      84,   756,   555,   556,   557,   558,    56,    57,    58,    59,
     522,     1,   758,   759,   760,   761,   108,     4,    96,   111,
      98,   575,   100,   115,   113,   114,   116,    70,    71,    72,
     490,    74,     4,   493,    77,   547,     4,   808,   809,   810,
     811,     3,     4,   596,    56,    57,     3,    59,     3,   561,
       4,   604,     1,   606,     3,     4,   827,   828,   829,   830,
       4,    73,   808,   809,   810,   811,    56,    57,     1,    59,
       3,     4,    62,   113,   114,    65,    66,    89,    69,    91,
      43,   827,   828,   829,   830,   660,   661,   662,   663,   664,
       3,     4,   667,    83,    84,    85,    86,    87,     3,     4,
     588,    10,   655,     4,    57,     3,    59,    59,   111,    62,
      62,     3,    64,    65,     3,   690,   691,     3,     4,    75,
      76,    73,     3,     4,   636,   637,   638,    36,    37,    38,
      39,    40,     3,   645,     3,   710,     3,     4,   692,     3,
       4,     3,    95,    96,    97,    98,    99,   100,   101,   102,
     103,     1,   105,     4,     4,     5,    92,   110,     4,   111,
     111,   649,     3,     4,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,     3,     4,   742,    59,
      60,    61,    62,    63,    64,   770,    36,    37,    38,    39,
      40,   111,   112,    73,   757,   758,   759,   760,   761,    49,
      50,    59,     3,     4,    62,     4,    64,    65,    88,    89,
      90,    91,     3,     4,     4,    73,     3,     4,     4,   804,
       3,     4,     4,   693,   694,    83,    84,    85,    86,    87,
      67,   111,     3,     4,   732,     3,     4,     3,     4,   803,
       3,     4,   111,   765,     4,   808,   809,   810,   811,   109,
     116,   111,     4,   111,   336,   337,     4,   339,   340,   341,
     342,   343,     4,     5,   827,   828,   829,   830,     3,   739,
       3,   741,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    48,   233,   234,   531,   532,   670,
     671,     4,    59,    57,     4,    59,     4,    64,    62,     4,
       3,    48,     4,     4,     4,   111,    73,     4,     4,     4,
      57,     4,    59,     4,     4,    62,    83,    84,    85,    86,
      87,   111,   112,    46,     4,     4,   116,   117,   118,   119,
       4,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      46,   105,   107,     4,   111,   116,   110,     4,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   109,   105,   111,
      64,    46,     8,   110,     8,    64,     4,    82,     4,    73,
       4,   297,     4,     3,    73,     3,    94,    55,   112,    83,
      84,    85,    86,    87,    83,    84,    85,    86,    87,   104,
       3,    51,    51,    51,     9,    10,   111,   112,    13,     4,
       4,   116,   117,   118,   119,     4,     4,   111,     4,     4,
     336,   337,   111,   339,   340,   341,   342,   343,     4,     4,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
     111,   112,     4,     4,     4,   116,   117,   118,   119,   111,
     112,     3,   113,   113,   116,   117,   118,   119,     3,     3,
     116,     4,     3,     3,    94,     4,     4,     4,     4,     4,
       4,     3,     3,    92,     4,     3,   111,     4,     4,   111,
     113,     4,     4,     3,     3,    81,     3,     3,   111,     4,
       4,     4,     4,     4,     4,     4,     3,   118,     4,    92,
      92,     3,    81,     4,     4,     4,     4,     4,     4,    81,
       4,     4,     3,     3,    80,     4,     3,    84,     4,     4,
       4,     4,     4,     4,     3,     3,   108,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     3,
      58,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,   500,
     530,   113,   642,   756,   576,   104,   533,   807,   694,   237,
     604,   606,   496,   214,    87,    -1,    41,    21,    20,    -1,
      -1,    26
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    28,   113,   114,   122,   123,   237,   250,   251,
     252,   253,   254,    11,   111,   113,   114,     0,     3,   250,
       6,    93,     3,   125,   111,   145,   151,   250,   250,   254,
     252,    12,    68,     1,     3,   124,   128,   130,   215,   216,
     217,   219,   220,   221,   223,   224,   225,   226,   228,   235,
     236,     4,   145,     7,   111,     1,   111,     9,    10,    13,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
     222,     4,   124,   124,   124,   124,   124,     3,   220,   124,
     124,     4,     4,   111,   126,   129,   111,   127,     1,   131,
       1,     3,   205,     1,   111,     1,   111,     1,   111,     1,
     111,   111,   144,   146,   152,     1,     3,   132,   133,     1,
     137,   147,   156,     3,   213,     3,   129,     4,     3,   216,
       4,     4,     5,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,   109,   111,   234,     4,    57,
      59,    62,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   105,   110,   210,     4,     4,    45,   233,     4,   233,
       4,   233,     4,   233,     4,   116,   146,     4,     1,   111,
     134,     4,   132,     4,     3,     4,   138,     4,   111,   116,
     154,    73,   111,   135,     3,   207,   212,    69,     4,   207,
     204,   205,   207,   207,   113,   114,   195,   207,   207,   207,
     195,   195,   195,   111,   206,   207,   111,     3,     3,     3,
       3,     3,     3,   111,   153,   155,     4,    92,   141,   143,
       1,   111,   140,   153,   155,   141,    59,    60,    61,    63,
      64,    73,    88,    89,    90,    91,   135,   198,   210,   211,
       4,   111,     4,     4,   205,     4,     4,   207,     4,   207,
     207,   207,   195,   207,   206,   207,   141,   141,   141,   141,
     141,    67,   144,   144,   111,   149,     4,   116,     4,   141,
     147,   147,     4,   207,   209,   209,   207,   207,   148,     3,
      92,   112,   193,   195,   196,     3,     3,     4,     4,     4,
       4,   207,   207,     4,     4,     4,     4,     4,     4,     4,
       4,   157,   143,   153,   155,     4,     4,   207,     4,   207,
       4,     4,    92,   151,   111,   112,   116,   117,   118,   119,
     127,   111,   193,   141,   141,     3,   130,   218,   238,   239,
     240,   242,   243,   244,     4,     4,   205,    46,    46,    46,
     107,   227,     4,   155,   141,   141,   116,   139,     4,   105,
     111,   150,   148,   148,   193,   193,   193,   193,     8,     4,
       4,     4,    35,    70,    71,    72,    74,    77,   241,   238,
     238,     4,   238,   238,   238,   238,   238,     4,     3,   199,
     203,   207,   207,   207,     3,    55,    94,     4,     4,     4,
     193,   193,   193,   193,   112,   207,   207,     1,     3,   200,
     205,   158,   144,    75,    76,     1,    78,    79,   245,   199,
       4,    48,    59,   210,    51,    51,    51,   142,   143,     3,
     182,     4,     4,     4,     4,   148,     4,     4,     4,    48,
      59,   210,     4,     3,     4,   159,   161,   188,   189,   214,
       4,   113,   113,     4,     3,    82,   112,   195,   197,   246,
       4,   111,   207,     4,   199,   207,   208,     3,     3,   163,
     167,   186,   187,   190,   191,   212,     3,   178,   163,     4,
     116,    56,    57,    59,    73,    89,    91,   183,   229,     4,
     111,   205,   200,   202,   205,     3,    64,    73,   108,   111,
     115,   136,     4,    76,     4,    82,   104,   111,   112,   116,
     117,   118,   119,   247,     4,   207,     4,     4,   199,   141,
      59,    64,    65,   210,     4,    59,    83,    84,     4,     4,
      94,     3,     3,   185,    92,     4,    47,    51,   205,     4,
       4,   200,   141,     3,   214,     3,   196,   111,   158,   148,
     113,     4,   111,   148,   148,   246,   246,   246,   246,     4,
       4,     4,     1,     3,   162,   164,   165,   186,   187,   190,
     191,   192,     3,   212,   207,     3,   179,   196,   196,   142,
     183,   183,     4,   182,    81,     3,   230,   232,     3,   168,
     170,   192,     4,     4,     4,   195,   207,     4,     4,     4,
       4,     4,     4,   246,   246,   248,   246,   249,   246,   199,
     207,     4,    59,    83,    84,    85,    86,    87,     4,   162,
     162,   162,     4,   162,   141,     4,   178,     3,    80,   194,
     194,    92,    92,   184,   193,    48,    56,    57,    58,    59,
       1,    56,    57,    59,    65,    66,    83,    84,   210,     3,
     201,     4,     3,   160,   195,   232,   248,   249,     4,   166,
     196,   196,   196,   196,   196,     4,     4,   118,     4,     4,
      81,    81,     4,   111,   232,   207,   207,   207,   231,     4,
       3,   174,   175,   177,   186,   187,   174,   169,   230,   207,
     196,   196,     3,    48,    59,   210,     4,   162,   195,     3,
       4,   186,   187,   192,   193,   193,   193,   193,   193,   163,
      80,   193,   184,   184,   232,     4,     4,     4,     4,     4,
     230,    59,    83,    84,    85,    86,    87,     4,     4,     4,
     168,   168,     3,   171,   172,     3,   194,   194,   141,   111,
     205,   202,     3,     4,   162,     4,     4,     4,     4,     4,
       4,   193,    80,     4,     4,     4,   176,   196,   196,   196,
     196,   196,     4,     1,    59,    66,    83,    84,   210,     4,
     118,     4,     4,     4,   205,     4,     4,   141,     4,     4,
       4,     4,     4,     3,     4,   177,   186,   187,     3,    92,
     180,   181,   195,   196,   180,   180,   180,   180,     4,   173,
     207,   196,   196,     3,   193,   168,     4,     4,   116,   117,
     118,   119,    81,   111,     4,     4,     4,     4,     4,     4,
     172,   172,   194,   194,   141,     4,   201,   180,   180,   180,
     180,     4,     4,     4,     4,     4,   180,   180,   180,   180,
     172,     4,     4,     4,     4,     4
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   121,   122,   122,   122,   123,   123,   124,   124,   124,
     124,   124,   124,   124,   124,   125,   126,   127,   128,   129,
     129,   130,   130,   131,   131,   132,   132,   133,   133,   134,
     135,   135,   136,   137,   137,   138,   138,   139,   139,   140,
     141,   141,   141,   142,   142,   143,   143,   144,   144,   144,
     145,   145,   146,   146,   147,   147,   147,   148,   148,   148,
     149,   150,   150,   151,   152,   153,   154,   155,   156,   156,
     157,   157,   158,   158,   158,   158,   158,   158,   159,   160,
     160,   161,   162,   162,   162,   162,   163,   163,   163,   163,
     163,   164,   164,   165,   165,   165,   166,   166,   166,   166,
     167,   167,   168,   168,   168,   168,   168,   168,   169,   169,
     170,   170,   170,   170,   170,   171,   171,   171,   172,   172,
     172,   172,   173,   173,   174,   174,   175,   175,   175,   176,
     176,   176,   176,   177,   177,   177,   177,   177,   178,   178,
     178,   179,   179,   180,   180,   180,   180,   180,   181,   181,
     181,   181,   182,   182,   182,   182,   183,   183,   183,   184,
     185,   185,   186,   187,   188,   189,   190,   191,   192,   192,
     192,   192,   192,   193,   193,   193,   193,   193,   193,   193,
     193,   194,   194,   194,   195,   195,   196,   196,   196,   196,
     197,   197,   197,   198,   198,   198,   198,   198,   199,   199,
     199,   199,   199,   200,   200,   200,   200,   200,   201,   201,
     201,   201,   202,   202,   203,   203,   203,   204,   204,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   206,   206,   207,   207,   207,   207,   207,
     207,   207,   207,   208,   208,   209,   209,   210,   211,   212,
     213,   214,   215,   215,   216,   216,   217,   217,   218,   218,
     219,   219,   220,   220,   220,   220,   220,   220,   221,   222,
     223,   224,   224,   225,   225,   226,   226,   227,   227,   228,
     228,   229,   229,   229,   230,   230,   231,   231,   232,   232,
     232,   232,   232,   233,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   235,
     236,   237,   237,   238,   238,   238,   238,   238,   238,   238,
     238,   239,   240,   241,   242,   243,   243,   244,   244,   244,
     245,   245,   246,   246,   246,   246,   246,   246,   247,   247,
     247,   247,   248,   248,   249,   249,   250,   250,   250,   250,
     251,   251,   252,   252,   253,   254,   254
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     5,     4,     2,     2,     2,
       2,     2,     2,     2,     1,     4,     1,     1,     4,     2,
       0,     4,     4,     2,     0,     2,     1,     4,     3,     1,
       1,     1,     1,     2,     0,     5,     3,     2,     0,     1,
       4,     4,     1,     4,     0,     3,     0,     4,     4,     1,
       2,     0,     2,     0,     4,     4,     1,     2,     3,     0,
       1,     1,     1,     1,     1,     4,     1,     1,     2,     0,
       2,     0,     6,     2,     2,     2,     2,     0,     5,     4,
       3,     4,     2,     2,     2,     0,     1,     1,     1,     1,
       1,     4,     1,     1,     1,     1,     2,     2,     2,     0,
       4,     4,     4,     7,     5,     5,     1,     1,     2,     0,
       4,     4,     5,     5,     3,     5,     5,     3,     4,     7,
       5,     1,     2,     0,     4,     1,     1,     1,     1,     2,
       2,     2,     0,     5,     5,     5,     5,     5,     5,     5,
       4,     2,     0,     1,     2,     1,     1,     2,     5,     5,
       5,     5,     4,     6,     9,     9,     1,     1,     1,     1,
       2,     0,     4,     1,     4,     1,     7,     5,     5,     5,
       5,     5,     5,     4,     5,     5,     5,     5,     1,     2,
       1,     5,     5,     1,     1,     1,     4,     4,     1,     6,
       4,     4,     1,     1,     1,     1,     1,     1,     1,     4,
       7,     3,     2,     4,     5,     4,     7,     1,     4,     5,
       4,     7,     2,     1,     4,     5,     1,     2,     1,     4,
       7,     4,     4,     4,     5,     4,     5,     5,     6,     6,
       5,     5,     5,     1,     1,     1,     4,     4,     4,     5,
       7,     7,     5,     2,     1,     2,     1,     1,     1,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       2,     1,     1,     1,     1,     1,     1,     1,     5,     1,
       5,    12,     4,    12,     4,    12,     4,     4,     0,    12,
       4,     3,     3,     0,     1,     4,     2,     0,     4,     4,
       4,     5,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       4,    12,     5,     2,     2,     2,     2,     2,     2,     2,
       0,     4,     4,     1,     4,     5,     4,     7,     5,     5,
       1,     1,     3,     1,     1,     1,     4,     3,     3,     3,
       3,     3,     1,     2,     1,     2,     2,     3,     3,     0,
       3,     1,     4,     1,     4,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

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
#line 255 "pddl+.y" /* yacc.c:1646  */
    {top_thing= (yyvsp[0].t_domain); current_analysis->the_domain= (yyvsp[0].t_domain);}
#line 2113 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 256 "pddl+.y" /* yacc.c:1646  */
    {top_thing= (yyvsp[0].t_problem); current_analysis->the_problem= (yyvsp[0].t_problem);}
#line 2119 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 257 "pddl+.y" /* yacc.c:1646  */
    {top_thing = (yyvsp[0].t_plan);}
#line 2125 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 263 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_domain)= (yyvsp[-1].t_domain); (yyval.t_domain)->name= (yyvsp[-2].cp);delete [] (yyvsp[-2].cp);
	if (types_used && !types_defined) {
		yyerrok; log_error(E_FATAL,"Syntax error in domain - no :types section, but types used in definitions.");
	}
	}
#line 2135 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 270 "pddl+.y" /* yacc.c:1646  */
    {yyerrok; (yyval.t_domain)=static_cast<domain*>(NULL);
       	log_error(E_FATAL,"Syntax error in domain"); }
#line 2142 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 276 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_domain)= (yyvsp[0].t_domain); (yyval.t_domain)->req= (yyvsp[-1].t_pddl_req_flag);}
#line 2148 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 277 "pddl+.y" /* yacc.c:1646  */
    {types_defined = true; (yyval.t_domain)= (yyvsp[0].t_domain); (yyval.t_domain)->types= (yyvsp[-1].t_type_list);}
#line 2154 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 278 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_domain)= (yyvsp[0].t_domain); (yyval.t_domain)->constants= (yyvsp[-1].t_const_symbol_list);}
#line 2160 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 279 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_domain)= (yyvsp[0].t_domain);
                                       (yyval.t_domain)->predicates= (yyvsp[-1].t_pred_decl_list); }
#line 2167 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 281 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_domain)= (yyvsp[0].t_domain);
                                       (yyval.t_domain)->functions= (yyvsp[-1].t_func_decl_list); }
#line 2174 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 283 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_domain)= (yyvsp[0].t_domain);
   				       (yyval.t_domain)->constraints = (yyvsp[-1].t_con_goal);}
#line 2181 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 285 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_domain) = (yyvsp[0].t_domain);}
#line 2187 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 286 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_domain)= new domain((yyvsp[0].t_structure_store)); }
#line 2193 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 289 "pddl+.y" /* yacc.c:1646  */
    {(yyval.cp)=(yyvsp[-1].cp);}
#line 2199 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 293 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_class)=current_analysis->classes_tab.new_symbol_put((yyvsp[0].cp));
       delete [] (yyvsp[0].cp); }
#line 2206 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 296 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_class) = current_analysis->classes_tab.symbol_get((yyvsp[0].cp)); delete [] (yyvsp[0].cp);}
#line 2212 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 298 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_dummy) = 0;}
#line 2218 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 300 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_dummy) = 0;}
#line 2224 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 301 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_dummy) = 0;}
#line 2230 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 306 "pddl+.y" /* yacc.c:1646  */
    {
	// Stash in analysis object --- we need to refer to it during parse
	//   but domain object is not created yet,
	current_analysis->req |= (yyvsp[-1].t_pddl_req_flag);
	(yyval.t_pddl_req_flag)=(yyvsp[-1].t_pddl_req_flag);
    }
#line 2241 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 313 "pddl+.y" /* yacc.c:1646  */
    {yyerrok;
       log_error(E_FATAL,"Syntax error in requirements declaration.");
       (yyval.t_pddl_req_flag)= 0; }
#line 2249 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 319 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_pddl_req_flag)= (yyvsp[-1].t_pddl_req_flag) | (yyvsp[0].t_pddl_req_flag); }
#line 2255 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 320 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_pddl_req_flag)= 0; }
#line 2261 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 326 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pred_decl_list)=(yyvsp[0].t_pred_decl_list); (yyval.t_pred_decl_list)->push_front((yyvsp[-1].t_pred_decl));}
#line 2267 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 328 "pddl+.y" /* yacc.c:1646  */
    {  (yyval.t_pred_decl_list)=new pred_decl_list;
           (yyval.t_pred_decl_list)->push_front((yyvsp[0].t_pred_decl)); }
#line 2274 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 333 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pred_decl)= new pred_decl((yyvsp[-2].t_pred_symbol),(yyvsp[-1].t_var_symbol_list),current_analysis->var_tab_stack.pop());}
#line 2280 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 335 "pddl+.y" /* yacc.c:1646  */
    {yyerrok;
        // hope someone makes this error someday
        log_error(E_FATAL,"Syntax error in predicate declaration.");
	(yyval.t_pred_decl)= static_cast<pred_decl*>(NULL); }
#line 2289 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 343 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_pred_symbol)=current_analysis->pred_tab.new_symbol_put((yyvsp[0].cp));
           current_analysis->var_tab_stack.push(
           				current_analysis->buildPredTab());
           delete [] (yyvsp[0].cp); }
#line 2298 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 350 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_pred_symbol)=current_analysis->pred_tab.symbol_ref("=");
	      requires(E_EQUALITY); }
#line 2305 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 352 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_pred_symbol)=current_analysis->pred_tab.symbol_get((yyvsp[0].cp)); delete [] (yyvsp[0].cp); }
#line 2311 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 360 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_pred_symbol)=current_analysis->pred_tab.symbol_get((yyvsp[0].cp)); delete [] (yyvsp[0].cp);}
#line 2317 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 366 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_func_decl_list)=(yyvsp[-1].t_func_decl_list); (yyval.t_func_decl_list)->push_back((yyvsp[0].t_func_decl));}
#line 2323 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 367 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_func_decl_list)=new func_decl_list; }
#line 2329 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 372 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_func_decl)= new func_decl((yyvsp[-3].t_func_symbol),(yyvsp[-2].t_var_symbol_list),current_analysis->var_tab_stack.pop());}
#line 2335 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 374 "pddl+.y" /* yacc.c:1646  */
    {yyerrok;
	 log_error(E_FATAL,"Syntax error in functor declaration.");
	 (yyval.t_func_decl)= NULL; }
#line 2343 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 380 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_dummy) = (int) NULL;}
#line 2349 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 380 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_dummy)= (int) NULL;}
#line 2355 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 384 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_func_symbol)=current_analysis->func_tab.new_symbol_put((yyvsp[0].cp));
           current_analysis->var_tab_stack.push(
           		current_analysis->buildFuncTab());
           delete [] (yyvsp[0].cp); }
#line 2364 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 397 "pddl+.y" /* yacc.c:1646  */
    {
      (yyval.t_var_symbol_list)= (yyvsp[-3].t_var_symbol_list);
      (yyval.t_var_symbol_list)->set_types((yyvsp[-1].t_type));           /* Set types for variables */
      (yyval.t_var_symbol_list)->splice((yyval.t_var_symbol_list)->end(),*(yyvsp[0].t_var_symbol_list));   /* Join lists */
      delete (yyvsp[0].t_var_symbol_list);                   /* Delete (now empty) list */
      requires(E_TYPING);
      types_used = true;
   }
#line 2377 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 406 "pddl+.y" /* yacc.c:1646  */
    {
      (yyval.t_var_symbol_list)= (yyvsp[-3].t_var_symbol_list);
      (yyval.t_var_symbol_list)->set_either_types((yyvsp[-1].t_type_list));    /* Set types for variables */
      (yyval.t_var_symbol_list)->splice((yyval.t_var_symbol_list)->end(),*(yyvsp[0].t_var_symbol_list));   /* Join lists */
      delete (yyvsp[0].t_var_symbol_list);                   /* Delete (now empty) list */
      requires(E_TYPING);
      types_used = true;
   }
#line 2390 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 415 "pddl+.y" /* yacc.c:1646  */
    {
       (yyval.t_var_symbol_list)= (yyvsp[0].t_var_symbol_list);
   }
#line 2398 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 422 "pddl+.y" /* yacc.c:1646  */
    {
      (yyval.t_var_symbol_list)= (yyvsp[-3].t_var_symbol_list);
      (yyval.t_var_symbol_list)->set_types(0);           /* Set types for variables */
      (yyval.t_var_symbol_list)->splice((yyval.t_var_symbol_list)->end(),*(yyvsp[0].t_var_symbol_list));   /* Join lists */
      delete (yyvsp[0].t_var_symbol_list);                   /* Delete (now empty) list */
   }
#line 2409 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 427 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_var_symbol_list) = new var_symbol_list;}
#line 2415 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 435 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_var_symbol_list)=(yyvsp[0].t_var_symbol_list); (yyvsp[0].t_var_symbol_list)->push_front((yyvsp[-1].t_var_symbol)); }
#line 2421 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 436 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_var_symbol_list)= new var_symbol_list; }
#line 2427 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 443 "pddl+.y" /* yacc.c:1646  */
    {
      (yyval.t_const_symbol_list)= (yyvsp[-3].t_const_symbol_list);
      (yyvsp[-3].t_const_symbol_list)->set_types((yyvsp[-1].t_type));           /* Set types for constants */
      (yyvsp[-3].t_const_symbol_list)->splice((yyvsp[-3].t_const_symbol_list)->end(),*(yyvsp[0].t_const_symbol_list)); /* Join lists */
      delete (yyvsp[0].t_const_symbol_list);                   /* Delete (now empty) list */
      requires(E_TYPING);
      types_used = true;
   }
#line 2440 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 452 "pddl+.y" /* yacc.c:1646  */
    {
      (yyval.t_const_symbol_list)= (yyvsp[-3].t_const_symbol_list);
      (yyvsp[-3].t_const_symbol_list)->set_either_types((yyvsp[-1].t_type_list));
      (yyvsp[-3].t_const_symbol_list)->splice((yyvsp[-3].t_const_symbol_list)->end(),*(yyvsp[0].t_const_symbol_list));
      delete (yyvsp[0].t_const_symbol_list);
      requires(E_TYPING);
      types_used = true;
   }
#line 2453 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 461 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)= (yyvsp[0].t_const_symbol_list);}
#line 2459 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 466 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)=(yyvsp[0].t_const_symbol_list); (yyvsp[0].t_const_symbol_list)->push_front((yyvsp[-1].t_const_symbol));}
#line 2465 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 467 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)=new const_symbol_list;}
#line 2471 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 471 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)=(yyvsp[0].t_const_symbol_list); (yyvsp[0].t_const_symbol_list)->push_front((yyvsp[-1].t_const_symbol));}
#line 2477 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 472 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)=new const_symbol_list;}
#line 2483 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 481 "pddl+.y" /* yacc.c:1646  */
    {
       (yyval.t_type_list)= (yyvsp[-3].t_type_list);
       (yyval.t_type_list)->set_types((yyvsp[-1].t_type));           /* Set types for constants */
       (yyval.t_type_list)->splice((yyval.t_type_list)->end(),*(yyvsp[0].t_type_list)); /* Join lists */
       delete (yyvsp[0].t_type_list);                   /* Delete (now empty) list */
   }
#line 2494 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 488 "pddl+.y" /* yacc.c:1646  */
    {
   // This parse needs to be excluded, we think (DPL&MF: 6/9/01)
       (yyval.t_type_list)= (yyvsp[-3].t_type_list);
       (yyval.t_type_list)->set_either_types((yyvsp[-1].t_type_list));
       (yyval.t_type_list)->splice((yyvsp[-3].t_type_list)->end(),*(yyvsp[0].t_type_list));
       delete (yyvsp[0].t_type_list);
   }
#line 2506 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 497 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_type_list)= (yyvsp[0].t_type_list); }
#line 2512 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 503 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_parameter_symbol_list)=(yyvsp[-1].t_parameter_symbol_list); (yyval.t_parameter_symbol_list)->push_back((yyvsp[0].t_const_symbol)); }
#line 2518 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 505 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_parameter_symbol_list)=(yyvsp[-2].t_parameter_symbol_list); (yyval.t_parameter_symbol_list)->push_back((yyvsp[0].t_var_symbol)); }
#line 2524 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 506 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_parameter_symbol_list)= new parameter_symbol_list;}
#line 2530 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 513 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_var_symbol)= current_analysis->var_tab_stack.top()->symbol_put((yyvsp[0].cp)); delete [] (yyvsp[0].cp); }
#line 2536 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 519 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_var_symbol)= current_analysis->var_tab_stack.symbol_get((yyvsp[0].cp)); delete [] (yyvsp[0].cp); }
#line 2542 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 520 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_var_symbol) = current_analysis->var_tab_stack.symbol_get("after");}
#line 2548 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 524 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_const_symbol)= current_analysis->const_tab.symbol_get((yyvsp[0].cp)); delete [] (yyvsp[0].cp); }
#line 2554 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 528 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_const_symbol)= current_analysis->const_tab.new_symbol_put((yyvsp[0].cp)); delete [] (yyvsp[0].cp);}
#line 2560 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 533 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_type_list)= (yyvsp[-1].t_type_list); }
#line 2566 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 538 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_type)= current_analysis->pddl_type_tab.symbol_ref((yyvsp[0].cp)); delete [] (yyvsp[0].cp);}
#line 2572 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 545 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_type)= current_analysis->pddl_type_tab.symbol_ref((yyvsp[0].cp)); delete [] (yyvsp[0].cp);}
#line 2578 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 550 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_type_list)= (yyvsp[-1].t_type_list); (yyval.t_type_list)->push_back((yyvsp[0].t_type));}
#line 2584 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 551 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_type_list)= new pddl_type_list;}
#line 2590 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 556 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_type_list)= (yyvsp[-1].t_type_list); (yyval.t_type_list)->push_back((yyvsp[0].t_type));}
#line 2596 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 557 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_type_list)= new pddl_type_list;}
#line 2602 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 562 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-5].t_effect_lists);
	  (yyval.t_effect_lists)->assign_effects.push_back(new assignment((yyvsp[-2].t_func_term),E_ASSIGN,(yyvsp[-1].t_num_expression)));
          if((yyvsp[-2].t_func_term)->getFunction()->getName()=="total-cost")
          {
          	requires(E_ACTIONCOSTS);
          	// Should also check that $5 is 0...
		  }
          else
          {
          	requires(E_NFLUENTS);
          }
	}
#line 2619 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 575 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->add_effects.push_back((yyvsp[0].t_simple_effect)); }
#line 2625 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 577 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->del_effects.push_back((yyvsp[0].t_simple_effect)); }
#line 2631 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 579 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->timed_effects.push_back((yyvsp[0].t_timed_effect)); }
#line 2637 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 581 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists) = (yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->supplied_effects.push_back((yyvsp[0].t_supplied_effect)); }
#line 2643 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 583 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists;}
#line 2649 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 588 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_supplied_effect) = (yyvsp[0].t_supplied_effect); (yyvsp[0].t_supplied_effect)->sup = (yyvsp[-1].t_goal); (yyvsp[0].t_supplied_effect)->name = current_analysis->op_tab.symbol_put((yyvsp[-2].cp));
	delete [] (yyvsp[-2].cp); requires(E_SUPPLYDEMAND);}
#line 2656 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 594 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_supplied_effect) = new supplied_effect(0,(yyvsp[-3].t_goal),(yyvsp[-2].t_num_expression)->double_value(),(yyvsp[-1].t_effect_lists)); delete (yyvsp[-2].t_num_expression);}
#line 2662 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 596 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_supplied_effect) = new supplied_effect(0,0,(yyvsp[-2].t_num_expression)->double_value(),(yyvsp[-1].t_effect_lists)); delete (yyvsp[-2].t_num_expression);}
#line 2668 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 602 "pddl+.y" /* yacc.c:1646  */
    { requires(E_TIMED_INITIAL_LITERALS);
   		(yyval.t_timed_effect)=new timed_initial_literal((yyvsp[-1].t_effect_lists),(yyvsp[-2].fval));}
#line 2675 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 607 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=(yyvsp[0].t_effect_lists); (yyval.t_effect_lists)->append_effects((yyvsp[-1].t_effect_lists)); delete (yyvsp[-1].t_effect_lists);}
#line 2681 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 608 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=(yyvsp[0].t_effect_lists); (yyval.t_effect_lists)->cond_effects.push_front((yyvsp[-1].t_cond_effect));
                                      requires(E_COND_EFFS);}
#line 2688 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 610 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=(yyvsp[0].t_effect_lists); (yyval.t_effect_lists)->forall_effects.push_front((yyvsp[-1].t_forall_effect));
                                      requires(E_COND_EFFS);}
#line 2695 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 612 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists(); }
#line 2701 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 621 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[0].t_effect_lists);}
#line 2707 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 622 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->add_effects.push_front((yyvsp[0].t_simple_effect));}
#line 2713 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 623 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->del_effects.push_front((yyvsp[0].t_simple_effect));}
#line 2719 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 624 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->cond_effects.push_front((yyvsp[0].t_cond_effect));}
#line 2725 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 625 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->forall_effects.push_front((yyvsp[0].t_forall_effect));}
#line 2731 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 629 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists);}
#line 2737 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 630 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[0].t_effect_lists);}
#line 2743 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 635 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->del_effects.push_front((yyvsp[0].t_simple_effect));}
#line 2749 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 637 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->add_effects.push_front((yyvsp[0].t_simple_effect));}
#line 2755 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 639 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->assign_effects.push_front((yyvsp[0].t_assignment));
         requires(E_NFLUENTS);}
#line 2762 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 645 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->del_effects.push_back((yyvsp[0].t_simple_effect));}
#line 2768 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 646 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->add_effects.push_back((yyvsp[0].t_simple_effect));}
#line 2774 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 647 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->assign_effects.push_back((yyvsp[0].t_assignment));
                                     requires(E_NFLUENTS); }
#line 2781 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 649 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists; }
#line 2787 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 654 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); }
#line 2793 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 656 "pddl+.y" /* yacc.c:1646  */
    {yyerrok; (yyval.t_effect_lists)=NULL;
	 log_error(E_FATAL,"Syntax error in (and ...)");
	}
#line 2801 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 664 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); }
#line 2807 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 669 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists;
          (yyval.t_effect_lists)->forall_effects.push_back(
	       new forall_effect((yyvsp[-1].t_effect_lists), (yyvsp[-3].t_var_symbol_list), current_analysis->var_tab_stack.pop()));
          requires(E_COND_EFFS);}
#line 2816 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 674 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists;
	  (yyval.t_effect_lists)->cond_effects.push_back(
	       new cond_effect((yyvsp[-2].t_goal),(yyvsp[-1].t_effect_lists)));
          requires(E_COND_EFFS); }
#line 2825 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 679 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists;
	  (yyval.t_effect_lists)->cond_assign_effects.push_back(
	       new cond_effect((yyvsp[-2].t_goal),(yyvsp[-1].t_effect_lists)));
          requires(E_COND_EFFS); }
#line 2834 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 684 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=new effect_lists;
          (yyval.t_effect_lists)->timed_effects.push_back((yyvsp[0].t_timed_effect)); }
#line 2841 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 687 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists;
	  (yyval.t_effect_lists)->assign_effects.push_front((yyvsp[0].t_assignment));
          requires(E_NFLUENTS); }
#line 2849 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 693 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); (yyvsp[-1].t_effect_lists)->append_effects((yyvsp[0].t_effect_lists)); delete (yyvsp[0].t_effect_lists); }
#line 2855 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 694 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists; }
#line 2861 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 700 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_timed_effect)=new timed_effect((yyvsp[-1].t_effect_lists),E_AT_START);}
#line 2867 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 702 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_timed_effect)=new timed_effect((yyvsp[-1].t_effect_lists),E_AT_END);}
#line 2873 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 704 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_timed_effect)=new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_timed_effect)->effs->assign_effects.push_front(
	     new assignment((yyvsp[-2].t_func_term),E_INCREASE,(yyvsp[-1].t_expression))); }
#line 2881 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 708 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_timed_effect)=new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_timed_effect)->effs->assign_effects.push_front(
	     new assignment((yyvsp[-2].t_func_term),E_DECREASE,(yyvsp[-1].t_expression))); }
#line 2889 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 712 "pddl+.y" /* yacc.c:1646  */
    {yyerrok; (yyval.t_timed_effect)=NULL;
	log_error(E_FATAL,"Syntax error in timed effect"); }
#line 2896 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 718 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_timed_effect)=new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_timed_effect)->effs->assign_effects.push_front(
	     new assignment((yyvsp[-2].t_func_term),E_INCREASE,(yyvsp[-1].t_expression))); }
#line 2904 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 722 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_timed_effect)=new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_timed_effect)->effs->assign_effects.push_front(
	     new assignment((yyvsp[-2].t_func_term),E_DECREASE,(yyvsp[-1].t_expression))); }
#line 2912 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 726 "pddl+.y" /* yacc.c:1646  */
    {yyerrok; (yyval.t_timed_effect)=NULL;
	log_error(E_FATAL,"Syntax error in conditional continuous effect"); }
#line 2919 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 732 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); }
#line 2925 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 737 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists;
          (yyval.t_effect_lists)->forall_effects.push_back(
	       new forall_effect((yyvsp[-1].t_effect_lists), (yyvsp[-3].t_var_symbol_list), current_analysis->var_tab_stack.pop()));
          requires(E_COND_EFFS);}
#line 2934 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 742 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists;
	  (yyval.t_effect_lists)->cond_assign_effects.push_back(
	       new cond_effect((yyvsp[-2].t_goal),(yyvsp[-1].t_effect_lists)));
          requires(E_COND_EFFS); }
#line 2943 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 747 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=new effect_lists;
          (yyval.t_effect_lists)->timed_effects.push_back((yyvsp[0].t_timed_effect)); }
#line 2950 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 752 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); (yyvsp[-1].t_effect_lists)->append_effects((yyvsp[0].t_effect_lists)); delete (yyvsp[0].t_effect_lists); }
#line 2956 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 753 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists; }
#line 2962 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 757 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists);}
#line 2968 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 758 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[0].t_effect_lists);}
#line 2974 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 763 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->del_effects.push_front((yyvsp[0].t_simple_effect));}
#line 2980 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 765 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->add_effects.push_front((yyvsp[0].t_simple_effect));}
#line 2986 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 767 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->assign_effects.push_front((yyvsp[0].t_assignment));
         requires(E_NFLUENTS);}
#line 2993 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 773 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->del_effects.push_back((yyvsp[0].t_simple_effect));}
#line 2999 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 774 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->add_effects.push_back((yyvsp[0].t_simple_effect));}
#line 3005 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 775 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->assign_effects.push_back((yyvsp[0].t_assignment));
                                     requires(E_NFLUENTS); }
#line 3012 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 777 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists; }
#line 3018 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 783 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_ASSIGN,(yyvsp[-1].t_expression)); }
#line 3024 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 785 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_INCREASE,(yyvsp[-1].t_expression)); }
#line 3030 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 787 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_DECREASE,(yyvsp[-1].t_expression)); }
#line 3036 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 789 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_SCALE_UP,(yyvsp[-1].t_expression)); }
#line 3042 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 791 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_SCALE_DOWN,(yyvsp[-1].t_expression)); }
#line 3048 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 796 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists;
         timed_effect * te = new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_effect_lists)->timed_effects.push_front(te);
         te->effs->assign_effects.push_front(
	     new assignment((yyvsp[-2].t_func_term),E_INCREASE,(yyvsp[-1].t_expression))); }
#line 3058 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 802 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists;
         timed_effect * te = new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_effect_lists)->timed_effects.push_front(te);
         te->effs->assign_effects.push_front(
	     new assignment((yyvsp[-2].t_func_term),E_DECREASE,(yyvsp[-1].t_expression))); }
#line 3068 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 808 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists) = (yyvsp[-1].t_effect_lists);}
#line 3074 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 812 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); (yyvsp[-1].t_effect_lists)->append_effects((yyvsp[0].t_effect_lists)); delete (yyvsp[0].t_effect_lists); }
#line 3080 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 813 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists; }
#line 3086 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 817 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_expression)= (yyvsp[0].t_expression);}
#line 3092 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 818 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_expression)= new special_val_expr(E_DURATION_VAR);
                    requires( E_DURATION_INEQUALITIES );}
#line 3099 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 820 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)=(yyvsp[0].t_num_expression); }
#line 3105 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 821 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= (yyvsp[0].t_func_term); }
#line 3111 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 822 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression) = current_analysis->getControlParam((yyvsp[0].cp)); delete [] (yyvsp[0].cp);}
#line 3117 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 827 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new plus_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); }
#line 3123 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 829 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new minus_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); }
#line 3129 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 831 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new mul_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); }
#line 3135 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 833 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new div_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); }
#line 3141 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 838 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_goal)= new conj_goal((yyvsp[-1].t_goal_list)); }
#line 3147 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 840 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_goal)= new timed_goal(new comparison((yyvsp[-4].t_comparison_op),
        			new special_val_expr(E_DURATION_VAR),(yyvsp[-1].t_expression)),E_AT_START); }
#line 3154 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 843 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_goal) = new timed_goal(new comparison((yyvsp[-5].t_comparison_op),
					new special_val_expr(E_DURATION_VAR),(yyvsp[-2].t_expression)),E_AT_START);}
#line 3161 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 846 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_goal) = new timed_goal(new comparison((yyvsp[-5].t_comparison_op),
					new special_val_expr(E_DURATION_VAR),(yyvsp[-2].t_expression)),E_AT_END);}
#line 3168 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 851 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_comparison_op)= E_LESSEQ; requires(E_DURATION_INEQUALITIES);}
#line 3174 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 852 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_comparison_op)= E_GREATEQ; requires(E_DURATION_INEQUALITIES);}
#line 3180 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 853 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_comparison_op)= E_EQUALS; }
#line 3186 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 861 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_expression)= (yyvsp[0].t_expression); }
#line 3192 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 866 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_goal_list)=(yyvsp[-1].t_goal_list); (yyval.t_goal_list)->push_back((yyvsp[0].t_goal)); }
#line 3198 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 868 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_goal_list)= new goal_list; }
#line 3204 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 873 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_simple_effect)= new simple_effect((yyvsp[-1].t_proposition)); }
#line 3210 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 878 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_simple_effect)= new simple_effect((yyvsp[0].t_proposition)); }
#line 3216 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 885 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_simple_effect)= new simple_effect((yyvsp[-1].t_proposition)); }
#line 3222 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 890 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_simple_effect)= new simple_effect((yyvsp[0].t_proposition)); }
#line 3228 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 895 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_forall_effect)= new forall_effect((yyvsp[-1].t_effect_lists), (yyvsp[-3].t_var_symbol_list), current_analysis->var_tab_stack.pop());}
#line 3234 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 900 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_cond_effect)= new cond_effect((yyvsp[-2].t_goal),(yyvsp[-1].t_effect_lists)); }
#line 3240 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 905 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_ASSIGN,(yyvsp[-1].t_expression)); }
#line 3246 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 907 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_INCREASE,(yyvsp[-1].t_expression)); }
#line 3252 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 909 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_DECREASE,(yyvsp[-1].t_expression)); }
#line 3258 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 911 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_SCALE_UP,(yyvsp[-1].t_expression)); }
#line 3264 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 913 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_SCALE_DOWN,(yyvsp[-1].t_expression)); }
#line 3270 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 918 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new uminus_expression((yyvsp[-1].t_expression)); requires(E_NFLUENTS); }
#line 3276 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 920 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new plus_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); requires(E_NFLUENTS); }
#line 3282 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 922 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new minus_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); requires(E_NFLUENTS); }
#line 3288 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 924 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new mul_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); requires(E_NFLUENTS); }
#line 3294 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 926 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new div_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); requires(E_NFLUENTS); }
#line 3300 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 927 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)=(yyvsp[0].t_num_expression); }
#line 3306 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 928 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression) = current_analysis->getControlParam((yyvsp[0].cp)); delete [] (yyvsp[0].cp);}
#line 3312 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 929 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= (yyvsp[0].t_func_term); requires(E_NFLUENTS); }
#line 3318 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 934 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new mul_expression(new special_val_expr(E_HASHT),(yyvsp[-1].t_expression)); }
#line 3324 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 936 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new mul_expression((yyvsp[-2].t_expression), new special_val_expr(E_HASHT)); }
#line 3330 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 938 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new special_val_expr(E_HASHT); }
#line 3336 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 943 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_num_expression)=new int_expression((yyvsp[0].ival));   }
#line 3342 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 944 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_num_expression)=new float_expression((yyvsp[0].fval)); }
#line 3348 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 948 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[-2].cp)), (yyvsp[-1].t_parameter_symbol_list)); delete [] (yyvsp[-2].cp); }
#line 3354 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 951 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[-2].cp)), (yyvsp[-1].t_parameter_symbol_list)); delete [] (yyvsp[-2].cp); }
#line 3360 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 953 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[0].cp)),
                            new parameter_symbol_list); delete [] (yyvsp[0].cp);}
#line 3367 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 956 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_func_term) = new class_func_term( (yyvsp[-4].t_class), current_analysis->func_tab.symbol_get((yyvsp[-2].cp)), (yyvsp[-1].t_parameter_symbol_list)); delete [] (yyvsp[-2].cp);}
#line 3373 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 973 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[-2].cp)), (yyvsp[-1].t_parameter_symbol_list)); delete [] (yyvsp[-2].cp); }
#line 3379 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 975 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[-2].cp)), (yyvsp[-1].t_parameter_symbol_list)); delete [] (yyvsp[-2].cp); }
#line 3385 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 977 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[0].cp)),
                            new parameter_symbol_list); delete [] (yyvsp[0].cp);}
#line 3392 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 982 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_comparison_op)= E_GREATER; }
#line 3398 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 983 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_comparison_op)= E_GREATEQ; }
#line 3404 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 984 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_comparison_op)= E_LESS; }
#line 3410 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 985 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_comparison_op)= E_LESSEQ; }
#line 3416 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 986 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_comparison_op)= E_EQUALS; }
#line 3422 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 999 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= (yyvsp[0].t_goal);}
#line 3428 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 1006 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal) = new conj_goal((yyvsp[-1].t_goal_list));}
#line 3434 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 1009 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new qfied_goal(E_FORALL,(yyvsp[-3].t_var_symbol_list),(yyvsp[-1].t_goal),current_analysis->var_tab_stack.pop());
        requires(E_UNIV_PRECS);}
#line 3441 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1011 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal) = new conj_goal(new goal_list);}
#line 3447 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1012 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal) = new conj_goal(new goal_list);}
#line 3453 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1017 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new preference((yyvsp[-1].t_con_goal));requires(E_PREFERENCES);}
#line 3459 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1019 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new preference((yyvsp[-2].cp),(yyvsp[-1].t_con_goal));requires(E_PREFERENCES);}
#line 3465 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1021 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new conj_goal((yyvsp[-1].t_goal_list));}
#line 3471 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 1024 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal)= new qfied_goal(E_FORALL,(yyvsp[-3].t_var_symbol_list),(yyvsp[-1].t_con_goal),current_analysis->var_tab_stack.pop());
                requires(E_UNIV_PRECS);}
#line 3478 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1027 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = (yyvsp[0].t_con_goal);}
#line 3484 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1032 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new preference((yyvsp[-1].t_con_goal));requires(E_PREFERENCES);}
#line 3490 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 1034 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new preference((yyvsp[-2].cp),(yyvsp[-1].t_con_goal));requires(E_PREFERENCES);}
#line 3496 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1036 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new conj_goal((yyvsp[-1].t_goal_list));}
#line 3502 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1039 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal)= new qfied_goal(E_FORALL,(yyvsp[-3].t_var_symbol_list),(yyvsp[-1].t_con_goal),current_analysis->var_tab_stack.pop());
                requires(E_UNIV_PRECS);}
#line 3509 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 1045 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal_list)=(yyvsp[-1].t_goal_list); if((yyvsp[0].t_con_goal)) {(yyvsp[-1].t_goal_list)->push_back((yyvsp[0].t_con_goal));};}
#line 3515 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 1047 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal_list)= new goal_list; if((yyvsp[0].t_con_goal)) {(yyval.t_goal_list)->push_back((yyvsp[0].t_con_goal));};}
#line 3521 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 1052 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new preference((yyvsp[-1].t_goal)); requires(E_PREFERENCES);}
#line 3527 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 1054 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new preference((yyvsp[-2].cp),(yyvsp[-1].t_goal)); requires(E_PREFERENCES);}
#line 3533 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 1058 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)=(yyvsp[0].t_goal);}
#line 3539 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 1063 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal_list) = (yyvsp[-1].t_goal_list); if((yyvsp[0].t_con_goal)) {(yyval.t_goal_list)->push_back((yyvsp[0].t_con_goal));}; }
#line 3545 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 1065 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal_list) = new goal_list; if((yyvsp[0].t_con_goal)) {(yyval.t_goal_list)->push_back((yyvsp[0].t_con_goal));}}
#line 3551 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1070 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal)= new conj_goal((yyvsp[-1].t_goal_list));}
#line 3557 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1072 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new qfied_goal(E_FORALL,(yyvsp[-3].t_var_symbol_list),(yyvsp[-1].t_con_goal),current_analysis->var_tab_stack.pop());
        requires(E_UNIV_PRECS);}
#line 3564 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1075 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_ATEND,(yyvsp[-1].t_goal));}
#line 3570 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1077 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_ALWAYS,(yyvsp[-1].t_goal));}
#line 3576 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1079 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_SOMETIME,(yyvsp[-1].t_goal));}
#line 3582 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1081 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_WITHIN,(yyvsp[-1].t_goal),NULL,(yyvsp[-2].t_num_expression)->double_value(),0.0);delete (yyvsp[-2].t_num_expression);}
#line 3588 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1083 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_ATMOSTONCE,(yyvsp[-1].t_goal));}
#line 3594 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1085 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_SOMETIMEAFTER,(yyvsp[-1].t_goal),(yyvsp[-2].t_goal));}
#line 3600 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 1087 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_SOMETIMEBEFORE,(yyvsp[-1].t_goal),(yyvsp[-2].t_goal));}
#line 3606 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 228:
#line 1089 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_ALWAYSWITHIN,(yyvsp[-1].t_goal),(yyvsp[-2].t_goal),(yyvsp[-3].t_num_expression)->double_value(),0.0);delete (yyvsp[-3].t_num_expression);}
#line 3612 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 1091 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_HOLDDURING,(yyvsp[-1].t_goal),NULL,(yyvsp[-2].t_num_expression)->double_value(),(yyvsp[-3].t_num_expression)->double_value());delete (yyvsp[-3].t_num_expression);delete (yyvsp[-2].t_num_expression);}
#line 3618 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 1093 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_HOLDAFTER,(yyvsp[-1].t_goal),NULL,0.0,(yyvsp[-2].t_num_expression)->double_value());delete (yyvsp[-2].t_num_expression);}
#line 3624 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 1095 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_AFTER,(yyvsp[-1].t_goal),(yyvsp[-2].t_goal));}
#line 3630 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 1097 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = 0;
		 current_analysis->goalnames[(yyvsp[-2].cp)] = (yyvsp[-1].t_goal);
		 delete [] (yyvsp[-2].cp);}
#line 3638 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 1104 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal) = (yyvsp[0].t_goal);}
#line 3644 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 1106 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal) = new named_goal(current_analysis->goalnames[(yyvsp[0].cp)]); delete [] (yyvsp[0].cp);}
#line 3650 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1111 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new simple_goal((yyvsp[0].t_proposition),E_POS);}
#line 3656 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 236:
#line 1113 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new neg_goal((yyvsp[-1].t_goal));simple_goal * s = dynamic_cast<simple_goal *>((yyvsp[-1].t_goal));
       if(s && s->getProp()->head->getName()=="=") {requires(E_EQUALITY);}
       else{requires(E_NEGATIVE_PRECONDITIONS);};}
#line 3664 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 237:
#line 1117 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new conj_goal((yyvsp[-1].t_goal_list));}
#line 3670 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 1119 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new disj_goal((yyvsp[-1].t_goal_list));
        requires(E_DISJUNCTIVE_PRECONDS);}
#line 3677 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 239:
#line 1122 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new imply_goal((yyvsp[-2].t_goal),(yyvsp[-1].t_goal));
        requires(E_DISJUNCTIVE_PRECONDS);}
#line 3684 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 1126 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new qfied_goal((yyvsp[-5].t_quantifier),(yyvsp[-3].t_var_symbol_list),(yyvsp[-1].t_goal),current_analysis->var_tab_stack.pop());}
#line 3690 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 1129 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new qfied_goal((yyvsp[-5].t_quantifier),(yyvsp[-3].t_var_symbol_list),(yyvsp[-1].t_goal),current_analysis->var_tab_stack.pop());}
#line 3696 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 242:
#line 1131 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new comparison((yyvsp[-3].t_comparison_op),(yyvsp[-2].t_expression),(yyvsp[-1].t_expression));
        requires(E_NFLUENTS);}
#line 3703 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 1137 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal_list)=(yyvsp[-1].t_goal_list); (yyvsp[-1].t_goal_list)->push_back((yyvsp[0].t_goal));}
#line 3709 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 244:
#line 1139 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal_list)= new goal_list; (yyval.t_goal_list)->push_back((yyvsp[0].t_goal));}
#line 3715 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 245:
#line 1144 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal_list)=(yyvsp[-1].t_goal_list); (yyvsp[-1].t_goal_list)->push_back((yyvsp[0].t_goal));}
#line 3721 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 246:
#line 1146 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal_list)= new goal_list; (yyval.t_goal_list)->push_back((yyvsp[0].t_goal));}
#line 3727 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 1156 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_quantifier)=E_FORALL;
        current_analysis->var_tab_stack.push(
        		current_analysis->buildForallTab());}
#line 3735 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 248:
#line 1163 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_quantifier)=E_EXISTS;
        current_analysis->var_tab_stack.push(
        	current_analysis->buildExistsTab());}
#line 3743 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 1170 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_proposition)=new proposition((yyvsp[-2].t_pred_symbol),(yyvsp[-1].t_parameter_symbol_list));}
#line 3749 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 1175 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_proposition) = new proposition((yyvsp[-2].t_pred_symbol),(yyvsp[-1].t_var_symbol_list));}
#line 3755 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 251:
#line 1180 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_proposition)=new proposition((yyvsp[-2].t_pred_symbol),(yyvsp[-1].t_parameter_symbol_list));}
#line 3761 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 1185 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pred_decl_list)= (yyvsp[-1].t_pred_decl_list);}
#line 3767 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 253:
#line 1187 "pddl+.y" /* yacc.c:1646  */
    {yyerrok; (yyval.t_pred_decl_list)=NULL;
	 log_error(E_FATAL,"Syntax error in (:predicates ...)");
	}
#line 3775 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 254:
#line 1194 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_func_decl_list)= (yyvsp[-1].t_func_decl_list);}
#line 3781 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 1196 "pddl+.y" /* yacc.c:1646  */
    {yyerrok; (yyval.t_func_decl_list)=NULL;
	 log_error(E_FATAL,"Syntax error in (:functions ...)");
	}
#line 3789 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 256:
#line 1203 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = (yyvsp[-1].t_con_goal);}
#line 3795 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 257:
#line 1205 "pddl+.y" /* yacc.c:1646  */
    {yyerrok; (yyval.t_con_goal)=NULL;
      log_error(E_FATAL,"Syntax error in (:constraints ...)");
      }
#line 3803 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 258:
#line 1212 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_con_goal) = (yyvsp[-1].t_con_goal);}
#line 3809 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 259:
#line 1214 "pddl+.y" /* yacc.c:1646  */
    {yyerrok; (yyval.t_con_goal)=NULL;
      log_error(E_FATAL,"Syntax error in (:constraints ...)");
      }
#line 3817 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 260:
#line 1220 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_structure_store)=(yyvsp[-1].t_structure_store); (yyval.t_structure_store)->push_back((yyvsp[0].t_structure_def)); }
#line 3823 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 261:
#line 1221 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_structure_store)= new structure_store; (yyval.t_structure_store)->push_back((yyvsp[0].t_structure_def)); }
#line 3829 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 262:
#line 1225 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_structure_def)= (yyvsp[0].t_action_def); }
#line 3835 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 1226 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_structure_def)= (yyvsp[0].t_event_def); requires(E_TIME); }
#line 3841 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 1227 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_structure_def)= (yyvsp[0].t_process_def); requires(E_TIME); }
#line 3847 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 1228 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_structure_def)= (yyvsp[0].t_durative_action_def); requires(E_DURATIVE_ACTIONS); }
#line 3853 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 1229 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_structure_def)= (yyvsp[0].t_derivation_rule); requires(E_DERIVED_PREDICATES);}
#line 3859 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 1230 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_structure_def) = (yyvsp[0].t_class_def); requires(E_MODULES);}
#line 3865 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 268:
#line 1238 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_class_def) = new class_def((yyvsp[-2].t_class),(yyvsp[-1].t_func_decl_list));}
#line 3871 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 269:
#line 1242 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_dummy)= 0;
    	current_analysis->var_tab_stack.push(
    					current_analysis->buildRuleTab());}
#line 3879 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 270:
#line 1253 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_derivation_rule) = new derivation_rule((yyvsp[-2].t_proposition),(yyvsp[-1].t_goal),current_analysis->var_tab_stack.pop());}
#line 3885 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 271:
#line 1265 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_action_def)= current_analysis->buildAction(current_analysis->op_tab.new_symbol_put((yyvsp[-9].cp)),
			(yyvsp[-6].t_var_symbol_list),(yyvsp[-3].t_goal),(yyvsp[-1].t_effect_lists),
			current_analysis->var_tab_stack.pop()); delete [] (yyvsp[-9].cp); }
#line 3893 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 272:
#line 1269 "pddl+.y" /* yacc.c:1646  */
    {yyerrok;
	 log_error(E_FATAL,"Syntax error in action declaration.");
	 (yyval.t_action_def)= NULL; }
#line 3901 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 1282 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_event_def)= current_analysis->buildEvent(current_analysis->op_tab.new_symbol_put((yyvsp[-9].cp)),
		   (yyvsp[-6].t_var_symbol_list),(yyvsp[-3].t_goal),(yyvsp[-1].t_effect_lists),
		   current_analysis->var_tab_stack.pop()); delete [] (yyvsp[-9].cp);}
#line 3909 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 1287 "pddl+.y" /* yacc.c:1646  */
    {yyerrok;
	 log_error(E_FATAL,"Syntax error in event declaration.");
	 (yyval.t_event_def)= NULL; }
#line 3917 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 1299 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_process_def)= current_analysis->buildProcess(current_analysis->op_tab.new_symbol_put((yyvsp[-9].cp)),
		     (yyvsp[-6].t_var_symbol_list),(yyvsp[-3].t_goal),(yyvsp[-1].t_effect_lists),
                     current_analysis->var_tab_stack.pop()); delete [] (yyvsp[-9].cp);}
#line 3925 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 1303 "pddl+.y" /* yacc.c:1646  */
    {yyerrok;
	 log_error(E_FATAL,"Syntax error in process declaration.");
	 (yyval.t_process_def)= NULL; }
#line 3933 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 277:
#line 1309 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_var_symbol_list) = (yyvsp[-1].t_var_symbol_list); current_analysis->setControlParams((yyval.t_var_symbol_list));}
#line 3939 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 1310 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_var_symbol_list) = NULL; current_analysis->setControlParams((yyval.t_var_symbol_list));}
#line 3945 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 279:
#line 1321 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_durative_action_def)= (yyvsp[-1].t_durative_action_def);
      (yyval.t_durative_action_def)->name= current_analysis->op_tab.new_symbol_put((yyvsp[-9].cp));
      (yyval.t_durative_action_def)->symtab= current_analysis->var_tab_stack.pop();
      (yyval.t_durative_action_def)->control_parameters = current_analysis->getControlParams();
      (yyval.t_durative_action_def)->parameters= (yyvsp[-6].t_var_symbol_list);
      (yyval.t_durative_action_def)->dur_constraint= (yyvsp[-2].t_goal);
      delete [] (yyvsp[-9].cp);
      delete (yyvsp[-4].t_var_symbol_list);
    }
#line 3959 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 280:
#line 1332 "pddl+.y" /* yacc.c:1646  */
    {yyerrok;
	 log_error(E_FATAL,"Syntax error in durative-action declaration.");
	 (yyval.t_durative_action_def)= NULL; }
#line 3967 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 281:
#line 1339 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_durative_action_def)=(yyvsp[-2].t_durative_action_def); (yyval.t_durative_action_def)->effects=(yyvsp[0].t_effect_lists);}
#line 3973 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 282:
#line 1341 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_durative_action_def)=(yyvsp[-2].t_durative_action_def); (yyval.t_durative_action_def)->precondition=(yyvsp[0].t_goal);}
#line 3979 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 283:
#line 1342 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_durative_action_def)= current_analysis->buildDurativeAction();}
#line 3985 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 284:
#line 1347 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_goal)=(yyvsp[0].t_goal); }
#line 3991 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 285:
#line 1349 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_goal)= new conj_goal((yyvsp[-1].t_goal_list)); }
#line 3997 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 286:
#line 1354 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_goal_list)=(yyvsp[-1].t_goal_list); (yyval.t_goal_list)->push_back((yyvsp[0].t_goal)); }
#line 4003 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 287:
#line 1356 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_goal_list)= new goal_list; }
#line 4009 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 288:
#line 1361 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new timed_goal((yyvsp[-1].t_goal),E_AT_START);}
#line 4015 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 289:
#line 1363 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new timed_goal((yyvsp[-1].t_goal),E_AT_END);}
#line 4021 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 290:
#line 1365 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)= new timed_goal((yyvsp[-1].t_goal),E_OVER_ALL);}
#line 4027 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 291:
#line 1367 "pddl+.y" /* yacc.c:1646  */
    {timed_goal * tg = dynamic_cast<timed_goal *>((yyvsp[-1].t_goal));
		(yyval.t_goal) = new timed_goal(new preference((yyvsp[-2].cp),tg->clearGoal()),tg->getTime());
			delete tg;
			requires(E_PREFERENCES);}
#line 4036 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 292:
#line 1372 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal) = new preference((yyvsp[-1].t_goal));requires(E_PREFERENCES);}
#line 4042 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 293:
#line 1376 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_dummy)= 0; current_analysis->var_tab_stack.push(
    				current_analysis->buildOpTab());}
#line 4049 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 294:
#line 1381 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_EQUALITY;}
#line 4055 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 295:
#line 1382 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_STRIPS;}
#line 4061 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 296:
#line 1384 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_TYPING;}
#line 4067 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 297:
#line 1386 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_NEGATIVE_PRECONDITIONS;}
#line 4073 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 298:
#line 1388 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_DISJUNCTIVE_PRECONDS;}
#line 4079 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 299:
#line 1389 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_EXT_PRECS;}
#line 4085 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 300:
#line 1390 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_UNIV_PRECS;}
#line 4091 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 301:
#line 1391 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_COND_EFFS;}
#line 4097 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 302:
#line 1392 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_NFLUENTS | E_OFLUENTS;}
#line 4103 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 303:
#line 1394 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_DURATIVE_ACTIONS;}
#line 4109 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 304:
#line 1395 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_TIME |
                      E_NFLUENTS |
                      E_DURATIVE_ACTIONS; }
#line 4117 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 305:
#line 1398 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)=E_ACTIONCOSTS | E_NFLUENTS;}
#line 4123 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 306:
#line 1401 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)=E_OFLUENTS;}
#line 4129 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 307:
#line 1402 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)=E_NFLUENTS;}
#line 4135 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 308:
#line 1403 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)=E_MODULES;}
#line 4141 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 309:
#line 1405 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_STRIPS |
		      E_TYPING |
		      E_NEGATIVE_PRECONDITIONS |
		      E_DISJUNCTIVE_PRECONDS |
		      E_EQUALITY |
		      E_EXT_PRECS |
		      E_UNIV_PRECS |
		      E_COND_EFFS;}
#line 4154 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 310:
#line 1414 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_EXT_PRECS |
		      E_UNIV_PRECS;}
#line 4161 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 311:
#line 1418 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_DURATION_INEQUALITIES;}
#line 4167 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 312:
#line 1421 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_CONTINUOUS_EFFECTS;}
#line 4173 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 313:
#line 1423 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag) = E_DERIVED_PREDICATES;}
#line 4179 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 314:
#line 1425 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag) = E_TIMED_INITIAL_LITERALS;}
#line 4185 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 315:
#line 1427 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag) = E_PREFERENCES;}
#line 4191 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 316:
#line 1429 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag) = E_CONSTRAINTS;}
#line 4197 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 317:
#line 1430 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag) = E_SUPPLYDEMAND;}
#line 4203 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 318:
#line 1432 "pddl+.y" /* yacc.c:1646  */
    {log_error(E_WARNING,"Unrecognised requirements declaration ");
       (yyval.t_pddl_req_flag)= 0; delete [] (yyvsp[0].cp);}
#line 4210 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 319:
#line 1438 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)=(yyvsp[-1].t_const_symbol_list);}
#line 4216 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 320:
#line 1442 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_type_list)=(yyvsp[-1].t_type_list); requires(E_TYPING);}
#line 4222 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 321:
#line 1452 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[-1].t_problem); (yyval.t_problem)->name = (yyvsp[-7].cp); (yyval.t_problem)->domain_name = (yyvsp[-3].cp);
		if (types_used && !types_defined) {
			yyerrok; log_error(E_FATAL,"Syntax error in problem file - types used, but no :types section in domain file.");
		}

	}
#line 4233 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 322:
#line 1460 "pddl+.y" /* yacc.c:1646  */
    {yyerrok; (yyval.t_problem)=NULL;
       	log_error(E_FATAL,"Syntax error in problem definition."); }
#line 4240 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 323:
#line 1466 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); (yyval.t_problem)->req= (yyvsp[-1].t_pddl_req_flag);}
#line 4246 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 324:
#line 1467 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); (yyval.t_problem)->objects= (yyvsp[-1].t_const_symbol_list);}
#line 4252 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 325:
#line 1468 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); (yyval.t_problem)->initial_state= (yyvsp[-1].t_effect_lists);}
#line 4258 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 326:
#line 1469 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); (yyval.t_problem)->the_goal= (yyvsp[-1].t_goal);}
#line 4264 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 327:
#line 1471 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); (yyval.t_problem)->constraints = (yyvsp[-1].t_con_goal);}
#line 4270 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 328:
#line 1472 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); if((yyval.t_problem)->metric == 0) {(yyval.t_problem)->metric= (yyvsp[-1].t_metric);}
											else {(yyval.t_problem)->metric->add((yyvsp[-1].t_metric));}}
#line 4277 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 329:
#line 1474 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); (yyval.t_problem)->length= (yyvsp[-1].t_length_spec);}
#line 4283 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 330:
#line 1475 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_problem)=new problem;}
#line 4289 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 331:
#line 1478 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)=(yyvsp[-1].t_const_symbol_list);}
#line 4295 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 332:
#line 1481 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists);}
#line 4301 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 333:
#line 1484 "pddl+.y" /* yacc.c:1646  */
    {(yyval.vtab) = current_analysis->buildOpTab();}
#line 4307 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 334:
#line 1487 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_goal)=(yyvsp[-1].t_goal);delete (yyvsp[-2].vtab);}
#line 4313 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 335:
#line 1492 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_metric)= new metric_spec((yyvsp[-2].t_optimization),(yyvsp[-1].t_expression)); }
#line 4319 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 336:
#line 1494 "pddl+.y" /* yacc.c:1646  */
    {yyerrok;
        log_error(E_FATAL,"Syntax error in metric declaration.");
        (yyval.t_metric)= NULL; }
#line 4327 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 337:
#line 1501 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_length_spec)= new length_spec(E_BOTH,(yyvsp[-3].ival),(yyvsp[-1].ival));}
#line 4333 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 338:
#line 1504 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_length_spec) = new length_spec(E_SERIAL,(yyvsp[-1].ival));}
#line 4339 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 339:
#line 1508 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_length_spec) = new length_spec(E_PARALLEL,(yyvsp[-1].ival));}
#line 4345 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 340:
#line 1514 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_optimization)= E_MINIMIZE;}
#line 4351 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 341:
#line 1515 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_optimization)= E_MAXIMIZE;}
#line 4357 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 342:
#line 1520 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_expression)= (yyvsp[-1].t_expression);}
#line 4363 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 343:
#line 1521 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_expression)= (yyvsp[0].t_func_term);}
#line 4369 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 344:
#line 1522 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_expression)= (yyvsp[0].t_num_expression);}
#line 4375 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 345:
#line 1523 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new special_val_expr(E_TOTAL_TIME); }
#line 4381 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 346:
#line 1525 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_expression) = new violation_term((yyvsp[-1].cp));}
#line 4387 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 347:
#line 1526 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new special_val_expr(E_TOTAL_TIME); }
#line 4393 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 348:
#line 1530 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new plus_expression((yyvsp[-1].t_expression),(yyvsp[0].t_expression)); }
#line 4399 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 349:
#line 1531 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new minus_expression((yyvsp[-1].t_expression),(yyvsp[0].t_expression)); }
#line 4405 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 350:
#line 1532 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new mul_expression((yyvsp[-1].t_expression),(yyvsp[0].t_expression)); }
#line 4411 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 351:
#line 1533 "pddl+.y" /* yacc.c:1646  */
    { (yyval.t_expression)= new div_expression((yyvsp[-1].t_expression),(yyvsp[0].t_expression)); }
#line 4417 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 352:
#line 1537 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_expression) = (yyvsp[0].t_expression);}
#line 4423 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 353:
#line 1539 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_expression) = new plus_expression((yyvsp[-1].t_expression),(yyvsp[0].t_expression));}
#line 4429 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 354:
#line 1543 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_expression) = (yyvsp[0].t_expression);}
#line 4435 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 355:
#line 1545 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_expression) = new mul_expression((yyvsp[-1].t_expression),(yyvsp[0].t_expression));}
#line 4441 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 356:
#line 1551 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_plan)= (yyvsp[0].t_plan);
         (yyval.t_plan)->push_front((yyvsp[-1].t_step)); }
#line 4448 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 357:
#line 1554 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_plan) = (yyvsp[0].t_plan);(yyval.t_plan)->insertTime((yyvsp[-1].fval));}
#line 4454 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 358:
#line 1556 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_plan) = (yyvsp[0].t_plan);(yyval.t_plan)->insertTime((yyvsp[-1].ival));}
#line 4460 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 359:
#line 1558 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_plan)= new plan;}
#line 4466 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 360:
#line 1563 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_step)=(yyvsp[0].t_step);
         (yyval.t_step)->start_time_given=1;
         (yyval.t_step)->start_time=(yyvsp[-2].fval);}
#line 4474 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 361:
#line 1567 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_step)=(yyvsp[0].t_step);
	 (yyval.t_step)->start_time_given=0;}
#line 4481 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 362:
#line 1573 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_step)= (yyvsp[-3].t_step);
	 (yyval.t_step)->duration_given=1;
         (yyval.t_step)->duration= (yyvsp[-1].fval);}
#line 4489 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 363:
#line 1577 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_step)= (yyvsp[0].t_step);
         (yyval.t_step)->duration_given=0;}
#line 4496 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 364:
#line 1583 "pddl+.y" /* yacc.c:1646  */
    {(yyval.t_step)= new plan_step(
              current_analysis->op_tab.symbol_get((yyvsp[-2].cp)),
	      (yyvsp[-1].t_const_symbol_list)); delete [] (yyvsp[-2].cp);
      }
#line 4505 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 365:
#line 1590 "pddl+.y" /* yacc.c:1646  */
    {(yyval.fval)= (yyvsp[0].fval);}
#line 4511 "pddl+.cpp" /* yacc.c:1646  */
    break;

  case 366:
#line 1591 "pddl+.y" /* yacc.c:1646  */
    {(yyval.fval)= (float) (yyvsp[0].ival);}
#line 4517 "pddl+.cpp" /* yacc.c:1646  */
    break;


#line 4521 "pddl+.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 1594 "pddl+.y" /* yacc.c:1906  */


#include <cstdio>
#include <iostream>
int line_no= 1;
using std::istream;
#include "pddl+.lex.yy.h"

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
