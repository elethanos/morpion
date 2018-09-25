#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
// TODO:
// 1) recommencer quand le joueur à gagné
// 2) compteur de points (joueur 1 a gagné 5 fois, joueur 2 a gagné 8 fois...)


// A faire
// 3) ==> Gérer le cas d'égalité (faire une fonction pour tester si le jeux est fini)
// 4) ==> Faire une fonction qui copie le morpion: int *copier_morpion(int morpion[][3])
// 5) Optionnel: continuer ia.

#define TAILLE 3
char conv(int valeur_case)
{
    if(valeur_case == 0)
	return(' ');
    else if(valeur_case == 1)
	return('O');
    else if(valeur_case == 2)
	return('X');
    else
	return('?');
}

void afficher_morpion(int morpion[][3])
{
    printf("  A   B   C \n");
    for(int i=0;i<3;i++)
    {
	printf("| %c |",conv(morpion[i][0]));
	printf(" %c |",conv(morpion[i][1]));
	printf(" %c |",conv(morpion[i][2]));
	printf(" %d \n",i+1);
    }
}
int verification(int coord[],int mon_morpion[][3])
{
    if(mon_morpion[coord[0]][coord[1]] == 0)
	return 1;    
    else
	return 0;
}

// Méthode pointeurs:
// void demander_case(int *x, int *y)
// Méthode tableau
int *demander_case(int mon_morpion[][3])
{
    int *mytab = malloc(2);   
    char input[20];
    printf("Veuillez entrer une case (ex: B2)\n");
    scanf("%2s", input);
    //Premiere methode
    if(input[0]=='A')
	mytab[1]=0;
    else if(input[0]=='B')
	mytab[1]=1;
    else if(input[0]=='C')
	mytab[1]=2;
    else
    {
	mytab[1]=-1;
    }
    // Deuxieme methode
    mytab[0]=input[1]-'1'; // Ou input[1] - 49
    if(mytab[0]>=3 || mytab[0]<0 || mytab[1]>=3 || mytab[1]<0)
    {
	printf("Veuillez entrer une autre case \n");
	free(mytab); 
	return demander_case(mon_morpion);
    }
    if(! verification(mytab, mon_morpion)) // ! = negation, ici "si ... est faux"
    {
	printf("Cette case est deja remplie !\n");
	free(mytab); 
	return demander_case(mon_morpion);
    }
    return mytab;
}

int compteur_general(int mon_morpion[][3],int case_depart_ligne,int case_depart_colonne ,int direction_ligne,int direction_colonne,int signe)
{
    int f=0;
    for(int i=0;i<3;i++)
    {
        if(mon_morpion[case_depart_ligne +i*direction_ligne][case_depart_colonne+i*direction_colonne]==signe)
	{
	    f++; 
	}	
    }
    return f;
}
    
int compteur_ligne(int mon_morpion[][3],int ligne, int signe)
{
    return compteur_general(mon_morpion,ligne,0,0,1,signe);
}
int compteur_colonne(int mon_morpion[][3],int colonne, int signe)
{
    return compteur_general(mon_morpion,0,colonne,1,0,signe);
}
int compteur_diago_asc(int mon_morpion[][3],int signe)
{
    return compteur_general(mon_morpion,2,0,-1,1,signe);
}
int compteur_diago_desc(int mon_morpion[][3],int signe)
{
    return compteur_general(mon_morpion,0,0,1,1,signe);
}
int a_gagner(int mon_morpion[][3], int signe)
{
    if(compteur_diago_desc(mon_morpion,signe)==3 || compteur_diago_asc(mon_morpion,signe)==3)
    {
	return 1;
    }
    for(int i=0;i<3;i++)
    {
	if(compteur_ligne(mon_morpion,i,signe)==3 || compteur_colonne(mon_morpion,i,signe)==3)
	{
	    return 1;
	}
    }
    return 0;    
}
//Utilise compteur_ligne pour verifier si toutes les cases sont remplies
int verifier_fin(int mon_morpion[][3])
{
    int g=0;
    for(int i=0;i<3;i++)
    {
	if(compteur_ligne(mon_morpion,i,0)==0)
	{
	    g++;
        }
    }
    return (g==3);
}
// Joueur: Entier, 1 ou 2 suivant le joueur
// res: coordonnées tapées par l'utilisateur, comprises entre 0 et 2 (inclus)
/* int remplir_case(int joueur,int *res) */
/* { */
/*     if() */
/* } */



// Intelligence artificielle, min-max

int note_minmax(int mon_morpion[][3])
{
    //Le vrai utilisateur a gagné
    if(a_gagner(mon_morpion,1))
	return -1000;
    //L'ordinateur a gagné
    else if(a_gagner(mon_morpion,2))
	return 1000;
    else
    {
	int score=0;
	if(mon_morpion[1][1]==1)
	    score -= 10;
	else if(mon_morpion[1][1]==2)
	    score += 10;
	for(int f=1;f<=2;f++)
	{
	    int autre_joueur = 3-f;
	    for(int i=0;i<3;i++)
	    {
		//printf("f=%d i=%d \n",f,i);
		if(compteur_ligne(mon_morpion,i,f)==2 && compteur_ligne(mon_morpion,i,autre_joueur)==0)
		{
		    if(f==2)
			score += 30;
		    else if(f==1)
			score -= 30;		 
		}
		if(compteur_colonne(mon_morpion,i,f)==2 && compteur_colonne(mon_morpion,i,autre_joueur)==0)
		{
		    if(f==2)
			score += 30;
		    else if(f==1)
			score -= 30;		   
		}		
	    }
	    if(compteur_diago_asc(mon_morpion,f)==2 && compteur_diago_asc(mon_morpion,autre_joueur)==0)
	    {
		if(f==2)
		    score += 30;
		else if(f==1)
		    score -= 30;
	    }
	     if(compteur_diago_desc(mon_morpion,f)==2 && compteur_diago_desc(mon_morpion,autre_joueur)==0)
	    {
		if(f==2)
		    score += 30;
		else if(f==1)
		    score -= 30;
	    }

	}
	return score;
    }
}


void copier_morpion(int mon_morpion[][3], int morpion_cible[][3])
{
    for(int j=0;j<3;j++)
    {
	for(int i=0;i<3;i++)
	{
	    morpion_cible[i][j] = mon_morpion[i][j];
	}
    }
}

// Méthode 1: à base de pointeur
int (*copier_morpion_malloc(int mon_morpion[][3]))[3]
{
	int (*morpion_cible)[3];
	morpion_cible = malloc(sizeof *morpion_cible * 3);
	copier_morpion(mon_morpion, morpion_cible);
	return morpion_cible;
	// Utilisation:
	// int (*morpion_cible)[3] = copier_morpion_malloc(mon_morpion);
	// printf("%d\n", morpion_cible[0][0]);
}

// Méthode 2: à base de struct
struct Morpion {
    int tab[3][3];
};

struct Morpion copier_morpion_struct(int mon_morpion[][3])
{
    struct Morpion morpion_cible_struct;
    copier_morpion(mon_morpion, morpion_cible_struct.tab);
    return morpion_cible_struct;
    // Utilisation:
    // int age = obtenir_age();
    // struct Morpion morpion_cible_struct = copier_morpion_struct(mon_morpion);
    // morpion_cible_struct.tab[0][0] = 42;
}

// Méthode 2 bis:
// Alias: 
typedef struct Morpion morpion;

morpion copier_morpion_typedef(int mon_morpion[][3])
{
    morpion morpion_cible_struct;
    copier_morpion(mon_morpion, morpion_cible_struct.tab);
    return morpion_cible_struct;
    // Utilisation:
    // morpion morpion_cible_struct = copier_morpion_typedef(mon_morpion);
    // morpion_cible_struct.tab[0][0] = 42;
}

int *demander_case_ia_stupide(int mon_morpion[][3])
{
    int *mytab = malloc(2);
    for(int i=0;i<3;i++)
    {
	for(int j=0;j<3;j++)
	{
	    if(mon_morpion[i][j]==0)
	    {
		mytab[0]=i;
		mytab[1]=j;
		return mytab;
	    }
	}
    }
}

struct Score_position
{
    int score;
    int pos_ligne;
    int pos_colonne;
};

typedef struct Score_position score_position;
    
score_position demander_case_aux(int mon_morpion[][3],int coups_en_avance,int joueur)
{
    // printf("num 1 [%d] \n",coups_en_avance);
    score_position resultat;
    // Renvoie un tableau de taille 3, avec le score, la position en
    // ligne/colonne
    if(coups_en_avance==0)
    {
	int score = note_minmax(mon_morpion);
	resultat.score = score;
	return resultat;
    }
    else
    {
	score_position meilleur_coup;
	if (verifier_fin(mon_morpion) || a_gagner(mon_morpion,1) || a_gagner(mon_morpion,2))
	{
	    meilleur_coup.score = note_minmax(mon_morpion);
	    meilleur_coup.pos_ligne = 42;
	    meilleur_coup.pos_colonne = 42;
	    return meilleur_coup;
	}
	
	if (joueur==1)
	    meilleur_coup.score = INT_MAX;
	else
	    meilleur_coup.score = INT_MIN;
	for(int i=0;i<3;i++)
	{
	    for(int j=0;j<3;j++)
	    {
		if(mon_morpion[i][j]==0)
		{
		    mon_morpion[i][j]=joueur;
		    score_position resultat_coup = demander_case_aux(mon_morpion,coups_en_avance -1,3-joueur);
		    if ((joueur==1 && resultat_coup.score<meilleur_coup.score) || (joueur==2 && resultat_coup.score>meilleur_coup.score))
		    {
			meilleur_coup.score = resultat_coup.score;
			meilleur_coup.pos_ligne = i;
			meilleur_coup.pos_colonne = j;
		    }
		    mon_morpion[i][j]=0;		    
		}
	    }	    
	}
	return meilleur_coup;
    }
}

int *demander_case_ia(int mon_morpion[][3])
{
    int *mytab = malloc(2);
    score_position coup_a_jouer = demander_case_aux(mon_morpion,3,2);
    mytab[0] = coup_a_jouer.pos_ligne;
    mytab[1] = coup_a_jouer.pos_colonne;
    return mytab;
}

void pour_rejouer();



int main()
{
    int score_1 = 0;
    int score_2 = 0;
    printf("debut 1er while\n");
    int mon_morpion[3][3]={{0}}; 
    int joueur_suivant = 1;
    while(1)
    {
	int *res;

	afficher_morpion(mon_morpion);
	//printf("Votre score est de %d \n",note_minmax(mon_morpion));
	printf("Joueur %d a toi\n",joueur_suivant);
	
	if(joueur_suivant==1)
	{
	    res = demander_case(mon_morpion);
	}
	else
	{
	    res = demander_case_ia(mon_morpion);
	    //res = demander_case_ia(mon_morpion);
	}
	
	printf("L'utilisateur a tapé la case %d x %d.\n", res[0], res[1]);
	mon_morpion[res[0]][res[1]] = joueur_suivant;
	
	if(a_gagner(mon_morpion,joueur_suivant))
	{
	    printf("Bravo! Le joueur %d a gagne\n",joueur_suivant);
	    if(joueur_suivant==1)
	    {
		printf("OK 1\n");
//		    score_1++;
//		    printf("Nb de victoire(s) du joueur 1:%d\n",score_1);
	    }
	    else if(joueur_suivant==2)
	    {
		printf("OK 2\n");
//		    score_2++;
//		    printf("Nb de victoire(s) du joueur 2:%d\n",score_2);
	    }
	    pour_rejouer();
	    return 1;
	}
	else if(verifier_fin(mon_morpion))
	{
	    printf("Le jeu est finit !\n");
	    pour_rejouer();
	    return 1;
	}
	// Passer au joueur suivant
	if(joueur_suivant==1)
	{
	    joueur_suivant = 2;
	}
	else if(joueur_suivant==2)
	{
	    joueur_suivant = 1;
	}
	else
	{
	    printf("Le joueur n'existe pas");
	    exit(1);
	}	
	//Libérer la mémoire utilisée par le malloc
	free(res);
    }
    return 0;
}

        
void pour_rejouer()
{
    char rejouer[20];
    printf("Voulez-vous rejouer [oui/non]?");
    scanf("%3s", rejouer);
    printf("je suis sorti\n");
    if(strcmp(rejouer,"non") == 0)
    //if(rejouer[0]=='n' && rejouer[1]=='o' && rejouer[2]=='n')
    {
	printf("Merci d'avoir joué\n");
    }
    else if(rejouer[0]=='o' && rejouer[1]=='u' && rejouer[2]=='i')
    {
	printf("Bonne chance !\n");
	main();
    }
    else
    {
	printf("Je n'ai pas compris\n");
	pour_rejouer();
    }    
}
