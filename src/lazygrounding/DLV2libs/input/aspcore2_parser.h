/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_SRC_INPUT_ASPCORE2_PARSER_H_INCLUDED
# define YY_YY_SRC_INPUT_ASPCORE2_PARSER_H_INCLUDED
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
    SYMBOLIC_CONSTANT = 258,
    NUMBER = 259,
    VARIABLE = 260,
    STRING = 261,
    DIRECTIVE_NAME = 262,
    DIRECTIVE_VALUE = 263,
    AGGR_COUNT = 264,
    AGGR_MAX = 265,
    AGGR_MIN = 266,
    AGGR_SUM = 267,
    ERROR = 268,
    NEWLINE = 269,
    DOT = 270,
    DDOT = 271,
    SEMICOLON = 272,
    COLON = 273,
    CONS = 274,
    QUERY_MARK = 275,
    PLUS = 276,
    TIMES = 277,
    SLASH = 278,
    ANON_VAR = 279,
    PARAM_OPEN = 280,
    PARAM_CLOSE = 281,
    SQUARE_OPEN = 282,
    SQUARE_CLOSE = 283,
    CURLY_OPEN = 284,
    CURLY_CLOSE = 285,
    EQUAL = 286,
    UNEQUAL = 287,
    LESS = 288,
    GREATER = 289,
    LESS_OR_EQ = 290,
    GREATER_OR_EQ = 291,
    DASH = 292,
    COMMA = 293,
    NAF = 294,
    AT = 295,
    WCONS = 296,
    VEL = 297,
    EXISTS = 298
  };
#endif
/* Tokens.  */
#define SYMBOLIC_CONSTANT 258
#define NUMBER 259
#define VARIABLE 260
#define STRING 261
#define DIRECTIVE_NAME 262
#define DIRECTIVE_VALUE 263
#define AGGR_COUNT 264
#define AGGR_MAX 265
#define AGGR_MIN 266
#define AGGR_SUM 267
#define ERROR 268
#define NEWLINE 269
#define DOT 270
#define DDOT 271
#define SEMICOLON 272
#define COLON 273
#define CONS 274
#define QUERY_MARK 275
#define PLUS 276
#define TIMES 277
#define SLASH 278
#define ANON_VAR 279
#define PARAM_OPEN 280
#define PARAM_CLOSE 281
#define SQUARE_OPEN 282
#define SQUARE_CLOSE 283
#define CURLY_OPEN 284
#define CURLY_CLOSE 285
#define EQUAL 286
#define UNEQUAL 287
#define LESS 288
#define GREATER 289
#define LESS_OR_EQ 290
#define GREATER_OR_EQ 291
#define DASH 292
#define COMMA 293
#define NAF 294
#define AT 295
#define WCONS 296
#define VEL 297
#define EXISTS 298

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 29 "src/input/aspcore2.y" /* yacc.c:1909  */

    char* string;
    char single_char;
    int integer;

#line 146 "src/input/aspcore2_parser.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (DLV2::InputDirector& director);

#endif /* !YY_YY_SRC_INPUT_ASPCORE2_PARSER_H_INCLUDED  */
