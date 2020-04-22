#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <map>

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
/*
 * type表示类别
 * id指向语义地址编号
 * type 1 关键字
 * type 2 自定义标识符
 * type 3 无符号整数常量
 * type 4 无符号浮点数常量
 * type 5 运算符等*/
const int BUFF_MAX = 100;
const int TOKEN_MAX = 120;
const int ADDTION_MAX = 20;
char ch;
char strToken[TOKEN_MAX + ADDTION_MAX];
char buff[BUFF_MAX * 2 + ADDTION_MAX];
int s_pos, e_pos, pos;
int txtalllenth;
int Tpos;
ifstream in;

const int Start_Keyword = 1e6, Start_Idfword = 2e6, Start_UInter = 3e6, Start_UFloat = 4e6;
map<string, int> Keyword_List, Idfword_List;
map<int, int> UInter_List;
map<double, int> UFloat_List;

inline void Getbuff() {//获取缓冲区
    int last, now;
    last = in.tellg();
    if (last == -1) {
        last = txtalllenth;
    }
    if (e_pos >= 2 * BUFF_MAX) {//超出两倍回到起点半边
        s_pos = 0;
    } else {//否则继续采用
        s_pos = e_pos;
    }
    in.read(buff + s_pos, BUFF_MAX);
    now = in.tellg();
    if (now == -1) {
        now = txtalllenth;
    }
    e_pos = s_pos + (now - last);
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
        in.seekg(-BUFF_MAX, ios::cur);
        in.clear();
    }
    ch = ' ';
}

int Reserve() {// 整型函数过程，对strToken中的字符串查找保留字表，若是保留字返回编码，否则返回0（假定0不是保留字编码)
    strToken[Tpos] = '\0';
    auto i = Keyword_List.find(strToken);
    if (i != Keyword_List.end()) {
        return i->second + Start_Keyword;
    }
    return 0;
}

int FindIdefID() {//strToken中的字符串查找标识符表，如果有则返回，否则则插入再返回标号
    strToken[Tpos] = '\0';
    auto i = Idfword_List.find(strToken);
    if (i != Idfword_List.end()) {
        return i->second + Start_Idfword;
    } else {
        int m = Idfword_List.size();
        Idfword_List[strToken] = m;
        return m + Start_Idfword;
    }
}

int FindUInterID(int n) {//查找无符号整形常数表，有则返回编号，没有则插入后再返回编号
    strToken[Tpos] = '\0';
    auto i = UInter_List.find(n);
    if (i != UInter_List.end()) {
        return i->second + Start_UInter;
    } else {
        int m = UInter_List.size();
        UInter_List[n] = m;
        return m + Start_UInter;
    }
}

int FindUFloatID(double n) {//查找无符号浮点型常数表，有则返回编号，没有则插入后再返回编号
    strToken[Tpos] = '\0';
    auto i = UFloat_List.find(n);
    if (i != UFloat_List.end()) {
        return i->second + Start_UFloat;
    } else {
        int m = UFloat_List.size();
        UFloat_List[n] = m;
        return m + Start_UFloat;
    }
}

void outans(int type, int ID) {//将结果输出
    cout << "< " << type << " , " << ID << " > " << strToken << endl;
}

bool KeyorIdef() {//处理关键字或者用户标识符
    //这里的Token里的语义信息指针都没有写好
    int type, ID;
    while (IsLetter() || IsDigit()) {
        Concat();
        Getchar();
    }//将整个单词读取出来
    Retract();
    ID = Reserve();
    if (ID == 0) {//自定义表示符
        type = 1;
        ID = FindIdefID();
    } else {
        type = 1;
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
    int type, ID;
    int n, m, p, e, t;
    //n处理尾数，m处理小数点位数，p处理指数，e表示指数正负，t=0表示整数，t=1表示浮点数
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
        if (ch == '-') { //获取符号我
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
    if (t == 0) {//输出结果
        int num = n * pow(10, e * p - m);
        type = 3;
        ID = FindUInterID(num);
    } else {
        double num = n * pow(10, e * p - m);
        type = 4;
        ID = FindUFloatID(num);
    }
    outans(type, ID);
    Retract();
    return true;
}

void printerror() {//输出错误
    cout << "error! " << strToken << endl;
}

void Keywordinit() {
    ifstream Keyin(R"(E:\just ice\Compiling principle\lexical-analyzer\in.txt)");
    string s;
    int ID;
    while (Keyin >> s >> ID) {
        Keyword_List[s] = ID;
    }
}

void init() {
    ch = ' ';
    s_pos = e_pos = pos = 0;
    in.seekg(0, ios::end);
    txtalllenth = in.tellg();
    in.seekg(0, ios::beg);
}

int main(int argc, char *argv[]) {
    in.open(R"(E:\just ice\Compiling principle\lexical-analyzer\in.txt)", ios::binary);
    //初始化操作
    init();

    while ((pos < e_pos) || (in.good() && !in.eof())) {
        GetBC();//读取空字符
        Tpos = 0;
        if (IsLetter()) {
            KeyorIdef();
        } else if (IsDigit()) {
            UConst();
<<<<<<< Updated upstream
=======
        } else if (ch == '=') {
            Concat();
            strToken[Tpos] = '\0';
            outans(5, 1);
            ch = ' ';
        } else if (ch == '+') {
            Concat();
            strToken[Tpos] = '\0';
            outans(5, 2);
            ch = ' ';
        } else if (ch == '-') {
            Concat();
            strToken[Tpos] = '\0';
            outans(5, 3);
            ch = ' ';
        } else if (ch == '*') {
            Concat();
            strToken[Tpos] = '\0';
            outans(5, 4);
            ch = ' ';
        } else if (ch == '/') {
            Concat();
            strToken[Tpos] = '\0';
            outans(5, 5);
            ch = ' ';
        } else if (ch == '(') {
            Concat();
            strToken[Tpos] = '\0';
            outans(6, 2);
            ch = ' ';
        } else if (ch == ')') {
            Concat();
            strToken[Tpos] = '\0';
            outans(6, 3);
            ch = ' ';
        } else if (ch == ';') {
            Concat();
            strToken[Tpos] = '\0';
            outans(6, 1);
            ch = ' ';
>>>>>>> Stashed changes
        } else {
            printerror();
            break;
        }
    }
    in.close();
    return 0;
}