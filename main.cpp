#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

/*
 ch 字符变量，存放最新读进源程序的字符
 strToken 字符数组，存放构成单词符号的字符串
 GetChar  将下一个输入字符读到ch中，搜索指示器前移一字符位置
 GetBC 检查ch中字符是否为空格，若是，则调用GetCahr直至ch进入一个非空白字符
 Concat 将ch连接到strToken之后
 IsLetter 布尔函数，判断ch字符是否为字母
 IsDigit 布尔函数，判断ch字符是否为数字
 Reserve 整型函数过程，对strToken中的字符串查找保留字表，若是保留字返回编码，否则返回0（假定0不是保留字编码)
 Retract 将搜索指示器回调一个字符的位置，将ch置为空白字符
 InsertId 整型函数，将strToken中的标识符插入符号表，返回符号表指针
 InsertConst 整型函数，将strToken中常数插入常数表，返回常数表指针
*/
const int BUFF_MAX = 100;
const int TOKEN_MAX = 120;
const int ADDTION_MAX = 20;
char ch;
char strToken[TOKEN_MAX + ADDTION_MAX];
char buff[BUFF_MAX * 2 + ADDTION_MAX];
int s_pos, e_pos, pos;
int Tpos;
ifstream in;

inline void Getbuff() {//获取缓冲区
    if (e_pos >= 2 * BUFF_MAX) {//超出两倍回到起点半边
        s_pos = 0;
        e_pos = 100;
    } else {//否则继续采用
        s_pos = e_pos;
        e_pos = s_pos + BUFF_MAX;
    }
    in.read(buff + s_pos, BUFF_MAX);
}

inline void Getchar() {//获取下一个字符
    if (pos == e_pos) {//如果到达了边缘，则继续读取
        Getbuff();
    }
    ch = buff[pos++];
    if (pos >= 2 * BUFF_MAX) {
        pos = pos % (2 * BUFF_MAX);
    }
}

inline void GetBC() {//跳过空格和换行
    while (ch == ' ' || ch == '\n' || ch == '\r') {
        Getchar();
    }
}

inline void Concat() {//将ch加入到strToken中
    strToken[Tpos++] = ch;
}

inline bool IsLetter() {//判断ch是否为字母
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

inline bool IsDigit() {//判断ch是否为数字
    return ch >= '0' && ch <= '9';
}

inline void Retract() {//缓冲区指针回退一格
    pos--;
    if (pos < s_pos) {//超出范围,回退整个缓冲区
        if (s_pos == 0) {
            s_pos = BUFF_MAX;
            e_pos = 2 * BUFF_MAX;
        } else {
            s_pos = 0;
            e_pos = 2 * BUFF_MAX;
        }
        pos = e_pos - 1;
        in.seekg(std::ifstream::cur - BUFF_MAX);
    }
    ch = ' ';
}

int Reserve() {// 整型函数过程，对strToken中的字符串查找保留字表，若是保留字返回编码，否则返回0（假定0不是保留字编码)
    //没写呢，别看了。
    return 0;
}

int FindIdefID() {//strToken中的字符串查找标识符表，如果有则返回，否则返回0（假定0不是编码)
    //没写呢，别看了。
    return 0;
}

int InsertId() {//整型函数，将strToken中常数插入常数表，返回常数表指针
    //也没写呢。
    return 0;
}

void outans(int type, int ID) {//将结果输出
    //也没写
    Tpos = 0;
}

bool KeyorIdef() {//处理关键字或者用户标识符
    //这里的Token里的语义信息指针都没有写好
    int type, ID;
    while (IsLetter() || IsDigit()) {
        Concat();
        Getchar();
    }//将整个单词读取出来
    Retract();
    type = Reserve();
    if (type == 0) {//自定义表示符
        type = FindIdefID();
        if (type == 0) {
            ID = InsertId();
        } else {
            ID = type;
        }
    } else {
        ID = type;
    }
    outans(type, ID);
    return true;
}

bool strtoint(int &n) {//字符串转化为整数
    if (IsDigit()) {
        n = n * 10 + ch - '0';
        return true;
    }
    return false;
}

bool UConst() {//处理无符号常量
    int n, m, p, e, t;
    n = m = p = t = 0;
    e = 1;
    while (IsDigit()) {//读到小数点前的读取尾数
        Concat();
        strtoint(n);
        Getchar();
    }
    if (ch == '.') {
        t = 1;
        Concat();
        Getchar();
        while (IsDigit()) {//读到小数点后的读取尾数
            Concat();
            strtoint(n);
            m++;
            Getchar();
        }
    }
    if (ch == 'e') {
        Concat();
        Getchar();
        if (ch == '-') {//获取符号我
            t = 1;
            e = -1;
            Concat();
            Getchar();
        } else if (ch == '+') {
            t = 1;
            e = -1;
            Concat();
            Getchar();
        }
        while (IsDigit()) {//获取指数部分
            Concat();
            strtoint(p);
            Getchar();
        }
    }
    if (ch != ' ' && ch != '\n') {//判断出错

    }
    if (t == 0) {//输出结果
        int num = n * pow(10, e * p - m);
        cout << num << endl;
        outans(t, n);
    } else {
        float num = n * pow(10, e * p - m);
        cout << num << endl;
        outans(t, n);
    }
    Retract();
    return true;
}

int main(int argc, char *argv[]) {
    in.open(R"(E:\just ice\Compiling principle\lexical analyzer\in.txt)", ios::binary);
    ch = ' ';
    s_pos = e_pos = pos = 0;
    while ((pos < e_pos) || (in.good() && !in.eof())) {
        GetBC();//读取空字符
        if (IsLetter()) {
            KeyorIdef();
        } else if (IsDigit()) {
            UConst();
        }//其他都没写呢
    }
    return 0;
}