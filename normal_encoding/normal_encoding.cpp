#include "normal_encoding.hpp"

#include <cmath>

static const float c_pi = 3.14159265358979323846f;
static const float c_2pi = 2 * c_pi;

struct alias
{
	int16_t n;
	int16_t k;
	int16_t d;
};

struct reference
{
	int16_t n;
	int16_t k;
	int16_t m;
};

static const int Nphi       = 227;
static const int Ntheta_avg = 289;

static const int16_t Nthetas[] = {
	1, 8, 15, 21, 27, 33, 40, 46, 52, 59, 65, 71, 77, 83, 90, 96, 
	102, 108, 114, 120, 126, 132, 138, 144, 150, 156, 162, 168, 174, 180, 185, 191, 
	197, 203, 208, 214, 219, 225, 230, 236, 241, 246, 252, 257, 262, 267, 272, 277, 
	282, 287, 292, 297, 301, 306, 311, 315, 320, 324, 329, 333, 337, 341, 346, 350, 
	354, 357, 361, 365, 369, 372, 376, 380, 383, 386, 390, 393, 396, 399, 402, 405, 
	408, 410, 413, 416, 418, 420, 423, 425, 427, 429, 431, 433, 435, 437, 438, 440, 
	441, 443, 444, 445, 447, 448, 449, 450, 450, 451, 452, 452, 453, 453, 453, 454, 
	454, 454, 454, 454, 453, 453, 453, 452, 452, 451, 450, 450, 449, 448, 447, 445, 
	444, 443, 441, 440, 438, 437, 435, 433, 431, 429, 427, 425, 423, 420, 418, 416, 
	413, 410, 408, 405, 402, 399, 396, 393, 390, 386, 383, 380, 376, 372, 369, 365, 
	361, 357, 354, 350, 346, 341, 337, 333, 329, 324, 320, 315, 311, 306, 301, 297, 
	292, 287, 282, 277, 272, 267, 262, 257, 252, 246, 241, 236, 230, 225, 219, 214, 
	208, 203, 197, 191, 185, 180, 174, 168, 162, 156, 150, 144, 138, 132, 126, 120, 
	114, 108, 102, 96, 90, 83, 77, 71, 65, 59, 52, 46, 40, 33, 27, 21, 
	15, 8, 1, 
};

static const alias alias_table[] = {
	{1, 115, 165}, {8, 113, 165}, {15, 111, 165}, {21, 117, 164}, 
	{27, 110, 164}, {33, 108, 164}, {40, 119, 163}, {46, 106, 163}, 
	{52, 105, 162}, {59, 122, 161}, {65, 103, 161}, {71, 102, 160}, 
	{77, 101, 159}, {83, 100, 158}, {90, 99, 156}, {96, 98, 155}, 
	{102, 97, 154}, {108, 96, 152}, {114, 95, 151}, {120, 94, 149}, 
	{126, 93, 148}, {132, 92, 146}, {138, 91, 144}, {144, 90, 142}, 
	{150, 89, 140}, {156, 88, 138}, {162, 87, 136}, {168, 85, 131}, 
	{174, 83, 127}, {180, 145, 121}, {185, 80, 119}, {191, 78, 113}, 
	{197, 151, 104}, {203, 74, 101}, {208, 72, 94}, {214, 157, 83}, 
	{219, 68, 80}, {225, 66, 72}, {230, 64, 65}, {236, 165, 52}, 
	{241, 167, 44}, {246, 169, 35}, {252, 55, 26}, {257, 53, 17}, 
	{262, 82, 14}, {267, 174, 12}, {272, 145, 12}, {277, 71, 10}, 
	{282, 140, 7}, {237, 158, -3}, {271, 177, -2}, {286, 165, -1}, 
	{286, 63, 2}, {274, 75, 12}, {276, 73, 11}, {278, 148, 9}, 
	{281, 141, 8}, {281, 69, 8}, {285, 160, 3}, {285, 139, 3}, 
	{239, 49, -2}, {272, 50, -1}, {273, 61, -1}, {274, 62, -1}, 
	{241, 60, -2}, {275, 63, -1}, {286, 51, 0}, {286, 66, 0}, 
	{243, 64, -2}, {286, 67, 0}, {245, 68, -2}, {247, 70, -2}, 
	{278, 144, 9}, {249, 71, -2}, {275, 86, 11}, {286, 89, 1}, 
	{276, 70, 11}, {285, 62, 3}, {274, 52, 12}, {284, 64, 6}, 
	{272, 83, 12}, {284, 158, 5}, {276, 155, 10}, {284, 88, 5}, 
	{276, 146, 10}, {286, 64, 1}, {286, 158, 0}, {251, 73, -2}, 
	{286, 69, 0}, {253, 87, -2}, {286, 141, 0}, {282, 67, 7}, 
	{278, 87, 9}, {274, 153, 11}, {269, 147, 12}, {265, 72, 13}, 
	{260, 74, 15}, {256, 173, 17}, {251, 171, 26}, {246, 57, 35}, 
	{241, 59, 44}, {236, 61, 52}, {231, 164, 57}, {226, 162, 65}, 
	{220, 67, 76}, {214, 69, 83}, {209, 156, 87}, {203, 153, 97}, 
	{197, 75, 104}, {191, 149, 110}, {191, 77, 110}, {180, 81, 121}, 
	{173, 84, 129}, {173, 143, 127}, {166, 86, 134}, {166, 141, 131}, 
	{185, 147, 116}, {185, 79, 116}, {179, 82, 124}, {203, 73, 97}, 
	{196, 76, 107}, {208, 155, 91}, {220, 160, 72}, {213, 70, 87}, 
	{225, 161, 68}, {230, 163, 61}, {235, 62, 57}, {239, 60, 48}, 
	{245, 58, 40}, {250, 56, 31}, {254, 54, 22}, {259, 78, 15}, 
	{263, 84, 13}, {268, 149, 12}, {272, 81, 12}, {276, 85, 10}, 
	{280, 154, 8}, {284, 161, 4}, {276, 65, -1}, {277, 138, -1}, 
	{286, 88, 0}, {286, 140, 0}, {278, 139, -1}, {285, 50, 3}, 
	{255, 89, -2}, {284, 65, 4}, {222, -1, -1}, {281, 66, 8}, 
	{257, 144, -2}, {280, 142, 8}, {225, 146, -3}, {279, 175, 8}, 
	{228, 150, -3}, {285, 163, 2}, {279, 142, -1}, {231, 152, -3}, 
	{280, 154, -1}, {259, 148, -2}, {234, 155, -3}, {281, 156, -1}, 
	{282, 159, -1}, {283, 160, -1}, {261, 157, -2}, {263, 162, -2}, 
	{284, 161, -1}, {285, 164, -1}, {284, 176, 3}, {285, 162, 2}, 
	{281, 51, 8}, {281, 156, 7}, {277, 68, 10}, {277, 152, 9}, 
	{274, 143, 11}, {273, 77, 12}, {279, 157, 8}, {265, 163, -2}, 
	{267, 175, -2}, {269, 176, -2}, {282, 159, 6}, {277, 150, 9}, 
	{272, 79, 12}, {267, 151, 12}, {262, 76, 14}, {257, 80, 15}, 
	{252, 172, 22}, {246, 170, 31}, {241, 168, 40}, {236, 166, 48}, 
	{230, 63, 61}, {225, 65, 68}, {219, 159, 76}, {214, 158, 80}, 
	{208, 71, 91}, {203, 154, 94}, {197, 152, 101}, {191, 150, 107}, 
	{185, 148, 113}, {180, 146, 119}, {174, 144, 124}, {168, 142, 129}, 
	{162, 140, 134}, {156, 139, 136}, {150, 138, 138}, {144, 137, 140}, 
	{138, 136, 142}, {132, 135, 144}, {126, 134, 146}, {120, 133, 148}, 
	{114, 132, 149}, {108, 131, 151}, {102, 130, 152}, {96, 129, 154}, 
	{90, 128, 155}, {83, 127, 156}, {77, 126, 158}, {71, 125, 159}, 
	{65, 124, 160}, {59, 104, 161}, {52, 123, 161}, {46, 121, 162}, 
	{40, 107, 163}, {33, 120, 163}, {27, 109, 164}, {21, 116, 164}, 
	{15, 118, 164}, {8, 112, 165}, {1, 114, 165}, 
};

static const int gap_begin  = 42416;
static const int gap_end    = 42483;
static const int gap_length = gap_end - gap_begin;

static const reference reference_table[] = {
	{1, 0, 0}, {8, 1, 0}, {15, 2, 0}, {21, 3, 0}, {27, 4, 0}, 
	{33, 5, 0}, {40, 6, 0}, {46, 7, 0}, {52, 8, 0}, {59, 9, 0}, 
	{65, 10, 0}, {71, 11, 0}, {77, 12, 0}, {83, 13, 0}, {90, 14, 0}, 
	{96, 15, 0}, {102, 16, 0}, {108, 17, 0}, {114, 18, 0}, {120, 19, 0}, 
	{126, 20, 0}, {132, 21, 0}, {138, 22, 0}, {144, 23, 0}, {150, 24, 0}, 
	{156, 25, 0}, {162, 26, 0}, {168, 27, 0}, {174, 28, 0}, {180, 29, 0}, 
	{185, 30, 0}, {191, 31, 0}, {197, 32, 0}, {203, 33, 0}, {208, 34, 0}, 
	{214, 35, 0}, {219, 36, 0}, {225, 37, 0}, {230, 38, 0}, {236, 39, 0}, 
	{241, 40, 0}, {246, 41, 0}, {252, 42, 0}, {257, 43, 0}, {262, 44, 0}, 
	{267, 45, 0}, {272, 46, 0}, {277, 47, 0}, {282, 48, 0}, {237, 49, 0}, 
	{287, 60, 239}, {271, 50, 0}, {288, 61, 272}, {292, 143, 285}, {286, 51, 0}, 
	{289, 66, 286}, {297, 168, 281}, {286, 52, 0}, {301, 78, 274}, {274, 53, 0}, 
	{306, 43, 257}, {276, 54, 0}, {311, 130, 254}, {278, 55, 0}, {315, 42, 252}, 
	{281, 56, 0}, {320, 129, 250}, {281, 57, 0}, {324, 99, 246}, {285, 58, 0}, 
	{329, 128, 245}, {285, 59, 0}, {333, 100, 241}, {239, 60, 0}, {287, 64, 241}, 
	{337, 127, 239}, {272, 61, 0}, {288, 62, 273}, {341, 101, 236}, {273, 62, 0}, 
	{288, 63, 274}, {292, 77, 285}, {346, 126, 235}, {274, 63, 0}, {288, 65, 275}, 
	{291, 52, 286}, {350, 188, 230}, {241, 64, 0}, {287, 68, 243}, {290, 85, 286}, 
	{295, 79, 284}, {354, 38, 230}, {275, 65, 0}, {288, 138, 276}, {293, 145, 284}, 
	{357, 189, 225}, {286, 66, 0}, {289, 67, 286}, {297, 147, 281}, {361, 37, 225}, 
	{286, 67, 0}, {289, 69, 286}, {296, 91, 282}, {365, 104, 220}, {243, 68, 0}, 
	{287, 70, 245}, {299, 170, 277}, {369, 36, 219}, {286, 69, 0}, {289, 88, 286}, 
	{297, 57, 281}, {372, 105, 214}, {245, 70, 0}, {287, 71, 247}, {300, 76, 276}, 
	{376, 123, 213}, {247, 71, 0}, {287, 73, 249}, {299, 47, 277}, {380, 192, 208}, 
	{278, 72, 0}, {302, 95, 265}, {383, 34, 208}, {249, 73, 0}, {287, 87, 251}, 
	{300, 54, 276}, {386, 119, 203}, {275, 74, 0}, {304, 96, 260}, {390, 33, 203}, 
	{286, 75, 0}, {301, 53, 274}, {393, 108, 197}, {276, 76, 0}, {303, 182, 262}, 
	{396, 120, 196}, {285, 77, 0}, {301, 173, 273}, {399, 110, 191}, {274, 78, 0}, 
	{304, 131, 259}, {402, 31, 191}, {284, 79, 0}, {301, 180, 272}, {405, 117, 185}, 
	{272, 80, 0}, {304, 183, 257}, {408, 30, 185}, {284, 81, 0}, {301, 134, 272}, 
	{410, 111, 180}, {276, 82, 0}, {303, 44, 262}, {413, 118, 179}, {284, 83, 0}, 
	{301, 80, 272}, {416, 28, 174}, {276, 84, 0}, {302, 132, 263}, {418, 112, 173}, 
	{286, 85, 0}, {299, 135, 276}, {420, 27, 168}, {286, 86, 0}, {300, 74, 275}, 
	{423, 114, 166}, {251, 87, 0}, {287, 89, 253}, {298, 92, 278}, {425, 26, 162}, 
	{286, 88, 0}, {289, 140, 286}, {294, 83, 284}, {427, 25, 156}, {253, 89, 0}, 
	{287, 144, 255}, {290, 75, 286}, {429, 24, 150}, {286, 90, 0}, {431, 23, 144}, 
	{282, 91, 0}, {433, 22, 138}, {278, 92, 0}, {435, 21, 132}, {274, 93, 0}, 
	{437, 20, 126}, {269, 94, 0}, {438, 19, 120}, {265, 95, 0}, {440, 18, 114}, 
	{260, 96, 0}, {441, 17, 108}, {256, 97, 0}, {443, 16, 102}, {251, 98, 0}, 
	{444, 15, 96}, {246, 99, 0}, {445, 14, 90}, {241, 100, 0}, {447, 13, 83}, 
	{236, 101, 0}, {448, 12, 77}, {231, 102, 0}, {449, 11, 71}, {226, 103, 0}, 
	{450, 10, 65}, {220, 104, 0}, {450, 217, 59}, {214, 105, 0}, {451, 8, 52}, 
	{209, 106, 0}, {452, 7, 46}, {203, 107, 0}, {452, 220, 40}, {197, 108, 0}, 
	{453, 5, 33}, {191, 109, 0}, {453, 222, 27}, {191, 110, 0}, {453, 4, 27}, 
	{180, 111, 0}, {454, 2, 15}, {173, 112, 0}, {454, 225, 8}, {173, 113, 0}, 
	{454, 1, 8}, {166, 114, 0}, {454, 226, 1}, {166, 115, 0}, {454, 0, 1}, 
	{185, 116, 0}, {453, 223, 21}, {185, 117, 0}, {453, 3, 21}, {179, 118, 0}, 
	{453, 224, 15}, {203, 119, 0}, {452, 6, 40}, {196, 120, 0}, {452, 221, 33}, 
	{208, 121, 0}, {451, 219, 46}, {220, 122, 0}, {450, 9, 59}, {213, 123, 0}, 
	{450, 218, 52}, {225, 124, 0}, {449, 216, 65}, {230, 125, 0}, {448, 215, 71}, 
	{235, 126, 0}, {447, 214, 77}, {239, 127, 0}, {445, 213, 83}, {245, 128, 0}, 
	{444, 212, 90}, {250, 129, 0}, {443, 211, 96}, {254, 130, 0}, {441, 210, 102}, 
	{259, 131, 0}, {440, 209, 108}, {263, 132, 0}, {438, 208, 114}, {268, 133, 0}, 
	{437, 207, 120}, {272, 134, 0}, {435, 206, 126}, {276, 135, 0}, {433, 205, 132}, 
	{280, 136, 0}, {431, 204, 138}, {284, 137, 0}, {429, 203, 144}, {276, 138, 0}, 
	{288, 139, 277}, {427, 202, 150}, {277, 139, 0}, {288, 142, 278}, {292, 59, 285}, 
	{425, 201, 156}, {286, 140, 0}, {289, 141, 286}, {296, 48, 282}, {423, 200, 162}, 
	{286, 141, 0}, {289, 90, 286}, {297, 56, 281}, {420, 115, 166}, {278, 142, 0}, 
	{288, 154, 279}, {297, 149, 280}, {418, 199, 168}, {285, 143, 0}, {300, 172, 274}, 
	{416, 113, 173}, {255, 144, 0}, {287, 148, 257}, {298, 72, 278}, {413, 198, 174}, 
	{284, 145, 0}, {301, 46, 272}, {410, 29, 180}, {222, 146, 0}, {286, 150, 225}, 
	{299, 84, 276}, {408, 197, 180}, {281, 147, 0}, {301, 94, 269}, {405, 116, 185}, 
	{257, 148, 0}, {287, 157, 259}, {298, 55, 278}, {402, 196, 185}, {280, 149, 0}, 
	{301, 133, 268}, {399, 109, 191}, {225, 150, 0}, {286, 152, 228}, {298, 179, 277}, 
	{396, 195, 191}, {279, 151, 0}, {301, 181, 267}, {393, 32, 197}, {228, 152, 0}, 
	{286, 155, 231}, {298, 171, 277}, {390, 194, 197}, {285, 153, 0}, {300, 93, 274}, 
	{386, 107, 203}, {279, 154, 0}, {288, 156, 280}, {297, 136, 280}, {383, 193, 203}, 
	{231, 155, 0}, {286, 158, 234}, {299, 82, 276}, {380, 121, 208}, {280, 156, 0}, 
	{288, 159, 281}, {296, 169, 281}, {376, 106, 209}, {259, 157, 0}, {287, 162, 261}, 
	{297, 174, 279}, {372, 35, 214}, {234, 158, 0}, {286, 49, 237}, {289, 86, 286}, 
	{294, 81, 284}, {369, 191, 214}, {281, 159, 0}, {288, 160, 282}, {295, 178, 282}, 
	{365, 190, 219}, {282, 160, 0}, {288, 161, 283}, {292, 58, 285}, {361, 122, 220}, 
	{283, 161, 0}, {288, 164, 284}, {293, 137, 284}, {357, 124, 225}, {261, 162, 0}, 
	{287, 163, 263}, {291, 167, 285}, {354, 103, 226}, {263, 163, 0}, {287, 175, 265}, 
	{291, 153, 285}, {350, 125, 230}, {284, 164, 0}, {288, 165, 285}, {346, 102, 231}, 
	{285, 165, 0}, {288, 51, 286}, {341, 39, 236}, {284, 166, 0}, {337, 187, 236}, 
	{285, 167, 0}, {333, 40, 241}, {281, 168, 0}, {329, 186, 241}, {281, 169, 0}, 
	{324, 41, 246}, {277, 170, 0}, {320, 185, 246}, {277, 171, 0}, {315, 98, 251}, 
	{274, 172, 0}, {311, 184, 252}, {273, 173, 0}, {306, 97, 256}, {279, 174, 0}, 
	{301, 45, 267}, {265, 175, 0}, {287, 176, 267}, {297, 151, 279}, {267, 176, 0}, 
	{287, 177, 269}, {292, 166, 284}, {269, 177, 0}, {287, 50, 271}, {282, 178, 0}, 
	{277, 179, 0}, {272, 180, 0}, {267, 181, 0}, {262, 182, 0}, {257, 183, 0}, 
	{252, 184, 0}, {246, 185, 0}, {241, 186, 0}, {236, 187, 0}, {230, 188, 0}, 
	{225, 189, 0}, {219, 190, 0}, {214, 191, 0}, {208, 192, 0}, {203, 193, 0}, 
	{197, 194, 0}, {191, 195, 0}, {185, 196, 0}, {180, 197, 0}, {174, 198, 0}, 
	{168, 199, 0}, {162, 200, 0}, {156, 201, 0}, {150, 202, 0}, {144, 203, 0}, 
	{138, 204, 0}, {132, 205, 0}, {126, 206, 0}, {120, 207, 0}, {114, 208, 0}, 
	{108, 209, 0}, {102, 210, 0}, {96, 211, 0}, {90, 212, 0}, {83, 213, 0}, 
	{77, 214, 0}, {71, 215, 0}, {65, 216, 0}, {59, 217, 0}, {52, 218, 0}, 
	{46, 219, 0}, {40, 220, 0}, {33, 221, 0}, {27, 222, 0}, {21, 223, 0}, 
	{15, 224, 0}, {8, 225, 0}, {1, 226, 0}, 
};

static const int16_t offset_table[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 
	32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 
	48, 49, 51, 54, 57, 59, 61, 63, 65, 67, 69, 71, 73, 76, 79, 83, 
	87, 92, 96, 100, 104, 108, 112, 116, 120, 123, 127, 130, 133, 136, 139, 142, 
	145, 148, 151, 154, 157, 160, 163, 166, 170, 174, 178, 180, 182, 184, 186, 188, 
	190, 192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220, 
	222, 224, 226, 228, 230, 232, 234, 236, 238, 240, 242, 244, 246, 248, 250, 252, 
	254, 256, 258, 260, 262, 264, 266, 268, 270, 272, 274, 277, 281, 285, 289, 293, 
	296, 300, 303, 307, 310, 314, 317, 321, 324, 328, 331, 335, 339, 343, 347, 352, 
	356, 360, 364, 368, 372, 375, 378, 380, 382, 384, 386, 388, 390, 392, 394, 396, 
	399, 402, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 
	418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 
	434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 
	450, 451, 452, 
};

uint16_t encode(const float3& v)
{
	float phi = std::acos(v.z);
	float theta = std::atan2(v.y, v.x);
	if (theta < 0) theta += c_2pi;

	int i = static_cast<int>(phi * (Nphi - 1) / c_pi + 0.5f);
	int Ntheta = Nthetas[i];
	int j = static_cast<int>(theta * Ntheta / c_2pi + 0.5f);
	if (j == Ntheta) j = 0;

	// map jagged array entry to a linearized index
	int index = 0;
	int prev = 0;
	for (int t = offset_table[i]; ; ++t)
	{
		reference ref = reference_table[t];
		if (j < ref.n)
		{
			index = ref.k * Ntheta_avg + ref.m + j - prev;
			break;
		}
		prev = ref.n;
	}

	if (index >= gap_end) index -= gap_length;
	return static_cast<uint16_t>(index);
}

float3 decode(uint16_t s)
{
	int index = s;
	if (index >= gap_begin) index += gap_length;
	int i = index / Ntheta_avg;
	int j = index % Ntheta_avg;

	// redirection by alias method
	alias a = alias_table[i];
	if (j >= a.n)
	{
		i = a.k;
		j = j + a.d;
	}

	float phi = i * c_pi / (Nphi - 1);
	float theta = j * c_2pi / Nthetas[i];
	float3 v = {
		std::sin(phi) * std::cos(theta),
		std::sin(phi) * std::sin(theta),
		std::cos(phi)
	};
	return v;
}


#if 0
////////////////////////////////////////////////////////////////////
// code for test only

// map jagged array entry to a linearized index
uint16_t jagged_to_linear(int i, int j)
{
	int index = 0;
	int prev = 0;
	for (int t = offset_table[i]; ; ++t)
	{
		reference ref = reference_table[t];
		if (j < ref.n)
		{
			index = ref.k * Ntheta_avg + ref.m + j - prev;
			break;
		}
		prev = ref.n;
	}

	if (index >= gap_end) index -= gap_length;
	return static_cast<uint16_t>(index);
}

// map linearized index to a jagged array entry
void linear_to_jagged(uint16_t s, int &i, int &j)
{
	int index = s;
	if (index >= gap_begin) index += gap_length;
	i = index / Ntheta_avg;
	j = index % Ntheta_avg;

	// redirection by alias method
	alias a = alias_table[i];
	if (j >= a.n)
	{
		i = a.k;
		j = j + a.d;
	}
}

void test()
{
	for (int i = 0; i < Nphi; ++i)
	{
		for (int j = 0; j < Nthetas[i]; ++j)
		{
			uint16_t index = jagged_to_linear(i, j);
			int ni, nj;
			linear_to_jagged(index, ni, nj);
			if (i != ni || j != nj)
			{
				__asm int 3;
			}
		}
	}
}
#endif
