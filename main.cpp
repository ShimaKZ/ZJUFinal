#include <iostream>
#include <vector>
#include<unordered_map>
#include<cstring>
#include<string>
using namespace std;
unordered_map<size_t,bool> hashTBProcedure;
unordered_map<size_t,int>hashTBMax;
const double FormerCompetitiveRatio=2.2117;
hash<int>hasher;
int target,guarantee;
bool EvaluateAlgorithm(vector<int>Machine,vector<int>Sequence,int t,int u);
bool EvaluateAdversary(vector<int>Machine,vector<int>Sequence);
int DynprogMax(vector<int>Sequence){
    if(Sequence.size()==0)return guarantee;
    int x=Sequence[(int)Sequence.size()-1];
    Sequence.pop_back();
    
	size_t seed = 0;
    for (int i : Sequence) {
        seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    }
    int mxGuar=hashTBMax[seed];

    Sequence.push_back(x);
    seed^=hasher(x) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    if(hashTBMax.count(seed))return hashTBMax[seed];


    int dp[2][51]={};
    int cur=1;
    int sum=0;
    dp[0][0]=1;
    for(int g=mxGuar;g>=1;g--){
        Sequence.push_back(g);
        for(int i=0;i<Sequence.size();i++){
            memset(dp[cur],0,sizeof(dp[cur]));
            for(int j=0;j<=guarantee;j++){
                if(dp[1-cur][j]){
                    int y=sum-j,x=j;
                    if(y+Sequence[i]<=guarantee){
                        dp[cur][max(y+Sequence[i],x)]=1;
                    }
                    if(x+Sequence[i]<=guarantee){
                        dp[cur][max(x+Sequence[i],y)]=1;
                    }
                }
            }
            sum+=Sequence[i];
            cur=1-cur;
        }
        for(int i=0;i<=guarantee;i++){
            if(dp[1-cur][i]){
                
                Sequence.pop_back();
                hashTBMax[seed]=g;
                return g;
            }
        }
        Sequence.pop_back();
    }
    hashTBMax[seed]=0;
    return 0;
}
bool EvaluateAlgorithm(vector<int>Machine,vector<int>Sequence,int t,int u){
    for(int i=0;i<2;i++){//tested
        if(Machine[i]+t+u<target){
            vector<int> tmpMachine=Machine;
            tmpMachine[i]=Machine[i]+t+u;
            vector<int> tmpSequence=Sequence;
            tmpSequence.push_back(u);
            if(EvaluateAdversary(tmpMachine,tmpSequence))return true;
        }
    }
    for(int i=0;i<2;i++){//tested
        if(Machine[i]+u<target){
            vector<int> tmpMachine=Machine;
            tmpMachine[i]=Machine[i]+u;
            vector<int> tmpSequence=Sequence;
            tmpSequence.push_back(t);
            if(EvaluateAdversary(tmpMachine,tmpSequence))return true;
        }
    }
    return false;
}
bool EvaluateAdversary(vector<int>Machine,vector<int>Sequence){
    
    
	size_t seed = 0;
    for (int i : Machine) {
        seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    }
    for (int i : Sequence) {
        seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    }
    if(hashTBProcedure.count(seed))return hashTBProcedure[seed];
    int g=DynprogMax(Sequence);
    for(int u=1;u<=g;u++){
        for(int t=1;t<=u;t++){
            if(!EvaluateAlgorithm(Machine,Sequence,t,u)){
				hashTBProcedure[seed]=false;
                return false;
            }
        }
    }
	hashTBProcedure[seed]=true;
    return true;
}

bool RunProcedure(int _t,int _g){
    hashTBProcedure.clear();
    hashTBMax.clear();
    hashTBMax[(size_t)0]=guarantee;
    target=_t,guarantee=_g;
    vector<int>initMachine,initSequence;
    initMachine.clear(),initSequence.clear();
    initMachine.push_back(0);
    initMachine.push_back(0);
    cout<<_g<<" "<<_t<<endl;
    return !EvaluateAdversary(initMachine,initSequence);
}
int main(int, char**){
//    freopen("test.out","w",stdout);
    for(int g=18;g<=20;g++){
        for(int t=g*FormerCompetitiveRatio+1;t<=2.5*g;t++){
            if(RunProcedure(t,g))cout<<"g: "<<g<<"t: "<<t<<endl;
        }
    }
    system("pause");
    return 0;
}
