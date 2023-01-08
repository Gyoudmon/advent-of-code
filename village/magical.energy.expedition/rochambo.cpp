#include <iostream>
#include <fstream>

/*************************************************************************************************/
enum class RPSShape { Rock, Paper, Scissor, _ };
enum class RPSOutcome { Win, Lose, Draw, _ };

static inline RPSShape char_to_shape(char ch) {
    switch (ch) {
        case 'A': case 'X': return RPSShape::Rock; break;
        case 'B': case 'Y': return RPSShape::Paper; break;
        case 'C': case 'Z': return RPSShape::Scissor; break;
        default: return RPSShape::_; break;
    }
}

static inline RPSOutcome char_to_outcome(char ch) {
    switch (ch) {
        case 'X': return RPSOutcome::Lose; break;
        case 'Y': return RPSOutcome::Draw; break;
        case 'Z': return RPSOutcome::Win; break;
        default: return RPSOutcome::_; break;
    }
}

static inline int shape_score(RPSShape shape) {
    int score = 0;

    switch (shape) {
        case RPSShape::Rock: score = 1; break;
        case RPSShape::Paper: score = 2; break;
        case RPSShape::Scissor: score = 3; break;
        default: /**/;
    }

    return score;
}

static inline int outcome_score(RPSOutcome outcome) {
    int score = 0;

    switch (outcome) {
        case RPSOutcome::Win: score = 6; break;
        case RPSOutcome::Draw: score = 3; break;
        case RPSOutcome::Lose: score = 0; break;
        default: /**/;
    }

    return score;
}

static inline int round_score(RPSShape self_shape, RPSOutcome outcome) {
    return shape_score(self_shape) + outcome_score(outcome);
}

static inline RPSOutcome round_end(RPSShape opponent, RPSShape self) {
    RPSOutcome end = RPSOutcome::_;

    if (opponent == self) {
        end = RPSOutcome::Draw;
    } else if (self == RPSShape::Rock) {
        end = (opponent == RPSShape::Paper) ? RPSOutcome::Lose : RPSOutcome::Win;
    } else if (self == RPSShape::Paper) {
        end = (opponent == RPSShape::Scissor) ? RPSOutcome::Lose : RPSOutcome::Win;
    } else if (self == RPSShape::Scissor) {
        end = (opponent == RPSShape::Rock) ? RPSOutcome::Lose : RPSOutcome::Win;
    }

    return end;
}

static inline RPSShape smart_shape(RPSShape op_play, RPSOutcome outcome) {
    RPSShape shape = RPSShape::_;

    if (outcome == RPSOutcome::Draw) {
        shape = op_play;
    } else if (op_play == RPSShape::Rock) {
        shape = (outcome == RPSOutcome::Win) ? RPSShape::Paper : RPSShape::Scissor;
    } else if (op_play == RPSShape::Paper) {
        shape = (outcome == RPSOutcome::Win) ? RPSShape::Scissor : RPSShape::Rock;
    } else if (op_play == RPSShape::Scissor) {
        shape = (outcome == RPSOutcome::Win) ? RPSShape::Rock : RPSShape::Paper;
    }

    return shape;
}

/*************************************************************************************************/
void simulate_with_guessed_strategy(std::istream& in) {
    std::string line;
    long total = 0;

    while (std::getline(in, line)) {
        if (line.size() >= 3) {
            RPSShape op_shape = char_to_shape(line[0]);
            RPSShape sf_shape = char_to_shape(line[2]);

            total += round_score(sf_shape, round_end(op_shape, sf_shape));
        }
    }

    std::cout << total << std::endl;
}

void simulate_with_top_secret_strategy(std::istream& in) {
    std::string line;
    long total = 0;

    while (std::getline(in, line)) {
        if (line.size() >= 3) {
            RPSShape op_shape = char_to_shape(line[0]);
            RPSOutcome outcome = char_to_outcome(line[2]);

            total += round_score(smart_shape(op_shape, outcome), outcome);
        }
    }

    std::cout << total << std::endl;
}

/*************************************************************************************************/
int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::ifstream src(argv[1]);

        if (src.is_open()) {
            simulate_with_top_secret_strategy(src);
            src.close();
        }
    } else {
        simulate_with_guessed_strategy(std::cin);
    }

    return 0;
}
