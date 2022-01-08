# Wordle Solver
a small commandline tool for solving [Wordle](https://www.powerlanguage.co.uk/wordle/)

# Build
Compile with a suitable C++17 compiler,

```
g++ main.cpp -std=c++17 -O3 -o wordle_solver
```

# Run
This tool supports two modes:

1. You provide a five-letter word, and it will make a sequence of guesses to try and gain information and determine the hidden word

```
% ./wordle_solver bench
guess 1: slate
matched: 
misplaced: {e, 4} 
unused: a l s t 
224 remaining words: 
bebop bedim beech beefy beery befog begin begum being bench berry biped bogey bored borer bowed 
bower boxed boxer breed brief brier buyer cecum check cheek cheep cheer chewy chief cider coder 
codex comer corer coven cover covey cower cozen credo creed creek creep crier cured cyder debug 
decor decoy decry defer deify deign demob demon demur denim derby dicey dimer diner diver domed 
doped dopey dower doyen dozen dried drier dryer dweeb ebony edged edger edify eider embed ember 
emend emery ended endow enemy enjoy ennui envoy epoch epoxy equip error every feign femur ferny 
ferry fever fewer fiber fiend fiery finer fired fiver fixed fixer foyer fried fryer fumed gecko 
given giver gofer goner gooey greed green grief heron hewer hexed hiker hired hirer hokey homer 
homey honey hooey hover hymen index infer inner ivied jemmy jenny jerky joker keyed medic mercy 
merry mezzo mined miner mired mixed mixer modem money moped moved mover mower needy neigh nervy 
never ocher offer order owned owner penny peony peppy perch perky perry piper poker power preen 
queen queer quern query recur reedy refer reign renew rerun rheum ricer rider rimed ripen river 
robed rodeo roper ropey rover rower udder umber under unfed unwed upend upper venom vexed video 
viper vireo vixen weedy weeny weepy weigh weird widen wiper wired wooer woven wreck xenon xerox 


guess 2: coder
matched: 
misplaced: {c, 0} {e, 3} {e, 4} 
unused: a d l o r s t 
2 remaining words: 
beech bench 

guess 3: bench
matched: {b, 0} {e, 1} {n, 2} {c, 3} {h, 4} 
misplaced: {c, 0} {e, 3} {e, 4} 
unused: a d l o r s t 
1 remaining words: 
bench 

found bench in 4 guess(es)
```

2. You provide no arguments, and it solves each word in its lexicon. After finishing, it reports data about how many words were solved in different numbers of guesses.

```
% ./wordle_solver
guess 1: slate
guess 2: crank
guess 3: aback
found aback in 4 guess(es)
guess 1: slate
guess 2: grunt
guess 3: abaft
found abaft in 4 guess(es)
guess 1: slate
guess 2: chess
guess 3: urban
found abase in 4 guess(es)
guess 1: slate
guess 2: crush
guess 3: awing
found abash in 4 guess(es)
guess 1: slate
guess 2: argon
found abate in 3 guess(es)
...
```
