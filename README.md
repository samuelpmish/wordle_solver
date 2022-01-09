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
% ./wordle_solver truth
guess 1: aloes
matched: 
misplaced: 
unused: a e l o s 
209 remaining words: 
biddy bight birch birth bitty brick bring brink briny bruin bruit brunt buddy buggy bumpy bunch 
bunny burgh burnt butty chick chimp chirp chuck chuff chump chunk churn cinch cirri civic crick 
crimp crumb crypt cubic cubit cuing cumin cupid curry curvy cynic digit dingy dinky dirty ditch 
ditty divvy dizzy drift drink drunk duchy ducky dummy dumpy durum dying fichu fifth fifty fight 
finch firth fizzy frizz fruit frump fungi funky funny furry fuzzy giddy gimpy girth grimy grind 
gruff grump grunt gummy gunny guppy hitch hubby huffy humid humph hunch hurry hutch icing incur 
infix input itchy jiffy jimmy juicy jumpy kitty kudzu middy might mimic mingy minim minty mirth 
mucky muddy mufti muggy mummy munch murky myrrh nifty night nimbi ninny ninth nippy nubby nutty 
nymph picky piggy pinch pinny pipit pitch pithy primp print privy pubic pudgy puffy punch puppy 
putty pygmy quick quiff quint quirk quirt right rigid ritzy ruddy ruing rugby rummy runic runny 
runty rutty thick thigh thing think third thrum thumb thump tying tight timid tinny tizzy trick 
truck trump trunk truth tubby tumid tummy tunic tunny uncut undid unfit unify unity unpin unzip 
vying vivid which whiff whiny width wight wimpy winch windy witch witty wring wrung yucky yummy 
zippy 

guess 2: mirth
matched: {t, 3} {h, 4} 
misplaced: {r, 2} 
unused: a e i l m o s 
1 remaining words: 
truth 

found truth in 3 guess(es)
```

2. You provide no arguments, and it solves each word in its lexicon. After finishing, it reports data about how many words were solved in different numbers of guesses.

```
% ./wordle_solver
guess 1: aloes
guess 2: print
guess 3: aback
found aback in 4 guess(es)
guess 1: aloes
guess 2: print
guess 3: aback
found abaft in 4 guess(es)
guess 1: aloes
guess 2: bound
found abase in 3 guess(es)
guess 1: aloes
guess 2: bight
found abash in 3 guess(es)
...
```

# Finding the "best" first guess
The file "word_data.txt" contains information about how effective each 5-letter word is as a starting guess. 

Each starting guess was tried against all the words in the lexicon, and the clues from the output (which tiles are matched, misplaced, unused) 
are used to determine how many possible words remain after the first guess. The number next to each guess is the worst-case results for that 
guess. Using this output, the word "aloes" is the best in the sense of producing the smallest pool of words to choose from after the first guess (in the
worst-case scenario).

Of course, this is just a heuristic, as not all lexicons of n words are equally "easy" to solve. In practice, though, guessing "aloes" first every time
does seem to be a respectable strategy.
respectable strategy
