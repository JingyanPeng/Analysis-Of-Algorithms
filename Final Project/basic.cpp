#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <ctype.h>

using namespace std;

int optimal=0;

int punish=30;

int judge(char a, char b)
{
    if(a==b) return 0;
    else if((a=='A'&& b=='C') || (a=='C'&&b=='A')) return 110;
    else if((a=='A'&& b=='G') || (a=='G'&&b=='A')) return 48;
    else if((a=='A'&& b=='T') || (a=='T'&&b=='A')) return 94;
    else if((a=='G'&& b=='C') || (a=='C'&&b=='G')) return 118;
    else if((a=='C'&& b=='T') || (a=='T'&&b=='C')) return 48;
    else return 110;
}

bool IsNumber(string str)
{
   stringstream sin(str);
   char c;
   double d;
   if(!(sin>>d)){
       return false;
   }
   if(sin>>c){
        return false;
   }
   return true;
}

string* StringGenerator(string input_name)
{
    string* str = new string[2];
    string str1, str2;
    string line;
    int a;
    ifstream inputFile;
    inputFile.open(input_name);
        
    while(getline(inputFile, line)){
        if(IsNumber(line)){
            a = stoi(line) + 1;
            if(str2.empty()){
                if(a >= str1.size()) str1.append(str1);
                else str1.insert(a, str1);
            }
            else{
                if(a >= str2.size()) str2.append(str2);
                else str2.insert(a, str2);
            }
        }
        else{
            if(!isalpha(line[line.size()-1])){
                line = line.substr(0,line.size()-1);
            }
            if(str1.empty()){
                str1 = line;
            }
            else{
                str2 = line;
            }
        }
    };

    inputFile.close();
    
    str[0] = str1;
    str[1] = str2;
    return str;
}

pair<string, string> SequenceAlignment(string s1, string s2)
{
    pair<string, string> result;
    
    int n=s1.size(),m=s2.size();
    int dp[n+1][m+1];
    for(int i=0;i<=m;i++){
        dp[0][i]=punish*i;
    }
    for(int i=0;i<=n;i++){
        dp[i][0]=punish*i;
    }
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            dp[i][j]=min(min(dp[i-1][j-1]+judge(s1[i-1],s2[j-1]),dp[i-1][j]+punish),dp[i][j-1]+punish);
        }
    }
    
    string s11=s1;
    string s22=s2;
    int i=n,j=m;
    while(i>=1 && j>=1)
    {
        if(dp[i-1][j]+punish < dp[i-1][j-1]+judge(s1[i-1],s2[j-1]) &&
           dp[i-1][j]+punish < dp[i][j-1]+punish)
        {
           s22.insert(j, "_");
           i--;
        }
        else if(dp[i][j-1]+punish < dp[i-1][j-1]+judge(s1[i-1],s2[j-1]) &&
           dp[i][j-1]+punish < dp[i-1][j]+punish)
        {
            s11.insert(i, "_");
            j--;
        }
        else
        {
            i--;
            j--;
        }
    }
    while(i>=1)
    {
        s22.insert(0,"_");
        i--;
    }
    while(j>=1)
    {
        s11.insert(0,"_");
        j--;
    }
    
    optimal=optimal+dp[n][m];
    result.first=s11;
    result.second=s22;
    return result;
}


void outputGenerator(pair<string, string> result, string filename, float time)
{
    string s11=result.first;
    string s22=result.second;
    if(s11.size()<=50)
    {
        ofstream write;
        write.open(filename);
        write <<s11<<"  "<<s11<<endl<<s22<<"  "<<s22<<endl<<optimal<<endl<<time<<endl;
        write.close();
    }
    else
    {
        ofstream write;
        write.open(filename);
        write <<s11.substr(0,50)<<"  "<<s11.substr(s11.size()-50,s11.size()-1)<<endl<<s22.substr(0,50)<<"  "<<s22.substr(s22.size()-50,s22.size()-1)<<endl<<optimal<<endl<<time<<endl;
        write.close();
    }
}

int main()
{
    
    string in;
    cin>>in;
    
    string* input = StringGenerator(in);
    pair<string,string> result;
    
    const clock_t begin_time_basic=clock();
    result=SequenceAlignment(input[0], input[1]);
    float seconds_basic=float(clock()-begin_time_basic)/1000;
    
    
    outputGenerator(result,"/Users/pengjingyan/Desktop/test570/output_basic.txt",seconds_basic);
    return 0;
}
