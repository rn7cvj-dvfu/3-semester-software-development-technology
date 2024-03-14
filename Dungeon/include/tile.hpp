#ifndef TILE_H
#define TILE_H

#define EMPTY_CHAR          ' '
#define FLOOR_CHAR          '.'
#define WALL_CHAR           '#'
#define PILLAR_CHAR         'O'
#define CLOSED_DOOR_CHAR    '+'
#define OPEN_DOOR_CHAR      '/'
#define SPAWNER_CHAR        '^'
#define MUT_BLOCK_CHAR      '='

class Tile {
    public:
    Tile();
    Tile(bool vis, char inC);
    bool isSeeThrough();
    bool isPassable();
    bool isMut();
    char getTileType();
    void setTileType(char in); 

    bool isVisible;

    private:
    char c;
};

#endif
