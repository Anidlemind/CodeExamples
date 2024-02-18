%skeleton "lalr1.cc"
%require "3.5"

%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    #include <iostream>
    #include <algorithm>

    class Statement;
    class Lines;
    class ExpressionStatement;
    class Variable;
    class NotStatement;
    class IncDecStatement;
    class BinaryStatement;
    class ConstStatement;
    class AssignmentStatement;
    class LoopStatement;
    class IfStatement;
    class PrintStatement;
    class InputStatement;

    enum class UnaryOperationType;
    enum class BinaryOperationType;
    enum class LoopType;
    enum class Type;

    class Scanner;
    class Driver;
}

%define parse.trace
%define parse.error verbose

%code {
    #include "headers/include_all.h"
    #include "location.hh"
    #include "parser.hh"

    class Scanner;
    class Driver;

    static yy::parser::symbol_type yylex(Scanner &scanner, Driver& driver) {
        return scanner.ScanToken();
    }
}

%lex-param { Scanner &scanner }
%lex-param { Driver &driver }
%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

%locations

%define api.token.prefix {TOK_}

%token
    END 0       "end of file"
    HAI         "HAI"
    KTHXBYE     "KTHXBYE"

    ASSIGN      "R"
    SUM         "SUM"
    SUB         "DIFF"
    MUL         "PRODUKT"
    DIV         "QUOSHUNT"
    MOD         "MOD"
    MAX         "BIGGR"
    MIN         "SMALLR"
    INC         "UPPIN"
    DEC         "NERFIN"
    CONC        "SMOOSH"

    EQUALS      "BOTH SAEM"
    DIFFER      "DIFFRINT"

    WIN         "WIN"
    FAIL        "FAIL"
    AND         "BOTH"
    OR          "EITHER"
    XOR         "WON"
    NOT         "NOT"

    LOOP_START  "IM IN YR"
    LOOP_END    "IM OUTTA YR"
    TILL        "TIL"
    WHILE       "WILE"

    IF_START    "O RLY?"
    IF_TRUE     "YA RLY"
    IF_FALSE    "NO WAI"
    IF_END      "OIC"

    WRITE       "VISIBLE"
    READ        "GIMMEH"
    VERSION     "VERSION"

    VAR_DECL    "I HAS A"
    ITZ         "ITZ"

    INCL_LIB    "CAN HAS"
    QUES        "?"

    YR          "YR"
    AN          "AN"
    OF          "OF"
;

%token <std::string> NAME "identifier"
%token <std::string> STRING "string"
%token <int> NUMBER "number"

%nterm <Lines*> Lines
%nterm <ExpressionStatement*> EXPR
%nterm <AssignmentStatement*> DECLARATION
%nterm <AssignmentStatement*> ASSIGNMENT
%nterm <IncDecStatement*> INC_DEC
%nterm <IfStatement*> CONDITION
%nterm <PrintStatement*> OUTPUT
%nterm <InputStatement*> INPUT
%nterm <LoopStatement*> LOOP

%printer { yyo << $$; } <*>;

%%
%start PROGRAM;
PROGRAM:
    HAI VERSION Lines KTHXBYE { driver.program = $3; }
    | HAI Lines KTHXBYE { driver.program = $2; }

Lines:
    %empty {
        $$ = new Lines();
    }
    | Lines DECLARATION { $$ = $1; $1->push_back($2); }
    | Lines ASSIGNMENT { $$ = $1; $1->push_back($2); }
    | Lines INC_DEC { $$ = $1; $1->push_back($2); }
    | Lines CONDITION { $$ = $1; $1->push_back($2); }
    | Lines OUTPUT { $$ = $1; $1->push_back($2); }
    | Lines INPUT { $$ = $1; $1->push_back($2); }
    | Lines LOOP { $$ = $1; $1->push_back($2); }
    ;

EXPR:
    NAME {
        $$ = new VariableStatement(driver.variables[$1]);
    }
    | WIN {
          $$ = new ConstStatement(Variable(true));
      }
    | FAIL {
          $$ = new ConstStatement(Variable(false));
      }
    | STRING {
          $$ = new ConstStatement(Variable($1));
      }
    | NUMBER {
          $$ = new ConstStatement(Variable($1));
      }
    | EQUALS EXPR AN EXPR {
        $$ = new BinaryStatement($2, $4, BinaryOperationType::EQUALS);
      }
    | DIFFER EXPR AN EXPR {
        $$ = new BinaryStatement($2, $4, BinaryOperationType::DIFFER);
      }
    | NOT EXPR {
          $$ = new NotStatement($2);
      }
    | AND OF EXPR AN EXPR {
          $$ = new BinaryStatement($3, $5, BinaryOperationType::AND);
      }
    | XOR OF EXPR AN EXPR {
          $$ = new BinaryStatement($3, $5, BinaryOperationType::XOR);
      }
    | OR  OF EXPR AN EXPR {
          $$ = new BinaryStatement($3, $5, BinaryOperationType::OR);
      }
    | SUM OF EXPR AN EXPR {
          $$ = new BinaryStatement($3, $5, BinaryOperationType::SUM);
      }
    | SUB OF EXPR AN EXPR {
          $$ = new BinaryStatement($3, $5, BinaryOperationType::SUB);
      }
    | MUL OF EXPR AN EXPR {
          $$ = new BinaryStatement($3, $5, BinaryOperationType::MUL);
      }
    | DIV OF EXPR AN EXPR {
          $$ = new BinaryStatement($3, $5, BinaryOperationType::DIV);
      }
    | MOD OF EXPR AN EXPR {
          $$ = new BinaryStatement($3, $5, BinaryOperationType::MOD);
      }
    | MAX OF EXPR AN EXPR {
          $$ = new BinaryStatement($3, $5, BinaryOperationType::MAX);
      }
    | MIN OF EXPR AN EXPR {
          $$ = new BinaryStatement($3, $5, BinaryOperationType::MIN);
      }
    | CONC EXPR AN EXPR {
          $$ = new BinaryStatement($2, $4, BinaryOperationType::CONC);
    }
    ;

DECLARATION:
    VAR_DECL NAME {
        $$ = new AssignmentStatement(new ConstStatement(Variable(0)), driver.variables[$2]);
    }
    | VAR_DECL NAME ITZ EXPR {
          $$ = new AssignmentStatement($4, driver.variables[$2]);
      }
    ;

ASSIGNMENT:
    NAME ASSIGN EXPR {
        $$ = new AssignmentStatement($3, driver.variables[$1]);
    }
    ;

INC_DEC:
    INC YR NAME {
        $$ = new IncDecStatement(UnaryOperationType::INC, driver.variables[$3]);
    }
    | DEC YR NAME {
          $$ = new IncDecStatement(UnaryOperationType::DEC, driver.variables[$3]);
      }
    ;

CONDITION:
    EXPR IF_START IF_TRUE Lines IF_END {
        $$ = new IfStatement($4, new Lines(), $1);
    }
    | EXPR IF_START IF_TRUE Lines IF_FALSE Lines IF_END {
        $$ = new IfStatement($4, $6, $1);
      }
    ;

OUTPUT:
    WRITE EXPR { $$ = new PrintStatement($2); }
    ;

INPUT:
    READ NAME { $$ = new InputStatement(driver.variables[$2]); }
    ;

LOOP:
    LOOP_START NAME INC_DEC WHILE EXPR Lines LOOP_END NAME {
        $$ = new LoopStatement($3, $5, $6, LoopType::WHILE);
    }
    | LOOP_START NAME INC_DEC TILL EXPR Lines LOOP_END NAME {
          $$ = new LoopStatement($3, $5, $6, LoopType::TILL);
      }
    ;


%%

void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
