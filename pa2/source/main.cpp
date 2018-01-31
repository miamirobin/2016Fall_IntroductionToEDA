// b03208022 李友岐

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>
#include <iomanip>
#include <stdlib.h>
#include <iomanip>
#include <stdio.h>
#include <sstream>
using namespace std;
int f(vector<vector<int> > &s,int x,int y);

int main( int argc, char** argv ){
    vector<int> vecA;
    ifstream ffin(argv[1]);
    int x,z=0;
    while (ffin >> x)
    {
        vecA.push_back(x);
        if (x>z){z=x;}
    }
    ffin.close();
    int y=vecA.size();
    int vecB[z][2];
    for (int i=1;i<=z;i++)
    {   int q=0,w=y/2-1;
        while (!(((vecA[q]==i) or (vecA[q+y/2]==i))  and ((vecA[w]==i) or (vecA[w+y/2]==i)))){
            if (!((vecA[q]==i) || (vecA[q+y/2]==i))){q+=1;}
            if (!((vecA[w]==i) || (vecA[w+y/2]==i))){w-=1;}
        }
        vecB[i-1][0]=q;
        vecB[i-1][1]=w;
    }
    int i=0;/*
    int **cc, *pData;
    cc = (int **)malloc(z*sizeof(int *)+z*z*sizeof(int));
    for (i = 0, pData = (int *)(cc+z); i < z; i++, pData += z){
        vecC[i]=pData;
    }*/
    vector<vector<int> > vecC;
    vector<int> vectemp;
    for (int i=0;i<z;i++){
        for (int j=0;j<z;j++){
            vectemp.push_back(0);
            for (int q=0;q<y/2;q++){
                if ( i+1==vecA[q] and j+1==vecA[q+y/2]){
                    vectemp[j]=1;
                }
            }
            
        }
        vecC.push_back(vectemp);
        vectemp.clear();
    }
    vector<int> vecS;
    for (int i=0;i<z;i++)
    {
        vecS.push_back(i);
    }
    int track[z];
    int t=0;
    vector<int> vecW;
    int d=f(vecC,z,z);
    while (vecS.size()>0){
        t+=1;
        int watermark=-1;
        while ( d==0){
            int p=9999999;
            int r=-1;
            for (int i=0;i<vecS.size();i++){
                int a=0;
                for (int k=0;k<z;k++){
                    a+=vecC[k][vecS[i]];
                }
                if (a==0){
                    if (vecB[vecS[i]][0]>watermark and vecB[vecS[i]][0]<p){ p=vecB[vecS[i]][0];
                     r=vecS[i];
                   }
                }
            }
            if (r==-1){break;}
            track[r]=t;
            watermark=vecB[r][1];
            for (int i=0;i<vecS.size();i++){
                if (vecS[i]==r){vecS.erase(vecS.begin()+i);}
            }
            vecW.push_back(r);
            vecC[r][r]=1;
            d=f(vecC,z,z);
        }
        for (int q=0;q<vecW.size();q++){
            for (int i=0;i<z;i++){
                if(vecW[q]!=i){
                    vecC[vecW[q]][i]=0;
                }
            }
        }
        vecW.clear();
        d=f(vecC,z,z);      
    }
    ofstream ofile(argv[2]);
    for (int i=1;i<=t;i++)
        {cout <<"Track"<<i<<": ";
         ofile<<"Track"<<i<<": ";
         for (int j=0;j<y/2;j++){
             for (int k=0;k<z;k++){
                 if (vecB[k][0]==j and track[k]==i){
                     cout<<"i"<<k+1<<" ";
                     ofile<<"i"<<k+1<<" ";
                    }
                }
            }
            cout<<""<<endl;
            ofile<<""<<endl;
        }
    ofile.close();
    return 0;
}
int f(vector<vector<int> > &s,int x,int y){
    for (int j=0;j<y;j++){
          int sum=0;
          for (int i=0;i<x;i++){
              sum+=s[j][i];
          }
          if (sum==0){return 0;}
    }
    return 1;
}

