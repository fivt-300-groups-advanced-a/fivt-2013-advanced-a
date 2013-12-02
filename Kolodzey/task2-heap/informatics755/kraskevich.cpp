#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

const long long INF=1<<15;



void sift_up(int i, vector<long long> &a, int n)
{
     while(a[i]>a[i/2] && i>1){
                        long long t=a[i];
                        a[i]=a[i/2];
                        a[i/2]=t;
                        i=i/2;
     }
}

void sift_down(int i, vector<long long> &a, int n)
{
     while((a[i]<a[2*i] || a[i]<a[i*2+1]) && i<n){
                       if(a[i*2]>a[i*2+1] && a[i]<a[i*2]){
                                          long long t=a[i];
                                          a[i]=a[i*2];
                                          a[i*2]=t;
                                          i=i*2;
                       }
                       else{
                            if(a[i]<a[i*2+1]){
                                              long long t=a[i];
                                              a[i]=a[2*i+1];
                                              a[i*2+1]=t;
                                              i=2*i+1;
                            }
                            else break;
                       }
     }
}

int main()
{
    int nTests = 16;
    for (int it = 1; it <= nTests; ++it)
    {
      vector<long long> a(1000000, -INF);
      int n;
      n=0;
      int k;
      ofstream out;
      ifstream in;
      char buf[50];
      sprintf(buf, "%02d.in", it);
      std::cerr << buf << std::endl;
      in.open(buf);
      for (int jt = 0; jt < 50; ++jt)
        buf[jt] = '\0';
      sprintf(buf, "%02d.out", it);
      std::cerr << buf << std::endl;
      out.open(buf);
      for (int jt = 0; jt < 50; ++jt)
        buf[jt] = '\0';
       in>>k;
      for(int i=0; i<k; i++){
              int t;
              in>>t;
              if(t==1){
                       out<<a[1]<<endl;
                       a[1]=a[n];
                       a[n]=-INF;
                       n--;
                       sift_down(1,a,n);
              }
              else
              {
                  int f;
                  in>>f;
                  n++;
                  a[n]=f;
                  sift_up(n,a,n);
              }
      }
      in.close();
      out.close();
    }
}