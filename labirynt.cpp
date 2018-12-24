/** Antonina Stefanowska I grupa, kierunek: informatyka, I rok, 2015r. */

#include <iostream>
#include <fstream> /** do obslugi plików */
#include <conio.h> /** do obslugi klawiszy */
#include <windows.h> /** do celów prezentacji graficznej, do zmian ustawien konsoli, tj. kolorów czcionek, rozmiarów okna i przesuwania kursora konsoli */
using namespace std;

/** do funkcji zmianiejacych ustawienia konsoli */
HANDLE hStdin=GetStdHandle(STD_INPUT_HANDLE);
HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE);

struct entity
{
    int x, y, klucz, kier;
    char symbol;
};
struct wynik
{
    int wartosc;
    string nick;
};
struct lista_lin /** struktura listy liniowej jednokierunkowej wykorzystywana do rankingów */
{
    wynik w;
    lista_lin *nast;
};
void kolor(int x) /** ustala kolor czczionki konsoli */
{
    SetConsoleTextAttribute(hStdout,x);
}
void rozmiar(int x, int y) /** ustala rozmiar okna konsoli */
{
     COORD RozmiarBufora;
     RozmiarBufora.X=x;
     RozmiarBufora.Y=y;
     SMALL_RECT RozmiarOkna;
     RozmiarOkna.Left=0;
     RozmiarOkna.Top=0;
     RozmiarOkna.Right=x-1;
     RozmiarOkna.Bottom=y-1;
     SetConsoleScreenBufferSize(hStdout,RozmiarBufora);
     SetConsoleWindowInfo(hStdout,true,&RozmiarOkna);
     SetConsoleScreenBufferSize(hStdout,RozmiarBufora);
}
int gotoxy(int x, int y) /** przesuwa kursor konsoli do miejsca o podanych wspólrzednych */
{
    COORD coord;
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
void czysc_wiersz(int x) /** czysci podany wiersz konsoli */
{
    int i;
    gotoxy(0,x);
    for (i=0; i<27; i++) cout<<' ';
}
void eksportuj_wyniki(lista_lin *lista, char wariant) /** zapisuje listy rankingowe do odpowiadajacych im plików */
{
    lista_lin *el=lista;
    ofstream wyjscie((string("wyniki")+wariant+string(".txt")).c_str());
    while (el)
    {
        wyjscie<<endl<<el->w.nick<<' '<<el->w.wartosc;
        el=el->nast;
    }
    wyjscie.close();
}
void lista_rankingowa(lista_lin *ranking) /** wyswietla liste rankingowa */
{
    int i, j, strona=0, key, n=1;
    lista_lin *el=ranking; /** wskaznik na aktualny element listy */

    for (i=1; i<=44; i++) czysc_wiersz(i); /** czyszczenie konsoli */
    gotoxy(0,3);

    /** wyswietlenie po kolei wyników */
    do
    {
        for(i=0; i<12; i++)
        {
            /** wyswietlenie linii i liczb porzadkowych */
            gotoxy(0,1+3*i);
            for(j=0; j<27; j++) cout<<'_';
            Sleep(20);
            gotoxy(1,3+3*i);
            cout<<i+1+strona*12<<'.';
            gotoxy(5,3+3*i);

            /** jesli nie osiagniety jeszcze zostal koniec listy, nastepuje wyswietlenie jej kolejnego elementu */
            if (el)
            {
                cout<<el->w.nick;
                gotoxy(20,3+3*i);
                cout<<el->w.wartosc;
                el=el->nast;
            }

        }
        do
        {
            /** jesli aktualna strona nie jest poczatkowa, wyswietlona jest opcja wyswietlenia poprzedniej strony */
            if (strona>0)
            {
                if ((key==75)&&(n>0)) n--; /** jesli wcisniety jest klawisz strzalki w lewo, wartosc zmiennej n maleje o 1 */
                gotoxy(2,42);
                if ((n==0)&&(key==75)) kolor(0x03); /** podswietlenie zaznaczonego wyboru */
                else kolor(0x30);
                cout<<"<-";
                kolor(0x30);
            }
            else /** opcja jest nieaktywna jesli uzytkownik znajduje sie na pierwszej stronie */
            {
                gotoxy(2,42);
                cout<<"  ";
                if ((key==75)&&(n>1)) n--;
            }

            /** jesli nie zostal jeszcze napotkany koniec listy, wyswietlona jest opcja wyswietlenia nastepnej strony */
            if (el)
            {
                if ((key==77)&&(n<2)) n++; /** jesli wcisniety jest klawisz strzalki w prawo, wartosc zmiennej n rosnie o 1 */
                gotoxy(21,42);
                if (n==2) kolor(0x03); /** podswietlenie zaznaczonego wyboru */
                else kolor(0x30);
                cout<<"->";
                kolor(0x30);
            }
            else /** opcja jest nieaktywna, jesli juz cala lista zostala wyswietlona i wskaznik el ma wartosc null */
            {
                gotoxy(21,42);
                cout<<"  ";
                if ((key==77)&&(n<1)) n++;
            }

            /** przycisk powrotu do menu */
            gotoxy(10,42);
            if (n==1) kolor(0x03);
            else kolor(0x30);
            cout<<"MENU";
            kolor(0x30);

            key=getch(); /** wczytanie klawisza */
        } while(key!=13); /** petla dziala az do momentu wcisniecia klawisza enter */
        if ((n==0)||(n==2))
        {
            for (j=1; j<=40; j++) czysc_wiersz(j);
            if (n==2) strona++; /** jesli wybór padl na opcje przejrzenia nastepnej strony */
            if (n==0) /** jesli wybór padl na opcje przejrzenia poprzedniej strony */
            {
                strona--;
                el=ranking; /** zresetowanie wskaznika */
                for(j=0; j<strona*12; j++) el=el->nast; /** w zaleznosci od numeru strony, wskaznik listy jest przesuwany do odpowiedniego miejsca */
            }
        }
    } while(n!=1);

    /** czyszczenie konsoli */
    for (i=0; i<44; i++)
    {
        czysc_wiersz(i);
        Sleep(20);
    }
}
lista_lin *dodaj(wynik rekord, lista_lin *lista) /** umieszcza element na liscie, tak aby wartosci na niej umieszczone byly uporzadkowane od najmniejszych do najwiekszych */
{
    lista_lin *k=lista, *el=new lista_lin;
    /** utworzenie nowego elementu */
    el->w=rekord;
    el->nast=0;

    if (!lista) return el; /** jesli lista jest pusta, jej poczatkiem zostaje aktualny element */

    /** jesli pierwszy element listy jest wiekszy niz aktualny, to aktualny element laduje na poczatek listy */
    if (rekord.wartosc<k->w.wartosc)
    {
        el->nast=lista;
        return el;
    }
    /** przesuwanie wskaznika tak dlugo az zostanie napotkana wartosc mniejsza lub osiagniety jest koniec listy */
    while ((k->nast)&&(rekord.wartosc>k->nast->w.wartosc))
        k=k->nast;

    /** wlaczanie elementu do listy */
    el->nast=k->nast;
    k->nast=el;
    return lista;
}
lista_lin *nakoniec(wynik rekord, lista_lin *lista) /** umieszcza element na koncu listy */
{
    lista_lin *kon=lista, *el=new lista_lin;

    /** utworzenie nowego elementu */
    el->w=rekord;
    el->nast=0;

    if (!lista) return el; /** jesli lista jest pusta, jej poczatkiem zostaje aktualny element */
    /** przesuwanie wskaznika na koniec listy */
    while (kon->nast)
        kon=kon->nast;

    kon->nast=el; /** wlaczanie elementu do listy */
    return lista;
}
void wczytaj_wyniki(lista_lin *&lista, char wariant) /** wczytuje zawartosc pliku z wynikami do listy */
{
    ifstream plik((string("wyniki")+wariant+string(".txt")).c_str());
    wynik rekord;
    if (!plik) /** jesli plik z wynikami nie istnieje, tworzony jest nowy, a funkcja zostaje przerwana */
    {
        ofstream utworz((string("wyniki")+wariant+string(".txt")).c_str());
        utworz.close();
        return;
    }

    while (!plik.eof())
    {
        plik>>rekord.nick>>rekord.wartosc;
        if (rekord.nick!="") lista=nakoniec(rekord,lista); /** jesli element w pliku nie jest pusty, umieszczany jest na koncu listy */
    }
    plik.close();
}
bool wczytaj_labirynt(int maze[][25], char &wariant, entity &gracz) /** wczytuje plik labiryntu */
{
    ifstream labirynt;
    wariant=char(rand()%3+49); /** losowanie wariantu */
    labirynt.open((string("labirynt")+wariant+string(".txt")).c_str()); /** otworzenie wylosowanego wariantu */
    /** przerwanie dzialania funkcji w razie, gdy plik z labiryntem nie istnieje, zwrócenie wartosci falsz do funkcji main() */
    if (!labirynt) return false;;

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
void pisz(string s) /** wyswietla komunikat */
{
    int i;
    kolor(0x30);
    czysc_wiersz(30); /** czysci poprzedni komunikat */
    gotoxy(1,30); /** przesuwa kursor konsoli */

    /** wyswietla dany napis znak po znaku z krótkimi odstepami czasu, tworzac efekt "pisania" */
    for (i=0; i<s.size(); i++)
    {
        cout<<s[i];
        Sleep(25);
    }
}
bool odp() /** pyta uzytkownika o podjeta decyzje */
{
    int n=0, key=75;
    do
    {
        /** zmiana wartosci zmiennej n w zaleznosci od kodu wcisnietego klawisza */
        if (key==77) n=0; /** strzalka w prawo */
        if (key==75) n=1; /** strzalka w lewo */

        /** podswietlanie mozliwego wyboru przez zmiane koloru czcionki konsoli */
        if (n==0) /** jesli zostala wcisnieta strzalka w prawo, podswietlony jest wybór "TAK" */
        {
            gotoxy(2,35);
            kolor(0x30);
            cout<<"NIE";
            gotoxy(22,35);
            kolor(0x03);
            cout<<"TAK";
        }
        else /** jesli zostala wcisnieta strzalka w lewo, podswietlony jest wybór "NIE" */
        {
            gotoxy(2,35);
            kolor(0x03);
            cout<<"NIE";
            gotoxy(22,35);
            kolor(0x30);
            cout<<"TAK";
        }
        key=getch(); /** wczytanie klawisza */
    } while (key!=13); /** petla dziala do momentu, az zostanie wcisniety klawisz enter */

    kolor(0x30);
    czysc_wiersz(30);
    czysc_wiersz(35);

    if (n%2==0) return true; /** jesli zostala wybrana opcja "TAK" */
    else return false; /** jesli zostala wybrana opcja "NIE" */
}
void wygrana(lista_lin *&ranking, char wariant, wynik rekord, bool &restart) /** procedury zwiazane z zakonczeniem gry */
{
    int key;
    pisz(string("GRATULACJE! WYGRANA!"));
    gotoxy(15,41);

    cout<<"WARIANT "<<wariant;
    do key=getch();
    while (key!=13);

    pisz(string("ZAPISAC WYNIK?"));
    if (odp()) /** wywolanie funkcji odp() */
    {
        pisz(string("PODAJ IMIE (max 10 znakow)"));
        gotoxy(1,35);
        /** wczytanie nazwy uzytkownika do wyswietlenia wyniku w rankingu */
        do cin>>rekord.nick;
        while((rekord.nick).size()>10);

        ranking=dodaj(rekord,ranking); /** dodanie wyniku w odpowiednie miejsce listy rankingowej */
        eksportuj_wyniki(ranking,wariant); /** zapisanie listy do pliku */

        czysc_wiersz(33);
        czysc_wiersz(35);
    }
    pisz(string("OD NOWA?"));

    /** wywolanie funkcji odp() i ustalenie wartosci zmiennej restart, od której zalezy dalsze dzialanie petli rozpoczynajacej gre w funkcji main() */
    if (odp()) restart=true;
    else restart=false;

    /** czyszczenie konsoli */
    for (int i=0; i<45; i=i+2)
    {
        if (i==44) i=1;
        czysc_wiersz(i);
        Sleep(20);
    }
}
void wyswietl(entity gracz, int maze[][25], int kroki) /** wyswietla na konsoli aktualny stan gry */
{
    int i, j;

    /** wyswietlenie aktualnego wyniku, tj. liczby przebytych kroków */
    gotoxy(4,41);
    kolor(0x30);
    if (kroki<10000)
    {
        if (kroki<1000) cout<<'0';
        if (kroki<100) cout<<'0';
        if (kroki<10) cout<<'0';
        cout<<kroki;
    }

    gotoxy(0,0);
    kolor(0x07);

    /** wyswietlenie calego labiryntu w razie wygranej */
    if (gracz.y==0)
    {
        gotoxy(gracz.x+1,gracz.y+1);
        cout<<gracz.symbol;
        for(i=0; i<25; i++)
        {
            for (j=0; j<25; j++)
            {
                gotoxy(j+1,i+1);
                if ((i!=gracz.y)||(j!=gracz.x))
                {
                    if (maze[i][j]==0) cout<<' ';
                    else cout<<char(219);
                }
            }
            Sleep(20);
        }
        return; /** zakonczenie dzialania funkcji */
    }
    /** wyswietlanie kwadratu 3x3 wokól polozenia gracza */
    for (i=-1; i<=1; i++) /** zmienna i sluzy do pionowego przesuwania sie po polach otaczajacych gracza */
    {
        for (j=-1; j<=1; j++) /** zmienna j sluzy do poziomego przesuwania sie po polach otaczajacych gracza */
        {
            if ((i==0)&&(j==0)) /** srodek kwadratu */
            {
                /** wyswietlanie symbolu reprezentujacego gracza w jego aktualnym polozeniu */
                gotoxy(gracz.x+1,gracz.y+1);
                cout<<gracz.symbol;
            }
            else
            {
                gotoxy(gracz.x+1+j,gracz.y+1+i); /** przesuwanie kursora konsoli */

                if (maze[gracz.y+i][gracz.x+j]==0) cout<<' '; /** puste przestrzenie w macierzy labiryntu reprezentowane sa przez liczbe 0 */
                if (maze[gracz.y+i][gracz.x+j]==1) cout<<char(219); /** sciany w macierzy labiryntu reprezentowane sa przez liczbe 1 */
                if (maze[gracz.y+i][gracz.x+j]>1) /** obiekty specjalne */
                {
                    if (maze[gracz.y+i][gracz.x+j]%2==0) /** klucze */
                    {
                        switch(maze[gracz.y+i][gracz.x+j])
                        {
                            case 2:
                                kolor(0x0C);
                                cout<<char(3);
                                break;
                            case 4:
                                kolor(0x09);
                                cout<<char(6);
                                break;
                            case 6:
                                kolor(0x0A);
                                cout<<char(5);
                                break;
                            case 8:
                                kolor(0x0E);
                                cout<<char(4);
                                break;
                        }
                        kolor(0x07);
                    }
                    else /** drzwi */
                    {
                        switch(maze[gracz.y+i][gracz.x+j])
                        {
                            case 3:
                                kolor(0xC0);
                                cout<<char(3);
                                break;
                            case 5:
                                kolor(0x90);
                                cout<<char(6);
                                break;
                            case 7:
                                kolor(0xA0);
                                cout<<char(5);
                                break;
                            case 9:
                                kolor(0xE0);
                                cout<<char(4);
                                break;
                        }
                        kolor(0x07);
                    }
                }
            }
        }
        switch(gracz.kier) /** w zaleznosci od kierunku, w którym skierowany jest gracz, 3 pola wyswietlone za nim, sa ukrywane */
        {
            case 0: /** kierunek: do góry, ukrywa 3 pola z dolu */
                if (gracz.y!=23) /** jesli gracz jest odwrócony tylem do sciany ograniczajacej labirynt, pola nie sa ukrywane */
                {
                    gotoxy(gracz.x+1+i,gracz.y+1+2);
                    cout<<' ';
                }
                break;
            case 1: /** kierunek: w dól, ukrywa 3 pola z góry */
                if (gracz.y!=1)
                {
                    gotoxy(gracz.x+1+i,gracz.y+1-2);
                    cout<<' ';
                }
                break;
            case 2: /** kierunek: w prawo, ukrywa 3 pola z lewej */
                if (gracz.x!=1)
                {
                    gotoxy(gracz.x+1-2,gracz.y+1+i);
                    cout<<' ';
                }
                break;
            case 3: /** kierunek: w lewo, ukrywa 3 pola z prawej */
                if (gracz.x!=23)
                {
                    gotoxy(gracz.x+1+2,gracz.y+1+i);
                    cout<<' ';
                }
                break;
        }
    }

    /** wyswietlenie posiadanego klucza */
    gotoxy(2,41);
    switch(gracz.klucz)
    {
        case 0:
            kolor(0x30);
            cout<<' ';
            break;
        case 2:
            kolor(0x3C);
            cout<<char(3);
            break;
        case 4:
            kolor(0x39);
            cout<<char(6);
            break;
        case 6:
            kolor(0x3A);
            cout<<char(5);
            break;
        case 8:
            kolor(0x3E);
            cout<<char(4);
            break;
    }
    gotoxy(1,30);
}
void graj(entity gracz, int maze[][25], wynik &rekord) /** funkcja odpowiadajaca za mechanizm glówny gry */
{
    int ky=0, kx=0, k=0, key;
    char symbol[4]={int(3),int(6),int(5),int(4)};
    while (true) /** nieskonczona petla */
    {
        getch();
        do key=getch(); /** wczytanie klawisza */
        while ((key!=72)&&(key!=80)&&(key!=77)&&(key!=75));

        czysc_wiersz(30);
        switch(key)
        {
            case 72: /** strzalka w góre */
                ky=-1; /** przesuniecie pionowe */
                kx=0; /** przesuniecie poziome */
                gracz.kier=0; /** nadanie kierunku wstepnego */
                gracz.symbol=char(30); /** graficzne przedstawienie kierunku w konsoli, wartosc ta wykorzystywana jest w funkcji wyswietl() */
                break;
            case 80: /** strzalka w dól */
                ky=1;
                kx=0;
                gracz.kier=1;
                gracz.symbol=char(31);
                break;
            case 77: /** strzalka w prawo */
                ky=0;
                kx=1;
                gracz.kier=2;
                gracz.symbol=char(16);
                break;
            case 75: /** strzalka w lewo */
                ky=0;
                kx=-1;
                gracz.kier=3;
                gracz.symbol=char(17);
                break;
        }
        if (gracz.kier==k) /** gracz porusza sie tylko wtedy, gdy zostanie przez niego wybrany kierunek ruchu, w przeciwnym wypadku jedynie sie obraca */
        {
            if (maze[gracz.y+ky][gracz.x+kx]!=1) /** jezeli pole, na które ma przemiescic sie gracz jest dozwolone, nastepuje przemieszczenie sie; sciany w macierzy reprezentowane sa przez wartosc 1 */
            {
                gracz.y=gracz.y+ky;
                gracz.x=gracz.x+kx;
                rekord.wartosc++;
            }
            if (maze[gracz.y][gracz.x]>1) /** jesli gracz natrafia na drzwi lub klucz */
            {
                if (maze[gracz.y][gracz.x]%2==0) /** jesli gracz natrafia na klucz, wartosci parzyste wieksze od 1 w macierzy labiryntu sa reprezentowane przez klucze */
                {
                    pisz(string("WZIAC KLUCZ ")+symbol[maze[gracz.y][gracz.x]/2-1]+string("?"));
                    if (odp()) /** wywolanie funkcji odp(), która pyta uzytkownika o jego wybór, zwraca falsz lub prawde */
                    {
                        if (gracz.klucz!=0) /** jesli gracz ma juz jakis klucz w posiadaniu */
                        {
                            pisz(string("BRAK MIEJSCA"));

                            /** potwierdzenie komunikatu enterem */
                            do key=getch();
                            while (key!=13);

                            pisz("ZAMIENIC KLUCZE?");
                            if (odp()) /** wywolanie funkcji odp() */
                            {
                                pisz(string("ZOSTAWIONO TU KLUCZ ")+symbol[gracz.klucz/2-1]);

                                do key=getch();
                                while (key!=13);

                                pisz(string("WZIETO KLUCZ ")+symbol[maze[gracz.y][gracz.x]/2-1]);
                                swap(maze[gracz.y][gracz.x],gracz.klucz); /** zamiana klucza w ekwipunku z tym lezacym na ziemi */

                                do key=getch();
                                while (key!=13);
                            }
                        }
                        else /** jesli gracz nie ma jeszcze zadnego klucza */
                        {
                            pisz(string("WZIETO KLUCZ ")+symbol[maze[gracz.y][gracz.x]/2-1]);
                            gracz.klucz=maze[gracz.y][gracz.x];
                            maze[gracz.y][gracz.x]=0;
                            do key=getch();
                            while (key!=13);
                        }
                    }
                    /** anulowanie przesuniecia sie, gdyz natrafiono na klucz */
                    gracz.y=gracz.y-ky;
                    gracz.x=gracz.x-kx;
                    rekord.wartosc--;
                }
                else /** jesli gracz natrafia na drzwi, wartosci nieparzyste wieksze od 1 w macierzy labiryntu reprezentowane sa przez drzwi */
                {
                    pisz(string("NATRAFIONO NA DRZWI ")+symbol[(maze[gracz.y][gracz.x]-1)/2-1]);
                    do key=getch();
                    while (key!=13);
                    czysc_wiersz(30);

                    if (gracz.klucz==maze[gracz.y][gracz.x]-1) /** jesli gracz posiada klucz odpowiadajacy napotkanym drzwiom */
                    {
                        pisz(string("OTWORZYC DRZWI ")+symbol[(maze[gracz.y][gracz.x]-1)/2-1]+string("?"));
                        if (odp()) /** wywolanie funkcji odp() */
                        {
                            pisz(string("OTWORZONO DRZWI ")+symbol[(maze[gracz.y][gracz.x]-1)/2-1]);
                            gracz.klucz=0; /** zuzycie klucza znajdujacego sie w ekwipunku */
                            maze[gracz.y][gracz.x]=0; /** usuniecie drzwi z labiryntu */

                            do key=getch();
                            while (key!=13);
                        }
                    }
                    /** anulowanie przesuniecia, gdyz natrafiono na drzwi */
                    gracz.y=gracz.y-ky;
                    gracz.x=gracz.x-kx;
                    rekord.wartosc--;
                }
            }
        }
        wyswietl(gracz,maze,rekord.wartosc); /** wyswietlenie na konsoli aktualnego stanu gry */
        k=gracz.kier; /** jesli zmienna kierunku gracz.kier nie zostanie zmieniona w nastepnym przejsciu petli (gracz sie nie obróci), to k bedzie równa gracz.kier i gracz ruszy naprzód */
        if (gracz.y==0) return; /** jesli gracz dotrze na koniec labiryntu, funkcja zostanie zakonczona, nastepuje powrót do funkcji main() i wywolanie funkcji wygrana() */
    }
}
void drukuj_plansze() /** drukuje obszar gry */
{
    int i, j, tab[27][27]={0}, losx, losy;

    /** czysci konsole */
    for (i=1; i<=27; i++)
    {
        czysc_wiersz(i);
        Sleep(10);
    }
    czysc_wiersz(42);

    kolor(0x07); /** zmiana koloru czcionki konsoli */

    /** animacja przejscia */
    for (i=0; i<729; i++)
    {
        /** losowanie pola do wyswietlenia tak dlugo, az natrafi sie na te, które jeszcze nie zostalo wydrukowane */
        do
        {
            losx=rand()%27;
            losy=rand()%27;
        } while (tab[losx][losy]!=0);

        gotoxy(losx,losy);
        tab[losx][losy]=1; /** zaznaczanie, ze pole to zostalo juz wyswietlone */

        /** ramka wokól labiryntu */
        if ((losx==0)||(losx==26))
        {
            if((losy==0)||(losy==26))
                cout<<char(219);
            else cout<<char(186);
        }
        else
        {
            if((losy==0)||(losy==26))
                cout<<char(205);
            else cout<<' ';
        }
        Sleep(1);
    }

    kolor(0x30); /** powrót do poprzedniego koloru czczionki */

    /** rysowanie linii */
    gotoxy(0,28);
    for (i=0; i<27; i++) cout<<'_';
    gotoxy(0,31);
    for (i=0; i<27; i++) cout<<'_';

    /** ramka otaczajaca posiadany klucz oraz aktualny wynik */
    for (i=0; i<3; i++)
    {
        for (j=0; j<8; j++)
        {
            gotoxy(1+j,40+i);
            if (i==0)
            {
                switch(j)
                {
                    case 0:
                        cout<<char(218);
                        break;
                    case 2:
                        cout<<char(194);
                        break;
                    case 7:
                        cout<<char(191);
                        break;
                    default:
                        cout<<char(196);
                        break;
                }
            }
            if (i==1)
            {
                if ((j==0)||(j==2)||(j==7)) cout<<char(179);
                else cout<<' ';
            }
            if (i==2)
            {
                switch(j)
                {
                    case 0:
                        cout<<char(192);
                        break;
                    case 2:
                        cout<<char(193);
                        break;
                    case 7:
                        cout<<char(217);
                        break;
                    default:
                        cout<<char(196);
                        break;
                }
            }
        }
    }
}
void logo() /** wyswietlenie ekranu powitalnego */
{
    int x=13, y=13, k=1, i=1, j;
    string s="LABIRYNT";

    /** rysowanie spirali */
    for (i=1; i<25; i=i+2, k=k*(-1))
    {
        for (j=0; j<i; j++)
        {
            x=x+k;
            gotoxy(x,y);
            cout<<char(219);
            Sleep(15-i/2);
        }
        if (i==23) j=2;
        else j=0;
        for (j; j<i; j++)
        {
            y=y+k;
            gotoxy(x,y);
            cout<<char(219);
            Sleep(15-i/2);
        }
        if (i/2-1<=s.size())
        {
            gotoxy(i+3,1);
            cout<<s[i/2-1];
        }
    }
    gotoxy(1,42);
    cout<<"Antonina Stefanowska 2015";
}
int menu(string choice1, string choice2, string choice3) /** funkcja wyswietlajaca uzytkownikowi dostepne opcje do wyboru */
{
    int i, j, n=0, key=75;

    /** narysowanie linii */
    gotoxy(0,28);
    for(i=0; i<4; i++)
    {
        gotoxy(0,28+3*i);
        for(j=0; j<27; j++)
        {
            Sleep(10);
            cout<<'_';
        }
    }

    do
    {
        /** zmiana wartosci zmiennej n w zaleznosci od wczytanego kodu klawisza */
        if ((key==72)&&(n>0)) n--; /** dla klawisza strzalki w lewo */
        if ((key==80)&&(n<2)) n++; /** dla klawisza strzalki w prawo */

        /** podswietlenie wybranej przez uzytkownika opcji, przez zmiane koloru czcionki konsoli */
        switch(n)
        {
            case 0:
                kolor(0x03);
                gotoxy(2,30);
                cout<<choice1;
                kolor(0x30);
                gotoxy(2,33);
                cout<<choice2;
                gotoxy(2,36);
                cout<<choice3;
                break;
            case 1:
                gotoxy(2,30);
                cout<<choice1;
                kolor(0x03);
                gotoxy(2,33);
                cout<<choice2;
                kolor(0x30);
                gotoxy(2,36);
                cout<<choice3;
                break;
            case 2:
                gotoxy(2,30);
                cout<<choice1;
                gotoxy(2,33);
                cout<<choice2;
                gotoxy(2,36);
                kolor(0x03);
                cout<<choice3;
                kolor(0x30);
                break;
        }
        key=getch(); /** wczytanie kodu klawisza */
    } while(key!=13); /** petla dziala do momentu wcisniecia przez uzytkownika klawisza enter */

    /** czyszczenie konsoli */
    for (i=0; i<10; i++)
    {
        Sleep(50);
        czysc_wiersz(28+i);
    }
    return n; /** zwrócenie wartosci wyboru uzytkownika do funkcji main() */
}
int main()
{
    int i, maze[25][25];
    bool restart=true;
    char wariant;
    entity gracz;
    wynik rekord;
    lista_lin *ranking1=0, *ranking2=0, *ranking3=0;
    srand(time(NULL));

    /** zmiana ustawien konsoli i jej czcionki */
    rozmiar(27,44);
    system("Color 30");

    /** wczytywanie poszczegolnych list rankingowych */
    wczytaj_wyniki(ranking1,'1');
    wczytaj_wyniki(ranking2,'2');
    wczytaj_wyniki(ranking3,'3');

    while(true)
    {
        logo(); /** wyswietlenie ekranu powitalnego */
        switch(menu(string("START"),string("RANKING"),string("KONIEC"))) /** wywolanie funkcji menu(), zwracajacej wartosc, od której zalezec bedzie przejscie do wybranej czesci programu */
        {
            case 0: /* start */
                do
                {
                    wariant=char(rand()%3+49); /** losowanie wariantu labiryntu */
                    if (!wczytaj_labirynt(maze,wariant,gracz)) /** wywolanie funkcji wczytujacej wylosowany labirynt */
                    {
                        gotoxy(1,26);
                        cout<<"BLAD Z PLIKIEM LABIRYNTU"<<endl;
                        system("Color 07");
                        rozmiar(80,25);
                        kolor(0x07);
                        return 0;
                    }
                    drukuj_plansze(); /** drukowanie obszaru do gry */

                    /** ustalanie stanu poczatkowego gracza */
                    gracz.klucz=0;
                    gracz.symbol=char(30);
                    gracz.kier=0;
                    rekord.wartosc=0;
                    wyswietl(gracz,maze,0);

                    graj(gracz,maze,rekord); /** rozpoczecie gry */

                    /** wywolanie funkcji wygrana() po zakonczeniu gry, w której odbywa sie ewentualne zapisanie wyniku do listy rankingowej zaleznej od wylosowanego wariantu */
                    switch(wariant)
                    {
                        case '1':
                            wygrana(ranking1,wariant,rekord,restart);
                            break;
                        case '2':
                            wygrana(ranking2,wariant,rekord,restart);
                            break;
                        case '3':
                            wygrana(ranking3,wariant,rekord,restart);
                            break;
                    }
                } while (restart); /** funkcja wygrana() ustala wartosc zmiennej restart w zaleznosci od wyboru uzytkownika */
                break;
            case 1: /** ranking */
                switch(menu(string("WARIANT 1"),string("WARIANT 2"),string("WARIANT 3"))) /** wywolanie funkcji menu() zwracajacej wartosc, od ktorej zalezec bedzie wyswietlenie listy rankingowej dla wybranego wariantu */
                {
                    case 0:
                        lista_rankingowa(ranking1);
                        break;
                    case 1:
                        lista_rankingowa(ranking2);
                        break;
                    case 2:
                        lista_rankingowa(ranking3);
                        break;
                }
                break;
            case 2: /** koniec */
                /** przywracanie ustawien domyslnych konsoli */
                system("Color 07");
                rozmiar(80,25);
                kolor(0x07);
                return 0; /** zakonczenie dzialania programu */
                break;
        }
    }
}
