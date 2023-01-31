#!/usr/bin/env python3

import sys
import enum

###################################################################################################
# Enum with 0 as value will not be treat as False implicitly

class RPSShape(enum.Enum):
    Rock = 1
    Paper = 2
    Scissor = 3

class RPSOutcome(enum.Enum):
    Win = 6
    Draw = 3
    Lose = 0

def char_to_shape(ch):
    if ch in ['A', 'X']:
        return RPSShape.Rock
    elif ch in ['B', 'Y']:
        return RPSShape.Paper
    elif ch in ['C', 'Z']:
        return RPSShape.Scissor
    else:
        return None

def char_to_outcome(ch):
    if ch == 'X':
        return RPSOutcome.Lose
    elif ch == 'Y':
        return RPSOutcome.Draw
    elif ch == 'Z':
        return RPSOutcome.Win
    else:
        return None

def shape_score(shape):
    return shape.value

def outcome_score(outcome):
    return outcome.value

def round_score(self_shape, outcome):
    return shape_score(self_shape) + outcome_score(outcome)

def round_outcome(opponent, self):
    if opponent is self:
        return RPSOutcome.Draw
    elif self is RPSShape.Rock:
        if opponent is RPSShape.Paper:
            return RPSOutcome.Lose
        else:
            return RPSOutcome.Win
    elif self is RPSShape.Paper:
        if opponent is RPSShape.Scissor:
            return RPSOutcome.Lose
        else:
            return RPSOutcome.Win
    elif self is RPSShape.Scissor:
        if opponent is RPSShape.Rock:
            return RPSOutcome.Lose
        else:
            return RPSOutcome.Win
    else:
        return None

def smart_shape(op_play, outcome):
    if outcome is RPSOutcome.Draw:
        return op_play
    elif op_play is RPSShape.Rock:
        if outcome is RPSOutcome.Win:
            return RPSShape.Paper
        else:
            return RPSShape.Scissor
    elif op_play is RPSShape.Paper:
        if outcome is RPSOutcome.Win:
            return RPSShape.Scissor
        else:
            return RPSShape.Rock
    elif op_play is RPSShape.Scissor:
        if outcome is RPSOutcome.Win:
            return RPSShape.Rock
        else:
            return RPSShape.Paper
    else:
        return None

###################################################################################################
def round_score_by_guessing(op_ch, sf_ch):
    op_play = char_to_shape(op_ch)
    sf_play = char_to_shape(sf_ch)
    score = 0

    if op_play and sf_play:
        score = round_score(sf_play, round_outcome(op_play, sf_play))

    return score

def round_score_by_design(op_ch, sf_ch):
    op_play = char_to_shape(op_ch)
    outcome = char_to_outcome(sf_ch)
    score = 0

    if op_play and outcome:
        score = round_score(smart_shape(op_play, outcome), outcome)

    return score

###################################################################################################
def simulate_with_strategy(src):
    guessed_total = 0
    designed_total = 0

    for line in src:
        if len(line) >= 3:
            op_ch, _, sf_ch = line[0:3]
            
            guessed_total += round_score_by_guessing(op_ch, sf_ch)
            designed_total += round_score_by_design(op_ch, sf_ch)

    print([guessed_total, designed_total])

###################################################################################################
def main(argc, argv):
    if argc > 1:
        try:
            with open(argv[1], 'r') as src:
                simulate_with_strategy(src)
        except FileNotFoundError:
            pass
    else:
        simulate_with_strategy(sys.stdin)

    return 0



###################################################################################################
if __name__ == "__main__":
    sys.exit(main(len(sys.argv), sys.argv))
