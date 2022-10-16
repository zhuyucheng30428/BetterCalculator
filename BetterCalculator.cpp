#include <iostream>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include "hashmap.cpp"
using namespace std;
HashMap hashmap=HashMap(1);
class SortResult;
class SortResult{
    public:
        string value;
        int pow;
        bool positive=1;
        SortResult(string a, int b){
            value=a;
            pow=b;
        }
        SortResult(string a, int b,bool c){
            value=a;
            pow=b;
            positive=c;
        }
        SortResult (string input){//string转sort
            pow=0;//10指数
            positive=1;
            //去前后空格
            if(input[0]==' '){
                input.erase(0,1);
            }
            if(input[input.length()-1]==' '){
                input.erase(input.length()-1,1);
            }
            //去负号
            if(input[0]=='-'){
                input.erase(0,1);
                positive=false;
            }
            int len=input.length();
            //去e
            for (int i=0;i<len;i++){
                if (input[i] == 'e') {
                    pow+=atoi(input.substr(i+1,len-i-1).c_str());
                    input=input.substr(0,i);
                    break;
                }
            }
            //去小数点
            len=input.length();
            for (int i=0;i<len;i++){
                if (input[i] == '.') {
                    pow-=len-i-1;
                    input=input.erase(i,1);
                    break;
                }
            }
            value=input;
            len=input.length();
            for (int i=0;i<len;i++){
                if (input[i] <48 || input[i]>57) {//检查异常字符
                    value=""; 
                    pow=0;  
                    break;
                }
            }
        }
        string toString(){//sort转string
            string result="";
            if(value=="0"){//是否为0
                    result="0";
            }

            //是小数
            else if(pow<0){
                if(((int)value.length()+pow)>0){//小数输出
                    value.insert((int)value.length()+pow,".");
                    for(int i=value.length()-1;i>0;i--){//去小数点后多余0
                        if (value[i]==48){
                            value.erase(i,1);
                        }
                        else if(value[i]=='.'){
                            value.erase(i,1);
                            break;
                        }
                        else break;
                    }
                }
                else if((int)value.length()+pow>-4){
                    value.insert(0,"0.");
                    value.insert(2,string((int)value.length()+pow,'0'));
                }
                else{//带e输出
                    value.insert(1,".");
                    pow+=value.length()-2;
                    value.append("e"+ to_string(pow));
                }
                result= value;
            }

            //是整数
            else {
                if (pow+(int)value.length()>9){
                    pow+=value.length()-1;
                    value.insert(1, ".");
                    for(int i=value.length()-1;i>0;i--){//去小数点后多余0
                        if (value[i]==48 || value[i]=='.'){
                            value.erase(i,1);
                        }
                        else break;
                    }
                    value.append("e"+ to_string(pow));
                }
                else{
                    value.append(pow,'0');
                }
                result= value;
            }
            if(positive==0 && result!="0"){//加负号
                result.insert(0,"-");
            }
            result.insert(0," ");
            result.append(" ");
            return result;
        }
};
bool compare(string a,string b);
SortResult sortInput(string input);
string strAdd(string in1, string in2);
string strMul(string in1, string in2);
string strSub(string in1, string in2);
string add(SortResult a,SortResult b);
string mul(SortResult a,SortResult b);
string trim(string input);
string calculate(string input);
bool compare(string a,string b){//true if a>=b
    if(a.length()>b.length()){
        return true;
    }
    else if(a.length()<b.length()){
        return false;
    }
    else{
        for(int i=0;i<a.length();i++){
            if(a[i]>b[i]){
                return true;
            }
            else if (a[i]<b[i]){
                return false;
            }
        }
        return true;
    }
}
string strAdd(string in1, string in2){//竖式相加
    string output="" ;
    int next=0;//进位
    int len1=in1.length();
    int len2=in2.length();
    for(int i=0;i<max(len1, len2);i++){
        int a,b;
        if(i<len1) a=in1[len1-i-1]-48;//char转int
        else a=0;
        if(i<len2) b=in2[len2-i-1]-48;
        else b=0;
        int thi=a+b+next;
        output=to_string((thi%10))+output;
        next=thi/10;
    }
    output= to_string(next)+output;
    while(output[0]=='0' && output.length()>1){//clear all the 0's in the front
        output=output.substr(1,output.length()-1);
    }
    return output;
}
string strMul(string in1, string in2){//竖式相乘
    string output="";//输出
    int len1=in1.length();
    int len2=in2.length();
    for(int i=0;i<len1;i++){
        int a=in1[len1-1-i]-48;
        string line="" ;
        int next=0;//进位
        for(int j=0;j<len2;j++){
            int b=in2[len2-1-j]-48;
            int thi=a*b+next;//本位运算结果
            line=to_string((thi%10))+line;//这一位
            next=thi/10;//进位
        }
        line= to_string(next)+line;
        for(int j=i;j>0;j--){
            line=line+"0";//向左错i位
        }
        output=strAdd(output,line);//竖式相加
    }
    return output;
}
string strSub(string in1, string in2){//相减（a>b)
    int len=in2.length();
    for(int i=0;i<in2.length();i++){
        in2[i]=(char)105-in2[i];//in2的十进制反码
    }
    in2[in2.length()-1]+=1;
    in1=strAdd(in1,in2);//加上in1
    for(int i=in1.length()-len-1;i>-1;i--){
        if(in1[i]=='0'){
            in1[i]='9';
        }
        else{
            in1[i]-=1;
            break;
        }
    }
    while(in1[0]=='0' && in1.length()>1){
        in1.erase(0,1);//删除前面多余的0
    }
    return in1;
}
string add(SortResult a,SortResult b){//无损加法
    SortResult result= SortResult("",0);
    int displacement=-a.pow+b.pow;//使小数点对齐
    if(displacement>0){
        b.value.append(string(displacement,'0'));
    }
    else{
        a.value.append(string(-displacement,'0'));
        a.pow+=displacement;
    }
    result.pow=a.pow;//确定小数点位置
    //分正负号讨论
    if(a.positive==b.positive){//若a,b同号
        result.value=strAdd(a.value,b.value);
        result.positive=a.positive;
    }
    else{//若a,b异号
        if(compare(a.value,b.value)){
            result.value=strSub(a.value,b.value);
            result.positive=a.positive;
        }
        else{
            result.value=strSub(b.value,a.value);
            result.positive=b.positive;
        }
    }
    return result.toString();
}
string mul(SortResult a,SortResult b){
    return SortResult(strMul(a.value,b.value),a.pow+b.pow,a.positive*b.positive).toString();
}
string div(SortResult a,SortResult b){
    stringstream ss;
    ss<<a.value;
    double aa;
    ss>>setprecision(16)>>aa;
    ss.clear();
    ss<<b.value;
    double bb;
    ss>>setprecision(16)>>bb;
    ss.clear();
    string s=to_string(aa/bb);
    for(int i=s.length()-1;i>s.length()-1-a.pow+b.pow;i--){//去小数点后多余0
        if (s[i]==48 || s[i]=='.'){
            s.erase(i,1);
        }
        else break;
    }
    SortResult c=SortResult(s);
    c.positive=a.positive*b.positive;
    c.pow+=a.pow-b.pow;
    return c.toString();
}
string sqrt(SortResult a){
    stringstream ss;
    ss<<a.value;
    double aa;
    ss>>setprecision(16)>>aa;
    ss.clear();
    aa*=pow(10,a.pow%2);
    string s=to_string(pow(aa,0.5));
    for(int i=s.length()-1;i>s.length()-1-a.pow/2;i--){//去小数点后多余0
        if (s[i]==48 || s[i]=='.'){
            s.erase(i,1);
        }
        else break;
    }
    SortResult c=SortResult(s);
    c.pow+=a.pow/2;
    return c.toString();
}
string trim(string input){//整理空格
    //消除空格
    bool found=1;
    while(found){
        found=0;
        for(int i=1;i<input.length();i++){
            if(input[i]==' '){
                input.erase(i,1);
                found=true;
                break;
            }
        }
    }
    //加空格
    string cache=" ";
    for(int i=0;i<input.length();i++){
        if( input[i]=='+' || input[i]=='*'|| input[i]=='/' || input[i]=='('|| input[i]==')'){
            if(cache[cache.length()-1]!=' '){
                cache.append(" "+string(1,input[i])+" ");
            }
            else{
                cache.append(string(1,input[i])+" ");
            }
        }
        else{
            cache.append(string(1,input[i]));
        }
    }
    cache.append(" ");
    input=cache;
    //检查是否是定义变量语句
    for(int i=1;i<input.length()-1;i++){
        if(input[i]=='='){
            string it=calculate(trim(input.substr(i+1,input.length()-1-i)));
            hashmap.HMInsert(input.substr(1,i-1),it.substr(1,it.length()-2));
            return "Defination";
        }
    }
    //替换变量名为变量值
    cache="";
    int i=0;
    while(i<input.length()){
        if(input[i]>58&&(input[i]!='e' || input[i+1]>58)){
            if(input.length()-i>3&&input.substr(i,4)=="sqrt"){//识别平方根运算
                cache.append("sqrt");
                i+=4;
            }
            else{
                for(int j=i+1;j<input.length();j++){
                    if(input[j]<=58){
                        string it = hashmap.HMFind(input.substr(i,j-i)).c_str();
                        if (it!="NULL"){
                            cache.append(it);
                        }
                        else {
                            cout<<input.substr(i,j-i)+"=";
                            string in;
                            cin>>in;
                            hashmap.HMInsert(input.substr(i,j-i),in);
                            cache.append(in);
                        }
                        i=j;//i跳转到词末
                        break;
                    }
                }
            }
        }
        else{
            cache.append(string(1,input[i]));
            i+=1;
        }
    }
    input=cache;
    //去除减号
    found=1;
    while(found){
        found=0;
        for(int i=2;i<input.length()-1;i++){
            if(input[i]=='-'){
                if(input[i-1]=='-'){
                    input[i-1]=' ';
                    found=true;
                    break;
                }
                else if (input[i-2]!='+' && input[i-1]!='e'){
                    input.insert(i," + ");
                    found=true;
                    break;
                }
            }
        }
    }
    return input;
}
string calculate(string input){//递归计算
    //拆括号
    bool ok=false;
    while (!ok){
        ok=true;
        int left=0;//左截取
        int right=0;//右截取
        for(int i=0;i<input.length();i++){
            if(input[i]=='('){
                left=i;
            }
            else if(input[i]==')'){
                right=i;
                break;
            }
        }
        if(right-left>0){
            ok=false;
            string cache=calculate(input.substr(left+1,right-left-1));//递归
            
            if(left>5&&input.substr(left-5,4)=="sqrt"){
                left-=5;
                input.erase(left-1,right-left+3);
                input.insert(left-1,sqrt(SortResult(cache)));
            }
            else{
                input.erase(left-1,right-left+3);
                input.insert(left-1,cache);
            }
            
        }
    }
    
    //做乘法
    ok=false;
    while(!ok){
        ok=true;
        int left=0;
        int right=0;
        for(int i=0;i<input.length();i++){
            if(input[i]=='*'){
                ok=false;
                for(int p=i-1;p>0;p--){
                    if(input[p-1]==' '){
                        left=p;
                        break;
                    }
                }
                for(int p=i+1;p<input.length()-1;p++){
                    if(input[p+1]==' '){
                        right=p;
                        break;
                    }
                }
                string cache=mul(SortResult(input.substr(left,i-left)),SortResult(input.substr(i+1,right-i)));
                input.erase(left-1,right-left+3);
                input.insert(left-1,cache);
                break;
            }
            else if(input[i]=='/'){
                ok=false;
                for(int p=i-1;p>0;p--){
                    if(input[p-1]==' '){
                        left=p;
                        break;
                    }
                }
                for(int p=i+1;p<input.length()-1;p++){
                    if(input[p+1]==' '){
                        right=p;
                        break;
                    }
                }
                string cache=div(SortResult(input.substr(left,i-left)),SortResult(input.substr(i+1,right-i)));
                input.erase(left-1,right-left+3);
                input.insert(left-1,cache);
                break;
            }
        }
    }
    //做加法
    ok=false;
    while(!ok){
        ok=true;
        int left=0;
        int right=0;
        for(int i=0;i<input.length();i++){
            if(input[i]=='+'){
                ok=false;
                for(int p=i-1;p>0;p--){
                    if(input[p-1]==' '){
                        left=p;
                        break;
                    }
                }
                for(int p=i+1;p<input.length()-1;p++){
                    if(input[p+1]==' '){
                        right=p;
                        break;
                    }
                }
                string cache=add(SortResult(input.substr(left,i-left)),SortResult(input.substr(i+1,right-i)));
                input.erase(left-1,right-left+3);
                input.insert(left-1,cache);
                break;
            }
        }
    }
    return input;
}


int main(){
    while(true){
        string input;
        getline(cin,input);
        if(input=="exit"){//输入exit退出
            break;
        }
        else{
            input=trim(input);
            if(input!="Defination"){//自定义变量运算
                input=calculate(input);
                hashmap.HMInsert("ans",input.substr(1,input.length()-2));
                cout<<"="+input.substr(1,input.length()-2)<<endl;
                cout<<endl;
            }
        }
    }
}
