#include <iostream>
#include <fstream> /** do obslugi plików */
#include <conio.h> /** do obslugi klawiszy */
using namespace std;
typedef struct
{
    int x, y, klucz;
} entity;
bool wczytaj(int maze[][25], entity &gracz) /** wczytuje plik labiryntu */
{
    ifstream labirynt("labirynt.txt");
    /** przerwanie dzialania funkcji w razie, gdy plik z labiryntem nie istnieje, zwrócenie wartosci falsz do funkcji main() */
    if (!labirynt) return false;

    /** wczytanie labiryntu do macierzy */
    int i, j;
    char k;
    for (i=0; i<25; i++)
        for (j=0; j<25; j++)
        {
            labirynt>>k;
            if (k=='x') /** polozenie poczatkowe gracza w pliku reprezentowane jest przez znak x, wiec gdy program na niego natrafia nastepuje zapisanie wspólrzednych poczatkowych */
            {
                gracz.x=j;
                gracz.y=i;
                maze[i][j]=0;
            }
            else maze[i][j]=int(k)-48;
        }
    labirynt.close();
    return true;
}
void graj(entity gracz, int maze[][25]) /** funkcja odpowiadajaca za mechanizm glówny gry */
{
    int ky=0, kx=0, k=0, key;
    char odp;
    while (true) /** nieskonczona petla */
    {
        /** polozenie i mozliwe kierunki */
        cout<<endl;
        if (maze[gracz.y-1][gracz.x]%2==0) cout<<"-GORA-";
        if (maze[gracz.y+1][gracz.x]%2==0) cout<<"-DOL-";
        if (maze[gracz.y][gracz.x+1]%2==0) cout<<"-PRAWO-";
        if (maze[gracz.y][gracz.x-1]%2==0) cout<<"-LEWO-";
        cout<<" WSPOLRZEDNE: "<<gracz.x<<' '<<gracz.y;

        getch();
        do key=getch(); /** wczytanie klawisza */
        while ((key!=72)&&(key!=80)&&(key!=77)&&(key!=75));

        switch(key)
        {
            case 72: /** strzalka w góre */
                ky=-1; /** przesuniecie pionowe */
                kx=0; /** przesuniecie poziome */
                break;
            case 80: /** strzalka w dól */
                ky=1;
                kx=0;
                break;
            case 77: /** strzalka w prawo */
                ky=0;
                kx=1;
                break;
            case 75: /** strzalka w lewo */
                ky=0;
                kx=-1;
                break;
        }
        if (maze[gracz.y+ky][gracz.x+kx]!=1) /** jezeli pole, na które ma przemiescic sie gracz jest dozwolone, nastepuje przemieszczenie sie; sciany w macierzy reprezentowane sa przez wartosc 1 */
        {
            gracz.y=gracz.y+ky;
            gracz.x=gracz.x+kx;
        }
        if (maze[gracz.y][gracz.x]>1) /** jesli gracz natrafia na drzwi lub klucz */
        {
            if (maze[gracz.y][gracz.x]%2==0) /** jesli gracz natrafia na klucz, wartosci parzyste wieksze od 1 w macierzy labiryntu sa reprezentowane przez klucze */
            {
                cout<<endl<<"CZY WZIAC KLUCZ NR "<<maze[gracz.y][gracz.x]/2<<"? T/N"<<endl;

                /** prosba o decyzje uzytkownika */
                do cin>>odp;
                while ((odp!='t')&&(odp!='T')&&(odp!='n')&&(odp!='T'));

                if ((odp=='t')||(odp=='T'))
                {
                    if (gracz.klucz!=0) /** jesli gracz ma juz jakis klucz w posiadaniu */
                    {
                        cout<<endl<<"W POSIADANIU JEST JUZ KLUCZ NR "<<gracz.klucz/2<<endl;
                        cout<<endl<<"ZWOLNIC MIEJSCE ZOSTAWIAJAC TEN KLUCZ NA ZIEMI? T/N"<<endl;

                        do cin>>odp;
                        while ((odp!='t')&&(odp!='t')&&(odp!='N')&&(odp!='T'));

                        if ((odp=='t')||(odp=='T'))
                        {
                            cout<<endl<<"ZOSTAWIONO NA ZIEMI KLUCZ NR "<<gracz.klucz/2<<endl;
                            cout<<endl<<"WZIETO KLUCZ NR "<<maze[gracz.y][gracz.x]/2<<endl;
                            swap(maze[gracz.y][gracz.x],gracz.klucz); /** zamiana klucza w ekwipunku z tym lezacym na ziemi */
                        }
                    }
                    else /** jesli gracz nie ma jeszcze zadnego klucza */
                    {
                        cout<<endl<<"WZIETO KLUCZ NR "<<maze[gracz.y][gracz.x]/2<<endl;
                        gracz.klucz=maze[gracz.y][gracz.x];
                        maze[gracz.y][gracz.x]=0;
                    }
                }
            }
            else /** jesli gracz natrafia na drzwi, wartosci nieparzyste wieksze od 1 w macierzy labiryntu reprezentowane sa przez drzwi */
            {
                cout<<endl<<"NATRAFIONO NA DRZWI NR "<<(maze[gracz.y][gracz.x]-1)/2<<endl;

                if (gracz.klucz==maze[gracz.y][gracz.x]-1) /** jesli gracz posiada klucz odpowiadajacy napotkanym drzwiom */
                {
                    cout<<"OTWORZYC DRZWI NR "<<(maze[gracz.y][gracz.x]-1)/2<<" ? T/N"<<endl;

                    do cin>>odp;
                    while ((odp!='t')&&(odp!='T')&&(odp!='n')&&(odp!='N'));

                    if ((odp=='t')||(odp=='T'))
                    {
                        cout<<"OTWORZONO DRZWI NR "<<(maze[gracz.y][gracz.x]-1)/2<<endl;
                        gracz.klucz=0; /** zuzycie klucza znajdujacego sie w ekwipunku */
                        maze[gracz.y][gracz.x]=0; /** usuniecie drzwi z labiryntu */
                    }
                }
                /** anulowanie przesuniecia, gdyz natrafiono na drzwi */
                gracz.y=gracz.y-ky;
                gracz.x=gracz.x-kx;
            }
        }
        if (gracz.y==0) return; /** jesli gracz dotrze na koniec labiryntu, funkcja zostanie zakonczona */
    }
}
int main()
{
    int i, maze[25][25];
    entity gracz;
    cout<<"Antonina Stefanowska 2015r. - LABIRYNT"<<endl;

    if (!wczytaj(maze,gracz)) /** wywolanie funkcji wczytujacej labirynt z pliku */
    {
        cout<<"BLAD PLIKU Z LABIRYNTEM"<<endl;
        return 0;
    }
    gracz.klucz=0;
    graj(gracz,maze);
    cout<<endl<<"GRATULACJE! WYGRANA!"<<endl;
    getch();
    return 0;
}
