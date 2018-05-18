


#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include "mesfonctions.h"
#include <unistd.h>
#include <pthread.h>





void initRgbImg(struct rgbImg **strImg, int h, int l)
{
	//On alloue la structure
	*strImg=malloc(sizeof(rgbImg));
	
	(*strImg)->hauteur = h;
	(*strImg)->largeur = l;
	
	(*strImg)->red=malloc(h*sizeof(Uint8*));
	(*strImg)->blue=malloc(h*sizeof(Uint8*));
	(*strImg)->green=malloc(h*sizeof(Uint8*));
	
	int i;
	for (i=0; i<h; i++)
	{
		(*strImg)->red[i]=malloc(l*sizeof(Uint8));
		(*strImg)->blue[i]=malloc(l*sizeof(Uint8));
		(*strImg)->green[i]=malloc(l*sizeof(Uint8));
	}
}




void initRGB(struct rgb **ptrRGB, int time, int h, int l)
{
	//On alloue la structure
	*ptrRGB=malloc(sizeof(rgb));
	(*ptrRGB)->temps = time;
	(*ptrRGB)->hauteur = h;
	(*ptrRGB)->largeur = l;
	
	(*ptrRGB)->red=malloc(time*sizeof(float*));
	(*ptrRGB)->green=malloc(time*sizeof(float*));
	(*ptrRGB)->blue=malloc(time*sizeof(float*));

	int i;
	for (i=0; i<time; i++)
	{
		(*ptrRGB)->red[i]=malloc(h*sizeof(float*));
		(*ptrRGB)->green[i]=malloc(h*sizeof(float*));
		(*ptrRGB)->blue[i]=malloc(h*sizeof(float*));
	}

	
	int j;
	for (i=0; i<time; i++)
	{
		for (j=0; j<h; j++)
		{
			(*ptrRGB)->red[i][j]=malloc(l*sizeof(float*));
			(*ptrRGB)->green[i][j]=malloc(l*sizeof(float*));
			(*ptrRGB)->blue[i][j]=malloc(l*sizeof(float*));
		}
	}

}

void initHSV(struct hsv **HSVTab,  int h, int l)
{
	int i;
	//On alloue la structure
	*HSVTab=malloc(sizeof(hsv));
	(*HSVTab)->hauteur = h;
	(*HSVTab)->largeur = l;
	
	(*HSVTab)->value=malloc(h*sizeof(float*));
	(*HSVTab)->hue=malloc(h*sizeof(float*));
	(*HSVTab)->sat=malloc(h*sizeof(float*));

	for (i=0; i<h; i++)
	{
		(*HSVTab)->value[i]=malloc(l*sizeof(float));
		(*HSVTab)->hue[i]=malloc(l*sizeof(float));
		(*HSVTab)->sat[i]=malloc(l*sizeof(float));
	}
}





void freeRgbImg(struct rgbImg **img)
{
    int i;
    for (i = 0; i < (*img)->hauteur; i++)
    {
        free((*img)->red[i]);
        free((*img)->green[i]);
        free((*img)->blue[i]);
    }
	
    free((*img)->red);
    free((*img)->green);
    free((*img)->blue);

    free(*img);

}


void freeRGB(struct rgb **img)
{
    int i,j;
 
    for (i=0; i<(*img)->temps; i++)
	{
		for (j=0; j<(*img)->hauteur; j++)
		{
			free((*img)->red[i][j]);
			free((*img)->green[i][j]);
			free((*img)->blue[i][j]);
		}
	}


    for (i = 0; i < (*img)->temps; i++)
    {
        free((*img)->red[i]);
        free((*img)->green[i]);
        free((*img)->blue[i]);
    }



    free((*img)->red);
    free((*img)->green);
    free((*img)->blue);
    free(*img);

}


void freeHSV(struct hsv **img)
{
 
	int i;
 

		for (i=0; i<(*img)->hauteur; i++)
		{
			free((*img)->value[i]);
			free((*img)->hue[i]);
			free((*img)->sat[i]);
		}
	
    free((*img)->hue);
    free((*img)->sat);
    free((*img)->value);
    free(*img);

}


void pauseProg()
{
	int continuer = 1;
	SDL_Event event;
	
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer = 0;
		}
	}
}



Uint32 GetPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;

    unsigned char *p = (unsigned char*)surface->pixels + x * surface->pitch + y * bpp;


    switch(bpp) {
    case 1:
        return *p;
    case 2:
        return *(Uint16 *)p;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32 *)p;
    default:
        return 0;
    }
}

void img2Struct(struct rgbImg *strImg, SDL_Surface *GrainDB) 
{ 

    Uint8 r,g,b;
    Uint32 pixel;

	int i;
    for (i = 0; i < GrainDB->h; i++)
    {
		int j;
        for (j = 0; j < GrainDB->w; j++)
        {
            pixel=GetPixel(GrainDB,i,j);
            SDL_GetRGB(pixel, GrainDB->format, &r, &g, &b);
            strImg->red[i][j]=r;
            strImg->green[i][j]=g;
            strImg->blue[i][j]=b;
     
            //printf("i=%d j=%d r=%03d g=%03d b=%03d \n",i+1,j+1,strImg->red[i][j],strImg->green[i][j],strImg->blue[i][j] );
           
        }
    }
}


void definirPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{

    /*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.

    En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur

    de l'image : 8, 16, 24 ou 32 bits.*/

    int nbOctetsParPixel = surface->format->BytesPerPixel;

    /*Ici p est l'adresse du pixel que l'on veut modifier*/

    /*surface->pixels contient l'adresse du premier pixel de l'image*/

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;


    /*Gestion différente suivant le nombre d'octets par pixel de l'image*/

    switch(nbOctetsParPixel)

    {

        case 1:

            *p = pixel;

            break;


        case 2:

            *(Uint16 *)p = pixel;

            break;


        case 3:

            /*Suivant l'architecture de la machine*/

            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)

            {

                p[0] = (pixel >> 16) & 0xff;

                p[1] = (pixel >> 8) & 0xff;

                p[2] = pixel & 0xff;

            }

            else

            {

                p[0] = pixel & 0xff;

                p[1] = (pixel >> 8) & 0xff;

                p[2] = (pixel >> 16) & 0xff;

            }

            break;


        case 4:

            *(Uint32 *)p = pixel;

            break;

    }

}

void RGB2IMG(struct rgbImg *RGB, SDL_Surface * Surface)
{
	Uint32 pixel;
	int i;
    for (i = 0; i < RGB->hauteur; i++)
    {
		int j;
        for (j = 0; j < RGB->largeur; j++)
        {
			pixel=SDL_MapRGB(Surface->format, RGB->red[i][j], RGB->green[i][j], RGB->blue[i][j]);
			definirPixel(Surface,j,i,pixel);
        }
    }
	
}





void Rgb2Hsv(struct rgbImg *RGB, struct hsv **HSV)
{

    float max;
    float min;
    
    int i;
    for (i = 0; i < RGB->hauteur; i++)
    {
		int j;
        for (j = 0; j < RGB->largeur; j++)
        {
			
			 	float red = (float)(RGB->red[i][j]/255.0);
				float green = (float)(RGB->green[i][j]/255.0);
				float blue = (float)(RGB->blue[i][j]/255.0);
				

				if (red > blue)
				{
					max = red;
					min = blue;
				}

				else
				{
					max = blue;
					min = red;
				}
				if (green > max)
				{
					max = green;
				}
				if (green < min)
				{
					min = green;
				}

				(*HSV)->value[i][j] = max;

				if(max==red)
					(*HSV)->hue[i][j]=(float)((int)((60*((green-blue)/(max-min))+360))%360);
        
				if(max==green)
					(*HSV)->hue[i][j]=(float)((60*((blue-red)/(max-min))+120));

        
				if(max==blue)
					(*HSV)->hue[i][j]=(float)((60*((red-green)/(max-min))+240));
      
				if( max-min<SEUIL_MINMAX)
					(*HSV)->hue[i][j]=-500;
    
				if (max == 0)
				{
					(*HSV)->sat[i][j] = 0;
				}
				else
				{
					(*HSV)->sat[i][j]=(float)(1 - min/max);
				}
        }
    }

    //Nettoie les bords de l'images
	//cleanBord(HSV,time);

}



void PrintRedImg(struct rgbImg * RGB, int** imgTamp)
{
	int i,j;
    for (i = 0; i < RGB->hauteur; i++)
    {
		
        for (j = 0; j < RGB->largeur; j++)
        {
            if ( imgTamp[i][j] == 1)
            {
				RGB->red[i][j] = 0;
				RGB->green[i][j] = 255;
				RGB->blue[i][j] = 0;
			}
			
        }
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////////

float calculDistanceCouleur(int h1,int w1,int h2,int w2, struct hsv *HSVTab)
{
	float hue,hue1,hue2,sat;
	
	if(h1>0 && w1>0 && h2>0 && w2>0 && h1<HSVTab->hauteur && w1<HSVTab->largeur && h2<HSVTab->hauteur && w2<HSVTab->largeur)
	{
		if (HSVTab->hue[h1][w1]>360)
		{
				hue1=360-(HSVTab->hue[h1][w1]);
		}
		else
		{
				hue1=HSVTab->hue[h1][w1];
		}

		if (HSVTab->hue[h2][w2]>360)
		{
				hue2=360-HSVTab->hue[h2][w2];
		}
		else
		{
				hue2=HSVTab->hue[h2][w2];
		}
		hue=hue1-hue2;

		hue=hue*hue; // on élève au carré

		sat=HSVTab->sat[h1][w1]*IMPORTANCE_SAT *360-HSVTab->sat[h2][w2]*IMPORTANCE_SAT *360;

		sat=sat*sat; 



		return hue+sat;
	}
	else
	{
		
		return -1;
	}


}










void croissanceRegion(int hSeed,int wSeed,int **tabPixel,struct hsv *HSVTab)
{
	int i,j,a,b,flag,flagChangement;
	int **listPixel; 
	float dist;
	
	// list pixel stock l'ensemble des points de la région. Pour chaque pixel
	// il a 3 champs : 0 est un flag pour savoir si le point appartient au bord, 1 est son height (hauteur) et 2 son weigh (largeur)
	// tabPixel est l'image ou les pixesl de la région valent 1 et les autres 0
	int hMedian,wMedian;
	long long int nombreDePointsRegion;


	hMedian=hSeed;
	wMedian=wSeed;
	flagChangement=0;
	flag=0;
	nombreDePointsRegion=1; // la seed

	listPixel=malloc((HSVTab->hauteur*HSVTab->largeur)*sizeof(int*));
	for (i = 0; i < HSVTab->hauteur*HSVTab->largeur; ++i)
	{
		listPixel[i]=malloc(4*sizeof(int));
	}

	listPixel[0][0]=1; // c'est un bord de la région
	listPixel[0][1]=hSeed;
	listPixel[0][2]=wSeed;

	for (i = 1; i < HSVTab->hauteur*HSVTab->largeur; ++i)
	{
		
		listPixel[i][0]=-1;
		listPixel[i][1]=0;
		listPixel[i][2]=0;
		
	}

	for (i = 0; i < HSVTab->hauteur; ++i)
	{
		for (j = 0; j < HSVTab->largeur; ++j)
		{
			tabPixel[i][j]=0;
		}
	}
	tabPixel[hSeed][wSeed]=1;


	while(flagChangement!=1)
	{
		for (i = 0; i < nombreDePointsRegion; ++i)
		{
			if(listPixel[i][0]==1)
			{

				for(a=-1;a<2;a++)
				{
					for (b = -1; b < 2; ++b)
					{
						
							
							dist=calculDistanceCouleur(hMedian,wMedian,listPixel[i][1]+a,listPixel[i][2]+b,HSVTab);
					//	printf("dist=%lf\n",dist );
							if(dist<TOLERANCE_CARRE && dist!=-1)
							{
								if (tabPixel[listPixel[i][1]+a][listPixel[i][2]+b]==0)
								{

									listPixel[nombreDePointsRegion+flag][0]=1;
									listPixel[nombreDePointsRegion+flag][1]=listPixel[i][1]+a;
									listPixel[nombreDePointsRegion+flag][2]=listPixel[i][2]+b;
									tabPixel[listPixel[i][1]+a][listPixel[i][2]+b]=1;
									flag++;
								}

							}
						

					}
				}
			}
		}


	
		if (flag!=0)
		{
			nombreDePointsRegion=nombreDePointsRegion+flag;
			flag=0;
		}
		else
		{
			flagChangement=1;
		}

		for (i = 0; i < nombreDePointsRegion; ++i)
		{
			
			if (listPixel[i][0]==1) // ceci est un bord
			{

				for(a=-1;a<2;a++)
				{
					for (b = -1; b < 2; ++b)
					{
						
					if(listPixel[i][1]+a>0 && listPixel[i][1]+a<HSVTab->hauteur && listPixel[i][2]+a>0 && listPixel[i][2]+a<HSVTab->largeur)
						if (tabPixel[listPixel[i][1]+a][listPixel[i][2]+b]==1)
						{
							
							flag++; 
							// on réutilise la variable flag mais par pour le parcours de la région, on cherche juste à voir si ce pixel est encore un bord 
						}
						
					}
				}
				if (flag==9)
				{
					listPixel[i][0]=0;
				}
				flag=0;



			}
		}
		

/*


		//tabSommeDistances=malloc(nombreDePointsRegion*sizeof(int));
		for (i = 0; i <nombreDePointsRegion; ++i)
		{
		//	tabSommeDistances[i]=0;
			listPixel[i][3]=0;
		}
		for(i=0; i<nombreDePointsRegion;i++)
		{
			for (j = 0; j < nombreDePointsRegion; ++j)
			{
				listPixel[i][3]+=calculDistanceCouleur(listPixel[i][1],listPixel[i][2],listPixel[j][1],listPixel[j][2], HSVTab);				
				
			}

		}

		min=listPixel[0][3];
		for (i = 1; i <nombreDePointsRegion; ++i)
		{
		if (min>=listPixel[i][3])
		{
			min=listPixel[i][3];
			hMedian=listPixel[i][1];
			wMedian=listPixel[i][2];
		}

		
		}
		
	//	printf("medianne : %d %d red=%d green=%d blue=%d nb de points : %d min=%lf\n",hMedian,wMedian,RGB->red[hMedian][wMedian],RGB->green[hMedian][wMedian],RGB->blue[hMedian][wMedian],nombreDePointsRegion,(float)((int)min)/((int)nombreDePointsRegion) );

*/
		

 ///////////////////////////////////////////////////

	

 //////////////////////////////////////////////////



	}

	for (i = 0; i < HSVTab->hauteur*HSVTab->largeur; ++i)
	{
		free(listPixel[i]);
	}
	free(listPixel);


}













void nivGrisHue(struct hsv *HSVTab, struct rgbImg *photoRGB)
{
	int i,j,val;

	for ( i = 0; i < photoRGB->hauteur; ++i)
	{
		for (j = 0; j < photoRGB->largeur; ++j)
		{
			val=(int)(HSVTab->hue[i][j]*255/360);
			photoRGB->red[i][j]=val;
			photoRGB->green[i][j]=val;
			photoRGB->blue[i][j]=val;
		}
	}
}



// void detecteHemorragie(struct hsv *HSVTab, struct rgbImg * photoRGB, int **tabPixel)
// {
// 	int i,j,k,l,compteur, comptLine=0, comptColonnes=0;
// 	compteur=0;
// 	int** tabReduce;
// 	tabReduce=malloc((photoRGB->hauteur/3)*sizeof(int*));
// 	for (i = 0; i < photoRGB->hauteur/3; ++i)
// 	{
// 		tabReduce[i]=malloc((photoRGB->largeur/3)*sizeof(int));
// 	}

// comptLine=0;

// 	for ( i = 1; i < photoRGB->hauteur-1; i+=3)
// 	{
// 	comptLine++;
// 		for (j = 1; j < photoRGB->largeur-1; j+=3)
// 		{
// 			comptColonnes++;
// 			for (k=-1; k<2;k++)
// 			{
// 				for (l=-1; l<2; l++)
// 				{		
// 					if((HSVTab->hue[i+k][j+l]<(float)SANG_MAX || HSVTab->hue[i+k][j+l]>(float)SANG_MIN) && HSVTab->sat[i+k][j+l]>(float)SAT_MAX )
// 					{
// 						compteur++;
// 					}
// 				}
// 			}
// 			if (compteur>7)
// 			{
// 			//	tabReduce[i][j]=1;
// 			//	printf("là je suis dans une grosse tache\n");
// 				for (k=-1; k<2;k++)
// 				{
// 					for (l=-1; l<2; l++)
// 					{		
// 						tabPixel[i+k][j+l]=1;
// 					}
// 				}
// 			}
// 			else
// 			{
// 		//		tabReduce[i][j]=0;

// 			}
// 			compteur=0;

// 	//	printf("nb colonnes =%d\n",comptColonnes );
// 		}
// 		comptColonnes=0;

// 	}
// 	//		printf("nb lignes =%d\n",comptLine );
// 		//	printf("longueur=%d, largeur=%d \n",photoRGB->hauteur,photoRGB->largeur );
	
// }




// carrees de 5

void detecteHemorragie(struct hsv *HSVTab, struct rgbImg * photoRGB, int **tabPixel)
{
	printf("azdiazdazdazdazdazd\n");
	int i,j,k,l,compteur, comptLine=0, comptColonnes=0;
	compteur=0;
	int** tabReduce;
	tabReduce=malloc(((photoRGB->hauteur)/5)*sizeof(int*));
	for (i = 0; i < (photoRGB->hauteur)/5; ++i)
	{
		tabReduce[i]=malloc(((photoRGB->largeur)/5)*sizeof(int));
	}

comptLine=0;

	for ( i = 2; i < photoRGB->hauteur-2; i+=5)
	{
	comptLine++;
		for (j = 2; j < photoRGB->largeur-2; j+=5)
		{
			comptColonnes++;
			for (k=-2; k<3;k++)
			{
				for (l=-2; l<3; l++)
				{		
					if((HSVTab->hue[i+k][j+l]<(float)SANG_MAX || HSVTab->hue[i+k][j+l]>(float)SANG_MIN) && HSVTab->sat[i+k][j+l]>(float)SAT_MAX )
					{
						compteur++;
					}
				}
			}
			if (compteur>20)
			{
			//	tabReduce[i][j]=1;
			//	printf("là je suis dans une grosse tache\n");
				for (k=-2; k<3;k++)
				{
					for (l=-2; l<3; l++)
					{		
						tabPixel[i+k][j+l]=1;
					}
				}
			}
			else
			{
		//		tabReduce[i][j]=0;
			}
			compteur=0;

	//	printf("nb colonnes =%d\n",comptColonnes );
		}
		comptColonnes=0;

	}
	//		printf("nb lignes =%d\n",comptLine );
		//	printf("longueur=%d, largeur=%d \n",photoRGB->hauteur,photoRGB->largeur );
	
}





void detecteHemorragieBinaryTab(int **tabPixel, int hauteur, int largeur, int tailleFiltre)
{
	int i,j,k,l,compteur, comptColonnes=0;
	compteur=0;
	int** tabReduce;
	tabReduce=malloc((hauteur/(2*tailleFiltre+1))*sizeof(int*));
	for (i = 0; i < hauteur/(2*tailleFiltre+1); ++i)
	{
		tabReduce[i]=malloc((largeur/(2*tailleFiltre+1))*sizeof(int));
	}


	for ( i = tailleFiltre; i < hauteur-tailleFiltre; i+=(2*tailleFiltre+1))
	{

		for (j = tailleFiltre; j < largeur-tailleFiltre; j+=(2*tailleFiltre+1))
		{
			comptColonnes++;
			for (k=-tailleFiltre; k<tailleFiltre+1;k++)
			{
				for (l=-tailleFiltre; l<tailleFiltre+1; l++)
				{		
					if(tabPixel[i+k][j+l]==1 )
					{
						compteur++;
					}
				}
			}
			if (compteur>7)
			{
				tabReduce[i][j]=1;
			//	printf("là je suis dans une grosse tache\n");
				for (k=-tailleFiltre; k<tailleFiltre+1;k++)
				{
					for (l=-tailleFiltre; l<tailleFiltre+1; l++)
					{		
						tabPixel[i+k][j+l]=1;
					}
				}
			}
			else
			{
				tabReduce[i][j]=0;

			}
			compteur=0;

	//	printf("nb colonnes =%d\n",comptColonnes );
		}
		comptColonnes=0;

	}
	
		//	printf("longueur=%d, largeur=%d \n",photoRGB->hauteur,photoRGB->largeur );
	
}








