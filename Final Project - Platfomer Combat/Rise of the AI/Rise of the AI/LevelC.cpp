#include "LevelC.h"
#include "Utility.h"
#include "BlindingSpider.h"
#include "Gate.h"
#include "TarnishedWidow.h"

#define LEVEL_WIDTH 31
#define LEVEL_HEIGHT 23

#define LEVEL_WIDTH_P2 31
#define LEVEL_HEIGHT_P2 42

constexpr char  BLINDING_SPIDER_FILEPATH[] = "assets/Sprites/Enemy/Blinding Spider/static idle.png",
                TARNISHED_WIDOW_FILEPATH[] = "assets/Sprites/Enemy/The Tarnished Widow 188x90/The Tarnished Widow full pack.png",
                CHICKBOY_FILEPATH[] = "assets/Sprites/Chick-Boy/ChikBoyFullPack_10x14.png", // 10 * 14 spritesheet
                TILESET_FILEPATH[] = "assets/Tileset/DARK Edition Tileset No background.png", // 14 * 16 tileset
                FONT_SPRITE_FILEPATH[] = "assets/font1.png";

constexpr char  LEVEL_C_BGM_FILEPATH[] = "assets/Sound/Background/Level C/1-10 - Grandma - Destruction.mp3";

int boss_phase = 0;
GLuint map_texture_id;
GLuint text_texture_id;

unsigned int LEVELC_DATA[] =
{
    //    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30
        142, 142, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 142, 142, 142, 142, 142, 0  , 0  ,           // 0
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 1
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 2
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 3
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 4
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 5
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 6
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 7
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 8
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 9
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 10
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 11
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 12
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 13
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 14
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 15
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 155, 156, 156, 156, 156, 156, 156,           // 16
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,           // 17
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,           // 18
        142, 142, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,           // 19
        142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,           // 20
        142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,           // 21
        142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,           // 22
};

unsigned int LEVELC_DATA_P2[] =
{
    //    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30
        142, 142, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 142, 142, 142, 142, 142, 0  , 0  ,           // 0
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 1
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 2
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 3
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 4
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 5
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 6
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 7
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 8
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 9
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 10
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 11
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 0  , 0  ,           // 12
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 13
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 14
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 15
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 155, 156, 156, 156, 156, 156, 156,           // 16
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,           // 17
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,           // 18
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 128, 128, 128, 128, 128, 128, 128,           // 19
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 20
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 21
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 22
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 23
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 24
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 25
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 26
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 27
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 28
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 29
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 30
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 31
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 32
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 33
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 34
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 35
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 36
        142, 143, 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  , 141, 142, 142, 142, 142, 142, 142,           // 37
        142, 142, 128, 128, 129, 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 127, 128, 128, 142, 142, 142, 142, 142, 142, 142,           // 38
        142, 142, 142, 142, 143, 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 142,           // 39
        142, 142, 142, 142, 143, 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 142,           // 40
        142, 142, 142, 142, 143, 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 141, 142, 142, 142, 142, 142, 142, 142, 142, 142,           // 41
        142, 142, 142, 142, 142, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142,           // 42
        142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142, 142            // 43
};

LevelC::~LevelC()
{
    //write_level_data(level_info, "level_A_data.json"); // store level data
    for (Enemy* enemy : m_game_state.enemies) delete enemy;
    for (StaticAnimatedObject* object : m_game_state.objects) delete object;
    m_game_state.enemies.clear();
    m_game_state.objects.clear();
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelC::reset() {
    for (Enemy* enemy : m_game_state.enemies) delete enemy;
    for (StaticAnimatedObject* object : m_game_state.objects) delete object;
    m_game_state.enemies.clear();
    m_game_state.objects.clear();
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelC::initialise()
{
    m_game_state.curr_scene_id = 2;
    m_game_state.next_scene_id = -1;
    data_file_path = "level_C_data.json";
    level_info = load_level_data(data_file_path);
    right_view_edge = 22.0f;

    map_texture_id = Utility::load_texture(TILESET_FILEPATH);
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 14, 16);

    text_texture_id = Utility::load_texture(FONT_SPRITE_FILEPATH);

    // Code from main.cpp's initialise()
    //ChickBoy//
    GLuint player_texture_id = Utility::load_texture(CHICKBOY_FILEPATH);

    std::vector<std::vector<int>> chickboy_animation =
    {
        { 0, 1, 2, 3, 4, 5 },                                   // idle
        { 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },             // idle extra
        { 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 },             // run
        { 27, 28 },                                             // jump start
        { 29 },                                                 // jumping
        { 30, 31, 32, 33 },                                     // jump end
        //{ 27, 28, 29, 30, 31, 32, 33 },                         // jump
        { 34 },                                                 // fall start
        { 35 },                                                 // falling
        { 36, 37, 38 },                                         // landing
        //{ 34, 35, 36, 37, 38 },                                 // fall
        { 42, 43 },												// double jump start
        { 44, 45 },												// double jump roll
        { 46, 47 },												// double jump top
        { 48 },													// double jump end
        //{ 42, 43, 44, 45, 46, 47, 48 },                         // double jump
        { 52, 53, 54, 55, 56, 57 },                             // wall grab
        { 58, 59, 60, 61 },                                     // wall slide
        { 62, 63, 64, 65 },                                     // floor slide
        { 66, 67, 68, 69, 70, 71, 72 },                         // dash
        { 4, 73, 74, 75 },                                      // take damage
        { 76, 77, 78, 79, 80 },                                 // attack 1
        { 81, 82, 83, 84 },                                     // attack 2
        { 90, 91 },                                             // attack 3
        { 92, 93 },                                             // attack 3 landing
        //{ 87, 88, 89, 90, 91, 92, 93 },                         // attack 3
        { 94, 95, 96, 97 },                                     // ducking
        { 97, 98, 99, 100 },                                    // crouch idle
        { 101, 102, 103, 104, 105, 106, 107, 108, 109, 110 },    // crouch walk
        //{ 0, 1, 111, 112, 113, 114, 115, 116, 116 },            // death 1
        //{ 117, 118, 119, 120, 121, 122, 123, 124, 124 }         // death 2
        { 85, 86, 81 }                                          // attack 2 to idle
    };

    glm::vec3 gravity = glm::vec3(0.0f, -30.0f, 0.0f);

    m_game_state.player = new Character(
        player_texture_id,         // texture id
        5.0f,                      // speed
        gravity,                   // acceleration
        9.0f,                      // jumping power
        chickboy_animation,        // animation index sets
        0.0f,                      // animation time
        0,                         // current animation index
        10,                        // animation column amount
        14,                        // animation row amount
        0.5f,                      // width
        1.0f                       // height
    );

    m_game_state.player->set_position(glm::vec3(29.0f, -18.0f, 0.0f));
    //m_game_state.player->set_position(glm::vec3(15.0f, -18.0f, 0.0f));

    if (player_life) m_game_state.player->set_life(player_life);

    // ----------------- ENEMIES ----------------- //

    m_game_state.enemies.push_back(new TarnishedWidow(Utility::load_texture(TARNISHED_WIDOW_FILEPATH)));

    //GLuint blinding_spider_texture_id = Utility::load_texture(BLINDING_SPIDER_FILEPATH);
    //m_game_state.enemies.push_back(new BlindingSpider(blinding_spider_texture_id));
    //m_game_state.enemies.push_back(new BlindingSpider(blinding_spider_texture_id));
    //m_game_state.enemies.push_back(new BlindingSpider(blinding_spider_texture_id));
    //m_game_state.enemies.push_back(new BlindingSpider(blinding_spider_texture_id));

    m_game_state.enemies[0]->set_position(glm::vec3(12.0f, -18.0f, 0.0f));
    //m_game_state.enemies[1]->set_position(glm::vec3(22.0f, -18.0f, 0.0f));
    //m_game_state.enemies[2]->set_position(glm::vec3(20.0f, -18.0f, 0.0f));
    //m_game_state.enemies[3]->set_position(glm::vec3(18.0f, -18.0f, 0.0f));
    //m_game_state.enemies[4]->set_position(glm::vec3(16.0f, -18.0f, 0.0f));

    static_cast<TarnishedWidow*>(m_game_state.enemies[0])->set_current_scene(this);

    // ----------------- OBJECTS ----------------- //

    m_game_state.objects.push_back(new Gate(glm::vec3(30.0f, -17.5f, 0.0f),         // position
                                            0.0,                                    // rotation angle
                                            glm::vec3(0.0f, 1.0f, 0.0f),            // rotation direction
                                            glm::vec3(2.0f),                        // scale
                                            DEBUG_COLLISION,                        // debug check collision
                                            0));                                    // id

    m_game_state.objects.push_back(new Gate(glm::vec3(24.0f, -17.5f, 0.0f),         // position
                                            3.1415926f,                             // rotation angle
                                            glm::vec3(0.0f, 1.0f, 0.0f),            // rotation direction
                                            glm::vec3(2.0f),                        // scale
                                            DEBUG_COLLISION,                        // debug check collision
                                            1));                                    // id

    static_cast<Gate*>(m_game_state.objects[0])->set_current_state(GATE_OPEN_IDLE);
    static_cast<Gate*>(m_game_state.objects[1])->set_current_state(GATE_OPEN_IDLE);

    // ------------ DEBUG COLLISION ------------ //

    m_game_state.player->debug_mode_collision(DEBUG_COLLISION);
    m_game_state.enemies[0]->debug_mode_collision(DEBUG_COLLISION);
    //m_game_state.enemies[1]->debug_mode_collision(DEBUG_COLLISION);
    //m_game_state.enemies[2]->debug_mode_collision(DEBUG_COLLISION);
    //m_game_state.enemies[3]->debug_mode_collision(DEBUG_COLLISION);

    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_game_state.bgm = Mix_LoadMUS(LEVEL_C_BGM_FILEPATH);
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 10);

    // Character SFX
    m_game_state.jump_sfx = Mix_LoadWAV(JUMP_SFX_FILEPATH);
    m_game_state.land_sfx = Mix_LoadWAV(LAND_SFX_FILEPATH);
    m_game_state.walk_1_sfx = Mix_LoadWAV(WALK_1_SFX_FILEPATH);
    m_game_state.walk_2_sfx = Mix_LoadWAV(WALK_2_SFX_FILEPATH);
    m_game_state.dash_1_sfx = Mix_LoadWAV(DASH_1_SFX_FILEPATH);
    //m_game_state.dash_2_sfx = Mix_LoadWAV(DASH_2_SFX_FILEPATH);
    m_game_state.idle_attack_1_sfx = Mix_LoadWAV(IDLE_ATTACK_1_SFX_FILEPATH);
    m_game_state.idle_attack_2_sfx = Mix_LoadWAV(IDLE_ATTACK_2_SFX_FILEPATH);
    m_game_state.idle_attack_3_sfx = Mix_LoadWAV(IDLE_ATTACK_3_SFX_FILEPATH);
    m_game_state.dash_attack_1_sfx = Mix_LoadWAV(DASH_ATTACK_1_SFX_FILEPATH);
    m_game_state.dash_attack_2_sfx = Mix_LoadWAV(DASH_ATTACK_2_SFX_FILEPATH);
    m_game_state.jump_attack_1_sfx = Mix_LoadWAV(JUMP_ATTACK_1_SFX_FILEPATH);
    m_game_state.jump_attack_2_sfx = Mix_LoadWAV(JUMP_ATTACK_2_SFX_FILEPATH);
    m_game_state.take_damage_sfx = Mix_LoadWAV(TAKE_DAMAGE_SFX_FILEPATH);

    Mix_VolumeChunk( m_game_state.jump_sfx, MIX_MAX_VOLUME / 15 );
    Mix_VolumeChunk( m_game_state.land_sfx, MIX_MAX_VOLUME / 8 );

    Mix_VolumeChunk( m_game_state.walk_1_sfx, MIX_MAX_VOLUME );
    Mix_VolumeChunk( m_game_state.walk_2_sfx, MIX_MAX_VOLUME / 2 );

    Mix_VolumeChunk( m_game_state.dash_1_sfx, MIX_MAX_VOLUME / 10 );
    //Mix_VolumeChunk( m_game_state.dash_2_sfx, MIX_MAX_VOLUME / 10 );

    Mix_VolumeChunk( m_game_state.idle_attack_1_sfx, MIX_MAX_VOLUME / 20 );
    Mix_VolumeChunk( m_game_state.idle_attack_2_sfx, MIX_MAX_VOLUME / 20 );
    Mix_VolumeChunk( m_game_state.idle_attack_3_sfx, MIX_MAX_VOLUME / 20 );

    Mix_VolumeChunk( m_game_state.dash_attack_1_sfx, MIX_MAX_VOLUME / 20 );
    Mix_VolumeChunk( m_game_state.dash_attack_2_sfx, MIX_MAX_VOLUME / 20 );
    Mix_VolumeChunk( m_game_state.jump_attack_1_sfx, MIX_MAX_VOLUME / 20 );
    Mix_VolumeChunk( m_game_state.jump_attack_2_sfx, MIX_MAX_VOLUME / 20 );

    Mix_VolumeChunk( m_game_state.take_damage_sfx, MIX_MAX_VOLUME / 10 );

    // Tarnished Widow SFX
    m_game_state.tarnished_widow_take_light_damage_1_sfx    = Mix_LoadWAV(TW_TAKE_LIGHT_DAMAGE_1_SFX_FILEPATH);
    m_game_state.tarnished_widow_take_light_damage_2_sfx    = Mix_LoadWAV(TW_TAKE_LIGHT_DAMAGE_2_SFX_FILEPATH);
    m_game_state.tarnished_widow_take_heavy_damage_sfx      = Mix_LoadWAV(TW_TAKE_HEAVY_DAMAGE_SFX_FILEPATH);
    m_game_state.tarnished_widow_walk_1_sfx                 = Mix_LoadWAV(TW_WALK_1_SFX_FILEPATH);
    m_game_state.tarnished_widow_walk_2_sfx                 = Mix_LoadWAV(TW_WALK_2_SFX_FILEPATH);
    m_game_state.tarnished_widow_jump_up_sfx                = Mix_LoadWAV(TW_JUMP_UP_SFX_FILEPATH);
    m_game_state.tarnished_widow_land_sfx                   = Mix_LoadWAV(TW_LAND_SFX_FILEPATH);
    m_game_state.tarnished_widow_heavy_land_sfx             = Mix_LoadWAV(TW_HEAVY_LAND_SFX_FILEPATH);
    m_game_state.tarnished_widow_attack_sfx                 = Mix_LoadWAV(TW_ATTACK_SFX_FILEPATH);
    m_game_state.tarnished_widow_spit_sfx                   = Mix_LoadWAV(TW_SPIT_SFX_FILEPATH);
    m_game_state.tarnished_widow_buff_sfx                   = Mix_LoadWAV(TW_BUFF_SFX_FILEPATH);
    m_game_state.tarnished_widow_roar_sfx                   = Mix_LoadWAV(TW_ROAR_SFX_FILEPATH);
    m_game_state.tarnished_widow_death_sfx                  = Mix_LoadWAV(TW_DEATH_SFX_FILEPATH);

    Mix_VolumeChunk(m_game_state.tarnished_widow_take_light_damage_1_sfx, MIX_MAX_VOLUME / 10);
    Mix_VolumeChunk(m_game_state.tarnished_widow_take_light_damage_2_sfx, MIX_MAX_VOLUME / 10);
    Mix_VolumeChunk(m_game_state.tarnished_widow_take_heavy_damage_sfx  , MIX_MAX_VOLUME / 10);
    Mix_VolumeChunk(m_game_state.tarnished_widow_walk_1_sfx             , MIX_MAX_VOLUME / 10);
    Mix_VolumeChunk(m_game_state.tarnished_widow_walk_2_sfx             , MIX_MAX_VOLUME / 10);
    Mix_VolumeChunk(m_game_state.tarnished_widow_jump_up_sfx            , MIX_MAX_VOLUME / 5);
    Mix_VolumeChunk(m_game_state.tarnished_widow_land_sfx               , MIX_MAX_VOLUME / 5);
    Mix_VolumeChunk(m_game_state.tarnished_widow_heavy_land_sfx         , MIX_MAX_VOLUME / 2);
    Mix_VolumeChunk(m_game_state.tarnished_widow_attack_sfx             , MIX_MAX_VOLUME / 5);
    Mix_VolumeChunk(m_game_state.tarnished_widow_spit_sfx               , MIX_MAX_VOLUME / 5);
    Mix_VolumeChunk(m_game_state.tarnished_widow_buff_sfx               , MIX_MAX_VOLUME / 5);
    Mix_VolumeChunk(m_game_state.tarnished_widow_roar_sfx               , MIX_MAX_VOLUME / 3);
    Mix_VolumeChunk(m_game_state.tarnished_widow_death_sfx              , MIX_MAX_VOLUME / 2);

    // Object SFX
    m_game_state.gate_close = Mix_LoadWAV(GATE_OPEN_SFX);
    m_game_state.floor_break = Mix_LoadWAV(FLOOR_BREAK);
    Mix_VolumeChunk(
        m_game_state.gate_close,
        MIX_MAX_VOLUME / 2
    );
    Mix_VolumeChunk(
        m_game_state.floor_break,
        MIX_MAX_VOLUME / 2
    );

    m_number_of_enemies = m_game_state.enemies.size();
}

void LevelC::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.enemies, m_game_state.objects, m_game_state.map);
    if (m_game_state.player->get_position().x <= 23.0f && boss_phase == 0) {
        static_cast<Gate*>(m_game_state.objects[1])->set_current_state(GATE_CLOSE);
        // + boss fight start animation effect
        static_cast<TarnishedWidow*> (m_game_state.enemies[0])->set_current_state(TARNISHEDWIDOW_MOVE);
        Mix_PlayChannel(1, m_game_state.gate_close, 0);
        boss_phase = 1;
    }
    if (static_cast<TarnishedWidow*> (m_game_state.enemies[0])->get_is_floor_break()) {
        delete m_game_state.map;
        m_game_state.map = new Map(LEVEL_WIDTH_P2, LEVEL_HEIGHT_P2, LEVELC_DATA_P2, map_texture_id, 1.0f, 14, 16);
        static_cast<TarnishedWidow*> (m_game_state.enemies[0])->set_is_floor_break(false);
    }

    for (int i = 0; i < m_game_state.enemies.size(); i++) {
        m_game_state.enemies[i]->update(delta_time,
            m_game_state.player,
            m_game_state.map);
    }

    for (StaticAnimatedObject* object : m_game_state.objects) {
        object->update(delta_time);
    }

    if (m_game_state.player->get_position().x >= 30.0f) m_game_state.next_scene_id = 0;

    // -------- Tarnished Widow SFX ---------- //

    TarnishedWidow* tarnished_widow = static_cast<TarnishedWidow*>(m_game_state.enemies[0]);
    //TarnishedWidowState tarnished_widow_state = tarnished_widow->get_current_state();
    if (tarnished_widow->get_is_tarnished_widow_take_light_damage_1_sfx()) {
        Mix_PlayChannel(5, m_game_state.tarnished_widow_take_light_damage_1_sfx, 0);
        tarnished_widow->set_is_tarnished_widow_take_light_damage_1_sfx(false);
    }
    if (tarnished_widow->get_is_tarnished_widow_take_light_damage_2_sfx()) {
        Mix_PlayChannel(5, m_game_state.tarnished_widow_take_light_damage_2_sfx, 0);
        tarnished_widow->set_is_tarnished_widow_take_light_damage_2_sfx(false);
    }
    if (tarnished_widow->get_is_tarnished_widow_take_heavy_damage_sfx()) {
        Mix_PlayChannel(5, m_game_state.tarnished_widow_take_heavy_damage_sfx, 0);
        tarnished_widow->set_is_tarnished_widow_take_heavy_damage_sfx(false);
    }
}

void LevelC::render(ShaderProgram* program)
{
    m_game_state.player->render(program);
    for (int i = 0; i < m_game_state.enemies.size(); i++) {
        m_game_state.enemies[i]->render(program);
    }
    for (int i = 0; i < m_game_state.objects.size(); i++) {
        m_game_state.objects[i]->render(program);
    }
    m_game_state.map->render(program);

    if (static_cast<TarnishedWidow*>(m_game_state.enemies[0])->get_life() <= 0) {
        Utility::draw_text(program, text_texture_id, std::string("YOU WIN!"), 0.25f, 0.01f, m_game_state.player->get_position() + glm::vec3(-0.5f, 2.0f, 0.0f));
    }
}
