/**
 * @file kyudo.h
 * @brief Programme de tests.
 *
 * Programme du main pour lancer une simulation de match.
 *
 * @author SANTOKI Nihar
 * @author ZHANG Victor
 * @version 0.1
 * @date 02 avril 2023
 */


#ifndef KYUDO_H
#define KYUDO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>
/*!< #include <windows.h> //Uniqument pour windows (pour Sleep() */

#define NB_JOUEUR 3
#define NB_TIRS 4
#define NB_EQUIPE_MAX 128
#define NB_TOUR_MAX 7
#define NB_MATCH_MAX 64

/* !< Mutex and semaphore initialization*/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
/*!< sem_t sem_debutDe_tour, sem_finDe_tour;*/


/**
 * @struct Joueur
 * @brief Objet Joueur
 *
 *Joueur est une structure compose d'un numero representant le joueur dans son équipe, d'un score representant le score total individuel du joueur et du nombr de tirs qu'il a effectue.
 */
 
typedef struct Joueur {
    int x; /*!< entier*/
    int score; /*!< entier*/
    int num_tirs; /*!< entier*/
} Joueur;

/**
 * @struct Joueur
 * @brief Objet Joueur
 *
 * Equipe est une structure compose du nom de l'équipe, c'est un tableau de chaines de caracteres de taille 500, d'un score representant le score total de l'équipe et d'un tableau de joueur.
 */

typedef struct Equipe {
    char nom_equipe[500]; /*!< tableau statique de chaine de caractères*/
    int score; /*!< entier*/
    Joueur joueurs[NB_JOUEUR]; /*!< tableau statique de joueur*/
} Equipe;

/**
 * @struct Match
 * @brief Objet Match
 *
 * Match est une structure compose du numero de tour, d'un numero de match, de deux equipes de types Equipe, d'un vainqueur de type Equipe, d'un temps de départ, ( a implemente ), d'un temps de fin ( a implemente ) , d'une durée de match
 *
 *
 */
 
typedef struct Match {
    int num_tour; /*!< entier*/
    int num_match; /*!< entier*/
    Equipe equipe1; /*!< une equipe*/
    Equipe equipe2; /*!< une equipe*/
    Equipe vainqueur; /*!< une equipe*/
    time_t start_time; /*!< un temps*/
    time_t end_time; /*!< un temps*/
    int duree_match; /*!< entier*/
} Match;


/**
 * @struct MatchArgs
 * @brief Objet MatchArgs
 *
 * Match est une structure compose d'un tableau de match d'une duree de match d'un temps de départ, ( a implemente )
 *
 */

typedef struct {
    Match *match; /*!< un tableau dynamique de match*/
    int duree_match; /*!< un entier*/
    time_t start_time; /*!< un temps*/
} MatchArgs;

Match matches[NB_TOUR_MAX][NB_MATCH_MAX]; /*!< un tableau global a double dimension de Match*/

/**
 * function bool est_une_fichier(char *fichier)
 * @brief Fonction qui verifie si la chaine de caractere dans le repertoire courant est un fichier
 *
 * @param une chaine de caracteres
 * @return true si aucune erreur, false sinon.
 */
 
bool est_une_fichier(char *fichier);



/**
 * function bool est_time(char *arg)
 * @brief Fonction qui verifie si la chaine de caractere est un serie de chiffre ou non
 *
 * @param une chaine de caracteres
 * @return true si aucune erreur, false sinon.
 */
 
bool est_time(char *arg);




/**
 * function int get_num_equipe(char* fichier);
 * @brief Fonction qui recupere le nombre d'equipe dans fichier avec l'aide d'une chaine de caractere en parametres
 *
 * @param une chaine de caracteres
 * @return un entier
 */
int get_num_equipe(char* fichier);


/**
 * function int VerfierPuissanceDeDeux(char* fichier) ;
 * @brief Fonction qui verifie si le nombre de equipe dans le fichier est une puissance de deux ou non.
 *
 * @param une chaine de caracteres
 * @return 1 si aucune erreur, 0 sinon.
 */

int VerfierPuissanceDeDeux(char* fichier) ;


/**
 * function int get_num_match(int num_equipe) ;
 * @brief Fonction qui recupere le numero du match en divisant le numero d'equipe en 2
 *
 * @param un entier
 * @return un entier
 */

int get_num_match(int num_equipe) ;


/**
 * function void initEquipesEtMatches(char* fichier, int num_equipe, Equipe *equipes);
 * @brief Fonction qui initie les equipes et les matchs
 *
 * @param une chaine de caracteres,  un entier, une equipe
 * @return void
 */

void initEquipesEtMatches(char* fichier, int num_equipe, Equipe *equipes);


/**
 * function void simuler_tir(Joueur *joueur) ;
 * @brief Fonction qui permet de simuler un tir et modifie le score et le nombre de tir d'un joueur
 *
 * @param un joueur
 * @return true si aucune erreur, false sinon.
 */

void simuler_tir(Joueur *joueur) ;


/**
 * function void calculer_score(Equipe *equipes)
 * @brief Fonction qui permet de calculer le score de chaque equipe
 *
 * @param une equipe
 * @return void
 */

void calculer_score(Equipe *equipes) ;


/**
 * function void *simuler_match(void *arg) ;
 * @brief Fonction qui permet de simuler un tout le déroulement d'un match
 *
 * @param une equipe
 * @return void
 */

void *simuler_match(void *arg) ;


/**
 * function void roundOne(int num_matches, int num_rounds, pthread_t threads[NB_TOUR_MAX][NB_MATCH_MAX], Equipe *equipes, Match match) ;
 * @brief Fonction gere le fonctionnement du premier round en utilisant simuler_match
 *
 * @param deux entiers, un tableau a double dimension de thread, un tableau d'equipe, un match
 * @return void
 */

void roundOne(int num_matches, int num_rounds, pthread_t threads[NB_TOUR_MAX][NB_MATCH_MAX], Equipe *equipes, Match match) ;


/**
 * function void rounds(int num_equipe, int num_matches, int num_rounds, pthread_t threads[NB_TOUR_MAX][NB_MATCH_MAX], Equipe *equipes, Equipe *vainqueurs, Match match) ;
 * @brief Fonction gere le fonctionnement des autres rounds en utilisant simuler_match
 *
 * @param trois entiers, un tableau a double dimension de thread, un tableau d'equipe, un tableau de vainqueurs, un match
 * @return void
 */

void rounds(int num_equipe, int num_matches, int num_rounds, pthread_t threads[NB_TOUR_MAX][NB_MATCH_MAX], Equipe *equipes, Equipe *vainqueurs, Match match) ;


/**
 * function void existe_fichier(void);
 * @brief Fonction qui si le fichier est rempli ou non, s'il l'est il sera detruit
 *
 * @param void
 * @return void
 */

void existe_fichier(void);










#endif /* KYUDO_H */
