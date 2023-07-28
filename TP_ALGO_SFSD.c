#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <Time.h>
#include <locale.h>
#include <math.h>
#include "conio2.h"
#include "conio.c"

void Color(int couleurDuTexte,int couleurDeFond) // fonction d'affichage en couleurs
{
    /*  LES NOMBRES QUI REPRESENTENT CHAQUE COULEUR:
        0 : Noir
        1 : Bleu foncé
        2 : Vert foncé
        3 : Turquoise
        4 : Rouge foncé
        5 : Violet
        6 : Vert caca d'oie
        7 : Gris clair
        8 : Gris foncé
        9 : Bleu fluo
        10 : Vert fluo
        11 : Turquoise
        12 : Rouge fluo
        13 : Violet 2
        14 : Jaune
        15 : Blanc
    */
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}


void conv_long_long_int_to_char(long long int s,char c[])  // convert long long int to char ( CCP ) pour LOVC
{
    int j=11,k ;
    char g ;
	while (s!=0 || j>=0)
	{
		k=s%10 ;
		g=k+'0';
		c[j]=g;
		j--;
		s=s/10;
	}
}


typedef struct enregistrement  // LES ENREGISTREMENT DE BLOC
{
	long long int n_ccp ;
	float solde ;
}enr;


typedef struct entete  // L'enregistrement d'entete qui se trauve au debut de fichier et contient deux champs :
{
    int nb_enr ; // champ1 :  le nombre des enregistrement
    int nb_bloc ; // champ2 :  le nombre des blocs
}entete;


typedef struct Tbloc  // les blocs de taille 1000 enregistrement
{
	enr t[1000];  // chaque bloc centien 1000 enregistement
	int nb ;
}Tbloc;



void LireDir (FILE *a,int i,Tbloc *buf) // lire directe pour TOF
{
	entete e ;
	if (a!=NULL)
	{
	   rewind(a);
	   fseek(a,sizeof(e)+(i-1)*sizeof(*buf), SEEK_SET);
	   fread(&*buf,sizeof(*buf),1,a);
   }
}


void EcrireDir(FILE **a,int i ,Tbloc buf) // ecrire direct pour TOF
{
	entete e ;
	if (a!=NULL)
	{
	   rewind(*a);
	   fseek(*a,sizeof(e)+(i-1)*sizeof(buf), SEEK_SET);
	   fwrite(&buf,sizeof(buf),1,*a);
    }
}


int  Entete(FILE *a,int i )  // entete de fichier CCP_SOLDE_1 qui se trouve au debut du fichier
{
	entete e ;
	if (a!=NULL)
	{
	   rewind(a);
	   fread(&e,sizeof(e),1,a);
	   if (i==1) return e.nb_bloc ;  // nombre du bloces
	   if (i==2) return e.nb_enr ;  // nombre d'enregistrement
   }
   else return 0;
}


void Aff_entete(FILE **a,int i,int val) // aff-entete de fichier TOF
{
	entete e ;
	if(*a!=NULL)
	{
	   rewind(*a);
	   fread(&e,sizeof(e),1,*a);
	   if (i==1) e.nb_bloc=val ;
	   else e.nb_enr=val ;
	   rewind(*a);
	   fwrite(&e,sizeof(e),1,*a);
   }
}



void Chargement_Fichier_F1(FILE **a, int n)  // module de charegement initiale de TOF
{
	int k=1,i=1,j=0;
	long long int h=111111 ;
	float r,q ,x,y ;
	Tbloc buf;
	Aff_entete(&*a,1,0);
	Aff_entete(&*a,2,0);
if (*a!=NULL && n!=0)
 {
	while(k<=n)
	{
		r=rand()%9;
		q=rand()%9;
		x=r/10.0;   // 0.0 -----> 0.9
		y=q/100.0;  // 0.00  -----> 0.09
		buf.t[j].solde=rand()%10000999-1000+x+y; // -1000 ----> 9999999   ( 10000999 = 9999999 + 1000 )
		h=h+rand()%((999999888888/n)-1)+1;  // pour que le fichier doit etre tries selon les numeros de CCP on ajoute a chaque fois un entier entre (1) et ( (999999999999 - 000000111111) div n) pour n'est pas depasser la valeur 999999999999
		buf.t[j].n_ccp=h;  //  999999888888 = 999999999999 - 000000111111
		j++;
		if (j>=700)  //  u=70 % ====>>  0.7*1000 = 700 
		{
		    buf.nb=j;
			EcrireDir(&*a,i,buf);
			j=0;
			i++;
		}
		k++;
	}
	if (j!=0)
	{
		buf.nb=j;
		EcrireDir(&*a,i,buf);
	}
    Aff_entete(&*a,1,i);
    Aff_entete(&*a,2,n);
}
else
{
    if (*a==NULL)  // si li fichier n'existe pas
    {
        Color(4,0);
        printf("\n OOPS ! FICHIER N'EXISTE PAS !! .. \n");
        Color(15,0);
    }

    else  // si le nombre d'enregistement est nil alors le fichier est vide
    {
        Color(4,0);
        printf("\n OOPS ! FICHIER VIDE !! .. \n");
        Color(15,0);
    }
}
}



int longeur_int(long long int n) // calculer la longeur d'un entier
{
    if (n!=0)  return floor(log10(abs(n))) + 1;
    else return 0 ;
}



void Chargement_Fichier_F2(FILE *f,FILE **g,int *m ) // charegement initaile de VIREMENT.TXT
{
	int k=0,i=1,j=0,n,j1,w ;
	Tbloc buf;
	float h,r,q,x,y;
	if (f!=NULL && *g!=NULL && Entete(f,2)!=0)
	{
		rewind(*g);
		*m=rand()%(Entete(f,2)); // le nombre du comptes CCP dons VIREMENT.TXTX
		if (*m==0) *m=1;
		n=Entete(f,2)/(*m);  // le nombre qui on ajout a chaque fois pour l'indice d'enregisterement que on veut lire
		while (k<*m)
		{
			LireDir(f,i,&buf);
			for ( w=1 ; w<=12-longeur_int(buf.t[j].n_ccp) ; w++) fprintf(*g,"0");
			fprintf(*g,"%lld ---> ",buf.t[j].n_ccp);
			r=rand()%9;
		    q=rand()%9;
		    x=r/10.0;
		    y=q/100.0;
			h=rand()%10000999-1000+x+y;
			if (k==*m-1)  fprintf(*g,"%.2f",h);
			else fprintf(*g,"%.2f\n",h);
			j1=rand()%n;
			if (j1==0) j1=1;
			j+=j1 ;
			if (j>=700)
			{
				i++;
				j-=700;
			}
			k++;
		}
	}
    else
    {
    if (Entete(f,2)==0)
    {
        Color(4,0);
        printf("\n OOPS ! Le fichier des soldes est vide !! .. \n");
        Color(15,0);
    }
    if (f==NULL)
    {
        Color(4,0);
        printf("\n OOPS ! Le fichier des soldes n'existe pas !! .. \n");
        Color(15,0);
    }
	if (*g==NULL)
    {
        Color(4,0);
        printf("\n OOPS ! Le fichier des virements n'existe pas !! .. \n");
        Color(15,0);
    }
    }
}



void Recherche_dic_TOF(FILE *a,long long int ccp , int *i, int *j)  // rechecher dichotomique pour TOF
{
if (a!=NULL && Entete(a,2)!=0)
{
	int binf=1 , bsup=Entete(a,1),trauve=0,inf,sup ;
	Tbloc buf ;
	while(trauve==0 && binf<=bsup)
	{
		*i=(binf+bsup)/2;
		LireDir(a,*i,&buf);
		if(( buf.t[0].n_ccp <= ccp ) && ( buf.t[buf.nb-1].n_ccp >= ccp ))
		{
		  inf=0;
		  sup=buf.nb-1;
		  while (trauve==0)
		  {
		  	*j=(inf+sup)/2;
		  	if (buf.t[*j].n_ccp==ccp) trauve=1;
		  	else
		  	{
		  	  if (buf.t[*j].n_ccp<ccp) inf=*j+1;
		  	  else sup=*j-1 ;
			}
		  }
		}
		else
		{
			if (buf.t[0].n_ccp>ccp ) bsup=*i-1;
			else binf=*i+1 ;
		}
	}
}
}


void Mises_a_jour_plus_Affichage(FILE *a,FILE **b)  // le miss a jour du CCP_SOLDE.bin et affichage du nouvelle soldes
{
if (a!=NULL && *b!=NULL && Entete(*b,1)!=0 && ftell(a)!=0)
{
	rewind(a);
	long long int n ;
	int i,j,w;
	float s ;
	Tbloc buf ;
	Color(0,11);
	printf("\n CODE CCP\t | SOLDE\t | MONTANT\t | NOUVEAU SOLDE \n");
	Color(15,0);
    char zero[6] ;
	char c[12] ;
	while (!feof(a))
	{
        fscanf(a,"%lld",&n);
        Recherche_dic_TOF(*b,n ,&i, &j);
        LireDir(*b,i,&buf);
        conv_long_long_int_to_char(buf.t[j].n_ccp,c);
        c[12]='\0';
        float cle_solde=buf.t[j].solde ;
        fseek(a, 6, SEEK_CUR);
        fscanf(a,"%f",&s);
        buf.t[j].solde+=s;
        EcrireDir(&*b,i,buf);
        printf("\n %s\t",c) ;
        printf(" | %.2f\t | ",cle_solde) ;
        printf(" %.2f\t | ",s) ;
        if(buf.t[j].solde>=0)
        {
            Color(10,0);
            printf(" %.2f\t \n",buf.t[j].solde) ;
            Color(15,0);
        }
        else
		{
            Color(12,0);
            printf(" %.2f\t \n",buf.t[j].solde) ;
            Color(15,0);
        }
   }
}
else
{
    if (Entete(*b,1)==0)
	{
        Color(4,0);
        printf("\n OOPS ! Le fichier des soldes est vide !! .. \n");
        Color(15,0);
    }
    if (*b==NULL)
	{
        Color(4,0);
        printf("\n OOPS ! Le fichier de soldes n'existe pas !!.. \n");
        Color(15,0);
    }
	if (a==NULL)
	{
        Color(4,0);
        printf("\n OOPS ! Le fichier de virements n'existe pas !!.. \n");
        Color(15,0);
    }
	if (ftell(a)==0)
	{
        Color(4,0);
        printf("\n OOPS ! Le fichier de virements est vide !!.. \n");
        Color(15,0);
	}
}
}



void Ouvrire(FILE **f , char nom[] , char a)
{
	int i=0;
	while (nom[i]!='.')
	{
		i++;
	}
	if (nom[i+1]=='b')  // fichier.bin
	{
	  if (a=='A') *f=fopen(nom,"rb+");
	  if (a=='N') *f=fopen(nom,"wb+");
    }
   if (nom[i+1]=='t')  // fichier.txt
    {
	  if (a=='A') *f=fopen(nom,"r+");
	  if (a=='N') *f=fopen(nom,"w+");
    }
}


void Fermer(FILE **a)
{
	fclose(*a);
}


void printfslow(char ch[],int temps) //ecrit une chaine de caractere avec un temp d'arret entre chaque caractere
{
    int i;
    for (i=0 ; i < strlen(ch); i++)
    {
        printf("%c",ch[i]);
        delay(temps);
    }
}


void menu()
{
 Color(11,0);
+
    printf("========================================================================================================================\n");
    Color(15,0);
    Color(11,0);
    printf("                          ===================================================================\n");
    Color(15,0);
    Color(0,7);
    printf("                                                  MENU PRINCIPAL                                                      \n");
    Color(15,0);
    Color(11,0);
    printf("                          ===================================================================\n");
    Color(15,0);
    Color(11,0);
    printf("========================================================================================================================\n");
    Color(15,0);
    printf("\n") ;
    Color(9,0);
    printf("                                       [ 01 ]-CRÉATION DU FICHIER BINAIRE CCP_BIN_1. \n");
    printf("\n");
    Color(15,0);
    Color(9,0);
    printf("                                       [ 02 ]-CREATION DU FICHIER TEXTE VIREMENTS. \n");
    printf("\n");
    Color(15,0);
    Color(9,0);
    printf("                                       [ 03 ]-MISE A JOUR DU FICHIER BINAIRE CCP_BIN_1. \n");
    printf("\n");
    Color(15,0);
    Color(9,0);
    printf("                                       [ 04 ]-CREATION DU FICHIER BINAIRE CCP_BIN_2 [ LOVC ]. \n");
    printf("\n");
    Color(15,0);
    Color(9,0);
    printf("                                       [ 05 ]-MISE A JOUR DU FICHIER BINAIRE CCP_BIN_2. \n");
    printf("\n");
    Color(15,0);
    Color(9,0);
    printf("                                       [ 06 ]-INSERTION D UN NOUVEL ENREGISTREMENT. \n");
    printf("\n");
    Color(15,0);
    Color(9,0);
    printf("                                       [ 07 ]-SUPPRESSION D UN COMPTE CCP. \n");
    printf("\n");
    Color(15,0);
    Color(9,0);
    printf("                                       [ 08 ]-CONSULTATION DES COPMTES AYANT UN SOLDE NEGATIF OU SUPERIEUR A 10000. \n");
    printf("\n");
    Color(15,0);
    Color(9,0);
    printf("                                       [ 09 ]-PRESENTATION GRAPHIQUE. \n");
    printf("\n");
    Color(15,0);
    Color(9,0);
    printf("                                       [ 10 ]-QUITTER LE PROGRAMME. \n");
    printf("\n");
    Color(15,0);
    Color(7,7);
    printf("------------------------------------------------------------------------------------------------------------------------\n",15);
    printf("\n");
    Color(15,0);
    printf("\n") ;
}



int timeoutgetnb(int* remaining,const char* chaine)
{
    int timeleft;
    char cache[11] = {0};
    int curs = 0;
    time_t starttime = time(NULL);
    printf("\r %s [%d]  %s          ",chaine,*remaining,cache);
    printf("Veuillez patienter...");
    do
    {
        time_t crt = time(NULL);
        if (difftime(crt,starttime)>=1.0)
        {
            starttime = crt;
            (*remaining)--;
            printf("\r %s [%d]  %s          ",chaine,*remaining,cache);
            printf("Veuillez patienter...");
        }
        if (_kbhit())
        {
            char c = (char)getch();
            if (c>='0' && c<='9' && curs<10)  cache[curs++] = c;
            if (c==0x0D)  return atoi(cache);
            if (c=='\b' && curs!=0)  cache[--curs] = '\0';
            printf("\r %s [%d]  %s          ",chaine,*remaining,cache);
        }
    }
    while ((*remaining)>0);
    printf("\n");
    return -1;
}



typedef struct entete2  // L'enregistrement d'entete qui se trauve au debut de fichier CCP_SOLDE_2.bin et contient 4 champs :
{
   	int tete ; // champ1 :  l'indix de la tete (premier bloc du liste )
   	int queu ; // champ2 :  l'indix de la dernier bloc du liste
	int pivq ; // champ3 :  l'indix de la premiere case vide dans la queu
    int nb_sup ; // champ4 :  nompbre de supresion
}entete2 ;



typedef struct Tbloc2  // le bloc de  CCP_SOLDE_2.bin
{
	char t[1030];
	int suiv ;// addrese de bloc suivant
}Tbloc2;


void LireDir2 (FILE *a,int i,Tbloc2 *buf)  // lire direct pour le fichier LOVC
{
	entete2 e ;
	if (a!=NULL)
	{
	   rewind(a);
	   fseek(a,sizeof(e)+(i-1)*sizeof(*buf), SEEK_SET);
	   fread(&*buf,sizeof(*buf),1,a);
   }
}


void EcrireDir2(FILE **a,int i ,Tbloc2 buf)   // ecrrie direct pour le fichier LOVC
{
	entete2 e ;
	if (a!=NULL)
	{
	   rewind(*a);
	   fseek(*a,sizeof(e)+(i-1)*sizeof(buf), SEEK_SET);
	   fwrite(&buf,sizeof(buf),1,*a);
    }
}



int  Entete2(FILE *a,int i )  // l'entete du fichier LOVC qui on trouv ou debut de fichier
{
	entete2 e ;
	if (a!=NULL)
	{
	   rewind(a);
	   fread(&e,sizeof(e),1,a);
	   if (i==1) return e.tete ;  // la tete du liste des blocs
	   if (i==2) return e.queu ;  // la te queu du liste des blocs
	   if (i==3) return e.pivq ;  // le premier indice vide dons la queu
	   if (i==4) return e.nb_sup ; // le nombre d'element suppreme
   }
   else return 0;
}


void Aff_entete2(FILE **a,int i,int val)
{
	entete2 e ;
	if(*a!=NULL)
	{
	   rewind(*a);
	   fread(&e,sizeof(e),1,*a);
	   if (i==1) e.tete=val ;
	   if (i==2) e.queu=val ;
	   if (i==3) e.pivq=val ;
	   if (i==4) e.nb_sup=val ;
	   rewind(*a);
	   fwrite(&e,sizeof(e),1,*a);
   }
}



int Longeur_float(float s)  // la longeur d'un reel 
{
   s=s*100;  // pour le 2 chiffre avant la virgule 
   long long int n ;
   n=(long long int)s;
   if (s>0) return longeur_int(n)+1;  // +1  pour la virgule
   if (s<0) return longeur_int(n)+2;  /// +1 pour la virgule et +1 pour le (-)
   if (s==0) return 4;
}




void conv_int_to_char(int s,char c[])  // convert int to char ( pour la taille )
{
    int j=1,k ;
    char g ;
	while (s!=0 || j>=0)
	{
		k=s%10 ;
		g=k+'0';
		c[j]=g;
		j--;
		s=s/10;
	}
}


int Alloc_bloc(FILE *a)  // pour allouer des nauvoux bloces differents du bloces existantes
{
Tbloc2 buf;
if (a!=NULL)
{
	int v=0;
	int l=rand()%499+1,i;
	while (v!=1)
	{
	   i=Entete2(a,1);
	   while (i!=-1 && l!=i)
	   {
	 	  LireDir2(a,i,&buf);
		  i=buf.suiv;
	   }
	   if (i==-1)
	   {
	      v=1;
	      return l;
	   }
	   else l=rand()%499+1;
   }
}
}



void Aff_Adr(FILE **a,int i2,int i3)  // pour qui le bloc n°:i2 pointe sur le bloc n°:i3 dons la liste
{
	if (*a!=NULL)
	{
	    Tbloc2 buf;
	    LireDir2(*a,i2,&buf);
	    buf.suiv=i3;
	    EcrireDir2(&*a,i2,buf);
    }
}



void Chargement_CCP_SOLDE_2( FILE **a , FILE *b, int tab[] , int *x ) // module de charegement initiale de CCP_SOLDE_2 a partir de CCP_SOLDE_1
{
	int i1=1,i2=-1,j1=0,j2=0,e,j3,i3;
	long long int h ;
	float f ;
	char sc[15],ss[15],sl[15],s[102];
	Tbloc buf1;
	Tbloc2 buf2;
	*x=0;
	Aff_entete2(&*a,1,-1);
	Aff_entete2(&*a,2,-1);
	Aff_entete2(&*a,3,-1);
	Aff_entete2(&*a,4,0);
if (*a!=NULL && b!=NULL && Entete(b,2)!=0)
{
	while (i1<=Entete(b,1))  // parcour sequenciel du fichier CCP_SOLDE.bin
	{
		LireDir(b,i1,&buf1);
		for (j1=0 ; j1<buf1.nb ; j1++ )
		{
			h=buf1.t[j1].n_ccp;
			f=buf1.t[j1].solde;
			e=Longeur_float(f);
			conv_int_to_char(e+12,sl);
			conv_long_long_int_to_char(h,sc);
			sprintf(ss,"%.2f", f);
			s[0]=sl[0];
			s[1]=sl[1];
			s[2]=0+'0';
			for(j3=3 ; j3<=14 ; j3++) s[j3]=sc[j3-3];
			for (j3=15 ; j3<=14+e ; j3++) s[j3]=ss[j3-15];
			for(j3=0; j3<=14+e ; j3++)
			{
				buf2.t[j2]=s[j3];
				j2++;
				if (j2>1023)
				{
				   j2=0;
				   i2=Alloc_bloc(*a);
				   tab[*x]=i2;
				   *x+=1;
				   if (Entete2(*a,1)==-1)
				   {
					    Aff_entete2(&*a,1,i2);
					    buf2.t[1024]='\0';
				        EcrireDir2(&*a,i2,buf2);
				        Aff_Adr(&*a,i2,-1);
				   }
				   else
				   {
				        Aff_Adr(&*a,i3,i2);
				        buf2.t[1024]='\0';
				        EcrireDir2(&*a,i2,buf2);
				        Aff_Adr(&*a,i2,-1);
			       }
				   i3=i2; // pour le chainage
			    }
		    }
	    }
	i1++;
   }
   Tbloc2 dernier_bloc;
   int j ;
   if (j2!=0)
   {
	  if(Entete2(*a,1)==-1)
	  {
	    i2=Alloc_bloc(*a);
	    tab[*x]=i2;
	    *x+=1;
	    Aff_entete2(&*a,1,i2);
	    buf2.t[j2]='\0';
		EcrireDir2(&*a,i2,buf2);
		Aff_Adr(&*a,i2,-1);
	  }
	  else
	  {
	  	i2=Alloc_bloc(*a);
	  	tab[*x]=i2;
	    *x+=1;
		Aff_Adr(&*a,i3,i2);
		for(j=0 ; j<j2 ; j++)   dernier_bloc.t[j]=buf2.t[j];
		dernier_bloc.t[j2]='\0';
		EcrireDir2(&*a,i2,dernier_bloc);
		Aff_Adr(&*a,i2,-1);
	  }
   }
   Aff_entete2(&*a,2,i2);
   Aff_entete2(&*a,3,j2);
   Aff_entete2(&*a,4,0);
   *x-=1;
}
else
{
	if (*a==NULL)
	{
		Color(4,0);
        printf("\n OOPS ! Le fichier CCP_SOLDE_2 n'existe pas !! .. \n");
        Color(15,0);
	}
	if (b==NULL)
	{
		Color(4,0);
        printf("\n OOPS ! Le fichier CCP_SOLDE_1 n'existe pas !! .. \n");
        Color(15,0);
	}
	if (Entete(b,2)==0)
	{
		Color(4,0);
        printf("\n OOPS ! Le fichier des CCP_SOLDE_1 est vide !! .. \n");
        Color(15,0);
	}
}
}



void convert_char_to_float(char c[],float *f)
{
	*f=(float) atof(c);
}




void conv_char_to_long_long_int (char c[],long long int *s)
{
	sscanf(c, "%lld", &*s);
}




void conv_char_int (char c[],int *s)
{
	sscanf(c, "%d", &*s);
}




void reglage(FILE *a,int *i,int *j , int tab[] )  // por miss a jour li indices : si  j>1023 ou j<0
{
Tbloc2 buf;
if ( a!=NULL )
{
	int b=0;
	if (  ( *j>1023 ) &&  (*j!=Entete2(a,3))  )  // si j>1023 on doit aller a le bloc suivant
	{
		*j=*j-1024;
		LireDir2(a,*i,&buf);
		*i=buf.suiv;
	}
	if (*j<0)  // si j<0 on doit aller a le bloc precedente
	{
		*j=*j+1024;
		while (tab[b]!=*i)
		{
	       b+=1;
		}
		*i=tab[b-1];
	}
}
}



//------ procedure qui permet de récuperer une chaine de longueur n dans le bloc i  lu dans buffer a partir de la position j-------------//
void recuperer_chaine(FILE *fichier,int n , int *i, int *j, char chaine[]  ) //
{
  if (fichier!=NULL)
  {
	Tbloc2 buf ;
    LireDir2(fichier,*i,&buf) ;
    int k=0;
    sprintf(chaine,"%s","");
    for(k=0;k<n;k++)                   // boucle de n itérations correspondant a la longueur de la chaine
    {
        if(*j<=1023)                   // si le kièmem caractère de la chaine correspondant au j ime car du buffer  est toujour dans le bloc i
        {
			chaine[k]=buf.t[*j];       // recuperation du caractère dans la position k de la chaine
            *j+=1;                     // deplacement d'une position dans le buffer
        }
        else                           // si le k ièeme caractère a recuperer sort du buffer
        {
            *i=buf.suiv;               // on passe au suivant du bloc i dans la liste
            LireDir2(fichier,*i,&buf);
            chaine[k]=buf.t[0] ;       // recuperation du premier caractère du tableau de carractère correspondant au ka ième car de la chaine
            *j=1;                      //positionnement eu second caractère dans le nouveau buffer
        }
    }
   chaine[k]='\0';     // fin de la chaine obtenue
  }
}




//********************************************module de recherche LOVC**************************************************************//
void recherche_LOVC(FILE *a,long long int cle,int *trouv,int *i, int *j , int tab[] )
{
  if (a!=NULL)
  {
	int t,stop=0;
	*i=Entete2(a,1);
	*j=0;
	*trouv=0;
	float f ;
	long long int n;
	char taille[4],ccp[20],solde[30],sup[4];
	while (  (( *i!=Entete2(a,2) )  || ( *j<Entete2(a,3) )) && (*trouv!=1) && (stop!=1)  )  // les conditions d'arret : si on trouv le cle ou on arrive a la fin de liste des bloc au en passe le cle (car la liste est ondonnee)
	{
		recuperer_chaine(a,2,&*i,&*j,taille) ;  // la taille d'enregistrement
		reglage(a,&*i,&*j,tab);
		recuperer_chaine(a,1,&*i,&*j,sup) ;  // le caracter d'effacement
		reglage(a,&*i,&*j,tab);
		recuperer_chaine(a,12,&*i,&*j,ccp) ;  // le n° de ccp
		reglage(a,&*i,&*j,tab);
		conv_char_int (taille,&t);  // convert char (taille) to int
		recuperer_chaine(a,t-12,&*i,&*j,solde) ;  // le solde de ccp
		reglage(a,&*i,&*j,tab);
		n = atol(ccp);  // convert char ( CCP) to long long int
		if ( (n==cle) && (sup[0]!=1) ) *trouv=1;
		if( cle<n) stop=1;
		if ( (*trouv==1) || (stop==1) )
		{
			*j=*j-t+12;          // pour revenir a la fin de n° ccp
			reglage(a,&*i,&*j,tab);
	    }
	}
  }
}





void Decalage (FILE **a , int n , int i , int j ,int b[] , int *x )   // decalage a droite
{
    int i2=-1,j2=Entete2(*a,3)-1,j3,cmp,fin=0,ind,bol,k=*x;
	Tbloc2 buf2,buf3,buf;
if (n!=0 && *a!=NULL)
{
	if (Entete2(*a,3)+n-1 > 1023 )  // pour le dernier bloc
	{
		i2=Alloc_bloc(*a);
		buf3.suiv=-1;
		EcrireDir2(&*a,i2,buf3);
		Aff_Adr(&*a,b[k],i2);
		b[k+1]=i2;
        *x+=1;
	}
	else
	{
	    if ( (Entete2(*a,1)!=Entete2(*a,2)) &&  ( Entete2(*a,2)!=i ) )
	    {
		   LireDir2(*a,b[k],&buf3);
		   j2=Entete2(*a,3)-1;
		   j3=j2+n;
		   buf3.t[j3+1]='\0';
		   while (j2>=0)
		   {
			buf3.t[j3]=buf3.t[j2];
			j2--;
			j3--;
		   }
		   EcrireDir2(&*a,b[k],buf3);
		   j2=0;
		   k--;
	    }
	    if ( (Entete2(*a,1)!=Entete2(*a,2)) &&  ( Entete2(*a,2)==i ) )
	    {
		   fin=1;
		   LireDir2(*a,b[k],&buf3);
		   j2=Entete2(*a,3)-1;
		   j3=j2+n;
		   buf3.t[j3+1]='\0';
		   while (j2>=j)
		   {
			buf3.t[j3]=buf3.t[j2];
			j2--;
			j3--;
		   }
		   EcrireDir2(&*a,b[k],buf3);
	    }
	}
	if (  (Entete2(*a,1)==Entete2(*a,2) ) && (Entete2(*a,3)+n-1 <= 1023 ) )
	{
		LireDir2(*a,Entete2(*a,1),&buf2);
		j2=Entete2(*a,3)-1;
		j3=j2+n;
		buf2.t[j3+1]='\0';
		while (j2>=j)
		{
			buf2.t[j3]=buf2.t[j2];
			j3--;
			j2--;
		}
		fin=1;
		EcrireDir2(&*a,Entete2(*a,1),buf2);
	}

if (fin==0)
{
	while (j2!=j || b[k]!=i )
	{
		j2=1023;
		LireDir2(*a,b[k],&buf2);
		j3=n-1;
		while(  (j3>=0)  &&  (  (j2>j)  ||  (b[k]!=i)  )  )
		{
			buf3.t[j3]=buf2.t[j2];
			j2-=1;
			j3-=1;
		}
		if ((j2==j) && (b[k]==i) ) buf3.t[j3]=buf2.t[j2];
		LireDir2(*a,b[k+1],&buf);
        for (ind=n-1 ; ind>=0 ; ind--) buf.t[ind]=buf3.t[ind] ;
        buf.t[1024]='\0';
    	EcrireDir2(&*a,b[k+1],buf);
    	if (b[k]==i) bol=j;
    	else bol=0;
    	j3=1023;
    	if ( (j2==j) && (b[k]==i) ) j2--;
        else j2=j3-n ;
    	while (j2>=bol)
    	{
    		buf2.t[j3]=buf2.t[j2];
        	j2-=1;
        	j3-=1;
		}
		buf2.t[1024]='\0';
		EcrireDir2(&*a,b[k],buf2);
		if (b[k]==i)
		{
           j2++;
           k++;
        }
		k--;
	}
}
	if (Entete2(*a,3)+n-1 > 1023 )  Aff_entete2(&*a,2,i2);
	Aff_entete2( &*a , 3 , (Entete2(*a,3)+n)%1024 );
}
}




void Ecrire_chaine(FILE **fichier , int n , char s[] , int i , int j , int tab[] , int *h)  // ecrire la chaine s de longeur n dons le bloc i position j dand le fichier
{
  if (*fichier!=NULL)
  {
	Tbloc2 buf,bufc ;
    int i1=-1,k,fini=i,finj=j;
    LireDir2(*fichier,i,&buf);
    for (k=0 ; k<n ; k++)
    {
        if(j<=1023)
        {
            buf.t[j]=s[k];
            j++;
        }
        else
		{
			buf.t[1024]='\0';
			EcrireDir2(&*fichier,i,buf);
			if (i==Entete2(*fichier,2))
			{
				i1=Alloc_bloc(*fichier);
				tab[*h+1]=i1;
				*h+=1;
				Aff_Adr(&*fichier,i,i1);
				i=i1;
				Aff_entete2(&*fichier,2,i1);
			}
			else
			{
			  i=buf.suiv;
			  LireDir2(*fichier,i,&buf);
		    }
			buf.t[0]=s[k] ;
            j=1;
        }
    }
    if (i1!=-1)
    {
        for (k=0 ; k<j ; k++) bufc.t[k]=buf.t[k];
	    EcrireDir2(&*fichier,i,bufc);
	    Aff_Adr(&*fichier,i1,-1);
	    Aff_entete2(&*fichier,2,i1);
	    Aff_entete2(&*fichier,3,j);
    }
    else
    {
	  if (  ( fini==Entete2(*fichier,2) ) &&  ( finj==Entete2(*fichier,3) )  )
	  {
	    Aff_entete2(&*fichier,3,j);
	    buf.t[j]='\0';
	  }
	  EcrireDir2(&*fichier,i,buf);
    }
  }
}




/*** inserer un nouvel enregistrement dans LOVC***/
void insertion_LOVC(FILE **fichier, long long int code_ccp, float solde , int tab[] , int *x)
{
  if (*fichier!=NULL && Entete2(*fichier,1)!=-1 )
  {
	int i,j,trouv=0; // declaration des variables
    Tbloc2 buf;
    recherche_LOVC(*fichier,code_ccp,&trouv,&i,&j,tab); // recherche de la cle por eviter les  dopublon
    if(trouv==0)
    {
        char sc[15],ss[15],sl[15],s[102];
        int j3, e=Longeur_float(solde);
        conv_int_to_char(e+12,sl);
        conv_long_long_int_to_char(code_ccp,sc);
        sprintf(ss,"%.2f", solde);
	    s[0]=sl[0];
	    s[1]=sl[1];
	    s[2]='0';
	    for(j3=3 ; j3<=14 ; j3++) s[j3]=sc[j3-3];
	    for (j3=15 ; j3<=14+e ; j3++) s[j3]=ss[j3-15];
        if (i==Entete2(*fichier,2) && j>=Entete2(*fichier,3))
        {
		    j=Entete2(*fichier,3);
    	    Ecrire_chaine(&*fichier,e+15,s,i,j,tab,&*x);
	    }
	    else
	    {
		    j=j-15;
		    reglage(*fichier,&i,&j,tab);
		    Decalage(&*fichier,e+15,i,j,tab,&*x);
		    Ecrire_chaine(fichier,e+15,s,i,j,tab,&*x);
	    }
	    Color(11,0);
	    printf("\n CLE BINE INSEREE . \n");
        Color(15,0);
    }
    else
    {
        Color(4,0);
        printf("\n OOPS ! insertion impossible , clé déja existante !! .. \n");
        Color(15,0);
    }
  }
  else
  {
  	if (*fichier==NULL)
	{
		Color(4,0);
        printf("\n OOPS ! fichier CCP_SOLDE_2 n'existe pas !!! ... \n");
        Color(15,0);
    }
    else
    {
    	Tbloc2 buf;
    	char ins[30];
    	buf.suiv=-1;
		int i2=Alloc_bloc(*fichier),j2=0,w;
    	Aff_entete2(&*fichier,1,i2);
    	Aff_entete2(&*fichier,2,i2);
    	Aff_entete2(&*fichier,4,0);
    	*x=0;
    	tab[0]=i2;
    	int l=Longeur_float(solde);
    	conv_int_to_char(l+12,ins);
    	buf.t[0]=ins[0];
    	buf.t[1]=ins[1];
    	buf.t[2]=0+'0';
    	conv_long_long_int_to_char(code_ccp,ins);
    	for (w=3 ; w<15 ; w++) buf.t[w]=ins[w-3];
    	sprintf(ins,"%.2f",solde);
    	for (w=15 ; w<15+l ; w++) buf.t[w]=ins[w-15];
    	buf.t[15+l]='\0';
    	EcrireDir2(&*fichier,i2,buf);
    	Aff_entete2(&*fichier,3,15+l);
    	Color(11,0);
	    printf("\n CLE BINE INSEREE . \n");
        Color(15,0);
	}
  }
}




void decalage2(FILE **a , int n , int id , int j ,int b[] , int *x )  // decalage a gouche
{
  if (*a!=NULL &&  n!=0 )
  {
	Tbloc2 buf,buf1,buf2 ;
	int k=*x,j3,j2,i=0,bol=0;

    while (b[i]!=id)
	{
		i++;
	}

	LireDir2(*a,b[i],&buf1);
	j2=j-n; j3=j;
	if (j2<0)
	{
		j2=1024+j2;
		j3=j;
	    i--;
		LireDir2(*a,b[i],&buf);
		LireDir2(*a,b[i+1],&buf1);
		while((j2<=1023) && (  (j3<Entete2(*a,3)) || (i!=k) ) )
		{
			buf.t[j2]=buf1.t[j3];
			j2++;
			j3++;
		}
		EcrireDir2(&*a,b[i],buf);
		if (j2<=1023)
		{
			Aff_Adr(&*a,b[i],-1);
			Aff_entete2(&*a,2,b[i]);
			Aff_entete2(&*a,3,j2);
			*x-=1;
			bol=1;
		}
		else
		{
		    j2=0 ;
			i++;
			while ( (j3<=1023) && (  (j3<Entete2(*a,3)) || (i!=k) )  )
			{
			    buf1.t[j2]=buf1.t[j3];
		        j2++;
		        j3++;
			}
			if (j3<=1023)
			{
			    buf1.t[j2]='\0';
				EcrireDir2(&*a,b[i],buf1);
				Aff_entete2(&*a,3,j2);
				bol=1;
			}
			else
			{
			    EcrireDir2(&*a,b[i],buf1);
			}
		}
	}
	else
	{
	    while ( (j3<=1023) && (  (j3<Entete2(*a,3)) || (i!=k) )  )
		{
		   buf1.t[j2]=buf1.t[j3];
		   j2++;
		   j3++;
	    }
	    if (j3>1023)
	    {
			EcrireDir2(&*a,b[i],buf1);
	    }
	    else
	    {
	    	buf1.t[j2]='\0';
	    	EcrireDir2(&*a,b[i],buf1);
	    	Aff_entete2(&*a,3,j2);
	    	bol=1;
		}
	}
	if (bol==0)
	{
		i++;
		j3=0;
		while (( j3<Entete2(*a,3)) || (i<=k) )
		{
			LireDir2(*a,b[i-1],&buf1);
			LireDir2(*a,b[i],&buf2);
			j2=1024-n;
			j3=0;
			while ( (j2<=1023) && ( ( j3<Entete2(*a,3)) || (i!=k) ))
	    	{
	    		buf1.t[j2]=buf2.t[j3];
	    		j2++;
	    		j3++;
			}
			if (j2<=1023)
			{
				buf1.t[j2]='\0';
	    	    EcrireDir2(&*a,b[i-1],buf1);
	    	    Aff_entete2(&*a,3,j2);
	    	    Aff_entete2(&*a,2,b[i-1]);
	    	    Aff_Adr(&*a,b[i-1],-1);
	    	    *x-=1;
			}
			else
			{
				EcrireDir2(&*a,b[i-1],buf1);
				j2=0;
				j3=j2+n;
				while ( (j3<=1023)  && ( (j3<Entete2(*a,3)) || (i!=k) ))
				{
					buf2.t[j2]=buf2.t[j3];
					j2++;
					j3++;
				}
				if (j3>1023)
				{
					EcrireDir2(&*a,b[i],buf2);
					i++;
				}
				else
				{
					buf2.t[j2]='\0';
					EcrireDir2(&*a,b[i],buf2);
					Aff_entete2(&*a,3,j2);
					i++;
				}
			}
		}
	}
  }
}




void Miss_a_jour2(FILE **a , FILE *b , int tab[] , int *x ) // miss a jour pour LOVC
{
  if (*a!=NULL && Entete2(*a,1)!=-1 && b!=NULL)
  {
	int trouv,i,j,jt,it,taille,jt2,it2,it3,jt3,d1;
	long long int n;
	float f1,f2,f ;
	char ch1[4],ch2[30];
	rewind(b);
	while (!feof(b))
	{
		fscanf(b,"%lld",&n);
		fseek(b, 6, SEEK_CUR);
		fscanf(b,"%f.2",&f1);
		trouv=0;
		recherche_LOVC(*a,n,&trouv,&i,&j,tab);
		reglage(*a,&i,&j,tab);
		jt=j-15;
		it=i;
		reglage(*a,&it,&jt,tab);
		it3=it;jt3=jt;
		recuperer_chaine(*a,2,&it,&jt,ch1);
		reglage(*a,&it,&jt,tab);
		conv_char_int (ch1,&taille);
		it2=i;
		jt2=j;
		recuperer_chaine(*a,taille-12,&it2,&jt2,ch2);
        reglage(*a,&it2,&jt2,tab);
		convert_char_to_float(ch2,&f2);
		f=f1+f2;
		d1=Longeur_float(f)-taille+12;  
		if (d1>0)
		{
			Decalage(&*a,d1,i,j,tab,&*x);
			taille+=d1;
			conv_int_to_char(taille,ch1);
			Ecrire_chaine(&*a,2,ch1,it3,jt3,tab,&*x);
		}
		if (d1<0)
		{
			int j0=j,i0=i;
			j0+=Longeur_float(f2);
			reglage(*a,&i0,&j0,tab);
			decalage2(&*a,-d1,i0,j0,tab,&*x);
			taille+=d1;
			conv_int_to_char(taille,ch1);
			Ecrire_chaine(&*a,2,ch1,it3,jt3,tab,&*x);
		}
		sprintf(ch2,"%.2f",f);
		Ecrire_chaine(&*a,Longeur_float(f),ch2,i,j,tab,&*x);
	}
  }
  else
  {
    if (Entete2(*a,1)==-1)
	{
		Color(4,0);
        printf("\n OOPS ! Le fichier des CCP_SOLDE_2 est vide !! .. \n");
        Color(15,0);
	}
	if (*a==NULL)
	{
		Color(4,0);
        printf("\n OOPS ! Le fichier des CCP_SOLDE_2 n'existe pas !! .. \n");
        Color(15,0);
	}
  	if (b==NULL)
	{
		Color(4,0);
        printf("\n OOPS ! Le fichier des VIREMENTS n'existe pas !! .. \n");
        Color(15,0);
	}
  }
}





void sup_ccp(FILE **fichier, long long int code_ccp , int tab[] )
{
   if (*fichier!=NULL && Entete2(*fichier,1)!=-1)
   {
    Tbloc2 buf ;
    int trouve,i,j ;
    recherche_LOVC(*fichier,code_ccp,&trouve,&i,&j,tab);
    if(trouve==0)
    {
        Color(4,0);
        printf("\n OOPS ! suppression impossible cle n'exite pas !! .. \n");
        Color(15,0);
    }
    else
	{
		j=j-13;
		reglage(*fichier,&i,&j,tab);
		LireDir2(*fichier,i, &buf) ;
		buf.t[j]='1';
		EcrireDir2(&*fichier,i,buf);
		Aff_entete2(&*fichier,4,Entete2(*fichier,4)+1);
		Color(11,0);
        printf("\n CODE BIEN SUPPRIME . \n");
        Color(15,0);
    }
  }
  else
   {
   	   if (Entete2(*fichier,1)==-1)
	   {
		    Color(4,0);
            printf("\n OOPS ! Le fichier des CCP_SOLDE_2 est vide !! .. \n");
            Color(15,0);
	   }
	   if (*fichier==NULL)
	   {
		    Color(4,0);
            printf("\n OOPS ! Le fichier des CCP_SOLDE_2 n'existe pas !! .. \n");
            Color(15,0);
	   }
   }
}




void consultation(FILE *a , int tab[])
{
  if (a!=NULL && Entete2(a,1)!=-1)
  {
	int i=Entete2(a,1),j=0,t;
	float f ;
	char taille[4],ccp[20],solde[30],sup[4];
	while (  ( i!=Entete2(a,2) )  || ( j<Entete2(a,3) )  )
	{
		recuperer_chaine(a,2,&i,&j,taille) ;
		reglage(a,&i,&j,tab);
		recuperer_chaine(a,1,&i,&j,sup) ;
		reglage(a,&i,&j,tab);
		recuperer_chaine(a,12,&i,&j,ccp) ;
		reglage(a,&i,&j,tab);
		conv_char_int (taille,&t);
		recuperer_chaine(a,t-12,&i,&j,solde) ;
		reglage(a,&i,&j,tab);
		convert_char_to_float(solde,&f);
	    if (  ( f<0 ) && ( sup[0]!=1)   )
        {
            printf("                                     | %s\t | compte avec solde",ccp);
            Color(4,0);
            printf(" negatif                    ");
            Color(15,0);
            printf("|\n");
        }
	    if (  ( f>=1000000 ) && ( sup[0]!=1)   )
	    {
            printf("                                     | %s\t | compte avec solde",ccp);
            Color(11,0);
            printf(" superieur à 1000000.00     ");
            Color(15,0);
            printf("|\n");
	    }
	}

  }
  else
  {
  	   if (Entete2(a,1)==-1)
	   {
		    Color(4,0);
            printf("\n OOPS ! Le fichier des CCP_SOLDE_2 est vide !! .. \n");
            Color(15,0);
	   }
	   if (a==NULL)
	   {
		    Color(4,0);
            printf("\n OOPS ! Le fichier des CCP_SOLDE_2 n'existe pas !! .. \n");
            Color(15,0);
	   }
  }
}




void Affichage(FILE *a)   // module d'affichage du 3 premier blocs du fichier LOVC
{
	if (a!=NULL && Entete2(a,1)!=-1)
	{
		int cpt=0,i=Entete2(a,1),j=0,k=0,l=0;
		Tbloc2 buf ;
		while (  (cpt<3)  &&  (  i!=-1  ) )
		{
			Color(4,0);
		    printf("                           ====================================================================\n");
			LireDir2(a,i,&buf);
			j=0;
			if (i==Entete2(a,2))
			{
				for (k=0 ; k<16 ; k++ )
				{
				    printf("                           ");
					Color(4,0);
					printf("| ");
					for (l=0 ; l<64 ; l++)
					{
						Color(3,0);
						if (j<Entete2(a,3))
						{
						    printf("%c",buf.t[j]);
						    j++;
						}
						else printf(" ");
					}
					Color(4,0);
					printf(" |\n");
				}
			}
			else
			{
				for (k=0 ; k<16 ; k++ )
				{
				    printf("                           ");
					Color(4,0);
					printf("| ");
					for (l=0 ; l<64 ; l++)
					{
						Color(3,0);
						printf("%c",buf.t[j]);
						j++;
					}
					Color(4,0);
					printf(" |\n");
				}
			}
			i=buf.suiv;
			cpt++;
			Color(4,0);
			printf("                           ====================================================================\n");
			if (i!=-1 && cpt<3)
			{
			    Color(9,0);
			    printf("                            ------------------------------------------------------------------- \n");
			    printf("                            |");
			    Color(4,0);
			    printf("                            SUIVANT                              ");
			    Color(9,0);
			    printf("|\n");
			    printf("                            ------------------------------------------------------------------- \n");
			    printf("                                                            --- \n");
			    printf("                                                            | | \n");
			    printf("                                                           _| |_ \n ");
			    printf("                                                          %c   / \n ",92);
			    printf("                                                           %c / \n",92);
			    printf("                                                             v \n");
		    }
		    if (i==-1 )
			{
			    Color(9,0);
			    printf("                            ------------------------------------------------------------------- \n");
			    printf("                            |");
			    Color(4,0);
			    printf("                            NULL                                 ");
			    Color(9,0);
			    printf("|\n");
			    printf("                            ------------------------------------------------------------------- \n");
		    }
		}
		Color(0,15);
		printf("                                                  AFFICHAGE D'ENTETE :                                                           ");
		Color(15,0);
		printf("\n");
		printf("      - La tete de liste du blocs :  ");
		Color(2,0);
		printf("                      %d\n",Entete2(a,1));
		Color(15,0);
		printf("      - La queu de liste du blocs :  ");
		Color(2,0);
		printf("                      %d\n",Entete2(a,2));
		Color(15,0);
		printf("      - L'indice du premier case vide dans la queu  :  ");
		Color(2,0);
		printf("  %d\n",Entete2(a,3));
		Color(15,0);
		printf("      - Le nombre du suppresions :  ");
		Color(2,0);
		printf("                        %d\n",Entete2(a,4));
		Color(15,0);
    }
	else
	{
	   if (Entete2(a,1)==-1)
	   {
		    Color(4,0);
            printf("\n OOPS ! Le fichier des CCP_SOLDE_2 est vide !! .. \n");
            Color(15,0);
	   }
	   if (a==NULL)
	   {
		    Color(4,0);
            printf("\n OOPS ! Le fichier des CCP_SOLDE_2 n'existe pas !! .. \n");
            Color(15,0);
	   }
	}
}






int main()  // Le programme principal :
{
    float solde ;
    int tab[300],nb=0; // tab contien les addreses des blocs du fichier LOVC et nb c'est l'indice du dernier cas dans tab 
    setlocale (LC_ALL,"");
    int a , m=0 ,k, choix ;
    long long int code_ccp;
	FILE *f=NULL,*g=NULL,*h=NULL;
    char F1[25]="CCP_SOLDE.bin",F2[25]="VIREMENTS.txt",F3[25]="CCP_SOLDE_2.bin"  ,d='N'; 
	Ouvrire(&f ,F1,d);
	Ouvrire(&g ,F2,d);
	Ouvrire(&h ,F3,d);
	if (f==NULL || h==NULL ) return 1 ;
	Aff_entete2(&h,1,-1);
	Aff_entete(&f,2,0);
	rewind(g);
	printf("\a");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
	Color(7,0);
    printfslow("========================================================================================================================\n",15);
    Color(15,0);
    Color(7,0);
    printfslow("                          ===================================================================\n",25);
    Color(15,0);
    Color(11,0);
    printfslow("                                BIENVENUE DANS NOTRE APPLICATION DE GESTION D'UN FICHIER \n",25);
    printfslow("                                 (L'ECOLE NATIONALE SUPERIEURE D'INFORMATIQUE)\n",25);
    printfslow("                                                TP REALISE PAR : \n",25);
    printfslow("                                            -->CHELIHI SOUFFIANE\n",25);
    printfslow("                                            -->DIF MERYEM MALAK \n",25);
    printfslow("                                                [  GROUPE 07  ]\n",25);
    Color(15,0);
    Color(7,0);
    printfslow("                          ===================================================================\n",25);
    Color(15,0);
    Color(7,0);
    printfslow("========================================================================================================================\n",15);
    Color(15,0);
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("Vous pouvez cliquez sur la touche Entrée pour aller au menu principal ! \n");
    getchar();
    system("cls");
   do
   {
        menu() ;
        Color(11,0);
        printf("Veuillez choisir l'action que vous souhaitez exécuter : ");
        Color(15,0);
        do
        {
            scanf("%d",&a);
            if(a>10 || a<1)   {
                     printf("\n") ;
                    printf("-Veuillez entrer un chiffre compris entre 1 et 11 .\n");}
        }
        while (a>10 || a<1);
        printf("\n") ;
        system("cls");
        switch(a)
        {
            case 1:
            {
                printf("\n");
                printf("\n");
                Color(7,8);
                printf("                                              CRÉATION DU FICHIER BINAIRE CCP_BIN_1                                            ");
                Color(15,0);
                printf("\n");
                printf("\n");
                Color(0,9);
		        printf(" Donner le nombre des comptes CCP : ");
                Color(15,0);
		        scanf("%d",&m);
                printf("\n");
                printf(" Veuillez patienter ...\n");
                Chargement_Fichier_F1(&f,m);
                printf("Vous pouvez cliquez sur la touche Entrée pour continuer ! \n");
                printf("\n") ;
                getchar();
                getchar();
                system("cls");
                break ;
           }
           case 2:
           {
                printf("\n");
                printf("\n");
                Color(7,8);
                printf("                                              CRÉATION DU FICHIER TEXTE DES VIREMENTS                                            ");
                Color(15,0);
                printf("\n");
                printf("\n");
                Chargement_Fichier_F2(f,&g,&k);
                printf("\n");
                Color(11,0);
                if (m!=0) printf("Genial ! Vous avez cree le fichier des virements avec succes .\n") ;
                printf("\n");
                Color(15,0);
                printf("Vous pouvez cliquez sur la touche Entrée pour continuer ! \n");
                printf("\n") ;
                getchar();
                getchar();
                system("cls");
                break ;
           }
           case 3:
           {
                printf("\n");
                printf("\n");
                Color(7,8);
                printf("                                              MISE A JOUR DU FICHIER                                            ");
                Color(15,0);
                printf("\n");
                printf("\n");
                Mises_a_jour_plus_Affichage(g,&f);
                printf("\n");
                printf("Vous pouvez cliquez sur la touche Entrée pour continuer ! \n");
        		getchar();
                getchar();
                system("cls");
                break ;
           }
            case 4:
           {
                printf("\n");
                printf("\n");
                Color(7,8);
                printf("                                              CRÉATION DU FICHIER BINAIRE CCP_BIN_2                                             ");
                Color(15,0);
                printf("\n");
                printf("\n");
                printf(" Veuillez patienter ...\n");
	            Chargement_CCP_SOLDE_2(&h,f,tab,&nb);
	            Color(11,0);
                if (m!=0) printf("Genial ! Vous avez cree le fichier ccp_bin_2 avec succes .\n") ;
                printf("\n");
                Color(15,0);
                printf("Vous pouvez cliquez sur la touche Entrée pour continuer ! \n");
                getchar();
                getchar();
                system("cls");
                break ;
           }
           case 5 :
           {
		        printf("\n");
                printf("\n");
                Color(7,8);
                printf("                                              MISE A JOUR DU FICHIER BINAIRE CCP_BIN_2                                            ");
                Color(15,0);
                printf("\n");
                printf("\n");
                printf(" Veuillez patienter ...\n");
                Miss_a_jour2(&h,g,tab,&nb);
                printf("\n");
                Color(11,0);
                if (m!=0)  printf("Genial ! Vous avez fait la mise a jour avec succés .\n") ;
                printf("\n");
                Color(15,0);
                printf("Vous pouvez cliquez sur la touche Entrée pour continuer ! \n");
                getchar();
                getchar();
                system("cls");
                break ;
           }
           case 6 :
           {
                printf("\n");
                printf("\n");
                Color(7,8);
                printf("                                              INSERER UN NOUVEL ENREGISTREMENT DANS CCP_BIN_2                                            ");
                Color(15,0);
                printf("\n");
                printf("\n");
                do
				{
                    Color(0,9);
	            	printf(" Entrez le code ccp que vous voulez inserer : ");
		            Color(15,0);
                    scanf("%lld",&code_ccp);
                    Color(0,9);
                    printf("\n") ;
	            	printf(" Entrez le solde du CCP sous la form ( int , int ) : ");
		            Color(15,0);
                    scanf("%f",&solde);
	                insertion_LOVC(&h,code_ccp,solde,tab,&nb);
	                printf("\n") ;
	                printf("\n") ;
                    Color(9,0);
                    printf("                                       [ 01 ]-INSERER UN AUTRE CODE CCP. \n");
                    printf("\n");
                    Color(15,0);
                    Color(9,0);
                    printf("                                       [ 02 ]-REVENIR AU MENU PRINCIPAL. \n");
                    printf("\n");
                    Color(15,0);
                    Color(11,0);
                    printf("Veuillez choisir l'action que vous souhaitez exécuter : ");
                    Color(15,0);

                    do
                    {
                        scanf("%d",&choix);
                         printf("\n");
                        if(choix>2 || choix<1)
                        printf("-Veuillez entrer un chiffre compris entre 1 et 2 .\n");
                    }
                    while (choix>2 || choix<1);
                }
                while(choix!=2);
                printf("\n");
	            Color(11,0);
                printf("Genial ! Insertion faite avec succes .\n") ;
                printf("\n");
                Color(15,0);
                printf("Vous pouvez cliquez sur la touche Entrée pour continuer ! \n");
	            getchar();
                getchar();
                system("cls");
                break ;
           }
           case 7:
           {
                printf("\n");
                printf("\n");
                Color(7,8);
                printf("                                              SUPPRESSION D UN ENREGISTREMENT DANS CCP_BIN_2                                            ");
                Color(15,0);
                printf("\n");
                printf("\n");
                do
				{
				    Color(0,9);
		            printf(" Entrez le code ccp que vous voulez supprimer : ");
		            Color(15,0);
                    scanf("%lld",&code_ccp);
	                sup_ccp(&h,code_ccp,tab);
	                printf("\n");
                    printf("\n") ;
                    Color(9,0);
                    printf("                                       [ 01 ]-SUPPRIMER UN AUTRE CODE CCP. \n");
                    printf("\n");
                    Color(15,0);
                    Color(9,0);
                    printf("                                       [ 02 ]-REVENIR AU MENU PRINCIPAL. \n");
                    printf("\n");
                    Color(15,0);
                    Color(11,0);
                    printf("Veuillez choisir l'action que vous souhaitez exécuter : ");
                    Color(15,0);
                    do
                    {
                        scanf("%d",&choix);
                        if(choix>2 || choix<1)
                        printf("-Veuillez entrer un chiffre compris entre 1 et 2 .\n");
                    }
                    while (choix>2 || choix<1);
               }
               while(choix!=2);
                printf("\n");
	            Color(11,0);
                if (m!=0) printf("Genial ! Suppression faite avec succes .\n") ;
                printf("\n");
                Color(15,0);
                printf("Vous pouvez cliquez sur la touche Entrée pour continuer ! \n");
	            getchar();
                getchar();
                system("cls");
	            break ;
           }
           case 8 :
           {
                printf("\n");
                printf("\n");
                Color(7,8);
                printf("                                              CONSULTATION DE CERTAINS COMPTES DANS CCP_BIN_2                                            ");
                Color(15,0);
                printf("\n");
                printf("\n");
                consultation(h,tab);
                printf("\n");
                printf("Vous pouvez cliquez sur la touche Entrée pour continuer ! \n");
                getchar();
                getchar();
                system("cls");
                break ;
           }
           case 9 :
           {
                printf("\n");
                printf("\n");
                Color(7,8);
                printf("                                              REPRESENTATION GRAPHIQUE                                            ");
                Color(15,0);
                printf("\n");
                printf("\n");
                Affichage(h);
                printf("\n") ;
                printf("\n");
                printf("Vous pouvez cliquez sur la touche Entrée pour continuer ! \n");
                getchar();
                getchar();
                system("cls");
                break ;
           }
       }

    }
	while(a!=10);

    system("cls");
    printf("\a");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    Color(7,0);
    printfslow("========================================================================================================================\n",15);
    Color(15,0);
    Color(7,0);
    printfslow("                          ==========================================================================\n",15);
    Color(15,0);
    Color(11,0);
    printfslow("                                    MERCI D'AVOIR UTILISER NOTRE APPLICATION  \n",25);
    Color(15,0);
    Color(7,0);
    printfslow("                          ==========================================================================\n",15);
    Color(15,0);
    Color(7,0);
    printfslow("========================================================================================================================\n",15);
    Color(15,0);
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
	Fermer(&f);
    Fermer(&g);
    Fermer(&h);
    return 0 ;
}
