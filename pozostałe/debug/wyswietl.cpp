#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;
int main()
{
    system("Color 70");
    ifstream plik("labirynt.txt");
    int j;
    char p;
    while(!plik.eof())
    {
        for(j=0; j<25; j++)
        {
            plik>>p;
            if (p=='1') cout<<char(219);
            else cout<<' ';
        }
        cout<<endl;
    }
}
