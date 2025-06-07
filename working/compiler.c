#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#define int int64_t
#define MAX_SYMBOLS 1024
#define MAX_TOKEN_LEN 64

int MAX_SIZE;

int *code,      // code segment
    *code_dump, // for dump
    *stack;     // stack segment
char *data;     // data segment

int *pc, // pc register
    *sp, // rsp register
    *bp; // rbp register

int ax, // common register
    cycle;

// instruction set:
enum
{
    IMM,
    LEA,
    JMP,
    JZ,
    JNZ,
    CALL,
    NVAR,
    DARG,
    RET,
    LI,
    LF,
    LC,
    SI,
    SF,
    SC,
    PUSH,
    OR,
    XOR,
    AND,
    EQ,
    NE,
    LT,
    GT,
    LE,
    GE,
    SHL,
    SHR,
    ADD,
    FADD,
    SUB,
    FSUB,
    MUL,
    FMUL,
    DIV,
    MOD,
    OPEN,
    READ,
    CLOS,
    PRTF,
    MALC,
    FREE,
    MSET,
    MCMP,
    EXIT
};

// classes/keywords, Do not support for.
enum
{
    Num_int = 128,
    Num_float,
    Fun,
    Sys,
    Glo,
    Loc,
    Id,
    Char,
    Int,
    Float,
    Enum,
    If,
    Else,
    Return,
    Sizeof,
    While,
    // operators in precedence order.
    Assign,
    Cond,
    Lor,
    Land,
    Or,
    Xor,
    And,
    Eq,
    Ne,
    Lt,
    Gt,
    Le,
    Ge,
    Shl,
    Shr,
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Inc,
    Dec,
    Brak
};

// fields of symbol_table
typedef struct
{
    int token;
    char name[MAX_TOKEN_LEN];
    int hash;
    int type;
    int class;
    int value;
    int Gclass;
    int Gtype;
    int Gvalue;
    int symsize;
} Symbol;

enum
{
    Token,
    Hash,
    Name,
    Class,
    Type,
    Value,
    GClass,
    GType,
    GValue,
    SymSize
};

// types of variables & functions in symbol_table
enum
{
    CHAR,
    INT,
    FLOAT,
    PTR
};

// src code & dump
char *src,
    *src_dump;

// symbol table & pointer
Symbol symbol_table[MAX_SYMBOLS],
    symbol_ptr[MAX_SYMBOLS],
    main_ptr[MAX_SYMBOLS];
int symbol_count = 0;

typedef struct
{
    int token;
    char lexeme[MAX_TOKEN_LEN];
} TokenInfo;

TokenInfo token_list[MAX_SYMBOLS];
int token_count = 0;

int token, token_val;
float float_val;
int line;

// Function to add a token to the token list
void add_token(int token, const char *lexeme)
{
    token_list[token_count].token = token;
    strncpy(token_list[token_count].lexeme, lexeme, MAX_TOKEN_LEN - 1);
    token_list[token_count].lexeme[MAX_TOKEN_LEN - 1] = '\0'; // Ensure null termination
    token_count++;
}

void tokenize()
{
    char *ch_ptr;
    while ((token = *src++))
    {
        if (token == '\n')
            line++;
        // skip marco
        else if (token == '#')
            while (*src != 0 && *src != '\n')
                src++;
        // handle symbol
        else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_'))
        {
            ch_ptr = src - 1;
            while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
                // use token store hash value
                token = token * 147 + *src++;
            // keep hash
            token = (token << 6) + (src - ch_ptr);
            // symbol_ptr = symbol_table;
            // // search same symbol in table
            // while (symbol_ptr.Token)
            // {
            //     if (token == symbol_ptr[Hash] && !memcmp((char *)symbol_ptr[Name], ch_ptr, src - ch_ptr))
            //     {
            //         token = symbol_ptr[Token];
            //         return;
            //     }
            //     symbol_ptr = symbol_ptr + SymSize;
            // }
            // // add new symbol
            // symbol_ptr[Name] = (int)ch_ptr;
            // symbol_ptr[Hash] = token;
            // token = symbol_ptr[Token] = Id;
            // return;
            // Check if the symbol is already in the table
            for (int i = 0; i < symbol_count; i++)
            {
                if (symbol_table[i].hash == token && !memcmp(symbol_table[i].name, ch_ptr, src - ch_ptr))
                {
                    token = symbol_table[i].token;
                    add_token(token, symbol_table[i].name); // Add token to the list
                    return;
                }
            }

            // Add new symbol to the table
            symbol_table[symbol_count].token = Id;
            symbol_table[symbol_count].hash = token;
            strncpy(symbol_table[symbol_count].name, ch_ptr, src - ch_ptr);
            symbol_table[symbol_count].name[src - ch_ptr] = '\0'; // Null-terminate the string
            add_token(Id, symbol_table[symbol_count].name);       // Add token to the list
            symbol_count++;
            token = Id;
            return;
        }
        // handle number
        else if (token >= '0' && token <= '9')
        {
            ch_ptr = src - 1;
            token_val = token - '0';
            while (*src >= '0' && *src <= '9')
                token_val = token_val * 10 + *src++ - '0';

            // Check for float
            if (*src == '.')
            {
                src++;
                float_val = (float)token_val;
                float base = 0.1f;
                while (*src >= '0' && *src <= '9')
                {
                    float_val += (*src++ - '0') * base;
                    base *= 0.1f;
                }
                token = Num_float;
                char num_str[MAX_TOKEN_LEN];
                snprintf(num_str, MAX_TOKEN_LEN, "%f", float_val);
                add_token(Num_float, num_str); // Add token to the list
            }
            else
            {
                token = Num_int;
                char num_str[MAX_TOKEN_LEN];
                snprintf(num_str, MAX_TOKEN_LEN, "%d", token_val);
                add_token(Num_int, num_str); // Add token to the list
            }
            return;
        }
        // handle string & char
        else if (token == '"' || token == '\'')
        {
            ch_ptr = data;
            while (*src != 0 && *src != token)
            {
                if ((token_val = *src++) == '\\')
                {
                    // only support escape char '\n'
                    if ((token_val = *src++) == 'n')
                        token_val = '\n';
                }
                // store string to data segment
                if (token == '"')
                    *data++ = token_val;
            }
            src++;
            if (token == '"')
            {
                token_val = (int)ch_ptr;
                add_token(token, (char *)ch_ptr); // Add token to the list
            } // single char is Num
            else
            {
                token = Num_int;
                char num_str[MAX_TOKEN_LEN];
                snprintf(num_str, MAX_TOKEN_LEN, "%d", token_val);
                add_token(Num_int, num_str); // Add token to the list
            }
            return;
        }
        // handle comments or divide
        else if (token == '/')
        {
            if (*src == '/')
            {
                // skip comments
                while (*src != 0 && *src != '\n')
                    src++;
            }
            else
            {
                // divide
                token = Div;
                add_token(Div, "/"); // Add token to the list
                return;
            }
        }
        // handle all kinds of operators.
        else if (token == '=')
        {
            if (*src == '=')
            {
                src++;
                token = Eq;
                add_token(Eq, "=="); // Add token to the list
            }
            else
            {
                token = Assign;
                add_token(Assign, "="); // Add token to the list
            }
            return;
        }
        else if (token == '+')
        {
            if (*src == '+')
            {
                src++;
                token = Inc;
                add_token(Inc, "++"); // Add token to the list
            }
            else
            {
                token = Add;
                add_token(Add, "+"); // Add token to the list
            }
            return;
        }
        else if (token == '-')
        {
            if (*src == '-')
            {
                src++;
                token = Dec;
                add_token(Dec, "--"); // Add token to the list
            }
            else
            {
                token = Sub;
                add_token(Sub, "-"); // Add token to the list
            }
            return;
        }
        else if (token == '!')
        {
            if (*src == '=')
            {
                src++;
                token = Ne;
                add_token(Ne, "!="); // Add token to the list
            }
            return;
        }
        else if (token == '<')
        {
            if (*src == '=')
            {
                src++;
                token = Le;
                add_token(Le, "<="); // Add token to the list
            }
            else if (*src == '<')
            {
                src++;
                token = Shl;
                add_token(Shl, "<<"); // Add token to the list
            }
            else
            {
                token = Lt;
                add_token(Lt, "<"); // Add token to the list
            }
            return;
        }
        else if (token == '>')
        {
            if (*src == '=')
            {
                src++;
                token = Ge;
                add_token(Ge, ">="); // Add token to the list
            }
            else if (*src == '>')
            {
                src++;
                token = Shr;
                add_token(Shr, ">>"); // Add token to the list
            }
            else
            {
                token = Gt;
                add_token(Gt, ">"); // Add token to the list
            }
            return;
        }
        else if (token == '|')
        {
            if (*src == '|')
            {
                src++;
                token = Lor;
                add_token(Lor, "||"); // Add token to the list
            }
            else
            {
                token = Or;
                add_token(Or, "|"); // Add token to the list
            }
            return;
        }
        else if (token == '&')
        {
            if (*src == '&')
            {
                src++;
                token = Land;
                add_token(Land, "&&"); // Add token to the list
            }
            else
            {
                token = And;
                add_token(And, "&"); // Add token to the list
            }
            return;
        }
        else if (token == '^')
        {
            token = Xor;
            add_token(Xor, "^"); // Add token to the list
            return;
        }
        else if (token == '%')
        {
            token = Mod;
            add_token(Mod, "%"); // Add token to the list
            return;
        }
        else if (token == '*')
        {
            token = Mul;
            add_token(Mul, "*"); // Add token to the list
            return;
        }
        else if (token == '[')
        {
            token = Brak;
            add_token(Brak, "["); // Add token to the list
            return;
        }
        else if (token == '?')
        {
            token = Cond;
            add_token(Cond, "?"); // Add token to the list
            return;
        }
        else if (token == '~' || token == ';' || token == '{' || token == '}' || token == '(' || token == ')' || token == ']' || token == ',' || token == ':')
        {
            char sym[2] = {token, '\0'};
            add_token(token, sym); // Add token to the list
            return;
        }
    }
}

// asserting
void assert(int tk)
{
    if (token != tk)
    {
        printf("in line %lld: expect token: %lld(%c), get: %lld(%c)\n", line, tk, (char)tk, token, (char)token);
        exit(-1);
    }
    tokenize();
}

void check_local_id()
{
    if (token != Id)
    {
        printf("in line %lld: invalid identifer\n", line);
        exit(-1);
    }
    if (symbol_ptr[symbol_count].class == Loc)
    {
        printf("in line %lld: duplicate declaration\n", line);
        exit(-1);
    }
}

void check_new_id()
{
    if (token != Id)
    {
        printf("in line %lld: invalid identifer\n", line);
        exit(-1);
    }
    if (symbol_ptr[symbol_count].class)
    {
        printf("in line %lld: duplicate declaration\n", line);
        exit(-1);
    }
}

// number
// void parse_enum()
// {
//     int i;
//     i = 0; // enum index
//     while (token != '}')
//     {
//         check_new_id();
//         assert(Id);
//         // handle custom enum index
//         if (token == Assign)
//         {
//             assert(Assign);
//             assert(Num_int);
//             i = token_val;
//         }
//         symbol_ptr[Class] = Num_int;
//         symbol_ptr[Type] = INT;
//         symbol_ptr[Value] = i++;
//         if (token == ',')
//             tokenize();
//     }
// }

void parse_enum()
{
    int i;
    float f;
    int is_float = 0; // flag to determine if the value is a float
    i = 0;            // enum index

    while (token != '}')
    {
        check_new_id();
        assert(Id);

        // handle custom enum index
        if (token == Assign)
        {
            assert(Assign);
            if (token == Num_int)
            {
                assert(Num_int);
                is_float = 0;
                i = token_val;
            }
            else if (token == Num_float)
            {
                assert(Num_float);
                is_float = 1;
                f = float_val;
            }
        }

        symbol_ptr[symbol_count].class = is_float ? Num_float : Num_int;
        symbol_ptr[symbol_count].type = is_float ? FLOAT : INT;
        if (is_float)
        {
            memcpy(&symbol_ptr[symbol_count].value, &f, sizeof(float));
        }
        else
        {
            symbol_ptr[symbol_count].value = i++;
        }

        if (token == ',')
            tokenize();
    }
}

// symbol
int parse_base_type()
{
    // parse base type
    if (token == Char)
    {
        assert(Char);
        return CHAR;
    }
    else if (token == Float)
    {
        assert(Float);
        return FLOAT;
    }
    else
    {
        assert(Int);
        return INT;
    }
}

void hide_global()
{
    symbol_ptr[symbol_count].Gclass = symbol_ptr[symbol_count].class;
    symbol_ptr[symbol_count].Gtype = symbol_ptr[symbol_count].type;
    symbol_ptr[symbol_count].Gvalue = symbol_ptr[symbol_count].value;
}

void recover_global()
{
    symbol_ptr[symbol_count].class = symbol_ptr[symbol_count].Gclass;
    symbol_ptr[symbol_count].type = symbol_ptr[symbol_count].Gtype;
    symbol_ptr[symbol_count].value = symbol_ptr[symbol_count].Gvalue;
}

// void foo() {
//     int x = 5;
//     在这里调用 hide_global() 来隐藏全局变量 x
//     hide_global();
//     处理局部变量 x 的代码
//     当离开局部作用域时，调用 recover_global() 来恢复全局变量 x
//     recover_global();
// }

int ibp; // number of params
void parse_param()
{
    int type, i;
    i = 0;
    while (token != ')')
    {
        type = parse_base_type();
        // parse pointer's star
        while (token == Mul)
        {
            assert(Mul);
            type = type + PTR;
        }
        check_local_id();
        assert(Id);
        hide_global();
        symbol_ptr[symbol_count].class = Loc;
        symbol_ptr[symbol_count].type = type;
        symbol_ptr[symbol_count].value = i++;
        if (token == ',')
            assert(',');
    }
    ibp = ++i;
}

int type; // pass type in recursive parse expr
void parse_expr(int precd)
{
    int tmp_type, i;
    Symbol *tmp_ptr;
    int *jmp_ptr;
    // const number
    if (token == Num_int)
    {
        tokenize();
        *++code = IMM;
        *++code = token_val;
        type = INT;
    }
    else if (token == Num_float)
    {
        *++code = IMM;
        *++code = *(int *)&float_val;
        tokenize();
        type = FLOAT;
    }
    // const string
    else if (token == '"')
    {
        *++code = IMM;
        *++code = token_val; // string addr
        assert('"');
        while (token == '"')
            assert('"');                     // handle multi-row
        data = (char *)((int)data + 8 & -8); // add \0 for string & align 8
        type = PTR;
    }
    else if (token == Sizeof)
    {
        tokenize();
        assert('(');
        type = parse_base_type();
        while (token == Mul)
        {
            assert(Mul);
            type = type + PTR;
        }
        assert(')');
        *++code = IMM;
        *++code = (type == CHAR) ? 1 : 8;
        type = INT;
    }
    // handle identifer: variable or function all
    else if (token == Id)
    {
        tokenize();
        tmp_ptr = symbol_ptr; // for recursive parse
        // function call
        if (token == '(')
        {
            assert('(');
            i = 0; // number of args
            while (token != ')')
            {
                parse_expr(Assign);
                *++code = PUSH;
                i++;
                if (token == ',')
                    assert(',');
            }
            assert(')');
            // native call
            if (tmp_ptr[symbol_count].class == Sys)
                *++code = tmp_ptr[symbol_count].value;
            // fun call
            else if (tmp_ptr[symbol_count].class == Fun)
            {
                *++code = CALL;
                *++code = tmp_ptr[symbol_count].value;
            }
            else
            {
                printf("in line %lld: invalid function call\n", line);
                exit(-1);
            }
            // delete stack frame for args
            if (i > 0)
            {
                *++code = DARG;
                *++code = i;
            }
            type = tmp_ptr[symbol_count].type;
        }
        // handle enum value
        else if (tmp_ptr[symbol_count].class == Num_int)
        {
            *++code = IMM;
            *++code = tmp_ptr[symbol_count].value;
            type = INT;
        }
        // handle float variable
        else if (tmp_ptr[symbol_count].class == Num_float)
        {
            *++code = IMM;
            *++code = *(int *)&tmp_ptr[symbol_count].value; // assuming Value stores float
            type = FLOAT;
        }
        // handle variables
        else
        {
            // local var, calculate addr base ibp
            if (tmp_ptr[symbol_count].class == Loc)
            {
                *++code = LEA;
                *++code = ibp - tmp_ptr[symbol_count].value;
            }
            // global var
            else if (tmp_ptr[symbol_count].class == Glo)
            {
                *++code = IMM;
                *++code = tmp_ptr[symbol_count].value;
            }
            else
            {
                printf("in line %lld: invalid variable\n", line);
                exit(-1);
            }
            type = tmp_ptr[symbol_count].type;
            *++code = (type == CHAR) ? LC : (type == FLOAT) ? LF
                                                            : LI;
        }
    }
    // cast or parenthesis
    else if (token == '(')
    {
        assert('(');
        if (token == Char || token == Int || token == Float)
        {
            int is_float = 0;
            if (token == Float)
                is_float = 1;

            tokenize();
            tmp_type = (is_float ? FLOAT : (token - Char + CHAR));

            while (token == Mul)
            {
                assert(Mul);
                tmp_type = tmp_type + PTR;
            }
            // use precedence Inc represent all unary operators
            assert(')');
            parse_expr(Inc);
            type = tmp_type;
        }
        else
        {
            parse_expr(Assign);
            assert(')');
        }
    }

    // derefer
    else if (token == Mul)
    {
        tokenize();
        parse_expr(Inc);
        if (type >= PTR)
            type = type - PTR;
        else
        {
            printf("in line %lld: invalid dereference\n", line);
            exit(-1);
        }
        *++code = (type == CHAR) ? LC : (type == FLOAT) ? LF
                                                        : LI;
    }
    // reference
    else if (token == And)
    {
        tokenize();
        parse_expr(Inc);
        if (*code == LC || *code == LI || *code == LF)
            code--; // rollback load by addr
        else
        {
            printf("line %lld: invalid reference\n", line);
            exit(-1);
        }
        type = type + PTR;
    }
    // Not
    else if (token == '!')
    {
        tokenize();
        parse_expr(Inc);
        *++code = PUSH;
        *++code = IMM;
        *++code = 0;
        *++code = EQ;
        type = INT;
    }
    // bitwise
    else if (token == '~')
    {
        tokenize();
        parse_expr(Inc);
        if (type != INT)
        {
            printf("line %lld: invalid type for bitwise NOT\n", line);
            exit(-1);
        }
        *++code = PUSH;
        *++code = IMM;
        *++code = -1;
        *++code = XOR;
        type = INT;
    }
    // positive
    else if (token == And)
    {
        tokenize();
        parse_expr(Inc);
        type = INT;
    }
    // negative
    else if (token == Sub)
    {
        tokenize();
        parse_expr(Inc);
        *++code = PUSH;
        *++code = IMM;
        if (type == FLOAT)
        {
            float neg_one = -1.0f;
            *++code = *(int *)&neg_one;
            *++code = FMUL;
        }
        else
        {
            *++code = -1;
            *++code = MUL;
            type = INT;
        }
    }
    // ++var --var
    else if (token == Inc || token == Dec)
    {
        i = token;
        tokenize();
        parse_expr(Inc);
        // save var addr, then load var val
        if (*code == LC)
        {
            *code = PUSH;
            *++code = LC;
        }
        else if (*code == LI)
        {
            *code = PUSH;
            *++code = LI;
        }
        else
        {
            printf("in line %lld: invalid Inc or Dec\n", line);
            exit(-1);
        }
        *++code = PUSH; // save var val
        *++code = IMM;
        *++code = (type > PTR) ? 8 : 1;
        *++code = (i == Inc) ? ADD : SUB;   // calculate
        *++code = (type == CHAR) ? SC : SI; // write back to var addr
    }
    else
    {
        printf("in line %lld: invalid expression\n", line);
        exit(-1);
    }
    // use [precedence climbing] method to handle binary(or postfix) operators
    while (token >= precd)
    {
        tmp_type = type;
        // assignment
        if (token == Assign)
        {
            tokenize();
            if (*code == LC || *code == LI || *code == LF)
                *code = PUSH;
            else
            {
                printf("in line %lld: invalid assignment\n", line);
                exit(-1);
            }
            parse_expr(Assign);
            type = tmp_type; // type can be cast
            *++code = (type == CHAR) ? SC : (type == FLOAT) ? SF
                                                            : SI;
        }
        // ? :, same as if stmt
        else if (token == Cond)
        {
            tokenize();
            *++code = JZ;
            tmp_ptr = ++code;
            parse_expr(Assign);
            assert(':');
            *tmp_ptr = (int)(code + 3);
            *++code = JMP;
            tmp_ptr = ++code; // save endif addr
            parse_expr(Cond);
            *tmp_ptr = (int)(code + 1); // write back endif point
        }
        // logic operators, simple and boring
        else if (token == Lor)
        {
            tokenize();
            *++code = JNZ;
            tmp_ptr = ++code;
            parse_expr(Land);
            *tmp_ptr = (int)(code + 1);
            type = INT;
        }
        else if (token == Land)
        {
            tokenize();
            *++code = JZ;
            tmp_ptr = ++code;
            parse_expr(Or);
            *tmp_ptr = (int)(code + 1);
            type = INT;
        }
        else if (token == Or)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Xor);
            *++code = OR;
            type = INT;
        }
        else if (token == Xor)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(And);
            *++code = XOR;
            type = INT;
        }
        else if (token == And)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Eq);
            *++code = AND;
            type = INT;
        }
        else if (token == Eq)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Lt);
            *++code = EQ;
            type = INT;
        }
        else if (token == Ne)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Lt);
            *++code = NE;
            type = INT;
        }
        else if (token == Lt)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Shl);
            *++code = LT;
            type = INT;
        }
        else if (token == Gt)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Shl);
            *++code = GT;
            type = INT;
        }
        else if (token == Le)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Shl);
            *++code = LE;
            type = INT;
        }
        else if (token == Ge)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Shl);
            *++code = GE;
            type = INT;
        }
        else if (token == Shl)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Add);
            *++code = SHL;
            type = INT;
        }
        else if (token == Shr)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Add);
            *++code = SHR;
            type = INT;
        }
        // arithmetic operators
        else if (token == Add)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Mul);

            if (tmp_type == FLOAT)
            {
                *++code = FADD;
            }
            else
            {
                // int pointer * 8
                if (tmp_type > PTR)
                {
                    *++code = PUSH;
                    *++code = IMM;
                    *++code = 8;
                    *++code = MUL;
                }
                *++code = ADD;
            }
            type = tmp_type;
        }
        else if (token == Sub)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Mul);
            if (tmp_type == FLOAT)
            {
                *++code = FSUB;
            }
            else
            {
                if (tmp_type > PTR && tmp_type == type)
                {
                    // pointer - pointer, ret / 8
                    *++code = SUB;
                    *++code = PUSH;
                    *++code = IMM;
                    *++code = 8;
                    *++code = DIV;
                    type = INT;
                }
                else if (tmp_type > PTR)
                {
                    *++code = PUSH;
                    *++code = IMM;
                    *++code = 8;
                    *++code = MUL;
                    *++code = SUB;
                    type = tmp_type;
                }
                else
                    *++code = SUB;
            }
        }
        else if (token == Mul)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Inc);
            *++code = MUL;
            type = INT;
        }
        else if (token == Div)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Inc);
            *++code = DIV;
            type = INT;
        }
        else if (token == Mod)
        {
            tokenize();
            *++code = PUSH;
            parse_expr(Inc);
            *++code = MOD;
            type = INT;
        }
        // var++, var--
        else if (token == Inc || token == Dec)
        {
            if (*code == LC)
            {
                *code = PUSH;
                *++code = LC;
            } // save var addr
            else if (*code == LI)
            {
                *code = PUSH;
                *++code = LI;
            }
            else
            {
                printf("in line %lld: invalid operator=%lld\n", line, token);
                exit(-1);
            }
            *++code = PUSH;
            *++code = IMM;
            *++code = (type > PTR) ? 8 : 1;
            *++code = (token == Inc) ? ADD : SUB;
            *++code = (type == CHAR) ? SC : SI; // save value ++ or -- to addr
            *++code = PUSH;
            *++code = IMM;
            *++code = (type > PTR) ? 8 : 1;
            *++code = (token == Inc) ? SUB : ADD; // restore value before ++ or --
            tokenize();
        }
        // a[x] = *(a + x)
        else if (token == Brak)
        {
            assert(Brak);
            *++code = PUSH;
            parse_expr(Assign);
            assert(']');
            if (tmp_type > PTR)
            {
                *++code = PUSH;
                *++code = IMM;
                *++code = 8;
                *++code = MUL;
            }
            else if (tmp_type < PTR)
            {
                printf("in line %lld: invalid index op\n", line);
                exit(-1);
            }
            *++code = ADD;
            type = tmp_type - PTR;
            *++code = (type == CHAR) ? LC : (type == FLOAT) ? LF
                                                            : LI;
        }
        else
        {
            printf("in line %lld: invalid token=%lld\n", line, token);
            exit(-1);
        }
    }
}

void parse_stmt()
{
    int *a;
    int *b;
    if (token == If)
    {
        assert(If);
        assert('(');
        parse_expr(Assign);
        assert(')');
        *++code = JZ;
        b = ++code;   // JZ to false
        parse_stmt(); // parse true stmt
        if (token == Else)
        {
            assert(Else);
            *b = (int)(code + 3); // write back false point
            // JZ_addr, JMP, JMP_addr
            *++code = JMP;
            b = ++code;   // JMP to endif
            parse_stmt(); // parse false stmt
        }
        *b = (int)(code + 1); // write back endif point
    }
    else if (token == While)
    {
        assert(While);
        a = code + 1; // write loop point
        assert('(');
        parse_expr(Assign);
        assert(')');
        *++code = JZ;
        b = ++code; // JZ to endloop
        parse_stmt();
        *++code = JMP;
        *++code = (int)a;     // JMP to loop point
        *b = (int)(code + 1); // write back endloop point
    }
    else if (token == Return)
    {
        assert(Return);
        if (token != ';')
            parse_expr(Assign);
        assert(';');
        *++code = RET;
    }
    else if (token == '{')
    {
        assert('{');
        while (token != '}')
            parse_stmt(Assign);
        assert('}');
    }
    else if (token == ';')
        assert(';');
    else
    {
        parse_expr(Assign);
        assert(';');
    }
}

void parse_fun()
{
    int type, i;
    i = ibp; // bp handle by NVAR itself.
    // local variables must be declare in advance
    while (token == Char || token == Int)
    {
        type = parse_base_type();
        while (token != ';')
        {
            // parse pointer's star
            while (token == Mul)
            {
                assert(Mul);
                type = type + PTR;
            }
            check_local_id();
            assert(Id);
            hide_global();
            symbol_ptr[symbol_count].class = Loc;
            symbol_ptr[symbol_count].type = type;
            symbol_ptr[symbol_count].value = ++i;
            if (token == ',')
                assert(',');
        }
        assert(';');
    }
    // new stack frame for vars
    *++code = NVAR;
    // stack frame size
    *++code = i - ibp;
    while (token != '}')
        parse_stmt();
    if (*code != RET)
        *++code = RET; // void function
    // recover global variables
    symbol_ptr = symbol_table;
    while (symbol_ptr[Token])
    {
        if (symbol_ptr[Class] == Loc)
            recover_global();
        symbol_ptr = symbol_ptr + SymSize;
    }
}

void parse()
{
    int type, base_type;
    int *p;
    line = 1;
    token = 1; // just for loop condition
    while (token > 0)
    {
        tokenize(); // start or skip last ; | }
        // parse enum
        if (token == Enum)
        {
            assert(Enum);
            if (token != '{')
                assert(Id); // skip enum name
            assert('{');
            parse_enum();
            assert('}');
        }
        else if (token == Int || token == Char || token == Float)
        {
            base_type = parse_base_type();
            // parse var or func definition
            while (token != ';' && token != '}')
            {
                // parse pointer's star
                type = base_type;
                while (token == Mul)
                {
                    assert(Mul);
                    type = type + PTR;
                }
                check_new_id();
                assert(Id);
                symbol_ptr[Type] = type;
                if (token == '(')
                {
                    // function
                    symbol_ptr[Class] = Fun;
                    symbol_ptr[Value] = (int)(code + 1);
                    assert('(');
                    parse_param();
                    assert(')');
                    assert('{');
                    parse_fun();
                }
                else
                {
                    // variable
                    symbol_ptr[Class] = Glo;
                    symbol_ptr[Value] = (int)data;
                    data = data + 8; // keep 64 bits for each var
                }
                // handle int a,b,c;
                if (token == ',')
                    assert(',');
            }
        }
    }
}

void keyword()
{
    int i;
    src = "char int float enum if else return sizeof while "
          "open read close printf malloc free memset memcmp exit void main";
    // add keywords to symbol table
    i = Char;
    while (i <= While)
    {
        tokenize();
        if (symbol_ptr[Token] != i - 1)
        {
            printf("Error adding keyword to symbol table\n");
            exit(-1);
        }
        symbol_ptr[Token] = i++;
    }
    // add Native CALL to symbol table
    i = OPEN;
    while (i <= EXIT)
    {
        tokenize();
        if (symbol_ptr[Class] != 0 || symbol_ptr[Type] != 0 || symbol_ptr[Value] != 0)
        {
            printf("Error adding system call to symbol table\n");
            exit(-1);
        }
        symbol_ptr[Class] = Sys;
        symbol_ptr[Type] = INT;
        symbol_ptr[Value] = i++;
    }
    tokenize();
    if (symbol_ptr[Token] != 0)
    {
        printf("Error handling void type\n");
        exit(-1);
    }
    symbol_ptr[Token] = Char; // handle void type
    tokenize();
    if (symbol_ptr[Token] != 0)
    {
        printf("Error keeping track of main\n");
        exit(-1);
    }
    main_ptr = symbol_ptr; // keep track of main
    src = src_dump;
}

int init_vm()
{
    // allocate memory for virtual machine
    if (!(code = code_dump = malloc(MAX_SIZE)))
    {
        printf("could not malloc(%lld) for code segment\n", MAX_SIZE);
        return -1;
    }
    if (!(data = malloc(MAX_SIZE)))
    {
        printf("could not malloc(%lld) for data segment\n", MAX_SIZE);
        return -1;
    }
    if (!(stack = malloc(MAX_SIZE)))
    {
        printf("could not malloc(%lld) for stack segment\n", MAX_SIZE);
        return -1;
    }
    if (!(symbol_table = malloc(MAX_SIZE / 16)))
    {
        printf("could not malloc(%lld) for symbol_table\n", MAX_SIZE / 16);
        return -1;
    }
    memset(code, 0, MAX_SIZE);
    memset(data, 0, MAX_SIZE);
    memset(stack, 0, MAX_SIZE);
    memset(symbol_table, 0, MAX_SIZE / 16);
    return 0;
}

int run_vm(int argc, char **argv)
{
    int op;
    int *tmp;
    // exit code for main
    bp = sp = (int *)((int)stack + MAX_SIZE);
    *--sp = EXIT;
    *--sp = PUSH;
    tmp = sp;
    *--sp = argc;
    *--sp = (int)argv;
    *--sp = (int)tmp;
    if (!(pc = (int *)main_ptr[Value]))
    {
        printf("main function is not defined\n");
        exit(-1);
    }
    cycle = 0;
    while (1)
    {
        cycle++;
        op = *pc++; // read instruction
        // load & save
        if (op == IMM)
            ax = *pc++; // load immediate(or global addr)
        else if (op == LEA)
            ax = (int)(bp + *pc++); // load local addr
        else if (op == LC)
            ax = *(char *)ax; // load char
        else if (op == LI)
            ax = *(int *)ax; // load int
        else if (op == LF)
            ax = *(float *)ax; // load float
        else if (op == SC)
            *(char *)*sp++ = ax; // save char to stack
        else if (op == SI)
            *(int *)*sp++ = ax; // save int to stack
        else if (op == SF)
            *(float *)*sp++ = *(float *)&ax; // save float to stack
        else if (op == PUSH)
            *--sp = ax; // push ax to stack
        // jump
        else if (op == JMP)
            pc = (int *)*pc; // jump
        else if (op == JZ)
            pc = ax ? pc + 1 : (int *)*pc; // jump if ax == 0
        else if (op == JNZ)
            pc = ax ? (int *)*pc : pc + 1; // jump if ax != 0
        // arithmetic
        else if (op == OR)
            ax = *sp++ | ax;
        else if (op == XOR)
            ax = *sp++ ^ ax;
        else if (op == AND)
            ax = *sp++ & ax;
        else if (op == EQ)
            ax = *sp++ == ax;
        else if (op == NE)
            ax = *sp++ != ax;
        else if (op == LT)
            ax = *sp++ < ax;
        else if (op == LE)
            ax = *sp++ <= ax;
        else if (op == GT)
            ax = *sp++ > ax;
        else if (op == GE)
            ax = *sp++ >= ax;
        else if (op == SHL)
            ax = *sp++ << ax;
        else if (op == SHR)
            ax = *sp++ >> ax;
        else if (op == ADD)
            ax = *sp++ + ax;
        else if (op == SUB)
            ax = *sp++ - ax;
        else if (op == MUL)
            ax = *sp++ * ax;
        else if (op == DIV)
            ax = *sp++ / ax;
        else if (op == MOD)
            ax = *sp++ % ax;
        else if (op == FADD)
            ax = *(float *)&ax + *(float *)sp++;
        else if (op == FSUB)
            ax = *(float *)&ax - *(float *)sp++;
        else if (op == FMUL)
            ax = *(float *)&ax * *(float *)sp++;
        // some complicate instructions for function call
        // call function: push pc + 1 to stack & pc jump to func addr(pc point to)
        else if (op == CALL)
        {
            *--sp = (int)(pc + 1);
            pc = (int *)*pc;
        }
        // new stack frame for vars: save bp, bp -> caller stack, stack add frame
        else if (op == NVAR)
        {
            *--sp = (int)bp;
            bp = sp;
            sp = sp - *pc++;
        }
        // delete stack frame for args: same as x86 : add esp, <size>
        else if (op == DARG)
            sp = sp + *pc++;
        // return caller: retore stack, retore old bp, pc point to caller code addr(store by CALL)
        else if (op == RET)
        {
            sp = bp;
            bp = (int *)*sp++;
            pc = (int *)*sp++;
        }
        // end for call function.
        // native call
        else if (op == OPEN)
        {
            ax = open((char *)sp[1], sp[0]);
        }
        else if (op == CLOS)
        {
            ax = close(*sp);
        }
        else if (op == READ)
        {
            ax = read(sp[2], (char *)sp[1], *sp);
        }
        else if (op == PRTF)
        {
            tmp = sp + pc[1] - 1;
            ax = printf((char *)tmp[0], tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5]);
        }
        else if (op == MALC)
        {
            ax = (int)malloc(*sp);
        }
        else if (op == FREE)
        {
            free((void *)*sp);
        }
        else if (op == MSET)
        {
            ax = (int)memset((char *)sp[2], sp[1], *sp);
        }
        else if (op == MCMP)
        {
            ax = memcmp((char *)sp[2], (char *)sp[1], *sp);
        }
        else if (op == EXIT)
        {
            printf("exit(%lld)\n", *sp);
            return *sp;
        }
        else
        {
            printf("unkown instruction: %lld, cycle: %lld\n", op, cycle);
            return -1;
        }
    }
    return 0;
}

char *insts; // just for debug
void write_as()
{
    int fd;
    char *buffer;
    insts = "IMM ,LEA ,JMP ,JZ  ,JNZ ,CALL,NVAR,DARG,RET ,LI  ,LC  ,SI  ,SC  ,PUSH,"
            "OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,"
            "LF  ,SF  ,FADD,FSUB,FMUL,"
            "OPEN,READ,CLOS,PRTF,MALC,FREE,MSET,MCMP,EXIT,";
    fd = open("assemble", 0x0001 | 0x0200);
    buffer = malloc(100);
    while (code_dump < code)
    {
        sprintf(buffer, "(%lld) %8.4s", ++code_dump, insts + (*code_dump * 5));
        write(fd, buffer, strlen(buffer));
        if (*code_dump < RET)
            sprintf(buffer, " %lld\n", *++code_dump);
        else
        {
            buffer[0] = '\n';
            buffer[1] = '\0';
        }
        write(fd, buffer, strlen(buffer));
    }
    close(fd);
    free(buffer);
}

int load_src(char *file)
{
    int fd, cnt;
    // use open/read/close for bootstrap.
    if ((fd = open(file, 0)) < 0)
    {
        printf("could not open source code(%s)\n", file);
        return -1;
    }
    if (!(src = src_dump = malloc(MAX_SIZE)))
    {
        printf("could not malloc(%lld) for source code\n", MAX_SIZE);
        return -1;
    }
    if ((cnt = read(fd, src, MAX_SIZE - 1)) <= 0)
    {
        printf("could not read source code(%lld)\n", cnt);
        return -1;
    }
    src[cnt] = 0;      // EOF
    if (close(fd) < 0) // Check if close is successful
    {
        printf("could not close the file descriptor\n");
        free(src); // Free allocated memory
        return -1;
    }
    return 0;
}

void save_tokens_to_file(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }
    for (int i = 0; i < token_count; i++)
    {
        fprintf(file, "%d, %s\n", token_list[i].token, token_list[i].lexeme);
    }
    fclose(file);
}

// after bootstrap use [int] istead of [int32_t]
int32_t main(int32_t argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <source_code_file>\n", argv[0]);
        return -1;
    }
    MAX_SIZE = 128 * 1024 * 8; // 1MB = 128k * 64bit

    // load source code
    if (load_src(*(argv + 1)) != 0)
    {
        printf("Error: Failed to load source code from %s\n", argv[1]);
        return -1;
    }

    // init memory & register
    if (init_vm() != 0)
    {
        printf("Error: Failed to initialize virtual machine\n");
        return -1;
    }

    // prepare keywords for symbol table
    keyword();
    // parse and generate vm instructions, save to vm
    parse();
    // print assembles: vm instructions. for debug
    write_as();
    // run vm and execute instructions
    int result = run_vm(--argc, ++argv);

    // Cleanup resources if necessary
    // For example, free memory allocated for source code

    return result;
}