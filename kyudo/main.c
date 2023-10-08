/**
 * @file main.c
 * @brief Programme de tests.
 * @author SANTOKI Nihar and ZHANG Victor
 * @version 0.1
 * @date 02 avril 2023
 *
 * Programme du main pour lancer une simulation de match.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>
//#include <windows.h> //Uniqument pour windows (pour Sleep()
#include "kyudo.h"

extern pthread_mutex_t mutex;
sem_t sem_debutDe_tour, sem_finDe_tour;

/**
 * function int main(int argc, char **argv)
 * @brief Fonction main qui teste tout le programme
 *
 * @param nom du compilateur (+ arguments optionnels)
 * @return 1 si tout c'est bien passer, 0 sinon
 */

int main(int argc, char **argv) {
    //vérifie si le fichier existe ou pas, s'il existe il vide le fichier
    existe_fichier();
    // Parse command ligne argument for number of equipes
    Match match;
    char *fichier;
    if (argc == 1) {
        fichier = "tournoi.txt";
        match.duree_match = 1000;
    } else if (argc == 2) {
        if (est_une_fichier(argv[1])) {
            fichier = argv[1];
        } else if (est_time(argv[1])) {
            match.duree_match = atoi(argv[2]);
        } else {
            printf("Mauvaise paramétres (le fichier que vous avez mis n'existe pas ou vous avez mis le durée de match dans une mauvaise format)\n");
        }
    } else if (argc == 3) {
        if (est_une_fichier(argv[1]) && est_time(argv[2])) {
            fichier = argv[1];
            match.duree_match = atoi(argv[2]);
        } else if (est_time(argv[1]) && est_une_fichier(argv[2])) {
            match.duree_match = atoi(argv[1]);
            fichier = argv[2];
        } else {
            printf("Mauvaise paramétres (le fichier que vous avez mis n'existe pas ou vous avez mis le durée de match dans une mauvaise format)\n");
        }
    } else {
        printf("Mauvaise paramétres (Vous avez mis plus de 3 arguments)\n");
    }

    int num_equipe = get_num_equipe(fichier);

    if(!VerfierPuissanceDeDeux(fichier)) {
        printf("Error: failed to open file, cause it doesn't contain power of 2 equipes %s\n", fichier);
        return 1;
    }

    Equipe equipes[num_equipe];
    initEquipesEtMatches(fichier, num_equipe, equipes);
    int num_matches = get_num_match(num_equipe);
    int num_rounds = 1;
    pthread_t threads[NB_TOUR_MAX][NB_MATCH_MAX];
    Equipe vainqueurs[num_matches / 2];  // store the vainqueurs of each match in each round

    roundOne(num_matches, num_rounds, threads, equipes, match);
    rounds(num_equipe, num_matches, num_rounds, threads, equipes, vainqueurs, match);
    // Destroy mutex and semaphore
    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_debutDe_tour);
    sem_destroy(&sem_finDe_tour);
    return 0;
}
