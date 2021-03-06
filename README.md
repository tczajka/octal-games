# Solving octal games

The goal here is to solve [octal games](https://en.wikipedia.org/wiki/Octal_game)
and [Grundy's game](https://en.wikipedia.org/wiki/Grundy%27s_game).

To solve these games is to find the period, if any, of the sequence of nimbers
(as defined by [Sprague-Grundy theorem](https://en.wikipedia.org/wiki/Sprague%E2%80%93Grundy_theorem))
for consecutive heap sizes.

Some results were published in the book
["Winning Ways for Your Mathematical Plays"](https://en.wikipedia.org/wiki/Winning_Ways_for_your_Mathematical_Plays).

More results were computed by [Achim Flammenkamp](http://wwwhomes.uni-bielefeld.de/achim/octal.html).

There are however still a lot of unsolved games, including Grundy's Game.

We are going to verify those results and solve some more games.

## Results

Many octal games are equivalent. Look up equivalent games in the [game lookup tables](GameLookup.md).

[Trivial games](TrivialGames.md) are those that can be solved by looking at the first 1000 nimbers.

[Solved games](SolvedGames.md) contain confirmed nontrivial results.

[Unsolved games](UnsolvedGames.md) are work in progress.

## Mistakes found in other people's results

In Winning Ways game locator: 
* .27 should map to .26 (rather than .06).
* .314 should not map to .31

In Achim Flammenkamp's [tables](http://wwwhomes.uni-bielefeld.de/achim/comp_octal.txt):
* .161 should not map to .36. The first difference between these two games is at heap size 519!
