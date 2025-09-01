# AiSD1
# Interpreter stosowego procesora

Interpreter prostego stosowego procesora napisanego w C++.  
Procesor działa na stosie list znakowych i obsługuje własny zestaw instrukcji.

## Opis działania

- Program wczytuje kod źródłowy z **pierwszej linii wejścia**.  
- **Druga linia wejścia** zawiera znaki, które będą podawane na standardowe wejście programu (instrukcja `.`).  
- Interpreter wykonuje program krok po kroku, modyfikując stos i wypisując dane na standardowe wyjście zgodnie z instrukcjami.  

### Format wejścia
1. Pierwsza linia – program do wykonania (ciąg instrukcji bez spacji).  
2. Druga linia – dane wejściowe (ciąg znaków, maks. 20000).  

### Format wyjścia
Wszystko to, co procesor wypisze na standardowe wyjście w trakcie działania programu.

---

## Lista instrukcji

| Instrukcja | Nazwa            | Działanie |
|------------|------------------|-----------|
| `'`        | apostrof         | włóż na stos pustą listę |
| `,`        | przecinek        | zdejmij listę ze stosu |
| `:`        | dwukropek        | włóż na stos kopię listy z wierzchołka stosu |
| `;`        | średnik          | zamień miejscami dwie górne listy ze stosu |
| `@`        | at               | zdejmij liczbę A i włóż kopię listy z A-tej pozycji stosu (0 = wierzchołek) |
| `.`        | kropka           | wczytaj znak z wejścia i dołącz go na początek listy na wierzchołku stosu |
| `>`        | większe niż      | wypisz pierwszy znak listy z wierzchołka stosu i zdejmij tę listę |
| `!`        | wykrzyknik       | negacja logiczna – pustą listę lub `'0'` zamień na `'1'`, w pozostałych przypadkach zamień na `'0'` |
| `<`        | mniejsze niż     | zdejmij A i B; jeśli B < A, włóż `1`, inaczej `0` |
| `=`        | równe            | zdejmij A i B; jeśli B = A, włóż `1`, inaczej `0` |
| `~`        | tylda            | włóż na stos numer bieżącej instrukcji |
| `?`        | znak zapytania   | zdejmij T i W; jeśli W ≠ pusty/`0`, ustaw wskaźnik instrukcji na T |
| `-`        | minus            | jeśli lista kończy się na `-`, usuń go; inaczej dodaj `-` na końcu |
| `^`        | daszek           | usuń końcowy `-` z listy (wartość bezwzględna) |
| `$`        | dolar            | odłącz pierwszy znak listy z wierzchołka stosu i włóż go jako nową listę |
| `#`        | hasz             | zdejmij listę A i dołącz ją na końcu listy z wierzchołka stosu |
| `+`        | plus             | zdejmij liczby A i B, włóż A+B |
| `&`        | ampersand        | wypisz cały stos w formacie: `n: lista` … `0: lista` |
| `]`        | nawias kwadratowy prawy | zdejmij liczbę A, włóż znak ASCII o kodzie A |
| `[`        | nawias kwadratowy lewy | zdejmij listę A, włóż kod ASCII pierwszego znaku z listy |
| pozostałe  | znaki            | każdy inny znak dołącz na początek listy z wierzchołka stosu |

---

## Kompilacja

Wystarczy standardowy kompilator C++17 lub nowszy:

```bash
g++ -std=c++17 -O2 -o interpreter main.cpp
