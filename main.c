#include <stdio.h>

#include "tga.h"
#include "model.h"

#define roundf(x) floor(x+0.5f)


void swap(int *a, int *b);
int abs(int a);
int height = 800;
int width = 800;

/*
* Using Digital Differential Analyzer algorihm
* to draw interval connecting (x0, y0) with (x1, y1)
* on image using color
*/
void line (tgaImage *image, 
           int x0, int y0,
           int x1, int y1,
           tgaColor color);


int max (int a, int b)
{
    if (a > b)
    {   return a;   } 
    else 
    {   return b;   }
}

void meshgrid (tgaImage *image, Model *model)
{
    int i,j;
    tgaColor white = tgaRGB (255,255,255);
    for (i=0; i < model->nface; ++i)
    {
        int screen_coords[3][2];
        for (j=0; j<3; ++j)
        {
            Vec3 *v = &(model -> vertices[model->faces[i][3*j]]);
            screen_coords[j][0] = ((*v)[0]+1) * image->width/2;
            screen_coords[j][1] = (1-(*v)[1]) * image->height/2;
        }
        for (j=0; j<3; ++j)
        {
            line (image, screen_coords[j][0], screen_coords[j][1], screen_coords[(j+1)%3][0], screen_coords[(j+1)%3][1], white);
        }
    }
}


int main(int argc, char **argv)
{
   
    int rv = 0;
    if (argc < 3) {
        fprintf(stderr, "Usage: %s outfile objfile\n", argv[0]);
        return -1;
    }
    Model * model = loadFromObj(argv[2]);
    tgaImage * image = tgaNewImage(height, width, RGB);

//    line(image, 100, 50, 20, 30, tgaRGB(255, 255, 255));
//    line(image, 110, 50, 150, 110, tgaRGB(255, 255, 255));
    meshgrid(image, model); 
    if (-1 == tgaSaveToFile(image, argv[1])) {
        perror("tgaSateToFile");
        rv = -1;
    }

    tgaFreeImage(image);  
    freeModel(model);  
    return rv;
}

void line (tgaImage *image, int x0, int y0,
           int x1, int y1, tgaColor color)

{
    int steep = 0;
    if (abs(y1 - y0) > abs(x1 - x0))
    {
        steep = 1;
        swap(&x0, &y0);
        swap(&x1, &y1);
    }

    if (x0 > x1)  {  swap(&x0, &x1);  swap(&y0, &y1);  }

    int x;
	int dx = x1-x0;
	int dy = y1-y0;
	int e = abs(dy)*2;
	int e2=0;
	int y;
//    double k = ((double)(y1 - y0))/(x1 - x0);  
 
    for (x = x0, y = y0; x <= x1; x++)
    {
        if (steep) {  tgaSetPixel(image, y, x, color);  } 
        else       {  tgaSetPixel(image, x, y, color);  }
    	e2 += e;
    	if  (e2 > dx)
       {
            y += (y1 > y0 ? 1:-1);
	        e2 -= dx*2;   
       }
    }
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int abs(int a) {
    return (a >= 0) ? a : -a;
}