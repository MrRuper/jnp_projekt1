#include <iostream>
#include <map>
#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <regex>
#include <iterator>

std::map<int,int> laczna_waga; //numer_piosenki, waga
std::map<int,int> top_historia; // numer_piosenki, numer_ostatniej_pozycji
std::map<int, int> pojedyncze_notowanie; // numer_piosenki, liczba_glosow
std::set<int> wyrzucone_piosenki;
std::map<int,int> poprzednie_notowanie; // numer_piosenki, miejsce

int max = INT_MIN;

// obsluga pojedynczego glosu
// zakladamy, że linia jest poprawnym głosem
void notowanie (std::vector<int>& glos){
    std::map<int,int>::iterator it;

    for (int i = 0; i < glos.size(); i++){
        it = pojedyncze_notowanie.find(glos[i]);

        if (it == pojedyncze_notowanie.end()){
            pojedyncze_notowanie[glos[i]] = 1;
        }
        else {
            it->second++;
        }
    }

    glos.clear();
}

void NEW_MAX (){
    std::map<int, std::set<int>> przepisz; // liczba_glosow, numery_piosenek
    std::vector<int> ostateczny_max;

    for (auto it : pojedyncze_notowanie){
        przepisz[it.second].insert(it.first);
    }

    int counter = 0;
    int miejsce = 1;

    for (auto iterator = przepisz.rbegin(); iterator != przepisz.rend(); iterator++){
        for (auto z : iterator->second){
            if (counter < 7){
                if (poprzednie_notowanie.find(z) != poprzednie_notowanie.end()){
                    std::cout << z << " " << poprzednie_notowanie[z] - miejsce << std::endl;
                }
                else {
                    std::cout << z << " -" << std::endl;
                }

                miejsce++;
                counter++;
                ostateczny_max.push_back(z);
            }
            else {
                break;
            }
        }
    }

    bool przeszlo_do_kolejnego;

    //aktualizacja wyrzuconych_piosenek
    for (auto x : poprzednie_notowanie){
        przeszlo_do_kolejnego = false;

        for (auto z : ostateczny_max){
            if (x.first == ostateczny_max[z]){
               przeszlo_do_kolejnego = true;
            }
        }

        if (!przeszlo_do_kolejnego){
            wyrzucone_piosenki.insert(x.first);
        }
    }

    poprzednie_notowanie.clear();

    //aktualizacja poprzednie_notowanie
    //akutalizacja laczna_waga
    for (int i = 0; i < ostateczny_max.size(); i++){
        poprzednie_notowanie[ostateczny_max[i]] = i + 1;
        laczna_waga[ostateczny_max[i]] += 7 - i;
    }

    pojedyncze_notowanie.clear();
    przepisz.clear();
}


void TOP (){
    std::map<int, std::set<int>> posortowana_waga; // liczba zdobytych puntkow, numery piosenek
    std::map<int,int> aktualny_top;

    for (auto it : laczna_waga){
        posortowana_waga[it.second].insert(it.first);
    }

    int counter = 1;
    int liczba_wzietych = 0;
    for (auto it = posortowana_waga.rbegin(); it != posortowana_waga.rend(); it++){
        for (auto it1 : it->second){
            if (liczba_wzietych < 7){
                auto z = top_historia.find(it1);

                if (z != top_historia.end()){
                    std::cout<< it1 << " " << z->second - counter << std::endl;
                }
                else {
                    std::cout << it1 << " -" << std::endl;
                }

                aktualny_top[it1] = counter;
                counter++;
                liczba_wzietych++;
            }
        }
    }

    top_historia.clear();

    top_historia = aktualny_top;
}

void error (const std::string& input, int numer_linii){
    std::cerr << "Error in line " << numer_linii << ": " << input << std::endl;
}

void funkcja_finalna (){
    int numer_linii = 1;
    int MAX_SONG = INT_MIN;
    int new_max;
    std::string s;
    int piosenka;
    int helper1, helper2;

    while (std::cin){
        std::getline(std::cin, s);

        std::regex word_regex ("(\\w+)");
        auto words_begin =
                std::sregex_iterator(s.begin(), s.end(), word_regex);
        auto words_end = std::sregex_iterator();

        size_t dst = std::distance (words_begin, words_end);

        if (dst == 1){
            if (words_begin->str() == "TOP"){
                TOP();
            }
            else {
                try{
                    piosenka = std::stoi (words_begin->str());
                }
                catch (std::invalid_argument &e){
                    error(s, numer_linii);
                    continue;
                }

                if (wyrzucone_piosenki.find(piosenka) != wyrzucone_piosenki.end()){
                    error(s, numer_linii);
                    continue;
                }
                if (piosenka > MAX_SONG || piosenka <= 0 || piosenka > 99999999){
                    error(s, numer_linii);
                    continue;
                }
                else {
                    std::vector<int> v;
                    v.push_back(piosenka);
                    notowanie(v);
                }
            }
        }
        else if (dst == 2){
                if (words_begin->str() == "NEW"){
                    try {
                        new_max = std::stoi(std::next(words_begin)->str());
                    }
                    catch(std::invalid_argument& e){
                        error(s, numer_linii);
                        continue;
                    }

                    if (new_max < MAX_SONG){
                        error(s, numer_linii);
                        continue;
                    }
                    else {
                        MAX_SONG = new_max;
                        NEW_MAX();
                    }
                }
                else {
                    try{
                        helper1 = std::stoi(words_begin->str());
                        helper2 = std::stoi(std::next(words_begin)->str());
                    }
                    catch(std::invalid_argument& e){
                        error(s, numer_linii);
                        continue;
                    }

                    if (wyrzucone_piosenki.find(helper2) != wyrzucone_piosenki.end() ||
                        wyrzucone_piosenki.find(helper1) != wyrzucone_piosenki.end() ||
                        helper1 == helper2){
                        error(s, numer_linii);
                        continue;
                    }
                    if (helper1 > MAX_SONG || helper1 <= 0 || helper1 > 99999999
                        || helper2 > MAX_SONG || helper2 <= 0 || helper2 > 99999999){
                        error(s, numer_linii);
                        continue;
                    }
                    else{
                        std::vector<int> wektor;
                        wektor.push_back(helper1);
                        wektor.push_back(helper2);
                        notowanie(wektor);
                    }
                }
            }
        else {
            std::set<int> pojawilo_sie;
            std::vector<int> glos;
            bool niepowodzenie = false;

            for (auto word = words_begin; word != words_end; word++){
                try{
                    piosenka = std::stoi (word->str());
                }
                catch (std::invalid_argument& e){
                    error(s, numer_linii);
                    niepowodzenie = true;
                    break;
                }

                if (wyrzucone_piosenki.find(piosenka) != wyrzucone_piosenki.end()
                    || piosenka > MAX_SONG || piosenka <= 0 || piosenka > 99999999){
                    error(s,numer_linii);
                    pojawilo_sie.clear();
                    glos.clear();
                    niepowodzenie = true;
                    break;
                }

                auto it = pojawilo_sie.insert(piosenka);

                if (!it.second){
                    error(s,numer_linii);
                    pojawilo_sie.clear();
                    glos.clear();
                    niepowodzenie = true;
                    break;
                }

                glos.push_back(piosenka);
            }
            pojawilo_sie.clear();

            if (!niepowodzenie){
                notowanie(glos);
            }
        }
        numer_linii++;
    }
}


int main() {
    funkcja_finalna();
    return 0;
}
