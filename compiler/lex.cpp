#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <string.h>
#define MAXN 1024 // �������������??
using namespace std;

string token;     // �����ݴ浥������
string ID[MAXN];  // ���ڴ洢�����е����б�ʶ��
int IDNum = 0;    // ���ڼ�¼��ʶ��������
int digitNum = 0; // ���ڼ�¼��������??

char keywords[33][20] = { // �ؼ�??,����main���ڹ���33??
    "auto", "short", "int", "long", "float", "double", "char", "struct",
    "union", "enum", "typedef", "const", "unsigned", "signed", "extern",
    "register", "static", "volatile", "void", "if", "else", "switch",
    "case", "for", "do", "while", "goto", "continue", "break", "default",
    "sizeof", "return", "main"};
char operators[38][10] = { // ����??,??38??
    "+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", ">=", "<", "<=",
    "&&", "||", "!", "=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=",
    "^=", "|=", "&", "|", "^", "~", "<<", ">>", "?", ":", ",", ".", "->"};
char delimiters[7] = {'(', ')', '[', ']', '{', '}', ';'}; // �ָ�??,??7??

bool isKey(char *s)
{ // �����ж��ַ����Ƿ�Ϊ�ؼ�??,���򷵻�true,���򷵻�false
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(s, keywords[i]) == 0)
            return true;
    }
    return false;
}

bool isOP(char *s)
{ // �����ж��ַ����Ƿ�Ϊ����??,���򷵻�true,���򷵻�false
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

int findID(const string &s)
{ // �ҳ���ʶ����ID�е�λ��,�ҵ������������??,���򷵻�-1
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
{                             // Ԥ��??
    char data[MAXN] = {'\0'}; // �����洢�����������
    char old_c = '\0';        // �����洢��һ����??
    char cur_c;               // �����洢��ǰ�ַ�
    int i = 0;                // ����??,��¼buff
    int j = 0;                // ����������¼data
    while (i < strlen(buff))
    {                      // ȥע??
        cur_c = buff[i++]; // ���Ƚ���ȡ���ַ����뻺��??
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
            { // �������ע��??
                j--;
                in_comment = 1;
            }
            else if (old_c == '/' && cur_c == '/')
            { // ���뵥��ע��??
                j--;
                in_comment = 2;
            }
            else
            { // ���������ֱ�ӽ�����д��data??
                data[j++] = cur_c;
            }
            break;
        case 1:
            if (old_c == '*' && cur_c == '/')
                in_comment = 0; // ����ע�ͽ���
            break;
        case 2:
            if (i == strlen(buff))
                in_comment = 0; // ����ע�͵����н���ʱ��־λ��??0
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
        old_c = cur_c; // ������һ����??
    }

    i = 0;
    int k = 0;
    while (k < j)
    { // �ָ�??
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
        { // ��Ϊ˫��??
            buff[i++] = data[k++];
            while (data[k] != '\"')
                buff[i++] = data[k++];
            buff[i++] = data[k++];
            buff[i++] = ' ';
        }
        else if (data[k] == '\'')
        { // ��Ϊ����??
            buff[i++] = data[k++];
            while (data[k] != '\'')
                buff[i++] = data[k++];
            buff[i++] = data[k++];
            buff[i++] = ' ';
        }
        else if (strchr("+-*/%=^~&|!><?:,.", data[k]) != NULL)
        { // ��Ϊ����??,�ٲ鿴��һ����??,Ҫ�����ܶ����һЩ�����
            switch (data[k])
            {
            case '+':
                buff[i++] = data[k++];
                if (data[k] == '+' || data[k] == '=')
                    buff[i++] = data[k++]; // ??++??+=����??
                break;
            case '-':
                buff[i++] = data[k++];
                if (data[k] == '-' || data[k] == '=' || data[k] == '>')
                    buff[i++] = data[k++]; // ??--??-=??->����??
                break;
            case '*':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??*=����??
                break;
            case '/':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??/=����??
                break;
            case '%':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??%=����??
                break;
            case '=':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??==����??
                break;
            case '^':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // Ϊ^=����??
                break;
            case '&':
                buff[i++] = data[k++];
                if (data[k] == '&' || data[k] == '=')
                    buff[i++] = data[k++]; // ??&&??&=����??
                break;
            case '|':
                buff[i++] = data[k++];
                if (data[k] == '|' || data[k] == '=')
                    buff[i++] = data[k++]; // Ϊ||��|=����??
                break;
            case '!':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??!=����??
                break;
            case '>':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??>=����??
                else if (data[k] == '>')
                {
                    buff[i++] = data[k++]; // ??>>����??
                    if (data[k] == '=')
                        buff[i++] = data[k++]; // ??>>=����??
                }
                break;
            case '<':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // ??<=����??
                else if (data[k] == '<')
                {
                    buff[i++] = data[k++]; // ??<<����??
                    if (data[k] == '<')
                        buff[i++] = data[k++]; // ??<<=����??
                }
                break;
            default:
                buff[i++] = data[k++];
            }
            buff[i++] = ' ';
        }
    }
    if (i > 0)
        buff[i] = '\0'; // ��������??,�����������һ����??
    else
        buff[i] = '\0';
}

void scanner(ifstream &fin, ofstream &fout)
{
    char buff[MAXN] = {'\0'};  // �����洢�������??
    char words[MAXN] = {'\0'}; // �����洢??
    bool first = true;         // �����ж��Ƿ�Ϊ��һ����
    int in_comment = 0;        // 0��ʾ��ǰ�ַ�δ��ע��??,1��ʾ�ڶ���ע����,2��ʾ�ڵ���ע����,3��ʾ��˫����??,4��ʾ�ڵ�����??
    char cur_c;                // �����洢��ǰ�ַ�
    int i = 0;                 // ����??,��¼buff
    int j = 0;                 // ����??,������¼ÿ���ʵĳ���
    while (fin.getline(buff, sizeof(buff)))
    {
        pre_process(buff, in_comment); // ����Ԥ��??,ȥ��ע��,�����֮�䡢���������֮����һ���ո����

        while (i < strlen(buff))
        {
            if (buff[i] == ' ' && j > 0)
            { // �����ո�,��˵����һ�����Ѿ�����??
                words[j] = '\0';
                token = words;
                if (isOP(words) || isKey(words) || isDE(words[0]))
                { // ������������ؼ��ֺͽ�??,������һ??
                    // cout << "(" << token << ", _)" << endl;
                    if (!first)
                        fout << '\n';
                    else
                        first = false;
                    fout << "(" << token << ", _)";
                }
                else if (isdigit(words[0]))
                { // ���ǳ���
                    if (j == 1)
                    { // ��������λ��??1,�����жϵ�һ���Ƿ�Ϊ0
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
                        { // �����ĵ�һ���ַ�����Ϊ0
                            cout << "ERROR: The first digit cannot be 0!" << endl;
                        }
                    }
                }
                else if (words[0] == '\'' || words[0] == '\"')
                { // �����ַ�����??
                    // cout << "(" << token << ", _)" << endl;
                    if (!first)
                        fout << '\n';
                    else
                        first = false;
                    fout << "(" << token << ", _)";
                }
                else
                { // ����Ϊ��ʶ��
                    if (token.length() <= 15)
                    {                                 // ��ʶ���Ϊ15����??
                        int location = findID(token); // �����жϸñ�ʶ���Ƿ��Ѿ�����??
                        if (location == -1)
                        { // ��û����??,�򽫸ñ�ʶ������ID����??,���ҿ���֪���ñ�ʶ�����ڵ�λ�þ���IDNum��??
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
             << "error"; // ����־λ����??0,��˵������ע�Ͳ���λ,û�н�����־
}

int main()
{
    ifstream fin("input.txt");
    if (!fin.is_open())
        cout << "�ļ�������!";
    ofstream fout("output.txt");
    scanner(fin, fout);

    fin.close();
    fout.close();
    return 0;
}
