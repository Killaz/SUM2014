/* FILENAME: SHADER.H
 * PROGRAMMER: AS4
 * PURPOSE: Shader handle declaration module.
 * LAST UPDATE: 18.06.2014
 */

#ifndef __SHADER_H_
#define __SHADER_H_

#include "def.h"

/* ������� ������������� ��������� ��������� */
UINT AS4_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName );

/* ������� �������� ��������� ��������� */
VOID AS4_ShadProgClose( UINT ProgId );

#endif