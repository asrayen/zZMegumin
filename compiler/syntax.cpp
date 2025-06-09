#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <string.h>
using namespace std;

char s[100][100] = {"\0"}; // �����洢��ʼ����
string str;                // �����洢���Ϻ������
int location = 0;          // ������λ�������ʽ
bool flag = true;          // �����жϸ��������ʽ�Ƿ�Ϸ�
string tree_map[100];      // �����洢�﷨��
const int width = 3;       // ���ü��Ϊ3
char token[100] = {"\0"};  // �����ݴ浥��
string error;              // ������¼������Ϣ

string tetrad[100]; // �����洢��Ԫʽ
int tetradNum = 0;  // ��¼��Ԫʽ�ĸ���

struct IDs
{
    string name = ""; // ��ʶ��������
    int type;         // ��Ϊ�ķ������У���������ֻ��int������type������ֻ������ȡֵ��1��ʾΪint���ͣ�0��ʾ������
    int value;        // ��ʶ����ֵ
    bool rel;         // ����boolֵ�Ĵ洢
};
IDs ID[100];   // �����洢��ʶ�������ͺ�ֵ
int IDNum = 0; // ���ڼ�¼��ʶ��������

struct LInfo
{
    int row; // ������ʶ�﷨��������
    int column;
    int interval = 0; // ��������֮����Ҫ���ֵļ��
    int addr;         // ����ָ���addr����Ԫʽ��������������ѭ�������ȷ����ת�ĵ�ַ
    IDs id;
}; // �����洢L�����ķ���Ҫ���ݵ���Ϣ�Լ����﷨����Ҫ����Ϣ

bool isKey(char *s);
bool isOP(char *s);
bool isDE(char &s);
void pre_process(char *buff, int &in_comment);
bool scanner(int k);

char keywords[34][20] = { // �ؼ���,����main���ڹ���34��
    "auto", "short", "int", "long", "float", "double", "char", "struct",
    "union", "enum", "typedef", "const", "unsigned", "signed", "extern",
    "register", "static", "volatile", "void", "if", "else", "switch",
    "case", "for", "do", "while", "goto", "continue", "break", "default",
    "sizeof", "return", "main", "include"};
char operators[38][10] = { // �����,��38��
    "+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", ">=", "<", "<=",
    "&&", "||", "!", "=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=",
    "^=", "|=", "&", "|", "^", "~", "<<", ">>", "?", ":", ",", ".", "->"};
char delimiters[7] = {'(', ')', '[', ']', '{', '}', ';'}; // �ָ���,��7��

bool isKey(char *s)
{ // �����ж��ַ����Ƿ�Ϊ�ؼ���,���򷵻�true,���򷵻�false
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(s, keywords[i]) == 0)
            return true;
    }
    return false;
}

bool isOP(char *s)
{ // �����ж��ַ����Ƿ�Ϊ�����,���򷵻�true,���򷵻�false
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++)
    {
        if (strcmp(s, operators[i]) == 0)
            return true;
    }
    return false;
}

bool isDE(char &s)
{ // �����ж��ַ��Ƿ�Ϊ�ָ���,���򷵻�true,���򷵻�false
    if (strchr(delimiters, s) != NULL)
        return true;
    return false;
}

void pre_process(char *buff, int &in_comment)
{                            // Ԥ����
    char data[100] = {'\0'}; // �����洢�����������
    char old_c = '\0';       // �����洢��һ���ַ�
    char cur_c;              // �����洢��ǰ�ַ�
    int i = 0;               // ������,��¼buff
    int j = 0;               // ����������¼data
    while (i < strlen(buff))
    {                      // ȥע��
        cur_c = buff[i++]; // ���Ƚ���ȡ���ַ����뻺����
        switch (in_comment)
        {
        case 0:
            if (cur_c == '\"')
            { // ����˫������
                data[j++] = cur_c;
                in_comment = 3;
            }
            else if (cur_c == '\'')
            { // ���뵥������
                data[j++] = cur_c;
                in_comment = 4;
            }
            else if (old_c == '/' && cur_c == '*')
            { // �������ע����
                j--;
                in_comment = 1;
            }
            else if (old_c == '/' && cur_c == '/')
            { // ���뵥��ע����
                j--;
                in_comment = 2;
            }
            else
            { // ���������ֱ�ӽ�����д��data��
                data[j++] = cur_c;
            }
            break;
        case 1:
            if (old_c == '*' && cur_c == '/')
                in_comment = 0; // ����ע�ͽ���
            break;
        case 2:
            if (i == strlen(buff))
                in_comment = 0; // ����ע�͵����н���ʱ��־λ��Ϊ0
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
        old_c = cur_c; // ������һ���ַ�
    }

    i = 0;
    int k = 0;
    while (k < j)
    { // �ָ���
        if (isalpha(data[k]) || data[k] == '_')
        { // ��Ϊ��ĸ��_
            while (!isDE(data[k]) && strchr("+-*/%=^~&|!><?:,.", data[k]) == NULL && !isspace(data[k]))
            {
                buff[i++] = data[k++];
            }
            buff[i++] = ' ';
        }
        else if (isdigit(data[k]))
        { // ��Ϊ����
            while (isdigit(data[k]))
            {
                buff[i++] = data[k++];
            }
            buff[i++] = ' ';
        }
        else if (isspace(data[k]))
        {
            while (isspace(data[k]))
            { // ��Ϊ�հ��ַ�
                k++;
            }
        }
        else if (isDE(data[k]))
        { // ��Ϊ���
            buff[i++] = data[k++];
            buff[i++] = ' ';
        }
        else if (data[k] == '\"')
        { // ��Ϊ˫����
            buff[i++] = data[k++];
            while (data[k] != '\"')
                buff[i++] = data[k++];
            buff[i++] = data[k++];
            buff[i++] = ' ';
        }
        else if (data[k] == '\'')
        { // ��Ϊ������
            buff[i++] = data[k++];
            while (data[k] != '\'')
                buff[i++] = data[k++];
            buff[i++] = data[k++];
            buff[i++] = ' ';
        }
        else if (strchr("+-*/%=^~&|!><?:,.", data[k]) != NULL)
        { // ��Ϊ�����,�ٲ鿴��һ���ַ�,Ҫ�����ܶ����һЩ�����
            switch (data[k])
            {
            case '+':
                buff[i++] = data[k++];
                if (data[k] == '+' || data[k] == '=')
                    buff[i++] = data[k++]; // Ϊ++��+=�����
                break;
            case '-':
                buff[i++] = data[k++];
                if (data[k] == '-' || data[k] == '=' || data[k] == '>')
                    buff[i++] = data[k++]; // Ϊ--��-=��->�����
                break;
            case '*':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // Ϊ*=�����
                break;
            case '/':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // Ϊ/=�����
                break;
            case '%':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // Ϊ%=�����
                break;
            case '=':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // Ϊ==�����
                break;
            case '^':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // Ϊ^=�����
                break;
            case '&':
                buff[i++] = data[k++];
                if (data[k] == '&' || data[k] == '=')
                    buff[i++] = data[k++]; // Ϊ&&��&=�����
                break;
            case '|':
                buff[i++] = data[k++];
                if (data[k] == '|' || data[k] == '=')
                    buff[i++] = data[k++]; // Ϊ||��|=�����
                break;
            case '!':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // Ϊ!=�����
                break;
            case '>':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // Ϊ>=�����
                else if (data[k] == '>')
                {
                    buff[i++] = data[k++]; // Ϊ>>�����
                    if (data[k] == '=')
                        buff[i++] = data[k++]; // Ϊ>>=�����
                }
                break;
            case '<':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // Ϊ<=�����
                else if (data[k] == '<')
                {
                    buff[i++] = data[k++]; // Ϊ<<�����
                    if (data[k] == '<')
                        buff[i++] = data[k++]; // Ϊ<<=�����
                }
                break;
            default:
                buff[i++] = data[k++];
            }
            buff[i++] = ' ';
        }
    }
    buff[i] = '\0'; // �������Ժ�,�����������һ���ո�
}

bool scanner(int k)
{                       // �ʷ���������
    int in_comment = 0; // 0��ʾû����,1��ʾ�ڶ���ע����,2��ʾ�ڵ���ע����,3��ʾ��˫������,4��ʾ�ڵ�������
    for (int i = 0; i < k; i++)
    {
        pre_process(s[i], in_comment); // ����Ԥ����,ȥ��ע��,�����֮�䡢���������֮����һ���ո����
    }
    if (in_comment != 0)
        return false; // ����־λ������0,��˵������ע�Ͳ���λ,û�н�����־
    else
        return true;
}

int draw_line(int row, int num);
void string_out(string s, int row, int column, int loc);
int tree_out(string s, int row, int loc);
void printTree(ofstream &fout);
int readToken();
void bindString(int k);
int findID(const string &words);
void printSequence(ofstream &fout);
LInfo Block(LInfo info);
LInfo Decls(LInfo info);
LInfo NameList(LInfo info);
LInfo NameList1(LInfo info);
bool Type(char *words);
bool Name(char *words);
LInfo STMTS(LInfo info);
LInfo STMT(LInfo info);
LInfo STMT1(LInfo info);
LInfo BOOL(LInfo info);
bool RelOp(char *words);
LInfo Expr(LInfo info);
LInfo Expr1(LInfo info);
LInfo Term(LInfo info);
LInfo Term1(LInfo info);
LInfo Factor(LInfo info);
bool AddOp(char *words);
bool MulOp(char *words);
int getData();

int draw_line(int row, int num)
{ // ����������,�����ֵܽڵ�,�����´ο�ʼ����ʼλ��
    tree_map[row].append(num, '-');
    return tree_map[row].size();
}

/**��������ַ���
 * ����columnΪ���е���ʼλ��,locΪ��һ�����ߵ�λ��,
 * locĬ��Ϊ0,��ʾû������,���ʱͨ��column�����ַ������뵽��Ӧλ��
 * ����Ϊ0,��ͨ��loc�Ը��ַ�������λ�õĴ���
 */
void string_out(string s, int row, int column, int loc = 0)
{
    if (loc == 0)
    {
        if (tree_map[row].size() < column)
        { // ������,��˵���м���Ҫ���ո�
            int n = column - tree_map[row].size();
            tree_map[row].append(n, ' ');
        }
        tree_map[row].append(s);
    }
    else
    {
        int n1 = s.size() / 2;
        if (loc - n1 <= column)
        { // ���ýڵ�ĳ��ȱȸ��ڵ㳤,����ͨ��column���
            if (tree_map[row].size() < column)
            { // ������,��˵���м���Ҫ���ո�
                int n = column - tree_map[row].size();
                tree_map[row].append(n, ' ');
            }
            tree_map[row].append(s);
        }
        else
        { // ��������������ո�
            int n = loc - n1 - tree_map[row].size();
            tree_map[row].append(n, ' ');
            tree_map[row].append(s);
        }
    }
}

/**�����ӽڵ�֮�������,s��ʾ���׽ڵ���ַ�,loc��ʾ���׽ڵ����ʼλ��
 * ����ֵ���ڴ����������λ��
 */
int tree_out(string s, int row, int column)
{
    int n1 = s.size() / 2;
    int n2 = column + n1 - tree_map[row].size();
    tree_map[row].append(n2, ' ');
    tree_map[row] += '|';
    return n1 + column;
}

void printTree(ofstream &fout)
{
    for (int i = 0; i < 100; i++)
    {
        if (!tree_map[i].empty())
        {
            fout << tree_map[i] << endl;
        }
        else
            break;
    }
}

int readToken()
{ // �������ݿո��str��ȡ�ʣ������ظôʵĳ��ȣ��Ա������λ����
    int i = 0;
    for (; str[location + i] != ' '; i++)
    {
        token[i] = str[location + i];
    }
    token[i] = '\0';
    return i;
}

void bindString(int k)
{ // ������s�����е��������ϵ�str��
    for (int i = 0; i <= k; i++)
    {
        str.append(s[i]);
    }
}

int findID(const string &words)
{ // �ӷ��ű��в��ң����ҵ����򷵻ض�Ӧ��λ�ã����򷵻�-1
    for (int i = 0; i < IDNum; i++)
    {
        if (words == ID[i].name)
            return i;
    }
    return -1;
}

void printSequence(ofstream &fout)
{
    for (int i = 0; i < tetradNum; i++)
    {
        fout << i << " : " << tetrad[i] << endl;
    }
}

LInfo Block(LInfo info)
{
    if (flag)
    {
        string_out("<Block>", info.row, info.column);
        int loc = tree_out("<Block>", ++info.row, info.column);
        int i = readToken();
        if (strcmp(token, "{") == 0)
        {
            location = location + i + 1;
            string_out(token, ++info.row, info.column, loc);
            info.column = draw_line(info.row, width);
            LInfo info1 = Decls(info);
            if (!flag)
                return info;
            info.column = draw_line(info.row, info1.interval + width);
            LInfo info2 = STMTS(info);
            if (!flag)
                return info;
            info.column = draw_line(info.row, info2.interval + width);
            i = readToken();
            if (strcmp(token, "}") == 0)
            {
                location = location + i + 1;
                string_out(token, info.row, info.column);
                info.interval = info1.interval + info2.interval + width * 3 + 1 + 7;
                return info;
            }
            else
            {
                string s = token;
                error = s + "֮ǰȱ��}";
                flag = false;
                return info;
            }
        }
        else
        {
            string s = token;
            error = s + "֮ǰȱ��{";
            flag = false;
            return info;
        }
    }
}

LInfo Decls(LInfo info)
{
    if (flag)
    {
        string_out("<Decls>", info.row, info.column);
        int loc = tree_out("<Decls>", ++info.row, info.column);
        int i = readToken();
        if (Type(token))
        {
            info.id.type = 1; // ��ǵ�ǰ������Ϊint����
            location = location + i + 1;
            string_out("<Type>", ++info.row, info.column, loc);
            loc = tree_out("<Type>", info.row + 1, info.column);
            string_out(token, info.row + 2, info.column, loc);
            info.column = draw_line(info.row, width);
            LInfo info1 = NameList(info);
            if (!flag)
                return info;
            info.column = draw_line(info.row, info1.interval + width);
            i = readToken();
            if (strcmp(token, ";") == 0)
            {
                location = location + i + 1;
                string_out(token, info.row, info.column);
                info.column = draw_line(info.row, width);
                LInfo info2 = Decls(info);
                if (!flag)
                    return info;
                info.interval = info1.interval + info2.interval + width * 3 + 1 + 7;
                return info;
            }
            else
            {
                string s = token;
                error = s + "֮ǰȱ��;";
                flag = false;
                return info;
            }
        }
        else
        { // �������Ϊempty
            string_out("empty", ++info.row, info.column, loc);
            info.interval = 7;
            return info;
        }
    }
}

LInfo NameList(LInfo info)
{
    if (flag)
    {
        string_out("<NameList>", info.row, info.column);
        int loc = tree_out("<NameList>", ++info.row, info.column);
        int i = readToken();
        if (Name(token))
        {
            ID[IDNum].name = token; // ����ʶ��������ű���
            ID[IDNum].type = info.id.type;
            IDNum++;
            location = location + i + 1;
            string_out("<Name>", ++info.row, info.column, loc);
            loc = tree_out("<Name>", info.row + 1, info.column + 2);
            string_out(token, info.row + 2, info.column, loc);
            info.column = draw_line(info.row, width);
            LInfo info1 = NameList1(info);
            if (!flag)
                return info;
            info.interval = info1.interval + width + 10;
            return info;
        }
        else
        {
            string s = token;
            error = s + "֮ǰȱ��id";
            flag = false;
            return info;
        }
    }
}

LInfo NameList1(LInfo info)
{
    if (flag)
    {
        string_out("<NameList1>", info.row, info.column);
        int loc = tree_out("<NameList1>", ++info.row, info.column);
        int i = readToken();
        if (strcmp(token, ",") == 0)
        {
            location = location + i + 1;
            string_out(token, ++info.row, info.column, loc);
            info.column = draw_line(info.row, width);
            i = readToken();
            if (Name(token))
            {
                ID[IDNum].name = token; // ����ʶ��������ű���
                ID[IDNum].type = info.id.type;
                IDNum++;
                location = location + i + 1;
                string_out("<Name>", ++info.row, info.column);
                tree_out("<Name>", info.row + 1, info.column);
                string_out(token, info.row + 2, info.column);
                info.column = draw_line(info.row, width);
                LInfo info1 = NameList1(info);
                if (!flag)
                    return info;
                info.interval = info1.interval + 6 + width * 2 + 11;
                return info;
            }
            else
            {
                string s = token;
                error = s + "֮ǰȱ��id";
                flag = false;
                return info;
            }
        }
        else
        { // �������Ϊempty
            string_out("empty", ++info.row, info.column, loc);
            info.interval = 11;
            return info;
        }
    }
}

bool Type(char *words)
{
    if (strcmp(words, "int") == 0)
        return true;
    else
        return false;
}

bool Name(char *words)
{
    if (!isOP(words) && !isKey(words) && !isDE(words[0]) && !isdigit(words[0]) && words[0] != '\'' && words[0] != '\"')
    {
        if (words[0] == '_' || isalpha(words[0]))
            return true;
    }
    return false;
}

LInfo STMTS(LInfo info)
{
    if (flag)
    {
        string_out("<STMTS>", info.row, info.column);
        int loc = tree_out("<STMTS>", ++info.row, info.column);
        int i = readToken();
        if (Name(token) || strcmp(token, "if") == 0 || strcmp(token, "while") == 0)
        {
            ++info.row;
            LInfo info1 = STMT(info);
            if (!flag)
                return info;
            info.column = draw_line(info.row, info1.interval + width);
            LInfo info2 = STMTS(info);
            if (!flag)
                return info;
            info.interval = info1.interval + info2.interval + 7;
            return info;
        }
        else
        { // �������Ϊempty
            string_out("empty", ++info.row, info.column, loc);
            info.interval = 7;
            return info;
        }
    }
}

LInfo STMT(LInfo info)
{
    if (flag)
    {
        string_out("<STMT>", info.row, info.column);
        int loc = tree_out("<STMT>", ++info.row, info.column);
        int i = readToken();
        location = location + i + 1;
        if (Name(token))
        { // ���Ǳ�ʶ��
            int num = findID(token);
            if (num == -1)
            { // ��Ϊ�Ǹ�ֵ��䣬����������Ҫ�жϸñ�ʶ���Ƿ���ǰ���Ѿ��������ˣ���δ�����򱨴�
                string s = token;
                error = "��ʶ��\"" + s + "\"δ����";
                flag = false;
                return info;
            }
            string_out("<Name>", ++info.row, info.column, loc);
            tree_out("<Name>", info.row + 1, info.column);
            string_out(token, info.row + 2, info.column, loc);
            info.column = draw_line(info.row, width);
            i = readToken();
            if (strcmp(token, "=") == 0)
            {
                location = location + i + 1;
                string_out(token, info.row, info.column);
                info.column = draw_line(info.row, width);
                LInfo info1 = Expr(info);
                if (!flag)
                    return info;
                ID[num].value = info1.id.value;
                tetrad[tetradNum++] = "(=, " + to_string(info1.id.value) + ", _, " + ID[num].name + ")";
                info.column = draw_line(info.row, info1.interval + width);
                i = readToken();
                if (strcmp(token, ";") == 0)
                {
                    location = location + i + 1;
                    string_out(token, info.row, info.column);
                    info.interval = info1.interval + width * 3 + 2 + 6;
                    return info;
                }
                else
                {
                    string s = token;
                    error = s + "֮ǰȱ��;";
                    flag = false;
                    return info;
                }
            }
            else
            {
                string s = token;
                error = s + "֮ǰȱ��=";
                flag = false;
                return info;
            }
        }
        else if (strcmp(token, "if") == 0)
        {
            string_out(token, ++info.row, info.column, loc);
            info.column = draw_line(info.row, width);
            i = readToken();
            if (strcmp(token, "(") == 0)
            {
                location = location + i + 1;
                string_out(token, info.row, info.column);
                info.column = draw_line(info.row, width);
                LInfo info1 = BOOL(info);
                if (!flag)
                    return info;
                info.column = draw_line(info.row, info1.interval + width);
                tetrad[tetradNum++] = "(jnz, BOOL, _, ";
                info.addr = tetradNum - 1;
                i = readToken();
                if (strcmp(token, ")") == 0)
                {
                    location = location + i + 1;
                    string_out(token, info.row, info.column);
                    info.column = draw_line(info.row, width);
                    LInfo info2 = STMT(info);
                    if (!flag)
                        return info;
                    info.column = draw_line(info.row, info2.interval + width);
                    tetrad[tetradNum++] = "(jmp, _, _, ";
                    tetrad[info.addr] = tetrad[info.addr] + to_string(tetradNum) + ")";
                    info.addr = tetradNum - 1;

                    LInfo info3 = STMT1(info);
                    if (!flag)
                        return info;
                    info.interval = info1.interval + info2.interval + info3.interval + width * 5 + 2 + 6;
                    tetrad[info.addr] = tetrad[info.addr] + to_string(tetradNum) + ")";
                    return info;
                }
                else
                {
                    string s = token;
                    error = s + "֮ǰȱ��)";
                    flag = false;
                    return info;
                }
            }
            else
            {
                string s = token;
                error = s + "֮ǰȱ��(";
                flag = false;
                return info;
            }
        }
        else if (strcmp(token, "while") == 0)
        {
            string_out(token, ++info.row, info.column, loc);
            info.column = draw_line(info.row, width);
            i = readToken();
            if (strcmp(token, "(") == 0)
            {
                location = location + i + 1;
                string_out(token, info.row, info.column);
                info.column = draw_line(info.row, width);
                LInfo info1 = BOOL(info);
                if (!flag)
                    return info;
                info.column = draw_line(info.row, info1.interval + width);
                i = readToken();
                if (strcmp(token, ")") == 0)
                {
                    location = location + i + 1;
                    string_out(token, info.row, info.column);
                    info.column = draw_line(info.row, width);
                    tetrad[tetradNum++] = "(jnz, BOOL, _, ";
                    info.addr = tetradNum - 1;

                    LInfo info2 = STMT(info);
                    if (!flag)
                        return info;
                    tetrad[tetradNum++] = "(jmp, _, _, " + to_string(info.addr) + ")";
                    tetrad[info.addr] = tetrad[info.addr] + to_string(tetradNum) + ")";
                    info.interval = info1.interval + info2.interval + width * 4 + 2 + 6;
                    return info;
                }
                else
                {
                    string s = token;
                    error = s + "֮ǰȱ��)";
                    flag = false;
                    return info;
                }
            }
            else
            {
                string s = token;
                error = s + "֮ǰȱ��(";
                flag = false;
                return info;
            }
        }
        else
        {
            string s = token;
            error = s + "֮ǰȱ��id";
            flag = false;
            return info;
        }
    }
}

LInfo STMT1(LInfo info)
{
    if (flag)
    {
        string_out("<STMT1>", info.row, info.column);
        int loc = tree_out("<STMT1>", ++info.row, info.column);
        int i = readToken();
        if (strcmp(token, "else") == 0)
        {
            location = location + i + 1;
            string_out(token, ++info.row, info.column, loc);
            info.column = draw_line(info.row, width);
            LInfo info1 = STMT(info);
            if (!flag)
                return info;
            info.interval = info1.interval + width + 7;
            return info;
        }
        else
        { // �������Ϊempty
            string_out("empty", ++info.row, info.column, loc);
            info.interval = 7;
            return info;
        }
    }
}

LInfo BOOL(LInfo info)
{
    if (flag)
    {
        string_out("<BOOL>", info.row, info.column);
        tree_out("<BOOL>", ++info.row, info.column);
        ++info.row;
        LInfo info1 = Expr(info);
        if (!flag)
            return info;
        info.column = draw_line(info.row, info1.interval + width);
        int i = readToken();
        if (RelOp(token))
        { // ���ǹ�ϵ�����
            location = location + i + 1;
            string_out("<RelOp>", info.row, info.column);
            int loc = tree_out("<RelOp>", info.row + 1, info.column);
            string_out(token, info.row + 2, loc);
            info.column = draw_line(info.row, width);
            string temp = token;
            LInfo info2 = Expr(info);
            if (!flag)
                return info;
            if (temp == "<")
            { // ����C++�����ԣ���������Ľ��1��ʾtrue��0��ʾfalse
                bool result = info1.id.value < info2.id.value;
                tetrad[tetradNum++] = "(<, " + to_string(info1.id.value) + ", " + to_string(info2.id.value) + ", " + to_string(result) + ")";
                info.id.rel = result;
            }
            else if (temp == "<=")
            {
                bool result = info1.id.value <= info2.id.value;
                tetrad[tetradNum++] = "(<=, " + to_string(info1.id.value) + ", " + to_string(info2.id.value) + ", " + to_string(result) + ")";
                info.id.rel = result;
            }
            else if (temp == ">")
            {
                bool result = info1.id.value > info2.id.value;
                tetrad[tetradNum++] = "(>, " + to_string(info1.id.value) + ", " + to_string(info2.id.value) + ", " + to_string(result) + ")";
                info.id.rel = result;
            }
            else if (temp == ">=")
            {
                bool result = info1.id.value >= info2.id.value;
                tetrad[tetradNum++] = "(>=, " + to_string(info1.id.value) + ", " + to_string(info2.id.value) + ", " + to_string(result) + ")";
                info.id.rel = result;
            }
            else if (temp == "==")
            {
                bool result = info1.id.value == info2.id.value;
                tetrad[tetradNum++] = "(==, " + to_string(info1.id.value) + ", " + to_string(info2.id.value) + ", " + to_string(result) + ")";
                info.id.rel = result;
            }
            else
            {
                bool result = info1.id.value != info2.id.value;
                tetrad[tetradNum++] = "(!=, " + to_string(info1.id.value) + ", " + to_string(info2.id.value) + ", " + to_string(result) + ")";
                info.id.rel = result;
            }
            info.interval = info1.interval + info2.interval + width * 2 + 6;
            return info;
        }
        else
        {
            string s = token;
            error = s + "֮ǰȱ�ٹ�ϵ�����";
            flag = false;
            return info;
        }
    }
}

bool RelOp(char *words)
{
    if (strcmp(words, "<") == 0 || strcmp(words, "<=") == 0 || strcmp(words, ">") == 0 ||
        strcmp(words, ">=") == 0 || strcmp(words, "==") == 0 || strcmp(words, "!=") == 0)
    {
        return true;
    }
    return false;
}

LInfo Expr(LInfo info)
{
    if (flag)
    {
        string_out("<Expr>", info.row, info.column);
        tree_out("<Expr>", ++info.row, info.column);
        ++info.row;
        LInfo info1 = Term(info);
        if (!flag)
            return info;
        info.column = draw_line(info.row, info1.interval + width);
        info.id = info1.id;
        LInfo info2 = Expr1(info);
        if (!flag)
            return info;
        info.interval = info1.interval + info2.interval + width + 6;
        info.id = info2.id;
        return info;
    }
}

LInfo Expr1(LInfo info)
{
    if (flag)
    {
        string_out("<Expr1>", info.row, info.column);
        int loc = tree_out("<Expr1>", ++info.row, info.column);
        int i = readToken();
        if (AddOp(token))
        { // ���ַ�Ϊ+��-
            location = location + i + 1;
            string_out("<AddOp>", ++info.row, info.column, loc);
            loc = tree_out("<AddOp>", info.row + 1, info.column);
            string_out(token, info.row + 2, info.column, loc);
            info.column = draw_line(info.row, width);
            string temp = token;
            LInfo info1 = Term(info);
            if (!flag)
                return info;
            if (temp == "+")
            {
                int result = info.id.value + info1.id.value;
                tetrad[tetradNum++] = "(+, " + to_string(info.id.value) + ", " + to_string(info1.id.value) + ", " + to_string(result) + ")";
                info.id.value = result;
            }
            else
            {
                int result = info.id.value - info1.id.value;
                tetrad[tetradNum++] = "(-, " + to_string(info.id.value) + ", " + to_string(info1.id.value) + ", " + to_string(result) + ")";
                info.id.value = result;
            }
            info.column = draw_line(info.row, info1.interval + width);
            LInfo info2 = Expr1(info);
            if (!flag)
                return info;
            info.interval = info1.interval + info2.interval + width * 2 + 7;
            info.id = info2.id;
            return info;
        }
        else
        { // �������Ϊempty
            string_out("empty", ++info.row, info.column, loc);
            info.interval = 7;
            return info;
        }
    }
}

LInfo Term(LInfo info)
{
    if (flag)
    {
        string_out("<Term>", info.row, info.column);
        tree_out("<Term>", ++info.row, info.column);
        ++info.row;
        LInfo info1 = Factor(info);
        if (!flag)
            return info;
        info.column = draw_line(info.row, info1.interval + width);
        info.id = info1.id;
        LInfo info2 = Term1(info);
        if (!flag)
            return info;
        info.interval = info1.interval + info2.interval + width + 6;
        info.id = info2.id;
        return info;
    }
}

LInfo Term1(LInfo info)
{
    if (flag)
    {
        string_out("<Term1>", info.row, info.column);
        int loc = tree_out("<Term1>", ++info.row, info.column);
        int i = readToken();
        if (MulOp(token))
        { // ���ַ�Ϊ*��/
            location = location + i + 1;
            string_out("<MulOp>", ++info.row, info.column, loc);
            loc = tree_out("<MulOp>", info.row + 1, info.column);
            string_out(token, info.row + 2, info.column, loc);
            info.column = draw_line(info.row, width);
            string temp = token;
            LInfo info1 = Factor(info);
            if (!flag)
                return info;
            if (temp == "*")
            {
                int result = info.id.value * info1.id.value;
                tetrad[tetradNum++] = "(*, " + to_string(info.id.value) + ", " + to_string(info1.id.value) + ", " + to_string(result) + ")";
                info.id.value = result;
            }
            else
            {
                int result = info.id.value / info1.id.value;
                tetrad[tetradNum++] = "(/, " + to_string(info.id.value) + ", " + to_string(info1.id.value) + ", " + to_string(result) + ")";
                info.id.value = result;
            }
            info.column = draw_line(info.row, info1.interval + width);
            LInfo info2 = Term1(info);
            if (!flag)
                return info;
            info.interval = info1.interval + info2.interval + width * 2 + 7;
            info.id = info2.id;
            return info;
        }
        else
        { // �������Ϊempty
            string_out("empty", ++info.row, info.column, loc);
            info.interval = 7;
            return info;
        }
    }
}

LInfo Factor(LInfo info)
{
    if (flag)
    {
        string_out("<Factor>", info.row, info.column);
        int loc = tree_out("<Factor>", ++info.row, info.column);
        int i = readToken();
        location = location + i + 1;
        if (Name(token))
        {
            int num = findID(token);
            if (num == -1)
            {
                string s = token;
                error = "��ʶ��\"" + s + "\"δ����";
                flag = false;
                return info;
            }
            info.id = ID[num];
            string_out(token, ++info.row, info.column, loc);
            info.interval = 8;
            return info;
        }
        else if (isdigit(token[0]))
        {
            LInfo info1;
            info1.id.value = atoi(token);
            info1.id.type = 1;
            info.id = info1.id;
            string_out(token, ++info.row, info.column, loc);
            info.interval = 8;
            return info;
        }
        else if (strcmp(token, "(") == 0)
        {
            string_out(token, ++info.row, info.column, loc);
            info.column = draw_line(info.row, width);
            LInfo info1 = Expr(info);
            if (!flag)
                return info;
            i = readToken();
            if (strcmp(token, ")") == 0)
            {
                location = location + i + 1;
                info.column = draw_line(info.row, info1.interval + width);
                string_out(token, info.row, info.column);
                info.interval = info1.interval + width * 2 + 8;
                info.id = info1.id;
                return info;
            }
            else
            { // ��һֱû��),��˵�����������ʽ����
                string s = token;
                error = s + "֮ǰȱ��)";
                flag = false;
                return info;
            }
        }
        else
        {
            string s = token;
            error = s + "֮ǰȱ��id";
            flag = false;
            return info;
        }
    }
}

bool AddOp(char *words)
{
    if (strcmp(words, "+") == 0 || strcmp(words, "-") == 0)
        return true;
    return false;
}

bool MulOp(char *words)
{
    if (strcmp(words, "*") == 0 || strcmp(words, "/") == 0)
        return true;
    return false;
}

int getData()
{
    int k = 0;
    cout << "������һ�������(#��ʾ����)��" << endl;
    cin.getline(s[0], 100);
    while (k < 100 && strcmp(s[k], "#") != 0)
    {
        cin.getline(s[++k], 100);
    }
    return k;
}

int main()
{
    int k = getData(); // ��ȡ����
    if (scanner(k))
    {                  // �Ƚ��дʷ�����
        bindString(k); // �����������������ϵ�string���͵�str��
        cout << str << endl;
        LInfo info;
        info.row = info.column = 0;
        Block(info); // �����﷨����
        if (str[location] == '#')
        {
            cout << "Correct!" << endl;
            cout << "����������﷨��!" << endl;
            ofstream fout("TreeOut.txt");
            printTree(fout);
            cout << "�﷨������ɹ�!�����TreeOut.txt�ļ�!" << endl;
            fout.close();
            fout.open("fourout.txt");
            printSequence(fout);
            fout.close();
            cout << "��Ԫʽ����ɹ�!�����fourout.txt�ļ�!" << endl;
        }
        else
        {
            cout << "Error!�﷨����δͨ��!" << endl;
            cout << error << endl; // ��������Ϣ���
        }
    }
    else
    {
        cout << "Error!Ԥ����δͨ��!" << endl;
    }
    return 0;
}
