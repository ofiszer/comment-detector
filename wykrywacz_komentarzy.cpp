//POP 2019-01-16 projekt 2 Fiszer Olgierd EIT 5 171763 srodowisko Dev-Cpp 5.11 TDM-GCC 4.9.2 Setup, kompilator TDM-GCC 4.9.2 64-bit Release
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

bool sprawdz_komentarz_jednolinijkowy(string linia, size_t *pozycja_komentarza);

/*
 Funkcja czyta pierwsze litery ze słów ze słownika, aby ustalić na którym miejscu znajduje się
 pierwsze słowo rozpoczynające się na kolejną literę alfabetu.
 */
void znajdz_pierwsze_litery(ifstream *slownik, char *wsk_litery,
		int *wsk_pozycje) {
	string linia;
	int nr_litery_alfabetu = 0;
	int nr_linii = 0;
	getline(*slownik, linia);
	nr_linii++;
	wsk_litery[nr_litery_alfabetu] = linia[0];
	wsk_pozycje[nr_litery_alfabetu] = nr_linii;
	while (getline(*slownik, linia)) {
		nr_linii++;
		if (linia[0] == wsk_litery[nr_litery_alfabetu]) {
			continue;
		} else {
			nr_litery_alfabetu++;
			wsk_litery[nr_litery_alfabetu] = linia[0];
			wsk_pozycje[nr_litery_alfabetu] = nr_linii;
		}
	}
}

/*
 Funkcja sprawdza, czy w linijce jest komentarz jednowierszowy.
 */
bool sprawdz_kom(string str) {
	if (str.find("//") != std::string::npos) {
		return true;
	}
	return false;
}

/*
 Funkcja sprawdza, czy w linijce ma otwarcie komentarz wieloowierszowy.
 */
bool sprawdz_rozp_kom(string str) {
	if (str.find("/*") != std::string::npos) {
		return true;
	}
	return false;
}

/*
 Funkcja sprawdza, czy w linijce ma zamknięcie komentarz wielowierszowy.
 */
bool sprawdz_zamkn_kom(string str) {
	if (str.find("*/") != std::string::npos) {
		return true;
	}
	return false;
}

/*
 Funkcja sprawdza, na podstawie obecności komentarzy, czy ma sprawdzić linijkę i od którego znaku (jeśli to komentarz jednolinijkowy).
 */
bool czy_sprawdzic_linie(string linia, bool *obecnie_kom, size_t *pozycja_komentarza) {
	*pozycja_komentarza = 0;
	if (*obecnie_kom == false && sprawdz_komentarz_jednolinijkowy(linia, pozycja_komentarza) == true) {
		return true;
	}
	if (sprawdz_rozp_kom(linia) == true) {
		*obecnie_kom = true;
		return false;
	}
	if (sprawdz_zamkn_kom(linia) == true) {
		*obecnie_kom = false;
		return false;
	}
	if (*obecnie_kom == true) {
		return true;
	}
	return false;
}

/*
 Funkcja porównuje słowa z komentarzy w kodzie programu ze słowami ze słownika.
 */
bool sprawdz_w_slowniku(ifstream *slownik, string slowo, char *wsk_litery,
		int *wsk_pozycje) {
	slownik->clear();
	slownik->seekg(0, ios::beg);
	string slowo_ze_slownika;
	bool znaleziono_w_alfabecie = false;
	int pozycja_pocz;
	int pozycja_konc;
	for (int i = 0; i < 32; i++) {
		if (slowo[0] == wsk_litery[i]) // Instrukcja szuka danego słowa z komentarza w zbiorze słów zaczynających się na tę samą literę.
				{
			pozycja_pocz = wsk_pozycje[i];
			pozycja_konc = wsk_pozycje[i + 1];
			znaleziono_w_alfabecie = true;
			break;
		}
	}
	if (znaleziono_w_alfabecie == true) {
		int nr_linii = 0;
		while (getline(*slownik, slowo_ze_slownika)) {
			nr_linii++;
			if (nr_linii < pozycja_pocz) {
				continue;
			}
			if (slowo == slowo_ze_slownika) {
				return true;
			}
		}
		if (nr_linii > pozycja_konc) {
			return false;
		}
	}
	return false;
}

/*
 Funkcja usuwa znaki interpunkcyjne znajdujące się na końcu słowa w komentarzach.
 Na przykład "test," zmieni na "test", a "koniec zdania." 
 na "koniec zdania".
 */
string usun_interpunkcje(string slowo) {
	string st;
	if (ispunct(slowo[slowo.size() - 1])) {
		st = slowo.substr(0, slowo.size() - 1);
		return st;
	}
	return slowo;
}

/*
 Funkcja zamienia pierwsze litery w sprawdzanych słowach na małe.
 */
void na_male_litery(string &slowo) {
	unsigned int i = 0;
	slowo[i] = tolower(slowo[i]);
}

/*
 Funkcja sprawdzająca czy w linii jest komentarz jednolinijkowy i w którym miejscu linii jest jego rozpoczęcie.
 Funkcja uwzględnia możliwość wystąpienia "cout" oraz cudzysłowiów.
 */
bool sprawdz_komentarz_jednolinijkowy(string linia, size_t *pozycja_komentarza) {
	*pozycja_komentarza = linia.find("//");
	if (*pozycja_komentarza != std::string::npos) { 				// Znaleziono '//'
		size_t pozycja_cout;
		pozycja_cout = linia.find("cout");
		if (pozycja_cout == std::string::npos) { 					// Nie znaleziono 'cout'
			return true;
		} else { 													// Znaleziono również 'cout'
			if (pozycja_cout > *pozycja_komentarza)
				return true;
			else {
				size_t pozycja_cudzyslowia;
				size_t ostatnia_pozycja_cudzyslowia;
				bool parzysty_cudzyslow = true;
				pozycja_cudzyslowia = linia.find("\"", pozycja_cout + 1);
				while (pozycja_cudzyslowia != std::string::npos) {	// Poszukiwanie cudzysłowów
					parzysty_cudzyslow = !parzysty_cudzyslow;
					ostatnia_pozycja_cudzyslowia = pozycja_cudzyslowia;
					pozycja_cudzyslowia = linia.find("\"",
							pozycja_cudzyslowia + 1);
				}
				if (parzysty_cudzyslow == true) {					// Sprawdzenie czy wystąpiła parzysta liczba cudzysłowiów
					*pozycja_komentarza = linia.find("//",
							ostatnia_pozycja_cudzyslowia + 1);
					if (*pozycja_komentarza != std::string::npos) { // Sprawdzenie czy po wszystkich cudzysłowiach występuje "//"
						return true;
					}
				}
			}
		}
	}
	return false;
}

/*
 Funkcja sprawdza słowa z linijki po uprzednim stwierdzeniu obecności komentarzy.
 */
void sprawdz_linie(ifstream *slownik, string linia, int nr_linii,
		bool obecnie_kom, char *wsk_litery, int *wsk_pozycje) {
	istringstream iss(linia);
	string slowo;
	while (iss >> slowo) {
		if (slowo == "//") {
			obecnie_kom = true;
			continue;
		}

		na_male_litery(slowo);
		if (obecnie_kom == true) {
			slowo = usun_interpunkcje(slowo);
			if (slowo.size() != 0) {
				if (sprawdz_w_slowniku(slownik, slowo, wsk_litery, wsk_pozycje) == false) {		// Jeśli słowo nie zostało znalezione w słowniku,
					cout << "linia " << nr_linii << ":" << " " << slowo << endl;				// zostaje wypisane w konsoli jako błędne.
				}
			}
		}
	}
}

int main() {
	char litery[32]; 							// litery polskiego alfabetu
	int pozycje[32]; 							// numer linii, w ktorej jest nastepna litera alfabetu
	int *wsk_pozycje;
	wsk_pozycje = &pozycje[0];
	ifstream plik_do_sprawdzenia("przykladowy_plik_6.cpp");  // otwarcie pliku testowego
	ifstream slownik("slowa.txt"); 				// otwarcie pliku z lista slowek
	if (!plik_do_sprawdzenia)					// sprawdzenie, czy plik został otwarty
	{
		cout << "Plik z kodem nie zostal otwarty\n";
		return 1;
	}
	if (!slownik)	 							// sprawdzenie, czy plik został otwarty
	{
		cout << "Plik z lista slowek nie zostal otwarty\n";
		return 1;
	}
	znajdz_pierwsze_litery(&slownik, litery, wsk_pozycje);
	int nr_linii = 0;
	bool obecnie_kom = false;
	string linia;
	size_t pozycja_komentarza = 0;
	while (getline(plik_do_sprawdzenia, linia)) {
		nr_linii++;
		if (czy_sprawdzic_linie(linia, &obecnie_kom, &pozycja_komentarza) == true) {
			sprawdz_linie(&slownik, linia.substr(pozycja_komentarza) , nr_linii, obecnie_kom, litery, wsk_pozycje);
		}

	}
	return 0;
}
