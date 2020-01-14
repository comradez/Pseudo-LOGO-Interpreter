#include <cctype>
#include <cstring>
#include <cmath>
#include <string>
#include <ctime>
#include <random>
#include "variable.hpp"
#include "error.hpp"
#include "expression.h"
using namespace std;
namespace expression{
	char a[105];
	int isop[305],CUR;
	bool VALID=1;
	int rand(){
		static mt19937_64 gen(
#ifdef _WIN32 //Windows下的std::random_device不大靠谱
			[]()->mt19937_64::result_type{
				char *tmp=new char;
				delete(tmp);
				return (size_t)tmp^(size_t)time(NULL);
			}()
#else
			random_device{}()
#endif
		);
		static uniform_int_distribution<int> dist;
		return dist(gen);
	}
	int get_digit(int l,int r,int &x){
		for(int i=l;i<=r;i++){
			if(isdigit(a[i])) x=x*10+a[i]-'0';
			else return i-1;
		}
		return r;
	}
	int rd(double x){
		int f=x>0?1:-1;
		x=abs(x);
		int k=x;
		if(x-k<0.5) return k*f;
		return f*(k+1);
	}
	double solve(int l,int r){//sin,cos,log,tan,exp,rand,acos,asin,atan
		if(l>r) return 0;
		int op[25],tot=0,sum=0;
		double b[25];
		for(int i=l;i<=r;i++){
			if(a[i]=='['){
				string var;
				for(int p=i+1;p<=r;p++){
					i=p;
					if(a[p]==']') break;
					var+=a[p];
				}
				b[++tot]=get(var,CUR);
			}
			if(a[i]>='a'&&a[i]<='z'){
				char C=a[i],C1=a[i+1];
				while(a[i]!='(') i++;
				int o=0,O=i,U=0;double WW;
				for(int p=i;p<=r;p++){
					i=p;
					if(a[p]=='(') o++;
					if(a[p]==')') o--;
					if(a[p]==','&&o==1){
						WW=solve(O+1,p-1);
						U=1;
						O=p;
					}
					if(o==0) break;
				}
				double ret=solve(O+1,i-1);
				if(C=='s'&&C1=='i') ret=sin(ret);
				if(C=='c') ret=cos(ret);
				if(C=='l') ret=log(ret);
				if(C=='t') ret=tan(ret);
				if(C=='e') ret=exp(ret);
				if(C=='s'&&C1=='q') ret=ret*ret;
				if(C=='r') ret=rand();
				if(C=='a'){
					if(C1=='c') ret=acos(ret);
					if(C1=='s') ret=asin(ret);
					if(C1=='t'&&U==0) ret=atan(ret);
					if(C1=='t'&&U) ret=atan2(WW,ret);
				}
				b[++tot]=ret;
				if(!VALID) return 0;
			}
			if(a[i]=='('){
				int o=0,O=i;
				for(int p=i;p<=r;p++){
					i=p;
					if(a[p]=='(') o++;
					if(a[p]==')') o--;
					if(o==0) break;
				}
				b[++tot]=solve(O+1,i-1);
				if(!VALID) return 0;
			}
			if(isdigit(a[i])){
				int x=0;
				i=get_digit(i,r,x);
				b[++tot]=x;
			}
			if(isop[a[i]]){
				if(a[i]=='-'&&sum==tot){
					int x=0;
					i=get_digit(i+1,r,x);
					b[++tot]=-x;
					continue;
				}
				op[++sum]=isop[a[i]];
			}
		}
		if(tot<=sum){
			VALID=0;
			return 0;
		}
		for(int i=1;i<=8;i++){
			for(int p=1;p<=sum;p++){
				if(op[p]==i){
					if(i==1) b[p]=b[p]/b[p+1];
					if(i==2) b[p]=rd(b[p])%rd(b[p+1]);
					if(i==3) b[p]=b[p]*b[p+1];
					if(i==4) b[p]=b[p]+b[p+1];
					if(i==5) b[p]=b[p]-b[p+1];
					if(i==6) b[p]=rd(b[p])^rd(b[p+1]);
					if(i==7) b[p]=rd(b[p])&rd(b[p+1]);
					if(i==8) b[p]=rd(b[p])|rd(b[p+1]);
					tot--;sum--;
					for(int j=p+1;j<=tot;j++) b[j]=b[j+1];
					for(int j=p;j<=sum;j++) op[j]=op[j+1];
				}
			}
		}
		return b[1];
	}
	double calc(const string &str,int cur){
		memset(a,0,sizeof(a));
		CUR=cur;
		VALID=1;
		isop['/']=1;
		isop['%']=2;
		isop['*']=3;
		isop['+']=4;
		isop['-']=5;
		isop['^']=6;
		isop['&']=7;
		isop['|']=8;
		int n=str.length();
		for(int i=1;i<n-1;i++) a[i]=str[i];
		int o=0;
		for(int i=1;i<=n;i++){
			if(a[i]=='(') o++;
			if(a[i]==')') o--;
			if(o<0){
				invalid_literaltype_value(str,cur);
				return 0;
			}
		}
		if(o>0){
			invalid_literaltype_value(str,cur);
			return 0;
		}
		double ans=solve(1,n-2);
		if(VALID) return ans;
		invalid_literaltype_value(str,cur);
		return 0;
	}
}//c下标从1开始,表达式有误返回1e9+7
