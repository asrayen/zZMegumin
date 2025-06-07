#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <string.h>
#define MAXN 1024 // 缓冲区的最大容??
using namespace std;

string token;     // 用于暂存单词序列
string ID[MAXN];  // 用于存储程序中的所有标识符
int IDNum = 0;    // 用于记录标识符的数量
int digitNum = 0; // 用于记录常数的数??

char keywords[33][20] = { // 关键??,包括main在内共有33??
    "auto", "short", "int", "long", "float", "double", "char", "struct",
    "union", "enum", "typedef", "const", "unsigned", "signed", "extern",
    "register", "static", "volatile", "void", "if", "else", "switch",
    "case", "for", "do", "while", "goto", "continue", "break", "default",
    "sizeof", "return", "main"};
char operators[38][10] = { // 运算??,??38??
    "+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", ">=", "<", "<=",
    "&&", "||", "!", "=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=",
    "^=", "|=", "&", "|", "^", "~", "<<", ">>", "?", ":", ",", ".", "->"};
char delimiters[7] = {'(', ')', '[', ']', '{', '}', ';'}; // 分隔??,??7??

bool isKey(char *s)
{ // 用来判断字符串是否为关键??,是则返回true,否则返回false
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(s, keywords[i]) == 0)
            return true;
    }
    return false;
}

bool isOP(char *s)
{ // 用来判断字符串是否为运算??,是则返回true,否则返回false
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++)
    {
        if (strcmp(s, operators[i]) == 0)
            return true;
    }
    return false;
}

bool isDE(char &s)
{ // 用来判断字符是否为分隔符,是则返回true,否则返回false
    if (strchr(delimiters, s) != NULL)
        return true;
    return false;
}

int findID(const string &s)
{ // 找出标识符在ID中的位置,找到返回数组的下??,否则返回-1
    int i = 0;
    while (i < IDNum)
    {
        if (ID[i] == s)
            return i;
        i++;
    }
    return -1;
}

void pre_process(char *buff, int &in_comment)
{                             // 预处??
    char data[MAXN] = {'\0'}; // 用来存储处理过的数据
    char old_c = '\0';        // 用来存储上一个字??
    char cur_c;               // 用来存储当前字符
    int i = 0;                // 计数??,记录buff
    int j = 0;                // 计数器，记录data
    while (i < strlen(buff))
    {                      // 去注??
        cur_c = buff[i++]; // 首先将获取的字符存入缓存??
        switch (in_comment)
        {
        case 0:
            if (cur_c == '\"')
            { // 进入双引号中
                data[j++] = cur_c;
                in_comment = 3;
            }
            else if (cur_c == '\'')
            { // 进入单引号中
                data[j++] = cur_c;
                in_comment = 4;
            }
            else if (old_c == '/' && cur_c == '*')
            { // 进入多行注释??
                j--;
                in_comment = 1;
            }
            else if (old_c == '/' && cur_c == '/')
            { // 进入单行注释??
                j--;
                in_comment = 2;
            }
            else
            { // 其他情况则直接将数据写入data??
                data[j++] = cur_c;
            }
            break;
        case 1:
            if (old_c == '*' && cur_c == '/')
                in_comment = 0; // 多行注释结束
            break;
        case 2:
            if (i == strlen(buff))
                in_comment = 0; // 单行注释到这行结束时标志位置??0
            break;
        case 3:
            data[j++] = cur_c;
            if (cur_c == '\"')
                in_comment = 0;
            break;
        case 4:
            data[j++] = cur_c;
            if (cur_c == '\'')
                in_comment = 0;
            break;
        }
        old_c = cur_c; // 保留上一个字??
    }

    i = 0;
    int k = 0;
    while (k < j)
    { // 分隔??
        if (isalpha(data[k]) || data[k] == '_')
        { // 若为字母或_
            while (!isDE(data[k]) && strchr("+-*/%=^~&|!><?:,.", data[k]) == NULL && !isspace(data[k]))
            {
                buff[i++] = data[k++];
            }
            buff[i++] = ' ';
        }
        else if (isdigit(data[k]))
        { // 若为数字
            while (isdigit(data[k]))
            {
                buff[i++] = data[k++];
            }
            buff[i++] = ' ';
        }
        else if (isspace(data[k]))
        {
            while (isspace(data[k]))
            { // 若为空白字符
                k++;
            }
        }
        else if (isDE(data[k]))
        { // 若为界符
            buff[i++] = data[k++];
            buff[i++] = ' ';
        }
        else if (data[k] == '\"')
        { // 若为双引??
            buff[i++] = data[k++];
            while (data[k] != '\"')
                buff[i++] = data[k++];
            buff[i++] = data[k++];
            buff[i++] = ' ';
        }
        else if (data[k] == '\'')
        { // 若为单引??
            buff[i++] = data[k++];
            while (data[k] != '\'')
                buff[i++] = data[k++];
            buff[i++] = data[k++];
            buff[i++] = ' ';
        }
        else if (strchr("+-*/%=^~&|!><?:,.", data[k]) != NULL)
        { // 若为运算??,再查看下一个字??,要尽可能多包含一些运算符
            switch (data[k])
            {
            case '+':
                buff[i++] = data[k++];
                if (data[k] == '+' || data[k] == '=')
                    buff[i++] = data[k++]; // ??++??+=运算??
                break;
            case '-':
                buff[i++] = data[k++];
                if (data[k] == '-' || data[k] == '=' || data[k] == '>')
                    buff[i++] = data[k++]; // ??--??-=??->运算??
                break;
            case '*':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??*=运算??
                break;
            case '/':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??/=运算??
                break;
            case '%':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??%=运算??
                break;
            case '=':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??==运算??
                break;
            case '^':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // 为^=运算??
                break;
            case '&':
                buff[i++] = data[k++];
                if (data[k] == '&' || data[k] == '=')
                    buff[i++] = data[k++]; // ??&&??&=运算??
                break;
            case '|':
                buff[i++] = data[k++];
                if (data[k] == '|' || data[k] == '=')
                    buff[i++] = data[k++]; // 为||或|=运算??
                break;
            case '!':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??!=运算??
                break;
            case '>':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??>=运算??
                else if (data[k] == '>')
                {
                    buff[i++] = data[k++]; // ??>>运算??
                    if (data[k] == '=')
                        buff[i++] = data[k++]; // ??>>=运算??
                }
                break;
            case '<':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??<=运算??
                else if (data[k] == '<')
                {
                    buff[i++] = data[k++]; // ??<<运算??
                    if (data[k] == '<')
                        buff[i++] = data[k++]; // ??<<=运算??
                }
                break;
            default:
                buff[i++] = data[k++];
            }
            buff[i++] = ' ';
        }
    }
    if (i > 0)
        buff[i] = '\0'; // 处理完以??,会在最后留上一个空??
    else
        buff[i] = '\0';
}

void scanner(ifstream &fin, ofstream &fout)
{
    char buff[MAXN] = {'\0'};  // 用来存储读入的数??
    char words[MAXN] = {'\0'}; // 用来存储??
    bool first = true;         // 用来判断是否为第一个词
    int in_comment = 0;        // 0表示当前字符未在注释??,1表示在多行注释中,2表示在单行注释中,3表示在双引号??,4表示在单引号??
    char cur_c;                // 用来存储当前字符
    int i = 0;                 // 计数??,记录buff
    int j = 0;                 // 计数??,用来记录每个词的长度
    while (fin.getline(buff, sizeof(buff)))
    {
        pre_process(buff, in_comment); // 首先预处??,去掉注释,词与词之间、词与运算符之间用一个空格隔开

        while (i < strlen(buff))
        {
            if (buff[i] == ' ' && j > 0)
            { // 遇到空格,则说明上一个词已经结束??
                words[j] = '\0';
                token = words;
                if (isOP(words) || isKey(words) || isDE(words[0]))
                { // 若是运算符、关键字和界??,处理方法一??
                    // cout << "(" << token << ", _)" << endl;
                    if (!first)
                        fout << '\n';
                    else
                        first = false;
                    fout << "(" << token << ", _)";
                }
                else if (isdigit(words[0]))
                { // 若是常数
                    if (j == 1)
                    { // 若常数的位数??1,则不用判断第一个是否为0
                        // cout << "(" << token << ", " << 1 + digitNum++ << ")" << endl;
                        if (!first)
                            fout << '\n';
                        else
                            first = false;
                        fout << "(" << token << ", " << 1 + digitNum++ << ")";
                    }
                    else
                    {
                        if (words[0] == '0')
                        {
                            // cout << "(" << token << ", " << 1 + digitNum++ << ")" << endl;
                            if (!first)
                                fout << '\n';
                            else
                                first = false;
                            fout << "(" << token << ", " << 1 + digitNum++ << ")";
                        }
                        else
                        { // 常数的第一个字符不能为0
                            cout << "ERROR: The first digit cannot be 0!" << endl;
                        }
                    }
                }
                else if (words[0] == '\'' || words[0] == '\"')
                { // 若是字符串常??
                    // cout << "(" << token << ", _)" << endl;
                    if (!first)
                        fout << '\n';
                    else
                        first = false;
                    fout << "(" << token << ", _)";
                }
                else
                { // 否则为标识符
                    if (token.length() <= 15)
                    {                                 // 标识符最长为15个字??
                        int location = findID(token); // 首先判断该标识符是否已经存在??
                        if (location == -1)
                        { // 若没有找??,则将该标识符存入ID数组??,并且可以知道该标识符所在的位置就是IDNum的??
                            ID[IDNum++] = token;
                            // cout << "(" << token << ", " << IDNum << ")" << endl;
                            if (!first)
                                fout << '\n';
                            else
                                first = false;
                            fout << "(" << token << ", " << IDNum << ")";
                        }
                        else
                        {
                            // cout << "(" << token << ", " << location + 1 << ")" << endl;
                            if (!first)
                                fout << '\n';
                            else
                                first = false;
                            fout << "(" << token << ", " << location + 1 << ")";
                        }
                    }
                    else
                    {
                        cout << "ERROR Identifier length can't exceed 15 characters" << endl;
                    }
                }
                j = 0;
            }
            else
            {
                words[j++] = buff[i];
            }
            i++;
        }
        i = j = 0;
    }
    if (in_comment != 0)
        fout << '\n'
             << "error"; // 若标志位不等??0,则说明多行注释不到位,没有结束标志
}

int main()
{
    ifstream fin("input.txt");
    if (!fin.is_open())
        cout << "文件不存在!";
    ofstream fout("output.txt");
    scanner(fin, fout);

    fin.close();
    fout.close();
    return 0;
}
