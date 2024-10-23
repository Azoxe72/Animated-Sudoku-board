#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

void affiche_plateau_(int** p) {
     for (int i=0; i < 9; i+=1) {
          printf(" ");
          for (int j=0; j < 9; j+=1) {
               if (p[i][j]!=0){
                    printf("%d ",p[i][j]);
               }
               else printf(". ");
          }
          printf("\n");
     }

}

#define N 9

void green(){
     printf("\033[0;32m");
}

void blanc(){
     printf("\033[0m");
}

bool est_valide(int grille[N][N], int ligne, int colonne, int num) {
    for (int x = 0; x < N; x++) {
        if (grille[ligne][x] == num || grille[x][colonne] == num) {
            return false;
        }
    }

    int debut_ligne = ligne - ligne % 3;
    int debut_colonne = colonne - colonne % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grille[i + debut_ligne][j + debut_colonne] == num) {
                return false;
            }
        }
    }

    return true;
}

bool trouve_case_vide(int grille[N][N], int* ligne, int* colonne) {
    for (*ligne = 0; *ligne < N; (*ligne)++) {
        for (*colonne = 0; *colonne < N; (*colonne)++) {
            if (grille[*ligne][*colonne] == 0) {
                return true;
            }
        }
    }
    return false;
}

void affiche_plateau(int grille[N][N], int grille_initiale[N][N], int ligne_teste, int colonne_teste, int num_teste) {
    clear();

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grille_initiale[i][j] != 0) {
                mvprintw(i, j * 2, "%d", grille_initiale[i][j]);
            } else if (i == ligne_teste && j == colonne_teste) {
                attron(A_BOLD | COLOR_PAIR(1));
                mvprintw(i, j * 2, "%d", num_teste);
                attroff(A_BOLD | COLOR_PAIR(1));
            } else if (grille[i][j] != 0) {
                mvprintw(i, j * 2, "%d", grille[i][j]);
            } else {
                mvprintw(i, j * 2, ".");
            }
        }
    }

    refresh();
    usleep(100000);
}

bool resoudre_sudoku_ncurses(int grille[N][N], int grille_initiale[N][N]) {
    int ligne, colonne;

    if (!trouve_case_vide(grille, &ligne, &colonne)) {
        return true;
    }

    for (int num = 1; num <= 9; num++) {
        if (est_valide(grille, ligne, colonne, num)) {
            grille[ligne][colonne] = num;

            affiche_plateau(grille, grille_initiale, ligne, colonne, num);

            if (resoudre_sudoku_ncurses(grille, grille_initiale)) {
                return true;
            }

            grille[ligne][colonne] = 0;

            affiche_plateau(grille, grille_initiale, ligne, colonne, num);
        }
    }

    return false;
}

void initialiser_grille(int grille[N][N], int grille_initiale[N][N]) {
    int grille_de_depart[N][N] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grille[i][j] = grille_de_depart[i][j];
            grille_initiale[i][j] = grille_de_depart[i][j];
        }
    }
}

int main() {
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    noecho();
    cbreak();

    int grille[N][N] = {0};
    int grille_initiale[N][N] = {0};

    initialiser_grille(grille, grille_initiale);

    if (resoudre_sudoku_ncurses(grille, grille_initiale)) {
        mvprintw(N + 1, 0, "Grille résolue !");
    } else {
        mvprintw(N + 1, 0, "Aucune solution trouvée.");
    }

    getch();
    endwin();

    return 0;
}
