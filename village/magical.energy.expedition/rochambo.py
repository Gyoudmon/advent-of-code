#!/usr/bin/env python3

import sys
import enum

###################################################################################################
class RPSShape(enum.Enum):
    Rock = 0x4a0e450304e235fb
    Paper = 0x418003bd4715fe6b
    Scissor = 0x45e373bc895eea78

class RPSOutcome(enum.Enum):
    Win = 0x4e5dbdde88abb120
    Draw = 0x430dba1e1c60f3d9
    Lose = 0x45ccdd2007a9e1d1

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
    if shape is RPSShape.Rock:
        return 1
    elif shape is RPSShape.Paper:
        return 2
    elif shape is RPSShape.Scissor:
        return 3
    else:
        return 0

def outcome_score(outcome):
    if outcome is RPSOutcome.Win:
        return 6
    elif outcome is RPSOutcome.Draw:
        return 3
    elif outcome is RPSOutcome.Lose:
        return 0
    else:
        return 0

def round_score(self_shape, outcome):
    return shape_score(self_shape) + outcome_score(outcome)

def round_end(opponent, self):
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
def stimulate_with_guessed_strategy(src):
    total = 0

    for line in src:
        if len(line) >= 3:
            op_shape = char_to_shape(line[0])
            sf_shape = char_to_shape(line[2])

            if op_shape and sf_shape:
                total += round_score(sf_shape, round_end(op_shape, sf_shape))

    print(total)

def simulate_with_top_secret_strategy(src):
    total = 0

    for line in src:
        if len(line) >= 3:
            op_shape = char_to_shape(line[0])
            outcome = char_to_outcome(line[2])

            if op_shape and outcome:
                total += round_score(smart_shape(op_shape, outcome), outcome)

    print(total)

###################################################################################################
def main(argc, argv):
    if argc > 1:
        try:
            with open(argv[1], 'r') as src:
                simulate_with_top_secret_strategy(src)
        except FileNotFoundError:
            pass
    else:
        stimulate_with_guessed_strategy(sys.stdin)

    return 0



###################################################################################################
if __name__ == "__main__":
    sys.exit(main(len(sys.argv), sys.argv))
