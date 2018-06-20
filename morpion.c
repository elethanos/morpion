#include <stdlib.h>
#include <stdio.h>
// TODO:
// 1) recommencer quand le joueur à gagné
// 2) compteur de points (joueur 1 a gagné 5 fois, joueur 2 a gagné 8 fois...)

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
// Joueur: Entier, 1 ou 2 suivant le joueur
// res: coordonnées tapées par l'utilisateur, comprises entre 0 et 2 (inclus)
/* int remplir_case(int joueur,int *res) */
/* { */
/*     if() */
/* } */

int main(int argc, char *argv[])
{  
    int mon_morpion[3][3]={{0}}; 
    int joueur_suivant = 1;
    while(1)
    {
	afficher_morpion(mon_morpion);
	printf("Joueur %d a toi\n",joueur_suivant);
	int *res = demander_case(mon_morpion);
	
        printf("L'utilisateur a tapé la case %d x %d.\n", res[0], res[1]);
	mon_morpion[res[0]][res[1]] = joueur_suivant;
	
	if(a_gagner(mon_morpion,joueur_suivant))
	{
	    printf("Bravo! Le joueur %d a gagne\n",joueur_suivant);
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
    
    //
    return 0;
}
