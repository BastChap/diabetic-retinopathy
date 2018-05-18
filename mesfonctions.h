#include <dirent.h> 

#define TOLERANCE_CARRE 50
#define SANG_MIN 320 // nombre de hue de sang enregistrées comme tel
#define SANG_MAX 2
#define SAT_MAX 0.6
#define SEUIL_MINMAX 0.08
#define IMPORTANCE_SAT 0.3 // hue a un rapport de 1

//RGB 1 image
struct rgbImg
{
    Uint8 **red;
    Uint8 **green;
    Uint8 **blue;
    
    int hauteur;      // nombre de lignes
    int largeur;      // nombre de colonnes
}rgbImg;

//HSV
struct hsv
{
    float **hue;	// teinte [0:360]
    float **sat;	//saturation [0:1]
    float **value;   //valeur [0:1]
    
                      // nombre de photos
    int hauteur;      // nombre de lignes
    int largeur;      // nombre de colonnes par lignes
}hsv;


//RGB
struct rgb
{
    float ***red;   // teinte [0:360[
    float ***green;   //saturation [0:1]
    float ***blue;   //valeur [0:1]
    
    int temps;        // nombre de photos
    int hauteur;      // nombre de lignes
    int largeur;      // nombre de colonnes par lignes
}rgb;




void pauseProg();

Uint32 GetPixel(SDL_Surface *surface, int x, int y);

// initialisation et liberation des structures
void freeRgbImg(struct rgbImg **img);
void initRgbImg(struct rgbImg **img, int L, int l);

void freeHSV(struct hsv **img);
void initHSV(struct hsv **ptrHsv, int h, int l);

void freeRGB(struct rgb **img);
void initRGB(struct rgb **ptrRGB, int time, int h, int l);



void img2Struct(struct rgbImg *strImg, SDL_Surface * img);
void img2StructTab(struct rgb *strImg, SDL_Surface *GrainDB, int numImage);

void Rgb2Hsv(struct rgbImg * RGB, struct hsv ** HSV);

void RGB2IMG(struct rgbImg *, SDL_Surface *);
//print
void PrintRedImg(struct rgbImg * RGB,int** imgTamp);



/////////////////////////////////////////////////////////////////////////

float calculDistanceCouleur(int h1,int w1,int h2,int w2, struct hsv *HSVTab);

void croissanceRegion(int hSeed,int wSeed,int **tabPixel,struct hsv *HSVTab);

void nivGrisHue(struct hsv *HSVTab, struct rgbImg *photoRGB);

void detecteHemorragie(struct hsv *HSVTab, struct rgbImg * photoRGB,int **tabPixel);

void detecteHemorragieBinaryTab(int **tabPixel, int hauteur, int largeur, int tailleFiltre);

int concluHemo(int **tabPixel);




