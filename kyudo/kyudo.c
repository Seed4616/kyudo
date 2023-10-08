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



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>
/*! <#include <windows.h> //Uniqument pour windows (pour Sleep()*/

#define NB_JOUEUR 3
#define NB_TIRS 4
#define NB_EQUIPE_MAX 128
#define NB_TOUR_MAX 7
#define NB_MATCH_MAX 64

/**
 * Initialization mutex et semaphore
*/
extern pthread_mutex_t mutex;
sem_t sem_debutDe_tour, sem_finDe_tour;

/**
 * @struct Joueur
 * @brief Objet Joueur
 *
 *Joueur est une structure compose d'un numero representant le joueur dans son équipe, d'un score representant le score total individuel du joueur et du nombr de tirs qu'il a effectue.
 */

typedef struct Joueur {
    int x; // numéro du joueur dans l'équipe
    int score; // score total du joueur
    int num_tirs; // nombre de coups tirés
} Joueur;

/**
 * @struct Joueur
 * @brief Objet Joueur
 *
 * Equipe est une structure compose du nom de l'équipe, c'est un tableau de chaines de caracteres de taille 500, d'un score representant le score total de l'équipe et d'un tableau de joueur.
 */

typedef struct Equipe {
    char nom_equipe[500]; // nom de l'équipe
    int score; // score total de l'équipe
    Joueur joueurs[NB_JOUEUR]; // tableau de joueurs dans l'équipe
} Equipe;

/**
 * @struct Match
 * @brief Objet Match
 *
 * Match est une structure compose du numero de tour, d'un numero de match, de deux equipes de types Equipe, d'un vainqueur de type Equipe, d'un temps de départ, ( a implemente ), d'un temps de fin ( a implemente ) , d'une durée de match
 *
 *
 */
/*! < Struct for storing match information*/

typedef struct Match {
    int num_tour;
    int num_match;
    Equipe equipe1;
    Equipe equipe2;
    Equipe vainqueur;
    time_t start_time;
    time_t end_time;
    int duree_match;
} Match;


/**
 * @struct MatchArgs
 * @brief Objet MatchArgs
 *
 * Match est une structure compose d'un tableau de match d'une duree de match d'un temps de départ, ( a implemente )
 *
 */
typedef struct {
    Match *match;
    int duree_match;
    time_t start_time;
} MatchArgs;

/**
 * Un tableau global a double dimension de Match
*/
Match matches[NB_TOUR_MAX][NB_MATCH_MAX];


 /**
 * function bool est_une_fichier(char *fichier)
 * @brief Fonction qui verifie si l'argument passer en commande ligne est un fichier ou non
 *
 * @param une chaine de caracteres
 * @return true si aucune erreur, false sinon.
 */
bool est_une_fichier(char *fichier) {
    FILE *fiche = fopen(fichier, "r");
    if (fiche != NULL) {
        fclose(fiche);
        return true;
    }
    return false;
}


/**
 * function bool est_time(char *arg)
 * @brief Fonction qui verifie si la chaine de caractere est un serie de chiffre ou non
 *
 * @param une chaine de caracteres
 * @return true si aucune erreur, false sinon.
 */

bool est_time(char *arg) {
    if (arg == NULL) {
        return false;
    }
    for (int i = 0; arg[i] != '\0'; i++) {
        if (!isdigit(arg[i])) {
            return false;
        }
    }
    int time = atoi(arg);
    if (time <= 0) {
        return false;
    }
    return true;
}

/**
 * function int get_num_equipe(char* fichier);
 * @brief Fonction qui recupere le nombre d'equipe dans fichier avec l'aide d'une chaine de caractere en parametres
 *
 * @param une chaine de caracteres
 * @return un entier
 */

int get_num_equipe(char* fichier) {
    FILE *fiche = fopen(fichier, "r");
    int num_equipe = 0;
    char ligne[500];
    while(fgets(ligne, sizeof(ligne), fiche)) {
        num_equipe++;
    }
    fclose(fiche);
    return num_equipe;
}
/**
 * function int VerfierPuissanceDeDeux(char* fichier) ;
 * @brief Fonction qui verifie si le nombre de equipe dans le fichier est une puissance de deux ou non.
 *
 * @param une chaine de caracteres
 * @return 1 si aucune erreur, 0 sinon.
 */


int VerfierPuissanceDeDeux(char* fichier) {
    int num_equipe = get_num_equipe(fichier);
    return (num_equipe != 0 && (num_equipe & (num_equipe - 1)) == 0);
}

/**
 * function int get_num_match(int num_equipe) ;
 * @brief Fonction qui recupere le numero du match en divisant le numero d'equipe en 2
 *
 * @param un entier
 * @return un entier
 */
int get_num_match(int num_equipe) {
    return num_equipe / 2;
}


/**
 * function void initEquipesEtMatches(char* fichier, int num_equipe, Equipe *equipes);
 * @brief Fonction qui initie les equipes et les matchs
 *
 * @param une chaine de caracteres,  un entier, une equipe
 * @return void
 */
void initEquipesEtMatches(char* fichier, int num_equipe, Equipe *equipes){
    FILE *fiche = fopen(fichier, "r");
    char ligne[500];
    int i = 0;
    while(fgets(ligne, sizeof(ligne), fiche)) {
        ligne[strcspn(ligne, "\r\n")] = '\0'; /*!< Supprime le caractère de fin de ligne*/
        Equipe equipe;
        strcpy(equipe.nom_equipe, ligne);
        equipe.score = 0;
        for (int j = 1; j <= NB_JOUEUR; j++) {
            Joueur joueur;
            joueur.x = j+1;
            joueur.score = 0;
            joueur.num_tirs = 0;
            equipe.joueurs[j] = joueur;
            printf("equipe : %s, joueur : %d, score : %d, num shots : %d\n", equipe.nom_equipe, j, joueur.score, joueur.num_tirs);
        }
        equipes[i] = equipe;
        i++;
    }
    fclose(fiche); /*!<  Fermeture du fichier*/
}

/**
 * function void simuler_tir(Joueur *joueur) ;
 * @brief Fonction qui permet de simuler un tir et modifie le score et le nombre de tir d'un joueur
 *
 * @param un joueur
 * @return true si aucune erreur, false sinon.
 */

void simuler_tir(Joueur *joueur) {
    joueur->num_tirs++;
    int r = rand() % 100; /*!< genère un nombre aléatoire entre 0 et 99*/
    if (r < 20) {
        joueur->score += 5; //Centre
    } else if (r < 80) {
        joueur->score += 3; //N'est pas centre
    } else {
        joueur->score += 0; //Rate
    }
}

/**
 * function void calculer_score(Equipe *equipes)
 * @brief Fonction qui permet de calculer le score de chaque equipe
 *
 * @param une equipe
 * @return void
 */
void calculer_score(Equipe *equipes) {
    for (int i = 0; i < sizeof(equipes); i++) {
        equipes[i].score = 0;
        for (int j = 0; j < NB_JOUEUR ; j++) {
            equipes[i].score += equipes[i].joueurs[j].score;
        }
    }
}

/**
 * function void *simuler_match(void *arg) ;
 * @brief Fonction qui permet de simuler un tout le déroulement d'un match
 *
 * @param une equipe
 * @return void
 */

void *simuler_match(void *arg) {
    /* Partie pour récuperer duree_match et start_time et nottament la structure Match
    Code qui ne fonctionne pas :
    MatchArgs *match_args = (MatchArgs *) arg;
    Match *match = match_args->match;
    int duree_match = match->duree_match
    time_t start_time ) match->start_time*/

    Match *match = (Match *) arg;
    int duree_match = match->duree_match;
    time_t start_time = match->start_time;
    printf("\nRound %d Match %d: equipe %s vs equipe %s\n", match->num_tour, match->num_match, match->equipe1.nom_equipe, match->equipe2.nom_equipe);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned int seed1 = tv.tv_sec * 100000000 + tv.tv_usec + match->num_match; /*!< use match number to make seed unique*/
    unsigned int seed2 = tv.tv_sec * 100000000 + tv.tv_usec + match->num_match + 1; /*!< use match number to make seed unique*/
    srand(seed1);
    srand(seed2);
    for (int i = 0; i < NB_JOUEUR; i++) {
        match->equipe1.joueurs[i].score = 0;
        match->equipe2.joueurs[i].score = 0;
    }

    //On aurait utiliser duree_match
    //start_time = time(NULL);
    //while ((difftime(time(NULL), start_time) < duree_match )){
        //printf("%d",match->duree_match);
        for (int i = 0; i < NB_TIRS; i++) {
            for (int j = 0; j < NB_JOUEUR; j++) {
                simuler_tir(&match->equipe1.joueurs[j]);
                printf("equipe %s, joueur %d, shot number %d, joueur's total score %d\n",
                       match->equipe1.nom_equipe, j+1, i+1, match->equipe1.joueurs[j].score);
            }
            for (int j = 0; j < NB_JOUEUR; j++) {
                simuler_tir(&match->equipe2.joueurs[j]);
                printf("equipe %s, joueur %d, shot number %d, joueur's total score %d\n",
                       match->equipe2.nom_equipe, j+1, i+1, match->equipe2.joueurs[j].score);
            }
        }
        //if (NB_TIRS == 4) break;
    //}

    /*!<  Calculer les scores pour deux equipes*/
    calculer_score(&match->equipe1);
    calculer_score(&match->equipe2);

    /*!<  Determiner le vainqueur */
    Equipe vainqueur;
    if (match->equipe1.score == match->equipe2.score) {
        srand(time(NULL));
        int random = rand() % 2; /*!<  generer une nombre aléatoire soit 0 soit 1*/
        if (random == 0) {
            vainqueur = match->equipe1;
        } else {
            vainqueur = match->equipe2;
        }
    }

    if (match->equipe1.score > match->equipe2.score) {
        vainqueur = match->equipe1;
    } else {
        vainqueur = match->equipe2;
    }

    pthread_mutex_lock(&mutex);
    match->vainqueur = vainqueur;
    printf("Round %d Match %d: %s - %d vs %s - %d : vainqueur: %s\n",
        match->num_tour, match->num_match,
        match->equipe1.nom_equipe, match->equipe1.score,
        match->equipe2.nom_equipe, match->equipe2.score,
        match->vainqueur.nom_equipe);

    FILE *fp;
    fp = fopen("match_results.txt", "a"); /*!< Ouvrir le fichier en mode append*/
    // Write to the file
    fprintf(fp, "Round %d Match %d: equipe %s (total score %d) vs equipe %s (total score %d), vainqueur equipe %s\n",
        match->num_tour, match->num_match,
        match->equipe1.nom_equipe, match->equipe1.score,
        match->equipe2.nom_equipe, match->equipe2.score,
        match->vainqueur.nom_equipe);

    for (int i = 0; i < NB_JOUEUR; i++) {
    fprintf(fp, "equipe %s, joueur %d, total score %d vs equipe %s joueur %d total score %d\n",
        match->equipe1.nom_equipe, i+1, match->equipe1.joueurs[i].score,
        match->equipe2.nom_equipe, i+1, match->equipe2.joueurs[i].score);
    }
    fprintf(fp, "\n");
    fclose(fp);
    pthread_mutex_unlock(&mutex);

    sem_post(&sem_finDe_tour);
    return NULL;
}

/**
 * function void roundOne(int num_matches, int num_rounds, pthread_t threads[NB_TOUR_MAX][NB_MATCH_MAX], Equipe *equipes, Match match) ;
 * @brief Fonction gere le fonctionnement du premier round en utilisant simuler_match
 *
 * @param deux entiers, un tableau a double dimension de thread, un tableau d'equipe, un match
 * @return void
 */

void roundOne(int num_matches, int num_rounds, pthread_t threads[NB_TOUR_MAX][NB_MATCH_MAX], Equipe *equipes, Match match) {
    /*!< Initialiser semaphores*/
    sem_init(&sem_debutDe_tour, 0, 0);
    sem_init(&sem_finDe_tour, 0, 0);

    for (int i = 0; i < num_matches; i++) {
        /*!<  Initialise l'information des match*/
        matches[num_rounds][i].num_tour = num_rounds;
        matches[num_rounds][i].num_match = i+1;
        matches[num_rounds][i].equipe1 = equipes[i * 2];
        matches[num_rounds][i].equipe2 = equipes[i * 2 + 1];
        /*!<  Commencer le match*/
        /*MatchArgs match_args = {&matches[num_rounds][i], match.duree_match, match.start_time};
        pthread_create(&threads[num_rounds][i], NULL, simuler_match, &match_args);*/
        pthread_create(&threads[num_rounds][i], NULL, simuler_match, &matches[num_rounds][i]);
        sleep(1); //Sleep(1000)pour windows;
    }
    /*!< Attendre que tout les match de ce tour ce fini*/
    for (int i = 0; i < num_matches; i++) {
        sem_wait(&sem_finDe_tour);
    }
}

/**
 * function void rounds(int num_equipe, int num_matches, int num_rounds, pthread_t threads[NB_TOUR_MAX][NB_MATCH_MAX], Equipe *equipes, Equipe *vainqueurs, Match match) ;
 * @brief Fonction gere le fonctionnement des autres rounds en utilisant simuler_match
 *
 * @param trois entiers, un tableau a double dimension de thread, un tableau d'equipe, un tableau de vainqueurs, un match
 * @return void
 */

void rounds(int num_equipe, int num_matches, int num_rounds, pthread_t threads[NB_TOUR_MAX][NB_MATCH_MAX], Equipe *equipes, Equipe *vainqueurs, Match match) {
    while (num_matches > 0) {
        /*!< Stocker les vainqueurs de tour*/
        for (int i = 0; i < num_matches; i++) {
            vainqueurs[i] = matches[num_rounds][i].vainqueur;
        }
        /*!< Commencer le tour avec les vainqueurs*/
        for (int i = 0; i < num_matches/2; i++) {
            // Initialiser match information
            matches[num_rounds+1][i].num_tour = num_rounds+1;
            matches[num_rounds+1][i].num_match = i+1;
            matches[num_rounds+1][i].equipe1 = vainqueurs[i*2];
            matches[num_rounds+1][i].equipe2 = vainqueurs[i*2+1];
            /*!< commencer les match */
            /*MatchArgs match_args = {&matches[num_rounds+1][i], match.duree_match, match.start_t
            pthread_create(&threads[num_rounds+1][i], NULL, simuler_match, &matches[num_rounds+1][i]);*/
            pthread_create(&threads[num_rounds+1][i], NULL, simuler_match, &matches[num_rounds+1][i]);
            sleep(1); //Sleep(1000) pour windows;
        }
        /*!< Signaler tout les match de tour pour démarrer*/
        for (int i = 0; i < num_matches/2; i++) {
            sem_post(&sem_debutDe_tour);
        }
        /*!< Attendre que tout les match démarre*/
        for (int i = 0; i < num_matches/2; i++) {
            sem_wait(&sem_debutDe_tour);
        }
        /*!<  Signaler tout les match de se terminer*/
        for (int i = 0; i < num_matches/2; i++) {
            sem_post(&sem_finDe_tour);
        }
        /*!<  Attendre que tout les match se termine*/
        for (int i = 0; i < num_matches/2; i++) {
            pthread_join(threads[num_rounds+1][i], NULL);
        }
        num_rounds++;
        if (num_rounds >= NB_TOUR_MAX) break;
        num_equipe /= 2;
        num_matches /= 2;
    }
}

/**
 * function void existe_fichier(void);
 * @brief Fonction qui verifie si le fichier match_results.txt est rempli ou non, s'il l'est il sera detruit
 *
 * @param void
 * @return void
 */

void existe_fichier(){
    bool existe_fichier = false;
    FILE *fp_check = fopen("match_results.txt", "r");
    if (fp_check != NULL) {
        fclose(fp_check);
        existe_fichier = true;
    }

    // enlever le fichier si il existe
    if (existe_fichier) {
        int status = remove("match_results.txt");
        if (status != 0) {
            printf("Error deleting file");
            exit(1);
        }
    }
}

