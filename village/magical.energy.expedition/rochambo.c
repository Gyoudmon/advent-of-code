#include <stdio.h>
#include <stdlib.h>

/*************************************************************************************************/
typedef enum RPSShape {
    Rock = 1,
    Paper = 2,
    Scissor = 3,
    _shape = 0
} shape_t;

typedef enum RPSOutcome {
    Win = 6,
    Draw = 3,
    Lose = 0,

    /* this is dangerous unless you really want to */
    /*   treat all invalid inputs as loses.        */
    _outcome = 0
} outcome_t;

static inline shape_t char_to_shape(char ch) {
    switch (ch) {
        case 'A': case 'X': return Rock; break;
        case 'B': case 'Y': return Paper; break;
        case 'C': case 'Z': return Scissor; break;
        default: return _shape; break;
    }
}

static inline outcome_t char_to_outcome(char ch) {
    switch (ch) {
        case 'X': return Lose; break;
        case 'Y': return Draw; break;
        case 'Z': return Win; break;
        default: return _outcome; break;
    }
}

static inline int shape_score(shape_t shape) {
    return shape;
}

static inline int outcome_score(outcome_t outcome) {
    return outcome;
}

static inline int round_score(shape_t self_shape, outcome_t outcome) {
    return shape_score(self_shape) + outcome_score(outcome);
}

static inline outcome_t round_end(shape_t opponent, shape_t self) {
    outcome_t end = _outcome;

    if (opponent == self) {
        end = Draw;
    } else if (self == Rock) {
        end = (opponent == Paper) ? Lose : Win;
    } else if (self == Paper) {
        end = (opponent == Scissor) ? Lose : Win;
    } else if (self == Scissor) {
        end = (opponent == Rock) ? Lose : Win;
    }

    return end;
}

static inline shape_t smart_shape(shape_t op_play, outcome_t outcome) {
    shape_t shape = _shape;

    if (outcome == Draw) {
        shape = op_play;
    } else if (op_play == Rock) {
        shape = (outcome == Win) ? Paper : Scissor;
    } else if (op_play == Paper) {
        shape = (outcome == Win) ? Scissor : Rock;
    } else if (op_play == Scissor) {
        shape = (outcome == Win) ? Rock : Paper;
    }

    return shape;
}

/*************************************************************************************************/
int round_score_by_guessing(char op_ch, char sf_ch) {
    shape_t op_play = char_to_shape(op_ch);
    shape_t sf_play = char_to_shape(sf_ch);
    int score = 0;

    if ((op_play != _shape) && (sf_play != _shape)) {
        score = round_score(sf_play, round_end(op_play, sf_play));
    }

    return score;
}

int round_score_by_design(char op_ch, char sf_ch) {
    shape_t op_play = char_to_shape(op_ch);
    outcome_t outcome = char_to_outcome(sf_ch);
    int score = 0;

    if (op_play != _shape) { /* treat all invalid inputs as loses by design */
        score = round_score(smart_shape(op_play, outcome), outcome);
    }

    return score;
}

/*************************************************************************************************/
void simulate_with_strategy(FILE* in) {
    char* line = NULL;
    size_t capacity = 0;
    ssize_t read;

    long guessed_total = 0;
    long designed_total = 0;

    while ((read = getline(&line, &capacity, in)) != -1) {
        char op_ch = line[0];
        char sf_ch = line[2];
        
        guessed_total += round_score_by_guessing(op_ch, sf_ch);  
        designed_total += round_score_by_design(op_ch, sf_ch);
    }

    printf("%ld\n", guessed_total);
    printf("%ld\n", designed_total);

    /* Don't forget to release the momery */
    if (line != NULL) {
        free(line);
    }
}

/*************************************************************************************************/
int main(int argc, char* argv[]) {
    if (argc > 1) {
        FILE* src = fopen(argv[1], "r");

        if (src != NULL) {
            simulate_with_strategy(src);
            fclose(src);
        }
    } else {
        simulate_with_strategy(stdin);
    }

    return 0;
}
