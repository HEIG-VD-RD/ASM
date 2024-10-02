/**
 * @file   path_finding.h
 * @author Rick Wertenbroek
 * @date   26.10.21
 *
 * @brief  Header for the path finding solver for ASM lab
 */

#ifndef __PATH_FINDING_H__
#define __PATH_FINDING_H__

typedef uint8_t tile_t;
// map[x][y] access
#define MAP_SIZE_X 8
#define MAP_SIZE_Y 10
#define MAP_MEMORY (MAP_SIZE_X*MAP_SIZE_Y*sizeof(tile_t))

#define QUEUE_CAPACITY (1024)
#define CACHE_CAPACITY (QUEUE_CAPACITY*2)

enum direction_enum {left, right, up, down};
typedef enum direction_enum direction_t;

/// @brief Pair data structure
typedef struct Pair {
    uint16_t x;
    uint16_t y;
} Pair;

typedef Pair Coordinates;

typedef tile_t map_line_t[MAP_SIZE_Y];
typedef map_line_t map_t[MAP_SIZE_X];

/// @brief Node for the A* search algorithm
struct Node;
typedef struct Node {
    const map_t *map;
    Coordinates position;
    uint32_t steps;
    struct Node *prev;
} Node;

typedef struct Path {
    uint32_t num_coords;
    Coordinates* coordinates;
} Path;

#define MAX_LEGAL_NEIGHBORS 8

/// @brief temporary structure to hold newly created neighbors
typedef struct neighbors_t {
    uint32_t num; // Number of set neighbors, between 0 and 4
    Coordinates coordinates[MAX_LEGAL_NEIGHBORS];
} neighbors_t;

/// @brief Priority queue data structure
typedef struct Queue {
    uint32_t size;
    Node *elements[QUEUE_CAPACITY];
    uint32_t priorities[QUEUE_CAPACITY];
} Queue;

/// @brief Cache entry data structure
typedef struct CacheEntry {
    uint32_t id;
    Node *node;
} CacheEntry;

/// @brief Cache data structure
typedef struct Cache {
    uint32_t size;
    CacheEntry entries[CACHE_CAPACITY];
} Cache;

typedef struct DistanceFunction {
    uint32_t (*f)(const Coordinates position, const Coordinates goal);
    const char *name;
} DistanceFunction;

// C versions of the functions
uint32_t manhattan_distance(const Coordinates position, const Coordinates target);
uint32_t euclid_distance(const Coordinates position, const Coordinates target);
uint32_t hamming_distance(const Coordinates position, const Coordinates target);
neighbors_t get_neighbors_simple(const map_t map, const Coordinates position);
neighbors_t get_legal_neighbors_4(const map_t map, const Coordinates position);
neighbors_t get_legal_neighbors_8(const map_t map, const Coordinates position);

// Student functions
extern uint32_t manhattan_distance_asm(const uint32_t a, const uint32_t b);
extern uint32_t ssd_semi_distance_asm(const uint32_t a, const uint32_t b);
extern void place_wall_with_hole_x(uint8_t* map, const uint32_t x_offset, const uint32_t y_hole_pos);
extern void place_wall_with_hole_y(uint8_t* map, const uint32_t y_offset, const uint32_t x_hole_pos);

#endif /* __PATH_FINDING_H__ */