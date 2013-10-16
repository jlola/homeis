/***************************************************************************
                          LOW_types.h  -  description
                             -------------------
    begin                : Tue Jul 9 2002
    copyright            : (C) 2002 by Harald Roelle
    email                : roelle@informatik.uni-muenchen.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LOW_TYPES_H
#define LOW_TYPES_H


#include <vector> 
#include <stdint.h> 


/** Byte vector. */
typedef std::vector<uint8_t> byteVec_t;

/** 1-Wire command. */
typedef uint8_t owCommand_t;


#endif
