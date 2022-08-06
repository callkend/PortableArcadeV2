
#include <stdint.h>

#define TEST_ARRAY

#ifdef TEST_ARRAY

const uint16_t PixelMap[] = {
    0,   31,  32,  63,  64,  95,  96,   127,  128,  159,  160,  191,  192,  223,  224,  255,
    1,   30,  33,  62,  65,  94,  97,   126,  129,  158,  161,  190,  193,  222,  225,  254,
    2,   29,  34,  61,  66,  93,  98,   125,  130,  157,  162,  189,  194,  221,  226,  253,
    3,   28,  35,  60,  67,  92,  99,   124,  131,  156,  163,  188,  195,  220,  227,  252,
    4,   27,  36,  59,  68,  91,  100,  123,  132,  155,  164,  187,  196,  219,  228,  251,
    5,   26,  37,  58,  69,  90,  101,  122,  133,  154,  165,  186,  197,  218,  229,  250,
    6,   25,  38,  57,  70,  89,  102,  121,  134,  153,  166,  185,  198,  217,  230,  249,
    7,   24,  39,  56,  71,  88,  103,  120,  135,  152,  167,  184,  199,  216,  231,  248,
    8,   23,  40,  55,  72,  87,  104,  119,  136,  151,  168,  183,  200,  215,  232,  247,
    9,   22,  41,  54,  73,  86,  105,  118,  137,  150,  169,  182,  201,  214,  233,  246,
    10,  21,  42,  53,  74,  85,  106,  117,  138,  149,  170,  181,  202,  213,  234,  245,
    11,  20,  43,  52,  75,  84,  107,  116,  139,  148,  171,  180,  203,  212,  235,  244,
    12,  19,  44,  51,  76,  83,  108,  115,  140,  147,  172,  179,  204,  211,  236,  243,
    13,  18,  45,  50,  77,  82,  109,  114,  141,  146,  173,  178,  205,  210,  237,  242,
    14,  17,  46,  49,  78,  81,  110,  113,  142,  145,  174,  177,  206,  209,  238,  241,
    15,  16,  47,  48,  79,  80,  111,  112,  143,  144,  175,  176,  207,  208,  239,  240
};

#else

const uint16_t PixelMap[] = {
0,   15,  16,  31,  32,  47,  48,  63,  64,  79,  80,  95,  96,  111, 112, 127, 128, 143, 144, 159, 160, 175, 176, 191, 192, 207, 208, 223, 224, 239, 240, 255,
1,   14,  17,  30,  33,  46,  49,  62,  65,  78,  81,  94,  97,  110, 113, 126, 129, 142, 145, 158, 161, 174, 177, 190, 193, 206, 209, 222, 225, 238, 241, 254,
2,   13,  18,  29,  34,  45,  50,  61,  66,  77,  82,  93,  98,  109, 114, 125, 130, 141, 146, 157, 162, 173, 178, 189, 194, 205, 210, 221, 226, 237, 242, 253,
3,   12,  19,  28,  35,  44,  51,  60,  67,  76,  83,  92,  99,  108, 115, 124, 131, 140, 147, 156, 163, 172, 179, 188, 195, 204, 211, 220, 227, 236, 243, 252,
4,   11,  20,  27,  36,  43,  52,  59,  68,  75,  84,  91,  100, 107, 116, 123, 132, 139, 148, 155, 164, 171, 180, 187, 196, 203, 212, 219, 228, 235, 244, 251,
5,   10,  21,  26,  37,  42,  53,  58,  69,  74,  85,  90,  101, 106, 117, 122, 133, 138, 149, 154, 165, 170, 181, 186, 197, 202, 213, 218, 229, 234, 245, 250,
6,   9,   22,  25,  38,  41,  54,  57,  70,  73,  86,  89,  102, 105, 118, 121, 134, 137, 150, 153, 166, 169, 182, 185, 198, 201, 214, 217, 230, 233, 246, 249,
7,   8,   23,  24,  39,  40,  55,  56,  71,  72,  87,  88,  103, 104, 119, 120, 135, 136, 151, 152, 167, 168, 183, 184, 199, 200, 215, 216, 231, 232, 247, 248,
256, 271, 272, 287, 288, 303, 304, 319, 320, 335, 336, 351, 352, 367, 368, 383, 384, 399, 400, 415, 416, 431, 432, 447, 448, 463, 464, 479, 480, 495, 496, 511,
257, 270, 273, 286, 289, 302, 305, 318, 321, 334, 337, 350, 353, 366, 369, 382, 385, 398, 401, 414, 417, 430, 433, 446, 449, 462, 465, 478, 481, 494, 497, 510,
258, 269, 274, 285, 290, 301, 306, 317, 322, 333, 338, 349, 354, 365, 370, 381, 386, 397, 402, 413, 418, 429, 434, 445, 450, 461, 466, 477, 482, 493, 498, 509,
259, 268, 275, 284, 291, 300, 307, 316, 323, 332, 339, 348, 355, 364, 371, 380, 387, 396, 403, 412, 419, 428, 435, 444, 451, 460, 467, 476, 483, 492, 499, 508,
260, 267, 276, 283, 292, 299, 308, 315, 324, 331, 340, 347, 356, 363, 372, 379, 388, 395, 404, 411, 420, 427, 436, 443, 452, 459, 468, 475, 484, 491, 500, 507,
261, 266, 277, 282, 293, 298, 309, 314, 325, 330, 341, 346, 357, 362, 373, 378, 389, 394, 405, 410, 421, 426, 437, 442, 453, 458, 469, 474, 485, 490, 501, 506,
262, 265, 278, 281, 294, 297, 310, 313, 326, 329, 342, 345, 358, 361, 374, 377, 390, 393, 406, 409, 422, 425, 438, 441, 454, 457, 470, 473, 486, 489, 502, 505,
263, 264, 279, 280, 295, 296, 311, 312, 327, 328, 343, 344, 359, 360, 375, 376, 391, 392, 407, 408, 423, 424, 439, 440, 455, 456, 471, 472, 487, 488, 503, 504,
512, 527, 528, 543, 544, 559, 560, 575, 576, 591, 592, 607, 608, 623, 624, 639, 640, 655, 656, 671, 672, 687, 688, 703, 704, 719, 720, 735, 736, 751, 752, 767,
513, 526, 529, 542, 545, 558, 561, 574, 577, 590, 593, 606, 609, 622, 625, 638, 641, 654, 657, 670, 673, 686, 689, 702, 705, 718, 721, 734, 737, 750, 753, 766,
514, 525, 530, 541, 546, 557, 562, 573, 578, 589, 594, 605, 610, 621, 626, 637, 642, 653, 658, 669, 674, 685, 690, 701, 706, 717, 722, 733, 738, 749, 754, 765,
515, 524, 531, 540, 547, 556, 563, 572, 579, 588, 595, 604, 611, 620, 627, 636, 643, 652, 659, 668, 675, 684, 691, 700, 707, 716, 723, 732, 739, 748, 755, 764,
516, 523, 532, 539, 548, 555, 564, 571, 580, 587, 596, 603, 612, 619, 628, 635, 644, 651, 660, 667, 676, 683, 692, 699, 708, 715, 724, 731, 740, 747, 756, 763,
517, 522, 533, 538, 549, 554, 565, 570, 581, 586, 597, 602, 613, 618, 629, 634, 645, 650, 661, 666, 677, 682, 693, 698, 709, 714, 725, 730, 741, 746, 757, 762,
518, 521, 534, 537, 550, 553, 566, 569, 582, 585, 598, 601, 614, 617, 630, 633, 646, 649, 662, 665, 678, 681, 694, 697, 710, 713, 726, 729, 742, 745, 758, 761,
519, 520, 535, 536, 551, 552, 567, 568, 583, 584, 599, 600, 615, 616, 631, 632, 647, 648, 663, 664, 679, 680, 695, 696, 711, 712, 727, 728, 743, 744, 759, 760,
};

#endif
