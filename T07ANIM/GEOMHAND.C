/* FILENAME: GEOMHAND.C
 * PROGRAMMER: AS4
 * PURPOSE: Geometry object handle functions.
 * LAST UPDATE: 14.06.2014
 */

#include <stdlib.h>

#include "anim.h"
#include "GEOM.H"
#include "SHADER.H"

/* Функция добавления материала к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       as4GEOM *G;
 *   - добавляемый материал:
 *       as4MATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного материала или -1
 *         при ошибке.
 */
INT AS4_GeomAddMaterial( as4GEOM *G, as4MATERIAL *Mtl )
{
  INT i;
  as4MATERIAL *M;

  /* ищем материал в библиотеке */
  for (i = 0; i < G->NumOfMtls; i++)
    if (strcmp(G->Mtls[i].Name, Mtl->Name)== 0)
    {
      G->Mtls[i] = *Mtl;
      return i;
    }

  if ((M = malloc(sizeof(as4MATERIAL) * (G->NumOfMtls + 1))) == NULL)
    return -1;
  if (G->Mtls != NULL)
  {
    /* копируем старые данные */
    memcpy(M, G->Mtls, sizeof(as4MATERIAL) * G->NumOfMtls);
    /* освобождаем память */
    free(G->Mtls);
  }
  /* добавляем */
  G->Mtls = M;
  M[G->NumOfMtls] = *Mtl;
  return G->NumOfMtls++;
} /* End of 'AS4_GeomAddMaterial' function */

/* Функция добавления примитива к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       as4GEOM *G;
 *   - добавляемый примитив:
 *       as4PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива или -1
 *         при ошибке.
 */
INT AS4_GeomAddPrim( as4GEOM *G, as4PRIM *Prim )
{
  as4PRIM *P;

  if ((P = malloc(sizeof(as4PRIM) * (G->NumOfPrims + 1))) == NULL)
    return -1;
  if (G->Prims != NULL)
  {
    /* копируем старые данные */
    memcpy(P, G->Prims, sizeof(as4PRIM) * G->NumOfPrims);
    /* освобождаем память */
    free(G->Prims);
  }
  /* добавляем */
  G->Prims = P;
  P[G->NumOfPrims] = *Prim;
  return G->NumOfPrims++;
} /* End of 'AS4_GeomAddPrim' function */

/* Функция освобождения */
VOID AS4_GeomFree( as4GEOM *G )
{
  INT i;

  for (i = 0; i < G->NumOfPrims; i++)
    AS4_PrimFree(G->Prims + i);
  free(G->Prims);
  for (i = 0; i < G->NumOfMtls; i++)
    glDeleteTextures(1, &G->Mtls[i].TexNo);
  free(G->Mtls);
  memset(G, 0, sizeof(as4GEOM));
} /* End of 'AS4_GeomFree' function */

/* Функция рисования */
VOID AS4_GeomDraw( as4GEOM *G )
{
  INT i, loc;
  MATR WVP, MatrWorldInvTrans;
  VEC V;

  /* вычислили матрицы преобразования */
  WVP = MatrMulMatr(AS4_Anim.RndMatrWorld,
    MatrMulMatr(AS4_Anim.RndMatrView, AS4_Anim.RndMatrProjection));
  MatrWorldInvTrans = MatrTranspose(MatrInverse(AS4_Anim.RndMatrWorld));

  /* выбор программы шейдеров вывода примитивов */
  glUseProgram(AS4_ShaderProg);
  loc = glGetUniformLocation(AS4_ShaderProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, WVP.A[0]);

  loc = glGetUniformLocation(AS4_ShaderProg, "MatrWorldInverseTranspose");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, MatrWorldInvTrans.A[0]);

  loc = glGetUniformLocation(AS4_ShaderProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AS4_Anim.RndMatrWorld.A[0]);

  loc = glGetUniformLocation(AS4_ShaderProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AS4_Anim.RndMatrView.A[0]);

  loc = glGetUniformLocation(AS4_ShaderProg, "Disc");
  if (loc != -1)
  {
    V = VecSet(AS4_Anim.Disc.x, AS4_Anim.Disc.y, AS4_Anim.Disc.z);
    glUniform3fv(loc, 1, &V.x);
  }

  loc = glGetUniformLocation(AS4_ShaderProg, "Len");
  if (loc != -1)
    glUniform3fv(loc, 1, &G->Len.x);

  loc = glGetUniformLocation(AS4_ShaderProg, "Time");
  if (loc != -1)
    glUniform1f(loc, AS4_Anim.Time);

  loc = glGetUniformLocation(AS4_ShaderProg, "ViewDir");
  if (loc != -1)
  {
    V = VecSet(-AS4_Anim.RndMatrView.A[0][2],
             -AS4_Anim.RndMatrView.A[1][2],
             -AS4_Anim.RndMatrView.A[2][2]);
    glUniform3fv(loc, 1, &V.x);
  }


  for (i = 0; i < G->NumOfPrims; i++)
  {
    INT mtl = G->Prims[i].Mtl;

    /* подготавливаем материал */
    if (G->Mtls != NULL)
    {
      INT loc;

      if (G->Mtls[mtl].TexNo == 0 && G->Mtls[mtl].MapD[0] != 0)
      {
        INT j;
        IMAGE Img;
                       
        ImageLoad(&Img, G->Mtls[mtl].MapD);

        /* получаем свободный номер текстуры */
        glGenTextures(1, &G->Mtls[mtl].TexNo);
        /* делаем ее активной */
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        for (j = 0; j < Img.W * Img.H; j++)
          Img.Bits[j] |= 0xFF000000;
        /* отправляем картинку в видеопамять */
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
          GL_BGRA_EXT, GL_UNSIGNED_BYTE, Img.Bits);

        /* Параметры вывода */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
      }
      /* передаем параметры */
      if (G->Mtls[mtl].TexNo != 0)
      {
        loc = glGetUniformLocation(AS4_ShaderProg, "DrawTexture");
        if (loc != -1)
          glUniform1i(loc, 0);
        /*glEnable(GL_TEXTURE_2D);*/
        /*glActiveTexture(GL_TEXTURE0);*/
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        /*glActiveTexture(GL_TEXTURE1);*/
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
      }
      loc = glGetUniformLocation(AS4_ShaderProg, "Ka");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ka.x);
      loc = glGetUniformLocation(AS4_ShaderProg, "Kd");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Kd.x);
      loc = glGetUniformLocation(AS4_ShaderProg, "Ks");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ks.x);
      loc = glGetUniformLocation(AS4_ShaderProg, "Phong");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Phong);
      loc = glGetUniformLocation(AS4_ShaderProg, "Trans");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Trans);
    }
    AS4_PrimDraw(G->Prims + i);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
} /* End of 'AS4_GeomDraw' function */

/* END OF 'GEOMHAND.C' FILE */
