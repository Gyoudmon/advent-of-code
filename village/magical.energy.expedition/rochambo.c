#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum RPSShape { Rock, Paper, Scissor, _shape } shape_t;
typedef enum RPSOutcome { Win, Lose, Draw, _outcome } outcome_t;

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
    int score = 0;

    switch (shape) {
        case Rock: score = 1; break;
        case Paper: score = 2; break;
        case Scissor: score = 3; break;
        default: /**/;
    }

    return score;
}

static inline int outcome_score(outcome_t outcome) {
    int score = 0;

    switch (outcome) {
        case Win: score = 6; break;
        case Draw: score = 3; break;
        case Lose: score = 0; break;
        default: /**/;
    }

    return score;
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

///////////////////////////////////////////////////////////////////////////////////////////////////
void simulate_with_strategy(FILE* in) {
    char* line = NULL;
    size_t capacity = 0;
    ssize_t read;

    long guessed_total = 0;
    long top_secret_total = 0;

    while ((read = getline(&line, &capacity, in)) != -1) {
        char op_ch = line[0];
        char sf_ch = line[2];
        shape_t op_play = char_to_shape(op_ch);
        shape_t guessed_sf_play = char_to_shape(sf_ch);
        outcome_t s_outcome = char_to_outcome(sf_ch);
        
        guessed_total += round_score(guessed_sf_play, round_end(op_play, guessed_sf_play));  
        top_secret_total += round_score(smart_shape(op_play, s_outcome), s_outcome);
    }

    printf("%ld\n", guessed_total);
    printf("%ld\n", top_secret_total);

    /* Don't forget to release the momery */
    if (line != NULL) {
        free(line);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
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
