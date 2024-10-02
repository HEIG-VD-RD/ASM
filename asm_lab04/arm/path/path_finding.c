/**
 * @file   path_finding.c
 * @author Rick Wertenbroek
 * @date   26.10.21
 *
 * @brief  Simple path finding A* solver for ASM lab
 */

#ifndef __QEMU_BARE__
#define __QEMU_BARE__ 0
#endif

#if __QEMU_BARE__
//#   warning "Compiling for QEMU Bare Metal (U-Boot)"
#   include <common.h>
#   include <exports.h>
#   define calloc(X, Y) (calloc_custom((X), (Y)))

    void *calloc_custom(unsigned int x, unsigned int y) {
        uint8_t *ptr = malloc(x*y);
        for(unsigned int i = 0; i < x*y; ++i) {
            *(ptr+i) = 0;
        }
        return (void *)ptr;
    }

    void memcpy_custom(uint8_t *dest, const uint8_t *src, unsigned int num) {
        while(num--) {
            *dest++ = *src++;
        }
    }
#   define memcpy memcpy_custom
#else
#   warning "Compiling for host"
#   include <stdint.h>
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
#endif

#include "path_finding.h"
#include "path.h"

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define ABSDIFF(X, Y) (MAX(X,Y) - MIN(X,Y))

// Wrapper for the student asm function
neighbors_t get_directions_student(const map_t map, const Coordinates position) {
    uint32_t pos = (((uint32_t)position.x) << 16) | position.y;
    uint32_t result_asm = get_directions_asm((const char*) &map[0][0], pos);
    neighbors_t result;
    result.num = 0;

    Coordinates north = {position.x - 1, position.y};
    Coordinates south = {position.x + 1, position.y};
    Coordinates east  = {position.x, position.y + 1};
    Coordinates west  = {position.x, position.y - 1};

    if (result_asm & 0x8) {
        result.coordinates[result.num++] = north;
    }
    if (result_asm & 0x4) {
        result.coordinates[result.num++] = south;
    }
    if (result_asm & 0x2) {
        result.coordinates[result.num++] = east;
    }
    if (result_asm & 0x1) {
        result.coordinates[result.num++] = west;
    }

    return result;
}

//////////////////////////////
// Global function pointers //
//////////////////////////////
uint32_t (*distance_function)(const Coordinates position, const Coordinates goal) = manhattan_distance;
#ifdef NEIGHBORS_STUDENT
neighbors_t (*get_legal_neighbors)(const map_t map, const Coordinates position) = get_directions_student;
#else
neighbors_t (*get_legal_neighbors)(const map_t map, const Coordinates position) = get_neighbors_simple;
#endif

/// @brief Prompts for a key to continue
void prompt_continue(void) {
#if __QEMU_BARE__
	printf("Hit any key to continue ... ");
	while (!tstc());
	/* consume input */
	(void) getc();
	printf("\n\n");
#else
    // Do Nothing
#endif
}

// Node functions
///////////////////

Coordinates find_player(const map_t map);
uint32_t get_position_id(const Coordinates position);

/// @brief Allocates an initial (no prev) node from a given puzzle
Node *node_from_initial_map(const map_t *map) {
    Node *node = malloc(sizeof(struct Node));
    node->map = map;
    node->position = find_player(*map); // Start
    printf("Initial position %d, %d\n", node->position.x, node->position.y);
    node->steps = 0; // Start
    node->prev = NULL; // No previous

    return node;
}

/// @brief Deallocates a node
void deallocate_node(Node* node) {
    free((void *)node);
}

// Solver Cache / Dynamic memory management
/////////////////////////////////////////////

/// @brief Slow O(n) insert // stupid lookup
uint8_t cache_lookup(Node* node, Cache* cache) {
    uint32_t id = get_position_id(node->position);
    for (uint32_t i = 0; i < cache->size; ++i) {
        if (cache->entries[i].id == id) {
            // Free on hit so we don't keep duplicates
            deallocate_node(node);
            return 1; // Cache hit
        }
    }

    if (cache->size == CACHE_CAPACITY) {
        printf("Cache is full\n");
        for(;;);
        return 0;
    }

    // Insertion
    // Here we redo the work, but who cares
    uint32_t i = cache->size;
    while((i > 0) && (id < cache->entries[i-1].id)) {
        cache->entries[i] = cache->entries[i-1];
        i--;
    }
    cache->entries[i].node = node;
    cache->entries[i].id = id;
    cache->size++;

    return 0;
}

/// @brief Clears the cache and deallocates every cache hit
void clear_cache(Cache* cache) {
    for (uint32_t i = 0; i < cache->size; ++i) {
        deallocate_node(cache->entries[i].node);
    }
    cache->size = 0;
}

// Solver priority queue functions
////////////////////////////////////

/// @brief prints the contents of the queue (for debug)
void print_queue(Queue* queue) {
    printf("---\n");
    for (uint32_t i = 0; i < queue->size; ++i) {
        printf("element 0x%08x prio : %d\n", get_position_id(queue->elements[i]->position), (unsigned int)queue->priorities[i]);
    }
}

/// @brief clears the queue
void clear_queue(Queue* queue) {
    queue->size = 0;
}

/// @brief adds element to the queue - Enqueue O(N)
void enqueue(Node *ptr, uint32_t priority, Queue* queue) {
    if (queue->size == QUEUE_CAPACITY) {
        printf("Queue is full, element dropped\n");
        for(;;);
        return;
    }

    // Lowest priority last (highest priority first)
    uint32_t i = queue->size;
    while((i > 0) && (priority > queue->priorities[i-1])) {
        queue->elements[i] = queue->elements[i-1];
        queue->priorities[i] = queue->priorities[i-1];
        i--;
    }
    queue->elements[i] = ptr;
    queue->priorities[i] = priority;
    queue->size++;
}

/// @brief Get lowest priority node - Dequeue O(1)
Node *dequeue(Queue* queue) {
    if (queue->size == 0) {
        printf("Empty queue, returning NULL pointer\n");
        return NULL;
    }

    return queue->elements[--(queue->size)];
}

// Other functions
////////////////////

/// @brief Get unique id from position
uint32_t get_position_id(const Coordinates position) {
    uint32_t id = position.y;
    id |= position.x << (sizeof(position.y)*8);
    return id;
}

/// @brief Prints a map
uint32_t print_map_with_additions(const map_t map, const Coordinates* position, const Coordinates* target) {
    uint32_t printed_lines = 0;
    printf("+-");
    for (uint32_t i = 0; i < MAP_SIZE_Y; ++i) {
        printf("-");
    }
    printf("> Y\n"); printed_lines++;
    for (uint32_t i = 0; i < MAP_SIZE_X; ++i) {
        printf("| ");
        for (uint32_t j = 0; j < MAP_SIZE_Y; ++j) {
            if (target && (i == target->x) && (j == target->y)) {
                printf("@"); // Target
            } else if (position && (i == position->x) && (j == position->y)) {
                printf("p"); // "Player p"
            } else {
                printf("%c", map[i][j]);
            }
        }
        printf("\n"); printed_lines++;
    }
    printf("v\nX\n"); printed_lines+=2;

    return printed_lines;
}

/// @brief Prints a map
uint32_t print_distance_map(const Coordinates target, uint32_t (*d)(const Coordinates, const Coordinates), uint16_t sol_6_5) {
    uint32_t printed_lines = 0;
    printf("+-");
    for (uint32_t i = 0; i < MAP_SIZE_Y; ++i) {
        printf("---");
    }
    printf("> Y\n"); printed_lines++;
    for (uint32_t i = 0; i < MAP_SIZE_X; ++i) {
        printf("| ");
        for (uint32_t j = 0; j < MAP_SIZE_Y; ++j) {
            if ((i == target.x) && (j == target.y)) {
                printf("  @"); // Target
            } else {
                Coordinates pos = {i,j};
                uint32_t distance = d(pos, target);
                printf("%3d", distance);
            }
        }
        printf("\n"); printed_lines++;
    }
    printf("v\nX   Distance of @ with itself is : %d (expected to be 0)\n", d(target, target)); printed_lines+=2;

    const Coordinates c = {6, 5};
    printf("v\nX   Distance of @ with (6, 5) is : %d (expected to be %d)\n", d(target, c), sol_6_5); printed_lines+=2;

    return printed_lines;
}

void print_map(const map_t map) {
    print_map_with_additions(map, (Coordinates *)NULL, (Coordinates *)NULL);
}

uint8_t on_map(Coordinates position) {
    if (position.x >= MAP_SIZE_X) return 0;
    if (position.y >= MAP_SIZE_Y) return 0;
    return 1;
}

void place_on_map(map_t map, Coordinates position, tile_t thing) {
    if (!on_map(position)) return;
    map[position.x][position.y] = thing;
}

#define P_AS_UINT32_T(P) ((((uint32_t) P.x) << 16) | P.y)

uint32_t discrete_distance(const Coordinates position, const Coordinates target) {
    return discrete_distance_asm(P_AS_UINT32_T(position), P_AS_UINT32_T(target));
}

uint32_t hamming_distance(const Coordinates position, const Coordinates target) {
    return hamming_distance_asm(P_AS_UINT32_T(position), P_AS_UINT32_T(target));
}

//uint32_t euclid_distance(const Coordinates position, const Coordinates target) {
//    uint32_t d1 = ABSDIFF(position.x, target.x);
//    uint32_t d2 = ABSDIFF(position.y, target.y);
//    return 10.0*sqrt(d1*d1+d2*d2); // 10x denormalization (because integer result)
//}

//uint32_t crippled_euclid_distance(const Coordinates position, const Coordinates target) {
//    uint32_t d1 = ABSDIFF(position.x, target.x);
//    uint32_t d2 = ABSDIFF(position.y, target.y);
//    return sqrt(d1*d1+d2*d2); // Here doubles are rounded down !
//}

// Sum of squares of differences semi distance
uint32_t ssd_semi_distance(const Coordinates position, const Coordinates target) {
    return ssd_semi_distance_asm(P_AS_UINT32_T(position), P_AS_UINT32_T(target));
}

uint32_t manhattan_distance(const Coordinates position, const Coordinates target) {
    return manhattan_distance_asm(P_AS_UINT32_T(position), P_AS_UINT32_T(target));
}

/// @brief The goal is reached if the distance to the goal is zero (any distance will do)
uint8_t is_goal(const Coordinates position, const Coordinates goal) {
    return (distance_function(position, goal) == 0);
}

/// @brief find the thing in a map, return 0,0 if not found
Coordinates find(const map_t map, const tile_t thing) {
    for (uint32_t i = 0; i < MAP_SIZE_X; ++i) {
        for (uint32_t j = 0; j < MAP_SIZE_Y; ++j) {
            if (map[i][j] == thing) {
                Coordinates result = {i,j};
                return result;
            }
        }
    }
    Coordinates result = {0,0};
    return result; // Default value
}

/// @brief find the goal in a map, return 0,0 if not found
Coordinates find_goal(const map_t map) {
    return find(map, GOAL);
}

/// @brief find the player in a map, return 0,0 if not found
Coordinates find_player(const map_t map) {
    return find(map, PLAYER);
}


/// @brief A standard abba swap
void swap_c(tile_t *a, tile_t* b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

/// @brief Checks if two coordinates are neighbors (N;E;S;W)
uint8_t is_neighbor(const Coordinates position_1, const Coordinates position_2) {
    return (manhattan_distance(position_1, position_2) == 1);
}

uint8_t legal_position(const map_t map, const Coordinates position) {
    // Cannot go past the map boundaries
    if (position.x >= MAP_SIZE_X) return 0;
    if (position.y >= MAP_SIZE_Y) return 0;
    // Cannot be on wall
    if (map[position.x][position.y] == WALL) return 0;

    return 1;
}

/// @brief Checks if a move is legal
uint8_t legal_move(const map_t map, const Coordinates player, const Coordinates new_position) {
    // Cannot jump more than one tile
    if (!is_neighbor(new_position, player)) return 0;
    if (!legal_position(map, new_position)) return 0;

    return 1;
}

void move_player(map_t map, const Coordinates position, const Coordinates new_position) {
    // Move must be legal
    if (legal_move(map, position, new_position)) {
        map[new_position.x][new_position.y] = PLAYER; // Player moved
        map[position.x][position.y] = EMPTY; // Space is now empty
    }
}

void move_player_in_direction(map_t map, direction_t direction) {
    Coordinates player_pos = find_player(map);
    Coordinates new_pos = player_pos;

    /// @todo check directions
    switch (direction)
    {
    case left:
        new_pos.y -= 1;
        break;
    case right:
        new_pos.y += 1;
        break;
    case up:
        new_pos.x += 1;
        break;
    case down:
        new_pos.x -= 1;
        break;

    default:
        break;
    }

    move_player(map, player_pos, new_pos);
}

// Does not handle terrain
neighbors_t get_neighbors_simple(const map_t map, const Coordinates position) {
    neighbors_t result;
    result.num = 0;

    Coordinates left  = {position.x, position.y - 1};
    Coordinates right = {position.x, position.y + 1};
    Coordinates up    = {position.x - 1, position.y};
    Coordinates down  = {position.x + 1, position.y};

    if (position.y) {
        result.coordinates[result.num++] = left;
    }
    if (position.y+1 < MAP_SIZE_Y) {
        result.coordinates[result.num++] = right;
    }
    if (position.x) {
        result.coordinates[result.num++] = up;
    }
    if (position.x+1 < MAP_SIZE_X) {
        result.coordinates[result.num++] = down;
    }
    return result;
}

// 0,1,2,3, or 4 neighbors
neighbors_t get_legal_neighbors_4(const map_t map, const Coordinates position) {
    neighbors_t result;

    result.num = 0;

    Coordinates left  = {position.x, position.y - 1};
    Coordinates right = {position.x, position.y + 1};
    Coordinates up    = {position.x - 1, position.y};
    Coordinates down  = {position.x + 1, position.y};

    if (legal_position(map, left)) {
        result.coordinates[result.num++] = left;
    }
    if (legal_position(map, right)) {
        result.coordinates[result.num++] = right;
    }
    if (legal_position(map, up)) {
        result.coordinates[result.num++] = up;
    }
    if (legal_position(map, down)) {
        result.coordinates[result.num++] = down;
    }

    return result;
}

// 0,1,2,3, ..., or 8 neighbors
neighbors_t get_legal_neighbors_8(const map_t map, const Coordinates position) {
    neighbors_t result = get_legal_neighbors_4(map, position);

    Coordinates ne = {position.x - 1, position.y + 1};
    Coordinates nw = {position.x - 1, position.y - 1};
    Coordinates se = {position.x + 1, position.y + 1};
    Coordinates sw = {position.x + 1, position.y - 1};

    if (legal_position(map, ne)) {
        result.coordinates[result.num++] = ne;
    }
    if (legal_position(map, se)) {
        result.coordinates[result.num++] = se;
    }
    if (legal_position(map, sw)) {
        result.coordinates[result.num++] = sw;
    }
    if (legal_position(map, nw)) {
        result.coordinates[result.num++] = nw;
    }

    return result;
}

uint8_t connecting_symbol(const Coordinates prev, const Coordinates next) {
    if (prev.x == next.x) return '-';
    if (prev.y == next.y) return '|';
    if (get_legal_neighbors == get_legal_neighbors_8) {
        Coordinates _ = {MAX(prev.x, next.x), MAX(prev.y, next.y)};
        if ((_.x == prev.x && _.y == prev.y) || (_.x == next.x && _.y == next.y)) return '\\';
        return '/';
    }
    return '+';
}

void print_path(const Path path) {
    for (uint32_t i = 0; i < path.num_coords; ++i) {
        printf("(%u,%u)\n", path.coordinates[i].x, path.coordinates[i].y);
    }
}

// This goes through the previous nodes until first node (prev = NULL) has been reached and prints them in reverse order (i.e., from initial position to solution)
/// @brief Prints the path to a given node based on the previous nodes (used to show path to solution)
void print_path_to_node(Node *solution_node) {
    uint32_t num_steps = 0;

    map_t solution_map;
    //printf("Size of map_t : %d\n", sizeof(map_t));
    memcpy((uint8_t *)&solution_map, (uint8_t *)solution_node->map, sizeof(map_t));

    Node *node = solution_node;
    while (node->prev) {
        if (node->prev->prev) {
            //place_on_map(solution_map, node->prev->position, PATH);
            place_on_map(solution_map, node->prev->position, connecting_symbol(node->position, node->prev->prev->position));
        }
        num_steps++;
        node = node->prev;
    }

    print_map(solution_map);
}

// A* search for a solution
/// @brief Does a single step towards a solution (A* search step)
Node *a_star_search_step(Queue *queue, Cache *cache, const Coordinates goal) {
    //print_queue(queue);

    Node *working_node = dequeue(queue);

    if (working_node) {
        if (is_goal(working_node->position, goal)) {
            // Goal reached !
            return working_node;
        }

        // Get the legal neighbors, between 0 and 4
        neighbors_t neighbors = get_legal_neighbors(*working_node->map, working_node->position);

        //printf("neighbors of position %08x\n", get_position_id(working_node->position));
        for (uint32_t i = 0; i < neighbors.num; ++i) {
            uint32_t distance = distance_function(neighbors.coordinates[i], goal);
            Node *node = malloc(sizeof(struct Node));
            node->map = working_node->map;
            node->position = neighbors.coordinates[i];
            //printf("n%d, %08x\n", i, get_position_id(node->position));
            node->steps = working_node->steps+1;
            node->prev = working_node;

            // Don't queue positions that have already been seen
            if (!cache_lookup(node, cache)) {
                //printf("e%d, %08x\n", i, get_position_id(node->position));
                // If "Miss" enqueue (the cache deallocates on "Hit" in order to free duplicates)
                enqueue(node, distance + node->steps, queue);
            }
        }
    } else {
        return (Node *)-1;
    }

    // Goal not yet reached
    return NULL;
}

// A* search in the map space
/// @brief Searches for a solution for a given map and goal doing an A* search
uint32_t a_star_search(const map_t *map) {
    //uint32_t correct = 1;
    uint32_t a_star_search_steps = 0;

    // Data structures
    Cache *cache = calloc(sizeof(Cache), 1);
    Queue *queue = calloc(sizeof(Queue), 1);

    Node *initial_position = node_from_initial_map(map);

    Coordinates goal = find_goal(*map);

    // Enqueue inital position
    enqueue(initial_position, distance_function(initial_position->position, goal) + initial_position->steps, queue);
    cache_lookup(initial_position, cache);

    // Launch the solver
    for (;;) {
        Node* solution = NULL;
        solution = a_star_search_step(queue, cache, goal);
        if (solution == (Node *)-1) {
            printf("Search space exhausted... no path found\n");
            //correct = 0;
            break;
        } else if (solution) {
            printf("Solution found !\n");
            printf("Number of steps = %u\n", solution->steps);
            print_path_to_node(solution);

            if (!is_goal(solution->position, goal)) {
                printf("The solution found is incorrect !\n");
                //correct = 0;
            }
            break;
        }
        a_star_search_steps++;
    }

    // Free the memory, here clear_cache() deallocates the puzzles e.g., allocated by create_neighbors() it also deallocates the nodes.
    clear_queue(queue);
    clear_cache(cache);
    free(queue);
    free(cache);

    return a_star_search_steps;
}

#define NUM_MAPS 5
#define NUM_D_FUNS 4

/// @brief The main entrypoint of the application
int main(int argc, char *argv[]) {
    int err = 0;

#ifdef PRINT
#else
    const map_t *maps[NUM_MAPS] = {&map, &prison, &prison_break, &map, &labyrinth};
#endif

    const DistanceFunction distance_functions[NUM_D_FUNS] = {
        //{euclid_distance, "Euclidian distance"},
        {discrete_distance, "Discrete distance"},
        {hamming_distance, "Hamming distance"},
        {manhattan_distance, "Manhattan distance"},
        {ssd_semi_distance, "Sum of Squared Differences distance"}
    };

    // For every distance
    for (uint32_t d = 0; d < NUM_D_FUNS; ++d) {
        printf("\n-----------------\n");
        printf("Distance used is : %s\n", distance_functions[d].name);
        distance_function = distance_functions[d].f;

#ifdef PRINT
        const uint16_t sol_6_5[NUM_D_FUNS] = {
        		1, 2, 7, 25
        };
        Coordinates myTarget = {2,2};
        print_distance_map(myTarget, distance_functions[d].f, sol_6_5[d]);
#else
        // Show map
        for (uint32_t i = 0; i < NUM_MAPS; ++i) {
            const map_t *current_map = maps[i];
            printf("Map is : \n");
            print_map(*current_map);
            uint32_t search_steps = a_star_search(current_map);
            printf("With %u A* search steps\n", search_steps);
        }
#endif
        printf("\nResults above are with %s\n", distance_functions[d].name);
        prompt_continue();
    }

//exit:
#if __QEMU_BARE__
	printf("Hit any key to exit ... ");
	while (!tstc());

	/* consume input */
	(void) getc();

	printf("\n\n");
#endif

	return err;
}

#ifdef __QEMU_BARE__
#undef calloc
#undef memcpy
#endif
#undef MIN
#undef MAX
