#include "agent.hpp"

#include "../big_bang/physics/random.hpp"

#include <complex>
#include <vector>
#include <optional>
#include <map>

using namespace WarGrey::STEM;
using namespace WarGrey::AoC;

namespace {
    struct FrameBranch {
        int frame_idx;
        int weight;
    };

    struct AgentFrames {
        int duration;
        std::vector<std::complex<int>> images;
        std::vector<FrameBranch> branches;
        std::optional<int> exit_branch;
        std::optional<std::string> sound;
    };

    struct Agent {
        std::map<std::string, const std::vector<AgentFrames>> frames;
        int width;
        int height;
        int layers;
        std::vector<std::string> sounds;
    };
}

static const std::vector<AgentFrames> RestPoseFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> LookRightFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 6, std::nullopt },
    { 100, { {  372,  930 /* [243](10,  3) */ } }, { }, 5, std::nullopt },
    { 100, { {  496,  930 /* [244](10,  4) */ } }, { }, 4, std::nullopt },
    { 600, { {  620,  930 /* [245](10,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496,  930 /* [244](10,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372,  930 /* [243](10,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> PrintFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { { 1612,  558 /* [157]( 6, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  558 /* [158]( 6, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  558 /* [159]( 6, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984,  558 /* [160]( 6, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108,  558 /* [161]( 6, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232,  558 /* [162]( 6, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { { 2356,  558 /* [163]( 6, 19) */ } }, { }, 29, "8" },
    { 120, { { 2480,  558 /* [164]( 6, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { { 2604,  558 /* [165]( 6, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { { 2728,  558 /* [166]( 6, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { { 2852,  558 /* [167]( 6, 23) */ } }, { }, std::nullopt, "8" },
    { 120, { {    0,  651 /* [168]( 7,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { {  124,  651 /* [169]( 7,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { {  248,  651 /* [170]( 7,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { {  372,  651 /* [171]( 7,  3) */ } }, { }, 29, "8" },
    { 120, { {  496,  651 /* [172]( 7,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { {  620,  651 /* [173]( 7,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { {  744,  651 /* [174]( 7,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { {  868,  651 /* [175]( 7,  7) */ } }, { }, std::nullopt, "8" },
    { 120, { {  992,  651 /* [176]( 7,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { { 1116,  651 /* [177]( 7,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { { 1240,  651 /* [178]( 7, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { { 1364,  651 /* [179]( 7, 11) */ } }, { }, 29, "8" },
    { 120, { { 1488,  651 /* [180]( 7, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { { 1612,  651 /* [181]( 7, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { { 1736,  651 /* [182]( 7, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { { 1860,  651 /* [183]( 7, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { { 1984,  651 /* [184]( 7, 16) */ } }, { }, std::nullopt, "8" },
    { 120, { { 2108,  651 /* [185]( 7, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 120, { { 2232,  651 /* [186]( 7, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356,  651 /* [187]( 7, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480,  651 /* [188]( 7, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604,  651 /* [189]( 7, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728,  651 /* [190]( 7, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852,  651 /* [191]( 7, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,  744 /* [192]( 8,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124,  744 /* [193]( 8,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248,  744 /* [194]( 8,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> LookDownFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 6, std::nullopt },
    { 100, { { 2852, 1395 /* [383](15, 23) */ } }, { }, 5, std::nullopt },
    { 100, { {    0, 1488 /* [384](16,  0) */ } }, { }, 4, std::nullopt },
    { 600, { {  124, 1488 /* [385](16,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 1488 /* [384](16,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1395 /* [383](15, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> IdleYawnFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 95 } }, 17, std::nullopt },
    { 100, { {  124,    0 /*   [1]( 0,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 17, std::nullopt },
    { 100, { {  248, 1023 /* [266](11,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1023 /* [267](11,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1023 /* [268](11,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 1023 /* [269](11,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 1023 /* [270](11,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 1023 /* [271](11,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 1023 /* [272](11,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1023 /* [273](11,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1023 /* [274](11, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 1023 /* [275](11, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488, 1023 /* [276](11, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 1023 /* [277](11, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 1023 /* [278](11, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 1023 /* [279](11, 15) */ } }, { }, 17, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> WritingFrames = {
    { 100, { {  248, 1674 /* [434](18,  2) */ } }, { }, std::nullopt, "13" },
    { 100, { {  372, 1674 /* [435](18,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1674 /* [436](18,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 1674 /* [437](18,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 1674 /* [438](18,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 1674 /* [439](18,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 1674 /* [440](18,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1674 /* [441](18,  9) */ } }, { }, std::nullopt, "16" },
    { 100, { { 1240, 1674 /* [442](18, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 1674 /* [443](18, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488, 1674 /* [444](18, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 1674 /* [445](18, 13) */ } }, { }, std::nullopt, "2" },
    { 100, { { 1736, 1674 /* [446](18, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 1674 /* [447](18, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 1674 /* [448](18, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108, 1674 /* [449](18, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 1674 /* [450](18, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 1674 /* [451](18, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1674 /* [452](18, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 450, { { 2604, 1674 /* [453](18, 21) */ } }, { }, 56, std::nullopt },
    { 100, { { 2604, 1674 /* [453](18, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 1674 /* [454](18, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1674 /* [455](18, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { {    0, 1767 /* [456](19,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 1767 /* [457](19,  1) */ } }, { { 29, 50 } }, 56, std::nullopt },
    { 100, { {  248, 1767 /* [458](19,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { {  372, 1767 /* [459](19,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1767 /* [460](19,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 1767 /* [461](19,  5) */ } }, { { 20, 80 }, { 33, 20 } }, 56, std::nullopt },
    { 100, { {  744, 1767 /* [462](19,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 1767 /* [463](19,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 1767 /* [464](19,  8) */ } }, { { 27, 80 } }, std::nullopt, std::nullopt },
    { 200, { { 2604, 1674 /* [453](18, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1767 /* [465](19,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1767 /* [466](19, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 50, { { 1364, 1767 /* [467](19, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 450, { { 1488, 1767 /* [468](19, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 1767 /* [469](19, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 450, { { 1488, 1767 /* [468](19, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1767 /* [466](19, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 1767 /* [470](19, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1767 /* [465](19,  9) */ } }, { { 19, 50 } }, 56, std::nullopt },
    { 100, { { 1860, 1767 /* [471](19, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 1767 /* [472](19, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108, 1767 /* [473](19, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 450, { { 2232, 1767 /* [474](19, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 1767 /* [475](19, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 350, { { 2232, 1767 /* [474](19, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1767 /* [476](19, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1767 /* [477](19, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 1767 /* [478](19, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 60, { { 2852, 1767 /* [479](19, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 60, { {    0, 1860 /* [480](20,  0) */ } }, { }, 54, std::nullopt },
    { 120, { {  124, 1860 /* [481](20,  1) */ } }, { { 51, 60 } }, 54, std::nullopt },
    { 100, { {  248, 1860 /* [482](20,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1860 /* [483](20,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1860 /* [484](20,  4) */ } }, { { 19, 100 } }, 57, std::nullopt },
    { 100, { {  620, 1860 /* [485](20,  5) */ } }, { }, std::nullopt, "11" },
    { 100, { {  744, 1860 /* [486](20,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 1860 /* [487](20,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 1860 /* [488](20,  8) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> CheckingSomethingFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { {  744, 1209 /* [318](13,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 1209 /* [319](13,  7) */ } }, { }, std::nullopt, "11" },
    { 100, { {  992, 1209 /* [320](13,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1209 /* [321](13,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1209 /* [322](13, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 1209 /* [323](13, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488, 1209 /* [324](13, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 1209 /* [325](13, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 1209 /* [326](13, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 1209 /* [327](13, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 1209 /* [328](13, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108, 1209 /* [329](13, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 1209 /* [330](13, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 1209 /* [331](13, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1209 /* [332](13, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 2604, 1209 /* [333](13, 21) */ } }, { }, 37, std::nullopt },
    { 200, { { 2728, 1209 /* [334](13, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1209 /* [335](13, 23) */ } }, { { 16, 75 }, { 28, 5 } }, 37, std::nullopt },
    { 100, { {    0, 1302 /* [336](14,  0) */ } }, { }, std::nullopt, "5" },
    { 100, { {  124, 1302 /* [337](14,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 1302 /* [338](14,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1302 /* [339](14,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1302 /* [340](14,  4) */ } }, { { 16, 80 }, { 28, 5 } }, 37, std::nullopt },
    { 100, { {  620, 1302 /* [341](14,  5) */ } }, { }, std::nullopt, "14" },
    { 100, { {  744, 1302 /* [342](14,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 1302 /* [343](14,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 1302 /* [344](14,  8) */ } }, { { 16, 90 } }, 37, std::nullopt },
    { 100, { { 1116, 1302 /* [345](14,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1302 /* [346](14, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 1302 /* [347](14, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 1200, { { 1488, 1302 /* [348](14, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 1302 /* [347](14, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1302 /* [346](14, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1302 /* [345](14,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 1302 /* [349](14, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 1302 /* [350](14, 14) */ } }, { { 16, 100 } }, std::nullopt, std::nullopt },
    { 100, { { 1860, 1302 /* [351](14, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 1302 /* [352](14, 16) */ } }, { }, std::nullopt, "11" },
    { 100, { { 2108, 1302 /* [353](14, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 1302 /* [354](14, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 1302 /* [355](14, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> GetAttentionFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 26, 50 } }, std::nullopt, "13" },
    { 100, { {  372,  744 /* [195]( 8,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496,  744 /* [196]( 8,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,  744 /* [197]( 8,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,  744 /* [198]( 8,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  744 /* [199]( 8,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  744 /* [200]( 8,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  744 /* [201]( 8,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  744 /* [202]( 8, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  744 /* [203]( 8, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 300, { { 1488,  744 /* [204]( 8, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  744 /* [205]( 8, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  744 /* [206]( 8, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  744 /* [207]( 8, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984,  744 /* [208]( 8, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108,  744 /* [209]( 8, 17) */ } }, { }, std::nullopt, "10" },
    { 100, { { 2232,  744 /* [210]( 8, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356,  744 /* [211]( 8, 19) */ } }, { }, std::nullopt, "10" },
    { 100, { { 2480,  744 /* [212]( 8, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604,  744 /* [213]( 8, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728,  744 /* [214]( 8, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852,  744 /* [215]( 8, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,  837 /* [216]( 9,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124,  837 /* [217]( 9,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248,  837 /* [218]( 9,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372,  837 /* [219]( 9,  3) */ } }, { { 38, 100 } }, 38, std::nullopt },
    { 100, { {  496,  837 /* [220]( 9,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,  837 /* [221]( 9,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,  837 /* [222]( 9,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  837 /* [223]( 9,  7) */ } }, { }, std::nullopt, "4" },
    { 100, { {  992,  837 /* [224]( 9,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  837 /* [225]( 9,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  837 /* [226]( 9, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  837 /* [227]( 9, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  837 /* [228]( 9, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  837 /* [229]( 9, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  837 /* [230]( 9, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  837 /* [231]( 9, 15) */ } }, { }, 38, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> IdleTailWagBFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 95 } }, 16, std::nullopt },
    { 100, { {  124,    0 /*   [1]( 0,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,  558 /* [144]( 6,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124,  558 /* [145]( 6,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248,  558 /* [146]( 6,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372,  558 /* [147]( 6,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496,  558 /* [148]( 6,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,  558 /* [149]( 6,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,  558 /* [150]( 6,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  558 /* [151]( 6,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  558 /* [152]( 6,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  558 /* [153]( 6,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  558 /* [154]( 6, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  558 /* [155]( 6, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  558 /* [156]( 6, 12) */ } }, { }, 16, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> DeepIdleEFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { {  868,  930 /* [247](10,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  930 /* [248](10,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  930 /* [249](10,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  930 /* [250](10, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  930 /* [251](10, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  930 /* [252](10, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  930 /* [253](10, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  930 /* [254](10, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  930 /* [255](10, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984,  930 /* [256](10, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108,  930 /* [257](10, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232,  930 /* [258](10, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356,  930 /* [259](10, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480,  930 /* [260](10, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604,  930 /* [261](10, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728,  930 /* [262](10, 22) */ } }, { }, std::nullopt, "9" },
    { 150, { { 2852,  930 /* [263](10, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { {    0, 1023 /* [264](11,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 350, { {  124, 1023 /* [265](11,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { {    0, 1023 /* [264](11,  0) */ } }, { { 22, 100 } }, std::nullopt, std::nullopt },
    { 100, { { 2852,  930 /* [263](10, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728,  930 /* [262](10, 22) */ } }, { { 24, 100 } }, std::nullopt, std::nullopt },
    { 100, { { 2604,  930 /* [261](10, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480,  930 /* [260](10, 20) */ } }, { { 27, 100 } }, std::nullopt, std::nullopt },
    { 100, { { 2356,  930 /* [259](10, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232,  930 /* [258](10, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108,  930 /* [257](10, 17) */ } }, { { 29, 100 } }, std::nullopt, std::nullopt },
    { 100, { { 1984,  930 /* [256](10, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  930 /* [255](10, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  930 /* [254](10, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  930 /* [253](10, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  930 /* [252](10, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  930 /* [251](10, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  930 /* [250](10, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  930 /* [249](10,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  930 /* [248](10,  8) */ } }, { { 38, 100 } }, 109, std::nullopt },
    { 100, { {  868,  930 /* [247](10,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 1023 /* [280](11, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108, 1023 /* [281](11, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 1023 /* [282](11, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 1023 /* [283](11, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1023 /* [284](11, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1023 /* [285](11, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 1023 /* [286](11, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1023 /* [287](11, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 1116 /* [288](12,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 1116 /* [289](12,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 1116 /* [290](12,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1116 /* [291](12,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1116 /* [292](12,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 1116 /* [293](12,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 1116 /* [294](12,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 1116 /* [295](12,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 1116 /* [296](12,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 2604 /* [680](28,  8) */ } }, { }, 59, std::nullopt },
    { 100, { { 1116, 1116 /* [297](12,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 300, { { 1240, 1116 /* [298](12, 10) */ } }, { }, 59, std::nullopt },
    { 200, { { 1364, 1116 /* [299](12, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 1488, 1116 /* [300](12, 12) */ } }, { }, 98, std::nullopt },
    { 100, { { 1612, 1116 /* [301](12, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 1736, 1116 /* [302](12, 14) */ } }, { }, 59, std::nullopt },
    { 300, { { 1860, 1116 /* [303](12, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 1612, 1116 /* [301](12, 13) */ } }, { }, 59, std::nullopt },
    { 200, { { 1488, 1116 /* [300](12, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 1364, 1116 /* [299](12, 11) */ } }, { }, 59, std::nullopt },
    { 200, { { 1984, 1116 /* [304](12, 16) */ } }, { { 58, 93 }, { 98, 1 } }, 98, std::nullopt },
    { 100, { { 1116, 2604 /* [681](28,  9) */ } }, { }, 59, std::nullopt },
    { 100, { { 1240, 2604 /* [682](28, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 2604 /* [683](28, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488, 2604 /* [684](28, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 2604 /* [685](28, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 2604 /* [686](28, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 2604 /* [687](28, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 2604 /* [688](28, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108, 2604 /* [689](28, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 2604 /* [690](28, 18) */ } }, { }, 87, std::nullopt },
    { 300, { { 2356, 2604 /* [691](28, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 2480, 2604 /* [692](28, 20) */ } }, { }, 87, std::nullopt },
    { 200, { { 2604, 2604 /* [693](28, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 2728, 2604 /* [694](28, 22) */ } }, { }, 87, std::nullopt },
    { 200, { { 2852, 2604 /* [695](28, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 300, { {    0, 2697 /* [696](29,  0) */ } }, { }, 87, std::nullopt },
    { 200, { { 2728, 2604 /* [694](28, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 2604, 2604 /* [693](28, 21) */ } }, { }, 87, std::nullopt },
    { 200, { { 2480, 2604 /* [692](28, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { {  124, 2697 /* [697](29,  1) */ } }, { { 77, 95 } }, 87, std::nullopt },
    { 200, { {  248, 2697 /* [698](29,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { {  372, 2697 /* [699](29,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 2697 /* [700](29,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 2697 /* [701](29,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 2697 /* [702](29,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 2697 /* [703](29,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 2697 /* [704](29,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 2697 /* [705](29,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 2697 /* [706](29, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 2697 /* [707](29, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488, 2697 /* [708](29, 12) */ } }, { { 111, 100 } }, std::nullopt, std::nullopt },
    { 100, { { 2108, 1116 /* [305](12, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 1116 /* [306](12, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 1116 /* [307](12, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1116 /* [308](12, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1116 /* [309](12, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 1116 /* [310](12, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1116 /* [311](12, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 1209 /* [312](13,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 1209 /* [313](13,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 1209 /* [314](13,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1209 /* [315](13,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1209 /* [316](13,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 1209 /* [317](13,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> IdleCleaningFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 95 } }, 30, std::nullopt },
    { 100, { {  124,    0 /*   [1]( 0,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1860 /* [489](20,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1860 /* [490](20, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1364, 1860 /* [491](20, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1488, 1860 /* [492](20, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1612, 1860 /* [493](20, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1736, 1860 /* [494](20, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1860, 1860 /* [495](20, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1984, 1860 /* [496](20, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 2108, 1860 /* [497](20, 17) */ } }, { { 13, 25 } }, 27, std::nullopt },
    { 150, { { 2232, 1860 /* [498](20, 18) */ } }, { { 5, 100 } }, 27, std::nullopt },
    { 100, { { 2356, 1860 /* [499](20, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1860 /* [500](20, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1860 /* [501](20, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 1860 /* [502](20, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1860 /* [503](20, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 1953 /* [504](21,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 1953 /* [505](21,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 1953 /* [506](21,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1953 /* [507](21,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1953 /* [508](21,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 1953 /* [509](21,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 1953 /* [510](21,  6) */ } }, { { 28, 25 } }, 28, std::nullopt },
    { 100, { {  868, 1953 /* [511](21,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 1953 /* [512](21,  8) */ } }, { { 17, 70 } }, 27, std::nullopt },
    { 100, { { 1116, 1953 /* [513](21,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1953 /* [514](21, 10) */ } }, { { 5, 50 } }, 29, std::nullopt },
    { 100, { { 1364, 1953 /* [515](21, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> LookLeftFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 6, std::nullopt },
    { 100, { {  248, 1488 /* [386](16,  2) */ } }, { }, 5, std::nullopt },
    { 100, { {  372, 1488 /* [387](16,  3) */ } }, { }, 4, std::nullopt },
    { 600, { {  496, 1488 /* [388](16,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1488 /* [387](16,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 1488 /* [386](16,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> LookDownRightFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 6, std::nullopt },
    { 100, { { 1488, 1953 /* [516](21, 12) */ } }, { }, 5, std::nullopt },
    { 100, { { 1612, 1953 /* [517](21, 13) */ } }, { }, 4, std::nullopt },
    { 600, { { 1736, 1953 /* [518](21, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 1953 /* [517](21, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488, 1953 /* [516](21, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> IdleTailWagAFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 95 } }, 13, std::nullopt },
    { 100, { {  124,    0 /*   [1]( 0,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248,    0 /*   [2]( 0,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372,    0 /*   [3]( 0,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496,    0 /*   [4]( 0,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,    0 /*   [5]( 0,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,    0 /*   [6]( 0,  6) */ } }, { }, 9, std::nullopt },
    { 100, { {  868,    0 /*   [7]( 0,  7) */ } }, { { 8, 95 } }, 9, std::nullopt },
    { 100, { {  992,    0 /*   [8]( 0,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,    0 /*   [9]( 0,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,    0 /*  [10]( 0, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,    0 /*  [11]( 0, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> IdleBlinkFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 95 } }, 2, std::nullopt },
    { 100, { {  124,    0 /*   [1]( 0,  1) */ } }, { { 0, 95 } }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> SearchingFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { {  372, 2139 /* [555](23,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 2139 /* [556](23,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 2139 /* [557](23,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 2139 /* [558](23,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 2139 /* [559](23,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 2139 /* [560](23,  8) */ } }, { }, std::nullopt, "11" },
    { 100, { { 1116, 2139 /* [561](23,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 2139 /* [562](23, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 2139 /* [563](23, 11) */ } }, { }, 11, std::nullopt },
    { 300, { { 1488, 2139 /* [564](23, 12) */ } }, { { 10, 95 } }, 11, std::nullopt },
    { 100, { { 1364, 2139 /* [563](23, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 2139 /* [562](23, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 2139 /* [561](23,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 2139 /* [565](23, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 2139 /* [566](23, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 300, { { 1860, 2139 /* [567](23, 15) */ } }, { { 16, 75 } }, 17, std::nullopt },
    { 100, { { 1984, 2139 /* [568](23, 16) */ } }, { { 16, 15 } }, 18, std::nullopt },
    { 300, { { 2108, 2139 /* [569](23, 17) */ } }, { { 18, 70 } }, 23, std::nullopt },
    { 100, { { 1736, 2139 /* [566](23, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 2139 /* [565](23, 13) */ } }, { }, std::nullopt, "11" },
    { 100, { { 1116, 2139 /* [561](23,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 2139 /* [562](23, 10) */ } }, { { 9, 100 } }, std::nullopt, std::nullopt },
    { 100, { { 2232, 2139 /* [570](23, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 2139 /* [571](23, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> IdleLegLickFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 95 } }, 19, std::nullopt },
    { 100, { {  124,    0 /*   [1]( 0,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1364, 2046 /* [539](22, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1488, 2046 /* [540](22, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1612, 2046 /* [541](22, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1736, 2046 /* [542](22, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1860, 2046 /* [543](22, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1984, 2046 /* [544](22, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 2108, 2046 /* [545](22, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 2232, 2046 /* [546](22, 18) */ } }, { { 14, 20 } }, 14, std::nullopt },
    { 150, { { 2356, 2046 /* [547](22, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 2480, 2046 /* [548](22, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 2604, 2046 /* [549](22, 21) */ } }, { { 7, 100 } }, std::nullopt, std::nullopt },
    { 150, { { 2728, 2046 /* [550](22, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 2852, 2046 /* [551](22, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { {    0, 2139 /* [552](23,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { {  124, 2139 /* [553](23,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { {  248, 2139 /* [554](23,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> IdleTwitchFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 95 } }, 10, std::nullopt },
    { 100, { {  124,    0 /*   [1]( 0,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 10, std::nullopt },
    { 100, { { 2728, 1395 /* [382](15, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1302 /* [356](14, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1302 /* [357](14, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 1302 /* [358](14, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1302 /* [357](14, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1302 /* [356](14, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 1395 /* [382](15, 22) */ } }, { }, 10, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> HearingFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { {  496,  837 /* [220]( 9,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,  837 /* [221]( 9,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,  837 /* [222]( 9,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  837 /* [223]( 9,  7) */ } }, { }, std::nullopt, "1" },
    { 100, { {  992,  837 /* [224]( 9,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  837 /* [225]( 9,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  837 /* [226]( 9, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  837 /* [227]( 9, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  837 /* [228]( 9, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  837 /* [229]( 9, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  837 /* [230]( 9, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  837 /* [231]( 9, 15) */ } }, { }, 13, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> ShowFrames = {
    { 10, { }, { }, std::nullopt, std::nullopt },
    { 10, { { 1364,  186 /*  [59]( 2, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 10, { { 1240,  186 /*  [58]( 2, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 10, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> GetTechyFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { { 2356,  837 /* [235]( 9, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480,  837 /* [236]( 9, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604,  837 /* [237]( 9, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728,  837 /* [238]( 9, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852,  837 /* [239]( 9, 23) */ } }, { { 7, 50 } }, 8, std::nullopt },
    { 100, { {    0,  930 /* [240](10,  0) */ } }, { { 5, 50 } }, 8, std::nullopt },
    { 100, { {  124,  930 /* [241](10,  1) */ } }, { { 5, 50 }, { 6, 50 } }, 8, std::nullopt },
    { 100, { {  248,  930 /* [242](10,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> LookUpRightFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 6, std::nullopt },
    { 100, { { 1984,  837 /* [232]( 9, 16) */ } }, { }, 5, std::nullopt },
    { 100, { { 2108,  837 /* [233]( 9, 17) */ } }, { }, 4, std::nullopt },
    { 600, { { 2232,  837 /* [234]( 9, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108,  837 /* [233]( 9, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984,  837 /* [232]( 9, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> SendMailFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { {    0,  279 /*  [72]( 3,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124,  279 /*  [73]( 3,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248,  279 /*  [74]( 3,  2) */ } }, { }, std::nullopt, "11" },
    { 100, { {  372,  279 /*  [75]( 3,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496,  279 /*  [76]( 3,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,  279 /*  [77]( 3,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,  279 /*  [78]( 3,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  279 /*  [79]( 3,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  279 /*  [80]( 3,  8) */ } }, { { 7, 50 } }, 10, std::nullopt },
    { 100, { { 1116,  279 /*  [81]( 3,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  279 /*  [82]( 3, 10) */ } }, { { 11, 50 } }, 12, std::nullopt },
    { 100, { { 1364,  279 /*  [83]( 3, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  279 /*  [84]( 3, 12) */ } }, { { 7, 50 } }, 14, std::nullopt },
    { 100, { { 1612,  279 /*  [85]( 3, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  279 /*  [86]( 3, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  279 /*  [87]( 3, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984,  279 /*  [88]( 3, 16) */ } }, { }, std::nullopt, "2" },
    { 100, { { 2108,  279 /*  [89]( 3, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232,  279 /*  [90]( 3, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356,  279 /*  [91]( 3, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480,  279 /*  [92]( 3, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604,  279 /*  [93]( 3, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728,  279 /*  [94]( 3, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852,  279 /*  [95]( 3, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,  372 /*  [96]( 4,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124,  372 /*  [97]( 4,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248,  372 /*  [98]( 4,  2) */ } }, { }, std::nullopt, "7" },
    { 100, { {  372,  372 /*  [99]( 4,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496,  372 /* [100]( 4,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,  372 /* [101]( 4,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,  372 /* [102]( 4,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  372 /* [103]( 4,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  372 /* [104]( 4,  8) */ } }, { }, 34, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> IdleButterFlyFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 95 } }, 26, std::nullopt },
    { 100, { {  124,    0 /*   [1]( 0,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 26, 70 } }, 26, std::nullopt },
    { 800, { { 2852, 1302 /* [359](14, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 1395 /* [360](15,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 1395 /* [361](15,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 1395 /* [362](15,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1395 /* [363](15,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1395 /* [364](15,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 1395 /* [365](15,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 1395 /* [366](15,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 1395 /* [367](15,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 1395 /* [368](15,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1395 /* [369](15,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1395 /* [370](15, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 1395 /* [371](15, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488, 1395 /* [372](15, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 1395 /* [373](15, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 1395 /* [374](15, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 1395 /* [375](15, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 1395 /* [376](15, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108, 1395 /* [377](15, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 1395 /* [378](15, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 1395 /* [379](15, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1395 /* [380](15, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1395 /* [381](15, 21) */ } }, { }, 26, std::nullopt },
    { 100, { { 1488,    0 /*  [12]( 0, 12) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> IdleStretchFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  930 /* [247](10,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  930 /* [248](10,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  930 /* [249](10,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  930 /* [250](10, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  930 /* [251](10, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  930 /* [252](10, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  930 /* [253](10, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  930 /* [254](10, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  930 /* [255](10, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984,  930 /* [256](10, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108,  930 /* [257](10, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232,  930 /* [258](10, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356,  930 /* [259](10, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480,  930 /* [260](10, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604,  930 /* [261](10, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728,  930 /* [262](10, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 2852,  930 /* [263](10, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { {    0, 1023 /* [264](11,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 350, { {  124, 1023 /* [265](11,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { {    0, 1023 /* [264](11,  0) */ } }, { }, 21, std::nullopt },
    { 100, { { 2728,  930 /* [262](10, 22) */ } }, { }, 23, std::nullopt },
    { 100, { { 2604,  930 /* [261](10, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480,  930 /* [260](10, 20) */ } }, { }, 24, std::nullopt },
    { 100, { { 2108,  930 /* [257](10, 17) */ } }, { }, 25, std::nullopt },
    { 100, { { 1860,  930 /* [255](10, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  930 /* [254](10, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  930 /* [253](10, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  930 /* [252](10, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  930 /* [251](10, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  930 /* [250](10, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  930 /* [249](10,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  930 /* [248](10,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  930 /* [247](10,  7) */ } }, { }, 34, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> SaveFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { { 1364,  372 /* [107]( 4, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  372 /* [108]( 4, 12) */ } }, { }, std::nullopt, "19" },
    { 100, { { 1612,  372 /* [109]( 4, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  372 /* [110]( 4, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  372 /* [111]( 4, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984,  372 /* [112]( 4, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108,  372 /* [113]( 4, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232,  372 /* [114]( 4, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356,  372 /* [115]( 4, 19) */ } }, { }, 34, std::nullopt },
    { 100, { { 2480,  372 /* [116]( 4, 20) */ } }, { }, std::nullopt, "20" },
    { 100, { { 2604,  372 /* [117]( 4, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728,  372 /* [118]( 4, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852,  372 /* [119]( 4, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,  465 /* [120]( 5,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124,  465 /* [121]( 5,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248,  465 /* [122]( 5,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372,  465 /* [123]( 5,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496,  465 /* [124]( 5,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,  465 /* [125]( 5,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,  465 /* [126]( 5,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  465 /* [127]( 5,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  465 /* [128]( 5,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  465 /* [129]( 5,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  465 /* [130]( 5, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  465 /* [131]( 5, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  465 /* [132]( 5, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  465 /* [133]( 5, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  465 /* [134]( 5, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  465 /* [135]( 5, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984,  465 /* [136]( 5, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108,  465 /* [137]( 5, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232,  465 /* [138]( 5, 18) */ } }, { }, std::nullopt, "21" },
    { 100, { { 2356,  465 /* [139]( 5, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480,  465 /* [140]( 5, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604,  465 /* [141]( 5, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728,  465 /* [142]( 5, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852,  465 /* [143]( 5, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> GestureDownFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 6, std::nullopt },
    { 100, { { 2852, 1395 /* [383](15, 23) */ } }, { }, 5, std::nullopt },
    { 100, { {    0, 1488 /* [384](16,  0) */ } }, { }, 4, std::nullopt },
    { 600, { {  124, 1488 /* [385](16,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 1488 /* [384](16,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1395 /* [383](15, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> EmptyTrashFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { { 1364, 2325 /* [611](25, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488, 2325 /* [612](25, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 2325 /* [613](25, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 2325 /* [614](25, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 2325 /* [615](25, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 2325 /* [616](25, 16) */ } }, { }, std::nullopt, "1" },
    { 100, { { 2108, 2325 /* [617](25, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 2325 /* [618](25, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 2325 /* [619](25, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 2325 /* [620](25, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 2325 /* [621](25, 21) */ } }, { }, std::nullopt, "6" },
    { 100, { { 2728, 2325 /* [622](25, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 2325 /* [623](25, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 2418 /* [624](26,  0) */ } }, { }, 20, std::nullopt },
    { 100, { {  124, 2418 /* [625](26,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 2418 /* [626](26,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 2418 /* [627](26,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 2418 /* [628](26,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 2418 /* [629](26,  5) */ } }, { }, 20, std::nullopt },
    { 100, { {  744, 2418 /* [630](26,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 2418 /* [631](26,  7) */ } }, { }, std::nullopt, "2" },
    { 100, { {  992, 2418 /* [632](26,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 2418 /* [633](26,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 2418 /* [634](26, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 2418 /* [635](26, 11) */ } }, { }, 28, std::nullopt },
    { 100, { { 1488, 2418 /* [636](26, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 2418 /* [637](26, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 2418 /* [638](26, 14) */ } }, { }, 29, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> WaveFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 27, 50 } }, std::nullopt, "13" },
    { 100, { {  992, 1488 /* [392](16,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1488 /* [393](16,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1488 /* [394](16, 10) */ } }, { }, std::nullopt, "17" },
    { 100, { { 1364, 1488 /* [395](16, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488, 1488 /* [396](16, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 1488 /* [397](16, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 1488 /* [398](16, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 1488 /* [399](16, 15) */ } }, { }, std::nullopt, "17" },
    { 100, { { 1984, 1488 /* [400](16, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108, 1488 /* [401](16, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 1488 /* [402](16, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 1488 /* [403](16, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1488 /* [404](16, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1488 /* [405](16, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 1488 /* [406](16, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1488 /* [407](16, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 1581 /* [408](17,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 1581 /* [409](17,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 1581 /* [410](17,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1581 /* [411](17,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1581 /* [412](17,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 1581 /* [413](17,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 1581 /* [414](17,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 1581 /* [415](17,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 1581 /* [416](17,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1581 /* [417](17,  9) */ } }, { { 44, 100 } }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1581 /* [418](17, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 1581 /* [419](17, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488, 1581 /* [420](17, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 1581 /* [421](17, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 1581 /* [422](17, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 1581 /* [423](17, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 1581 /* [424](17, 16) */ } }, { }, std::nullopt, "2" },
    { 100, { { 2108, 1581 /* [425](17, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 1581 /* [426](17, 18) */ } }, { }, std::nullopt, "2" },
    { 450, { { 2356, 1581 /* [427](17, 19) */ } }, { }, std::nullopt, "9" },
    { 100, { { 2480, 1581 /* [428](17, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 1200, { { 2356, 1581 /* [427](17, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1581 /* [429](17, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 1581 /* [430](17, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1581 /* [431](17, 23) */ } }, { }, std::nullopt, "3" },
    { 100, { {    0, 1674 /* [432](18,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 1674 /* [433](18,  1) */ } }, { }, 44, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> GestureRightFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 6, std::nullopt },
    { 100, { {  372,  930 /* [243](10,  3) */ } }, { }, 5, std::nullopt },
    { 100, { {  496,  930 /* [244](10,  4) */ } }, { }, 4, std::nullopt },
    { 600, { {  620,  930 /* [245](10,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496,  930 /* [244](10,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372,  930 /* [243](10,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> ExplainFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248,    0 /*   [2]( 0,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372,    0 /*   [3]( 0,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496,    0 /*   [4]( 0,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,    0 /*   [5]( 0,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,    0 /*   [6]( 0,  6) */ } }, { }, 6, std::nullopt },
    { 100, { {  992,    0 /*   [8]( 0,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,    0 /*   [9]( 0,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,    0 /*  [10]( 0, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,    0 /*  [11]( 0, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> LookDownLeftFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 6, std::nullopt },
    { 100, { {  496, 2790 /* [724](30,  4) */ } }, { }, 5, std::nullopt },
    { 100, { {  620, 2790 /* [725](30,  5) */ } }, { }, 4, std::nullopt },
    { 600, { {  744, 2790 /* [726](30,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 2790 /* [725](30,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 2790 /* [724](30,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> Idle1Frames = {
    { 200, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 95 } }, 131, std::nullopt },
    { 120, { {  124,    0 /*   [1]( 0,  1) */ } }, { { 0, 80 } }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 15, 50 }, { 29, 20 }, { 41, 15 } }, std::nullopt, std::nullopt },
    { 0, { }, { { 74, 10 }, { 89, 5 } }, std::nullopt, std::nullopt },
    { 100, { {  248,    0 /*   [2]( 0,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372,    0 /*   [3]( 0,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496,    0 /*   [4]( 0,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,    0 /*   [5]( 0,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,    0 /*   [6]( 0,  6) */ } }, { }, 10, std::nullopt },
    { 100, { {  868,    0 /*   [7]( 0,  7) */ } }, { { 9, 95 } }, 10, std::nullopt },
    { 100, { {  992,    0 /*   [8]( 0,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,    0 /*   [9]( 0,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,    0 /*  [10]( 0, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,    0 /*  [11]( 0, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 87 } }, 28, std::nullopt },
    { 100, { {    0,  558 /* [144]( 6,  0) */ } }, { }, 132, std::nullopt },
    { 100, { {  124,  558 /* [145]( 6,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248,  558 /* [146]( 6,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372,  558 /* [147]( 6,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496,  558 /* [148]( 6,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,  558 /* [149]( 6,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,  558 /* [150]( 6,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  558 /* [151]( 6,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  558 /* [152]( 6,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  558 /* [153]( 6,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  558 /* [154]( 6, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  558 /* [155]( 6, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  558 /* [156]( 6, 12) */ } }, { { 15, 70 } }, 28, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 98 } }, 132, std::nullopt },
    { 100, { { 2852,   93 /*  [47]( 1, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,  186 /*  [48]( 2,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124,  186 /*  [49]( 2,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248,  186 /*  [50]( 2,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372,  186 /*  [51]( 2,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496,  186 /*  [52]( 2,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,  186 /*  [53]( 2,  5) */ } }, { { 31, 90 } }, 11, std::nullopt },
    { 100, { {  744,  186 /*  [54]( 2,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  186 /*  [55]( 2,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  186 /*  [56]( 2,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  186 /*  [57]( 2,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 98 } }, 132, std::nullopt },
    { 100, { {  868,  930 /* [247](10,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  930 /* [248](10,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  930 /* [249](10,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  930 /* [250](10, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  930 /* [251](10, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  930 /* [252](10, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  930 /* [253](10, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  930 /* [254](10, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  930 /* [255](10, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984,  930 /* [256](10, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108,  930 /* [257](10, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232,  930 /* [258](10, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356,  930 /* [259](10, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480,  930 /* [260](10, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604,  930 /* [261](10, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728,  930 /* [262](10, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 2852,  930 /* [263](10, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { {    0, 1023 /* [264](11,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 350, { {  124, 1023 /* [265](11,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { {    0, 1023 /* [264](11,  0) */ } }, { }, 65, std::nullopt },
    { 100, { { 2728,  930 /* [262](10, 22) */ } }, { }, 65, std::nullopt },
    { 100, { { 2480,  930 /* [260](10, 20) */ } }, { }, 65, std::nullopt },
    { 100, { { 2108,  930 /* [257](10, 17) */ } }, { }, 65, std::nullopt },
    { 100, { { 1860,  930 /* [255](10, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  930 /* [254](10, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  930 /* [253](10, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  930 /* [252](10, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  930 /* [251](10, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  930 /* [250](10, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  930 /* [249](10,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  930 /* [248](10,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  930 /* [247](10,  7) */ } }, { }, 132, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 95 } }, 132, std::nullopt },
    { 100, { {  248, 1023 /* [266](11,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1023 /* [267](11,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1023 /* [268](11,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 1023 /* [269](11,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 1023 /* [270](11,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 1023 /* [271](11,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 1023 /* [272](11,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1023 /* [273](11,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1023 /* [274](11, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 1023 /* [275](11, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488, 1023 /* [276](11, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 1023 /* [277](11, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 1023 /* [278](11, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 1023 /* [279](11, 15) */ } }, { }, 132, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 90 }, { 89, 5 } }, 132, std::nullopt },
    { 100, { { 1984, 1023 /* [280](11, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108, 1023 /* [281](11, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 1023 /* [282](11, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 1023 /* [283](11, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1023 /* [284](11, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1023 /* [285](11, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 1023 /* [286](11, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1023 /* [287](11, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 1116 /* [288](12,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 1116 /* [289](12,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 1116 /* [290](12,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1116 /* [291](12,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1116 /* [292](12,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 1116 /* [293](12,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 1116 /* [294](12,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 1116 /* [295](12,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 1116 /* [296](12,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 1116 /* [296](12,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1116 /* [297](12,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 300, { { 1240, 1116 /* [298](12, 10) */ } }, { }, 118, std::nullopt },
    { 200, { { 1364, 1116 /* [299](12, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 1488, 1116 /* [300](12, 12) */ } }, { }, 118, std::nullopt },
    { 100, { { 1612, 1116 /* [301](12, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 1736, 1116 /* [302](12, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 300, { { 1860, 1116 /* [303](12, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 1612, 1116 /* [301](12, 13) */ } }, { }, 118, std::nullopt },
    { 200, { { 1488, 1116 /* [300](12, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 1364, 1116 /* [299](12, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 1984, 1116 /* [304](12, 16) */ } }, { { 109, 93 } }, 118, std::nullopt },
    { 100, { { 2108, 1116 /* [305](12, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 1116 /* [306](12, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 1116 /* [307](12, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1116 /* [308](12, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1116 /* [309](12, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 1116 /* [310](12, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1116 /* [311](12, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 1209 /* [312](13,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 1209 /* [313](13,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 1209 /* [314](13,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1209 /* [315](13,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1209 /* [316](13,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 1209 /* [317](13,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 100 } }, 132, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> LookUpFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 6, std::nullopt },
    { 100, { { 1116,  372 /* [105]( 4,  9) */ } }, { }, 5, std::nullopt },
    { 100, { { 1240,  372 /* [106]( 4, 10) */ } }, { }, 4, std::nullopt },
    { 600, { {  744,  930 /* [246](10,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  372 /* [106]( 4, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  372 /* [105]( 4,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> CongratulateFrames = {
    { 100, { { 1488,    0 /*  [12]( 0, 12) */ } }, { }, std::nullopt, "13" },
    { 150, { { 1612,    0 /*  [13]( 0, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1736,    0 /*  [14]( 0, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,    0 /*  [15]( 0, 15) */ } }, { }, 16, std::nullopt },
    { 100, { { 1984,    0 /*  [16]( 0, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108,    0 /*  [17]( 0, 17) */ } }, { }, 16, std::nullopt },
    { 100, { { 2232,    0 /*  [18]( 0, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356,    0 /*  [19]( 0, 19) */ } }, { }, 16, std::nullopt },
    { 100, { { 2480,    0 /*  [20]( 0, 20) */ } }, { }, std::nullopt, "17" },
    { 100, { { 2604,    0 /*  [21]( 0, 21) */ } }, { }, 16, std::nullopt },
    { 100, { { 2728,    0 /*  [22]( 0, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852,    0 /*  [23]( 0, 23) */ } }, { }, 16, std::nullopt },
    { 100, { {    0,   93 /*  [24]( 1,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124,   93 /*  [25]( 1,  1) */ } }, { }, 16, std::nullopt },
    { 100, { {  248,   93 /*  [26]( 1,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372,   93 /*  [27]( 1,  3) */ } }, { { 9, 65 } }, 16, std::nullopt },
    { 100, { {  496,   93 /*  [28]( 1,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,   93 /*  [29]( 1,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,   93 /*  [30]( 1,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,   93 /*  [31]( 1,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,   93 /*  [32]( 1,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,   93 /*  [33]( 1,  9) */ } }, { }, 22, std::nullopt },
    { 200, { { 1488,    0 /*  [12]( 0, 12) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> ThinkingFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 4, std::nullopt },
    { 100, { { 1116,  372 /* [105]( 4,  9) */ } }, { }, 3, std::nullopt },
    { 100, { { 1240,  372 /* [106]( 4, 10) */ } }, { { 2, 100 } }, 3, std::nullopt },
    { 100, { { 1116,  372 /* [105]( 4,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> LookUpLeftFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 6, std::nullopt },
    { 100, { {  620, 1488 /* [389](16,  5) */ } }, { }, 5, std::nullopt },
    { 100, { {  744, 1488 /* [390](16,  6) */ } }, { }, 4, std::nullopt },
    { 600, { {  868, 1488 /* [391](16,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 1488 /* [390](16,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 1488 /* [389](16,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> IdleScratchFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 95 } }, 14, std::nullopt },
    { 100, { {  124,    0 /*   [1]( 0,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852,   93 /*  [47]( 1, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,  186 /*  [48]( 2,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124,  186 /*  [49]( 2,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248,  186 /*  [50]( 2,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372,  186 /*  [51]( 2,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496,  186 /*  [52]( 2,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,  186 /*  [53]( 2,  5) */ } }, { { 5, 90 } }, 10, std::nullopt },
    { 100, { {  744,  186 /*  [54]( 2,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  186 /*  [55]( 2,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  186 /*  [56]( 2,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  186 /*  [57]( 2,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> ProcessingFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { {  744, 1209 /* [318](13,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 1209 /* [319](13,  7) */ } }, { }, std::nullopt, "11" },
    { 100, { {  992, 1209 /* [320](13,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1209 /* [321](13,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1209 /* [322](13, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 1209 /* [323](13, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488, 1209 /* [324](13, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 1209 /* [325](13, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 1209 /* [326](13, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 1209 /* [327](13, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 1209 /* [328](13, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108, 1209 /* [329](13, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 1209 /* [330](13, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 1209 /* [331](13, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1209 /* [332](13, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 2604, 1209 /* [333](13, 21) */ } }, { }, 37, std::nullopt },
    { 200, { { 2728, 1209 /* [334](13, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1209 /* [335](13, 23) */ } }, { { 16, 75 }, { 28, 5 } }, 37, std::nullopt },
    { 100, { {    0, 1302 /* [336](14,  0) */ } }, { }, std::nullopt, "5" },
    { 100, { {  124, 1302 /* [337](14,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 1302 /* [338](14,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1302 /* [339](14,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 1302 /* [340](14,  4) */ } }, { { 16, 80 }, { 28, 5 } }, 37, std::nullopt },
    { 100, { {  620, 1302 /* [341](14,  5) */ } }, { }, std::nullopt, "14" },
    { 100, { {  744, 1302 /* [342](14,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 1302 /* [343](14,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 1302 /* [344](14,  8) */ } }, { { 16, 90 } }, 37, std::nullopt },
    { 100, { { 1116, 1302 /* [345](14,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1302 /* [346](14, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 1302 /* [347](14, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 1200, { { 1488, 1302 /* [348](14, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 1302 /* [347](14, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 1302 /* [346](14, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 1302 /* [345](14,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 1302 /* [349](14, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 1302 /* [350](14, 14) */ } }, { { 16, 100 } }, std::nullopt, std::nullopt },
    { 100, { { 1860, 1302 /* [351](14, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 1302 /* [352](14, 16) */ } }, { }, std::nullopt, "11" },
    { 100, { { 2108, 1302 /* [353](14, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 1302 /* [354](14, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 1302 /* [355](14, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> AlertFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 6, std::nullopt },
    { 100, { { 2480, 1302 /* [356](14, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1302 /* [357](14, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 1302 /* [358](14, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1302 /* [357](14, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1302 /* [356](14, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> GetWizardyFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { {  496,  837 /* [220]( 9,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,  837 /* [221]( 9,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,  837 /* [222]( 9,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  837 /* [223]( 9,  7) */ } }, { }, std::nullopt, "4" },
    { 100, { {  992,  837 /* [224]( 9,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  837 /* [225]( 9,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  837 /* [226]( 9, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  837 /* [227]( 9, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  837 /* [228]( 9, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  837 /* [229]( 9, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  837 /* [230]( 9, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  837 /* [231]( 9, 15) */ } }, { }, 13, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> HideFrames = {
    { 10, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 10, { { 1240,  186 /*  [58]( 2, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 10, { { 1364,  186 /*  [59]( 2, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 10, { }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> DeepIdleAFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { {  868,  930 /* [247](10,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  930 /* [248](10,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  930 /* [249](10,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  930 /* [250](10, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  930 /* [251](10, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  930 /* [252](10, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  930 /* [253](10, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  930 /* [254](10, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  930 /* [255](10, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984,  930 /* [256](10, 16) */ } }, { }, std::nullopt, "9" },
    { 100, { { 2108,  930 /* [257](10, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232,  930 /* [258](10, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356,  930 /* [259](10, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480,  930 /* [260](10, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604,  930 /* [261](10, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728,  930 /* [262](10, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852,  930 /* [263](10, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 1023 /* [264](11,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 1023 /* [265](11,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 2139 /* [572](23, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 2139 /* [573](23, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 2139 /* [574](23, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 2139 /* [575](23, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 2232 /* [576](24,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 2232 /* [577](24,  1) */ } }, { }, 51, std::nullopt },
    { 100, { {  248, 2232 /* [578](24,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 2232 /* [579](24,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 2232 /* [580](24,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 2232 /* [581](24,  5) */ } }, { }, 51, std::nullopt },
    { 300, { {  744, 2232 /* [582](24,  6) */ } }, { }, 51, std::nullopt },
    { 200, { {  868, 2232 /* [583](24,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { {  992, 2232 /* [584](24,  8) */ } }, { }, 51, std::nullopt },
    { 200, { { 1116, 2232 /* [585](24,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 1240, 2232 /* [586](24, 10) */ } }, { }, 51, std::nullopt },
    { 300, { { 1364, 2232 /* [587](24, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { { 1116, 2232 /* [585](24,  9) */ } }, { }, 51, std::nullopt },
    { 200, { {  992, 2232 /* [584](24,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 200, { {  868, 2232 /* [583](24,  7) */ } }, { }, 51, std::nullopt },
    { 200, { {  744, 2232 /* [582](24,  6) */ } }, { { 30, 95 } }, std::nullopt, std::nullopt },
    { 100, { { 1488, 2232 /* [588](24, 12) */ } }, { }, 51, std::nullopt },
    { 100, { { 1612, 2232 /* [589](24, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 2232 /* [590](24, 14) */ } }, { }, 51, std::nullopt },
    { 100, { { 1860, 2232 /* [591](24, 15) */ } }, { }, 51, std::nullopt },
    { 100, { { 1984, 2232 /* [592](24, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108, 2232 /* [593](24, 17) */ } }, { { 45, 85 } }, 51, std::nullopt },
    { 100, { { 2232, 2232 /* [594](24, 18) */ } }, { { 45, 70 } }, std::nullopt, std::nullopt },
    { 100, { { 2356, 2232 /* [595](24, 19) */ } }, { }, 51, std::nullopt },
    { 100, { { 2480, 2232 /* [596](24, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 2232 /* [597](24, 21) */ } }, { { 30, 95 } }, 51, std::nullopt },
    { 100, { { 2728, 2232 /* [598](24, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 2232 /* [599](24, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 2325 /* [600](25,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 2325 /* [601](25,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 2325 /* [602](25,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 2325 /* [603](25,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 2325 /* [604](25,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 2325 /* [605](25,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 2325 /* [606](25,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 2325 /* [607](25,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 2325 /* [608](25,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 2325 /* [609](25,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 2325 /* [610](25, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> GetArtsyFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { { 1240, 1581 /* [418](17, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 1581 /* [419](17, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488, 1581 /* [420](17, 12) */ } }, { }, std::nullopt, "1" },
    { 100, { { 1612, 1581 /* [421](17, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 1581 /* [422](17, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 1581 /* [423](17, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 1581 /* [424](17, 16) */ } }, { }, std::nullopt, "2" },
    { 100, { { 2108, 1581 /* [425](17, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 1953 /* [519](21, 15) */ } }, { }, std::nullopt, "2" },
    { 100, { { 1984, 1953 /* [520](21, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108, 1953 /* [521](21, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 1953 /* [522](21, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 1953 /* [523](21, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 1953 /* [524](21, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1953 /* [525](21, 21) */ } }, { }, std::nullopt, "2" },
    { 100, { { 2728, 1953 /* [526](21, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1953 /* [527](21, 23) */ } }, { }, std::nullopt, "2" },
    { 100, { {    0, 2046 /* [528](22,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 2046 /* [529](22,  1) */ } }, { }, std::nullopt, "2" },
    { 100, { {  248, 2046 /* [530](22,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 2046 /* [531](22,  3) */ } }, { }, std::nullopt, "2" },
    { 100, { {  496, 2046 /* [532](22,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 2046 /* [533](22,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 2046 /* [534](22,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 2046 /* [535](22,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 2046 /* [536](22,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 2046 /* [537](22,  9) */ } }, { }, std::nullopt, "9" },
    { 2000, { { 1240, 2046 /* [538](22, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 1581 /* [429](17, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 1581 /* [430](17, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 1581 /* [431](17, 23) */ } }, { }, std::nullopt, "3" },
    { 100, { {    0, 1674 /* [432](18,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 1674 /* [433](18,  1) */ } }, { }, 34, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> GoodByeFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, "13" },
    { 100, { { 1612, 2697 /* [709](29, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 2697 /* [710](29, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 2697 /* [711](29, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 2697 /* [712](29, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108, 2697 /* [713](29, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 2697 /* [714](29, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 2697 /* [715](29, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 2697 /* [716](29, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 2697 /* [717](29, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 2697 /* [718](29, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 2697 /* [719](29, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 2790 /* [720](30,  0) */ } }, { }, std::nullopt, "15" },
    { 100, { {  124, 2790 /* [721](30,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 2790 /* [722](30,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 2790 /* [723](30,  3) */ } }, { }, 16, std::nullopt },
    { 100, { }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> GreetingFrames = {
    { 100, { }, { { 46, 33 }, { 23, 33 } }, std::nullopt, std::nullopt },
    { 100, { { 1860, 2418 /* [639](26, 15) */ } }, { }, std::nullopt, "13" },
    { 100, { { 1984, 2418 /* [640](26, 16) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2108, 2418 /* [641](26, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 2418 /* [642](26, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 2418 /* [643](26, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 2418 /* [644](26, 20) */ } }, { }, std::nullopt, "18" },
    { 100, { { 2604, 2418 /* [645](26, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 2418 /* [646](26, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852, 2418 /* [647](26, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0, 2511 /* [648](27,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 2511 /* [649](27,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 2511 /* [650](27,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 2511 /* [651](27,  3) */ } }, { }, std::nullopt, "1" },
    { 300, { {  248, 2511 /* [650](27,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 2511 /* [652](27,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 2511 /* [653](27,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 2511 /* [654](27,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 2511 /* [655](27,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992, 2511 /* [656](27,  8) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116, 2511 /* [657](27,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240, 2511 /* [658](27, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364, 2511 /* [659](27, 11) */ } }, { { 55, 100 } }, 55, std::nullopt },
    { 100, { { 1488, 2511 /* [660](27, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612, 2511 /* [661](27, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736, 2511 /* [662](27, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860, 2511 /* [663](27, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984, 2511 /* [664](27, 16) */ } }, { }, std::nullopt, "12" },
    { 100, { { 2108, 2511 /* [665](27, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232, 2511 /* [666](27, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356, 2511 /* [667](27, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480, 2511 /* [668](27, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604, 2511 /* [669](27, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728, 2511 /* [670](27, 22) */ } }, { { 55, 50 } }, std::nullopt, std::nullopt },
    { 100, { {  496,  837 /* [220]( 9,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620,  837 /* [221]( 9,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744,  837 /* [222]( 9,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868,  837 /* [223]( 9,  7) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  992,  837 /* [224]( 9,  8) */ } }, { }, std::nullopt, "4" },
    { 100, { { 1116,  837 /* [225]( 9,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  837 /* [226]( 9, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1364,  837 /* [227]( 9, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1488,  837 /* [228]( 9, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  837 /* [229]( 9, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  837 /* [230]( 9, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  837 /* [231]( 9, 15) */ } }, { { 55, 100 } }, 55, std::nullopt },
    { 100, { { 2852, 2511 /* [671](27, 23) */ } }, { }, std::nullopt, "15" },
    { 100, { {    0, 2604 /* [672](28,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  124, 2604 /* [673](28,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 2604 /* [674](28,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 2604 /* [675](28,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  496, 2604 /* [676](28,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  620, 2604 /* [677](28,  5) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  744, 2604 /* [678](28,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  868, 2604 /* [679](28,  7) */ } }, { }, 55, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> GestureUpFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 6, std::nullopt },
    { 100, { { 1116,  372 /* [105]( 4,  9) */ } }, { }, 5, std::nullopt },
    { 100, { { 1240,  372 /* [106]( 4, 10) */ } }, { }, 4, std::nullopt },
    { 600, { {  744,  930 /* [246](10,  6) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1240,  372 /* [106]( 4, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1116,  372 /* [105]( 4,  9) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> IdleTailWagDFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 95 } }, 15, std::nullopt },
    { 100, { {  124,    0 /*   [1]( 0,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 15, std::nullopt },
    { 100, { { 1488,  186 /*  [60]( 2, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,  186 /*  [61]( 2, 13) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1736,  186 /*  [62]( 2, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1860,  186 /*  [63]( 2, 15) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1984,  186 /*  [64]( 2, 16) */ } }, { }, 9, std::nullopt },
    { 100, { { 2108,  186 /*  [65]( 2, 17) */ } }, { { 8, 75 } }, 9, std::nullopt },
    { 100, { { 2232,  186 /*  [66]( 2, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356,  186 /*  [67]( 2, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480,  186 /*  [68]( 2, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604,  186 /*  [69]( 2, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728,  186 /*  [70]( 2, 22) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2852,  186 /*  [71]( 2, 23) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> GestureLeftFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, 6, std::nullopt },
    { 100, { {  248, 1488 /* [386](16,  2) */ } }, { }, 5, std::nullopt },
    { 100, { {  372, 1488 /* [387](16,  3) */ } }, { }, 4, std::nullopt },
    { 600, { {  496, 1488 /* [388](16,  4) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  372, 1488 /* [387](16,  3) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {  248, 1488 /* [386](16,  2) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

static const std::vector<AgentFrames> IdleTailWagCFrames = {
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { { 0, 95 } }, 16, std::nullopt },
    { 100, { {  124,    0 /*   [1]( 0,  1) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1240,   93 /*  [34]( 1, 10) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1364,   93 /*  [35]( 1, 11) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1488,   93 /*  [36]( 1, 12) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 1612,   93 /*  [37]( 1, 13) */ } }, { { 6, 95 } }, 7, std::nullopt },
    { 150, { { 1736,   93 /*  [38]( 1, 14) */ } }, { }, std::nullopt, std::nullopt },
    { 150, { { 1860,   93 /*  [39]( 1, 15) */ } }, { }, 10, std::nullopt },
    { 100, { { 1984,   93 /*  [40]( 1, 16) */ } }, { }, 10, std::nullopt },
    { 100, { { 2108,   93 /*  [41]( 1, 17) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2232,   93 /*  [42]( 1, 18) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2356,   93 /*  [43]( 1, 19) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2480,   93 /*  [44]( 1, 20) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2604,   93 /*  [45]( 1, 21) */ } }, { }, std::nullopt, std::nullopt },
    { 100, { { 2728,   93 /*  [46]( 1, 22) */ } }, { }, 16, std::nullopt },
    { 100, { {    0,    0 /*   [0]( 0,  0) */ } }, { }, std::nullopt, std::nullopt }
};

/* Link is the name of the cat */
static const Agent the_agent_link = {
    /* animations */ {
        { "RestPose", RestPoseFrames },
        { "LookRight", LookRightFrames },
        { "Print", PrintFrames },
        { "LookDown", LookDownFrames },
        { "IdleYawn", IdleYawnFrames },
        { "Writing", WritingFrames },
        { "CheckingSomething", CheckingSomethingFrames },
        { "GetAttention", GetAttentionFrames },
        { "IdleTailWagB", IdleTailWagBFrames },
        { "DeepIdleE", DeepIdleEFrames },
        { "IdleCleaning", IdleCleaningFrames },
        { "LookLeft", LookLeftFrames },
        { "LookDownRight", LookDownRightFrames },
        { "IdleTailWagA", IdleTailWagAFrames },
        { "IdleBlink", IdleBlinkFrames },
        { "Searching", SearchingFrames },
        { "IdleLegLick", IdleLegLickFrames },
        { "IdleTwitch", IdleTwitchFrames },
        { "Hearing", HearingFrames },
        { "Show", ShowFrames },
        { "GetTechy", GetTechyFrames },
        { "LookUpRight", LookUpRightFrames },
        { "SendMail", SendMailFrames },
        { "IdleButterFly", IdleButterFlyFrames },
        { "IdleStretch", IdleStretchFrames },
        { "Save", SaveFrames },
        { "GestureDown", GestureDownFrames },
        { "EmptyTrash", EmptyTrashFrames },
        { "Wave", WaveFrames },
        { "GestureRight", GestureRightFrames },
        { "Explain", ExplainFrames },
        { "LookDownLeft", LookDownLeftFrames },
        { "Idle1", Idle1Frames },
        { "LookUp", LookUpFrames },
        { "Congratulate", CongratulateFrames },
        { "Thinking", ThinkingFrames },
        { "LookUpLeft", LookUpLeftFrames },
        { "IdleScratch", IdleScratchFrames },
        { "Processing", ProcessingFrames },
        { "Alert", AlertFrames },
        { "GetWizardy", GetWizardyFrames },
        { "Hide", HideFrames },
        { "DeepIdleA", DeepIdleAFrames },
        { "GetArtsy", GetArtsyFrames },
        { "GoodBye", GoodByeFrames },
        { "Greeting", GreetingFrames },
        { "GestureUp", GestureUpFrames },
        { "IdleTailWagD", IdleTailWagDFrames },
        { "GestureLeft", GestureLeftFrames },
        { "IdleTailWagC", IdleTailWagCFrames }
    },
    
    /* width */  124,
    /* height */ 93,
    /* layers */ 1,
    
    /* Sound */ {
        "1", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "11", "12", "13", "14",
        "15", "16", "17", "18", "19", "20", "21"
    }
};

static std::string idles [] = {
    "Idle1", "IdleTailWagA", "IdleTailWagB", "IdleTailWagC", "IdleTailWagD",
    "DeepIdleA", "DeepIdleE", "IdleCleaning", "IdleBlink", "IdleLegLick",
    "IdleTwitch", "IdleButterFly", "IdleStretch", "IdleScratch", "IdleYawn",
    "RestPose"
};

static std::map<int, std::vector<std::pair<std::string, int>>> the_frames;

static int throw_dice_for_branching(std::vector<FrameBranch>& branches, std::optional<int>& exit_branch) {
    int next_idx = exit_branch.value_or(-1);
    int probability_boundary = 100;
    
    for (int idx = 0; (probability_boundary >= 1) && (idx < branches.size()); idx ++) {
        int dice = random_uniform(1, probability_boundary);

        if (dice <= branches[idx].weight) {
            next_idx = branches[idx].frame_idx;
            break;
        } else {
            probability_boundary -= branches[idx].weight;
        }
    }

    return next_idx;
}

static bool is_ending_frame(std::vector<FrameBranch>& branches, std::optional<int>& exit_branch) {
    bool yes = false;
    
    if (branches.size() > 0) {
        if (!exit_branch.has_value()) {
            int probability = 0;

            for (auto branch :  branches) {
                probability += branch.weight;
            }

            yes = (probability >= 100);
        } else {
            yes = true;
        }
    }

    return yes;
}

/*************************************************************************************************/
WarGrey::AoC::AgentSpriteSheet::AgentSpriteSheet(const std::string& pathname, int row, int col)
    : SpriteGridSheet(pathname, row, col) {}

/*************************************************************************************************/
WarGrey::AoC::AgentLink::AgentLink()
    : AgentSpriteSheet(imgdb_build_path("spritesheet/agent/", "link", ".png"), 31, 24) {}

int WarGrey::AoC::AgentLink::submit_action_frames(std::vector<std::pair<int, int>>& frame_refs, const std::string& action) {
    int next_branch = -1;
    
    if (the_agent_link.frames.find(action) != the_agent_link.frames.end()) {
        next_branch = this->push_action_frames(frame_refs, action, 0);       
    }

    return next_branch;
}

int WarGrey::AoC::AgentLink::submit_idle_frames(std::vector<std::pair<int, int>>& frame_refs, int& times) {
    static size_t idle_count = sizeof(idles) / sizeof(std::string);
    int idx = random_uniform(0, idle_count - 1);

    return this->push_action_frames(frame_refs, idles[idx], 0);
}

int WarGrey::AoC::AgentLink::update_action_frames(std::vector<std::pair<int, int>>& frame_refs, int next_branch) {
    int selection_idx = this->find_agent_frames_by_index(next_branch);

    frame_refs.clear();
    
    if (selection_idx < 0) {
        next_branch = -1;
    } else {
        auto frame = the_frames[next_branch][selection_idx];

        next_branch = this->push_action_frames(frame_refs, frame.first, frame.second);
    }

    return next_branch;
}

int WarGrey::AoC::AgentLink::find_agent_frames_by_index(int frame_idx) {
    int selection_idx = -1;

    if (the_frames.find(frame_idx) == the_frames.end()) {    
        for (auto animations : the_agent_link.frames) {
            auto frames = animations.second;

            for (int idx = 0; idx < frames.size(); idx ++) {
                auto locations = frames[idx].images;
            
                if (locations.size() > 0) {
                    if (this->grid_cell_index(locations[0].real(), locations[0].imag()) == frame_idx) {
                        the_frames[frame_idx].push_back(std::pair<std::string, int>(animations.first, idx));
                    }
                }
            }
        }
    }

    if ((the_frames.find(frame_idx) != the_frames.end()) && (the_frames[frame_idx].size() > 0)) {
        selection_idx = random_uniform(0, the_frames[frame_idx].size() - 1);
    }

    return selection_idx;
}

int WarGrey::AoC::AgentLink::push_action_frames(std::vector<std::pair<int, int>>& frame_refs, const std::string& action, int idx0) {
    auto frames = the_agent_link.frames.at(action);
    size_t frame_size = frames.size();
    std::vector<int> indices(frame_size, -1);
    std::vector<int> choices = { 0 };
    int next_branch = -1;

    for (int idx = 0; idx < frame_size; idx ++) {
        auto frame = frames[idx];
        auto locations = frames[idx].images;
    
        if (locations.size() > 0) {
            indices[idx] = this->grid_cell_index(locations[0].real(), locations[0].imag());
        }

        if (is_ending_frame(frame.branches, frame.exit_branch)) {
            if (idx + 1 < frame_size) {
                choices.push_back(idx + 1);
            }
        }
    }

    if ((idx0 == 0) && (choices.size() > 1)) {
        idx0 = choices[random_uniform(0, choices.size() - 1)];
    }

    for (int idx = idx0; idx < frame_size; idx ++) {
        auto frame = frames[idx];
        auto branching = frame.branches;
        
        if (indices[idx] >= 0) {
            frame_refs.push_back({ indices[idx], frame.duration });
        }
        
        if (branching.size() > 0) {
            int branch_idx = throw_dice_for_branching(branching, frame.exit_branch);

            if (branch_idx >= 0) {
                auto branch_pos = std::find(indices.begin(), indices.end(), branch_idx);

                if (branch_pos != indices.end()) {
                    idx = std::distance(indices.begin(), branch_pos) - 1;
                    continue;
                } else {
                    next_branch = branch_idx;
                    break;
                }
            }
        }
    }
    
    return next_branch;
}
