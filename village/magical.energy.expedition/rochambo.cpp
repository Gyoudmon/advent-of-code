#include <iostream>
#include <fstream>

/*************************************************************************************************/
enum class RPSShape {
    Rock = 1,
    Paper = 2,
    Scissor = 3,
    _ = 0
};

enum class RPSOutcome {
    Win = 6,
    Draw = 3,
    Lose = 0,

    // this is dangerous unless you really want to
    //   treat all invalid inputs as loses.  
    _ = 0
};

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
    return static_cast<int>(shape);
}

static inline int outcome_score(RPSOutcome outcome) {
    return static_cast<int>(outcome);
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
int round_score_by_guessing(char op_ch, char sf_ch) {
    RPSShape op_shape = char_to_shape(op_ch);
    RPSShape sf_shape = char_to_shape(sf_ch);
    int score = 0;

    if ((op_shape != RPSShape::_) && (sf_shape != RPSShape::_)) {
        score = round_score(sf_shape, round_end(op_shape, sf_shape));
    }

    return score;
}

int round_score_by_design(char op_ch, char sf_ch) {
    RPSShape op_shape = char_to_shape(op_ch);
    RPSOutcome outcome = char_to_outcome(sf_ch);
    int score = 0;

    if (op_shape != RPSShape::_) { // treat all invalid inputs as loses by design
        score = round_score(smart_shape(op_shape, outcome), outcome);
    }

    return score;
}

/*************************************************************************************************/
void simulate_with_strategy(std::istream& in) {
    std::string line;
    long guessed_total = 0;
    long designed_total = 0;

    while (std::getline(in, line)) {
        if (line.size() >= 3) {
            char op_ch = line[0];
            char sf_ch = line[2];

            guessed_total += round_score_by_guessing(op_ch, sf_ch);  
            designed_total += round_score_by_design(op_ch, sf_ch);
        }
    }

    std::cout << guessed_total << std::endl;
    std::cout << designed_total << std::endl;
}

/*************************************************************************************************/
int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::ifstream src(argv[1]);

        if (src.is_open()) {
            simulate_with_strategy(src);
            src.close();
        }
    } else {
        simulate_with_strategy(std::cin);
    }

    return 0;
}
