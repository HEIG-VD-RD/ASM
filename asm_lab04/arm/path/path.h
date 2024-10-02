/**
 * @file   path.h
 * @author Rick Wertenbroek
 * @date   26.10.21
 *
 * @brief  Paths
 */

#include "path_finding.h"

#ifndef __PATH_H__
#define __PATH_H__

#define PATH '*'
#define GOAL '@'
#define PLAYER 'p'
#define EMPTY '.'
#define WALL 'W'

// You can try out other maps by changing the values below
const map_t map =
    {{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', 'W', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', 'W', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', 'W', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', 'W', '.', '.', '.', '.', '@', '.', '.'},
     {'p', '.', 'W', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', 'W', '.', '.', '.', '.', '.', '.', '.'}};

// Don't change the prison maps
const map_t prison =
    {{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', 'W', 'W', 'W', 'W', '.', '.', '.', '.'},
     {'.', '.', 'W', '.', '.', 'W', '.', '.', '.', '.'},
     {'.', '.', 'W', '.', '.', 'W', '.', '.', '.', '.'},
     {'.', '.', 'W', '.', 'p', 'W', '.', '@', '.', '.'},
     {'.', '.', 'W', '.', '.', 'W', '.', '.', '.', '.'},
     {'.', '.', 'W', '.', '.', 'W', '.', '.', '.', '.'}};

const map_t prison_break =
    {{'@', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', 'W', 'W', 'W', 'W', '.', '.', '.', '.'},
     {'.', '.', 'W', '.', '.', 'W', '.', '.', '.', '.'},
     {'.', '.', 'W', '.', '.', 'W', '.', '.', '.', '.'},
     {'.', '.', '.', '.', 'p', 'W', '.', '.', '.', '.'},
     {'.', '.', 'W', '.', '.', 'W', '.', '.', '.', '.'},
     {'.', '.', 'W', '.', '.', 'W', '.', '.', '.', '.'}};

const map_t labyrinth =
    {{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
     {'W', 'W', '.', 'W', 'W', 'W', 'W', '.', '.', '.'},
     {'.', '.', '.', 'W', '.', 'W', '.', '.', '.', '.'},
     {'.', 'W', 'W', 'W', '.', 'W', '.', '.', '.', '.'},
     {'.', '.', '.', '.', '.', 'W', '.', 'W', '.', '.'},
     {'W', '.', 'W', 'W', 'p', 'W', '.', '.', '.', '.'},
     {'.', '.', '.', 'W', 'W', 'W', 'W', '.', '.', '.'},
     {'.', 'W', '.', 'W', '@', '.', '.', '.', '.', '.'}};

const map_t goal =
    {{'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', '.', '.', '.', '.', '.', '@', '.', '.'},
     {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
     {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.'}};

#endif /* __PATH_H__ */