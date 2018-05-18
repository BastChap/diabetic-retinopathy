#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "mesfonctions.h"
#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include <time.h>
#include <sys/stat.h>
#include <string.h>		// pour les strcpy et strcat
#include <unistd.h>		// pour le execl
#include <sys/types.h> // pour le wait
#include <sys/wait.h>	// pour le wait








int main(int argc, char *argv[])
{

	struct rgbImg *photoRGB, *copieRef,*photoGrey;
	struct hsv *HSVTab;
	int i,j,nbSang;
	int hSeed, wSeed;
	int continuer=1;
	int **tabPixel, **tabPixelTemp; 

	

		//Initialisation des surfaces et chargement des image
	SDL_Surface *ecran = NULL,*image=NULL, *imageDeFond = NULL; // init de struct d'image
    SDL_Rect positionFond;
    SDL_Event event;
    positionFond.x = 0;
    positionFond.y = 0;
	

	//Initialisation de la SDL1.2
	SDL_Init(SDL_INIT_EVERYTHING);

if (argc>1)
{
	printf("%d\n",argc );
	image = IMG_Load((const char *)(argv[argc-1])); 
}
else
{
	printf("image par défaut : ./largeHemo.jpg\n");
	image = IMG_Load("./largeHemo.jpg"); // on pourra répéter ce traitement pour chaque image
}

	
	SDL_SaveBMP(image,"hemo.bmp");
	

	initRgbImg(&photoRGB, image->h, image->w);
	initRgbImg(&photoGrey, image->h, image->w);
	initRgbImg(&copieRef, image->h, image->w);  

		//Conversion image => RGB
	img2Struct(photoRGB,image);
	img2Struct(photoGrey,image);
	img2Struct(copieRef,image);

	tabPixel=malloc((photoRGB->hauteur)*sizeof(int*));
	tabPixelTemp=malloc((photoRGB->hauteur)*sizeof(int*));
		for (i = 0; i < photoRGB->hauteur; ++i)
		{
			tabPixel[i]=malloc(photoRGB->largeur*sizeof(int));
			tabPixelTemp[i]=malloc(photoRGB->largeur*sizeof(int));
		}





	initHSV(&HSVTab,photoRGB->hauteur,photoRGB->largeur);

	Rgb2Hsv(photoRGB, &HSVTab);
	ecran = SDL_SetVideoMode(image->w, image->h, 24, SDL_RESIZABLE); //24 bit per pixel





		for (i = 0; i < photoRGB->hauteur; ++i)
		{
			for (j = 0; j < photoRGB->largeur; ++j)
			{
				tabPixel[i][j]=0;
				tabPixelTemp[i][j]=0;
			}
		}

while (continuer)

{


 

    	// On blitte par-dessus l'écran
    SDL_BlitSurface(image, NULL, ecran, &positionFond);
    SDL_Flip(ecran);


	SDL_WaitEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
        continuer = 0;
            break;
       
       case SDL_MOUSEBUTTONDOWN:
       hSeed=event.motion.y;
       wSeed=event.motion.x;


       	for (i = 0; i < photoRGB->hauteur; ++i)
		{
			for (j = 0; j < photoRGB->largeur; ++j)
			{
				if (tabPixelTemp[i][j]==1)
				{
					tabPixel[i][j]=1;
					tabPixelTemp[i][j]=0;
				}
				
			}
		}
	
		
              printf("mousse event x=%d, y=%d\n",event.motion.x,event.motion.y );
	

		SDL_SaveBMP(image,"hemoModified.bmp");


 printf("croissance de région, veuillez patienter\n");
		croissanceRegion(hSeed,wSeed,tabPixelTemp,HSVTab);
		printf("fin de la croissance\n");




		//Conversion image => RGB
	img2Struct(photoRGB,image);

	PrintRedImg(photoRGB, tabPixelTemp);
	PrintRedImg(photoRGB, tabPixel);
	// on met l'auigine en noir
	photoRGB->red[hSeed][wSeed]=0;
	photoRGB->green[hSeed][wSeed]=0;
	photoRGB->blue[hSeed][wSeed]=0;



	RGB2IMG(photoRGB, image);
	SDL_SaveBMP(image,"testemain");
	


		  break;



       case SDL_KEYDOWN: // une touche est appuyée
        switch(event.key.keysym.sym)

        {

            case SDLK_ESCAPE: // Flèche haut

                return 0;

            break;
             case SDLK_UNKNOWN: // Flèche haut

                return 0;

            break;
            case SDLK_BACKSPACE:


            if (argc>1)
				{
					printf("%d\n",argc );
					image = IMG_Load((const char *)(argv[argc-1])); 
				}
				else
				{
					image = IMG_Load("../illustration/largeHemo.jpg"); // on pourra répéter ce traitement pour chaque image
				}
        
				for (i = 0; i < photoRGB->hauteur; ++i)
				{
					for (j = 0; j < photoRGB->largeur; ++j)
					{
						tabPixelTemp[i][j]=0;
					}
				}
				img2Struct(photoRGB,image);
				PrintRedImg(photoRGB, tabPixel);
				RGB2IMG(photoRGB, image);

            break;

        }
       
    }



}
  

continuer=1;















// for ( k= 0; k < photoRGB->hauteur; ++k)
// {
// 	for (l = 0; l < photoRGB->largeur; ++l)
// 	{
// 		//printf("hue=%lf, sat=%lf\n",HSVTab->hue[k][l],HSVTab->sat[k][l] );



//   ecran = SDL_SetVideoMode(image->w, image->h, 24, SDL_HWSURFACE); //24 bit per pixel

//     	// On blitte par-dessus l'écran
//     SDL_BlitSurface(image, NULL, ecran, &positionFond);
//     SDL_Flip(ecran);

// while (continuer)

// {
// 	SDL_WaitEvent(&event);
//     switch(event.type)
//     {
//         case SDL_QUIT:
//         continuer = 0;
//             break;
       
//        case SDL_MOUSEBUTTONDOWN:
//        printf("x=%d, y=%d\n",event.motion.x,event.motion.y );
//        hSeed=event.motion.y;
//        wSeed=event.motion.x;
//               printf("x=%d, y=%d\n",hSeed,wSeed );

//        break;
//        case SDL_KEYDOWN: // une touche est appuyée
//         switch(event.key.keysym.sym)

//         {

//             case SDLK_ESCAPE: // Flèche haut

//                 return 0;

//             break;
//         }

       
//     }
// }

// 	  if(((HSVTab->hue[k][l]>(float)SANG_MIN && HSVTab->hue[k][l]<=(float)360) || (HSVTab->hue[k][l]<(float)SANG_MAX && HSVTab->hue[k][l]>=(float)0)) && HSVTab->sat[k][l]>(float)SAT_MAX )
// 	  {
// 	    hSeed=k;
// 		wSeed=l;

	
// 		for (i = 0; i < photoRGB->hauteur; ++i)
// 		{
// 			for (j = 0; j < photoRGB->largeur; ++j)
// 			{
// 				tabPixel[i][j]=0;
// 			}
// 		}
// 	/*
// 		do{
// 		printf("Choisissez les coordonées de la seed, ces dernières doivent être compris entre %d et %d\n",image->h, image->w );
// 		printf("val de h=\n");
// 		scanf("%d",&hSeed);
// 		printf("val de w=\n");
// 		scanf("%d",&wSeed);
// 		}while(hSeed<0 || hSeed>image->h || wSeed<0 || wSeed>image->w);

// 	*/
// 		  // hSeed=836;
// 		  // wSeed=783;

// 		// hSeed=84;
// 		// wSeed=78;

// 	//	printf("calcul de la croissance des régions\n");

// 	//	nivGrisHue(HSVTab,photoRGB);
// 	//	RGB2IMG(photoRGB, image);
// 	//	SDL_SaveBMP(image,"hemoModified.bmp");



// 		croissanceRegion(hSeed,wSeed,tabPixel,HSVTab);
// 		PrintRedImg(photoRGB, tabPixel);
// 		// on met l'auigine en noir
// 		photoRGB->red[hSeed][wSeed]=0;
// 		photoRGB->green[hSeed][wSeed]=0;
// 		photoRGB->blue[hSeed][wSeed]=0;


// 	}
//   }



//detecteHemorragie(HSVTab, photoRGB, tabPixel);

nbSang=0;
for (i = 0; i < photoRGB->hauteur; ++i)
{
	for (j = 0; j < photoRGB->largeur; ++j)
	{
		nbSang+=tabPixel[i][j];
	}
}
if(nbSang>0.1* photoRGB->hauteur*photoRGB->largeur)
{
    printf("présence de réthinopathie diabétique");
}
else
{
    printf("il ne semble pas y avoir de rétinopathie diabétique");
}

SDL_SaveBMP(image,"imageModifiee.bmp");

	//On libère les surfaces
    SDL_FreeSurface(imageDeFond);
	SDL_FreeSurface(image);
	SDL_Quit();
//	freeHSV(&HSVTab);
//	freeRGB(&RGBTab);
	printf("FIN\n");
	return EXIT_SUCCESS;



}

