/* FILENAME: IMAGE.H
 * PROGRAMMER: AS4
 * PURPOSE: Image handle declaration module.
 * LAST UPDATE: 06.06.2014
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Структура хранения изображения */
typedef struct tagIMAGE
{
  INT W, H;    /* Размер изображения */
  HBITMAP hBm; /* Дескриптор загруженного изображения */
  DWORD *Bits; /* Цвета точек */
} IMAGE;

/* Функция загрузки изображения */
BOOL ImageLoad( IMAGE *Img, CHAR *FileName );

/* Функция освобождения памяти из-под изображения */
VOID ImageFree( IMAGE *Img );

/* Функция получения цвета точки изображения */
DWORD ImageGetP( IMAGE *Img, INT X, INT Y );

#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */
