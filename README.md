# Final Game Concepts

Our game is an $N$ player game with $K$ seats and $R$ rows. Players are at first randomly seated and then, for each round, able to move to different seats in any row. 

Their goal is to maximize their utility up to a threshold $T$, where their utility is assigned by how far (how many seats away) are their nearest neighbor in the same row. If someone's utility is zero, it means that they are sharing a space with someone else; this means that no matter how many people are sitting in one seat, they all get utility 0. If they are in a seat alone, where either or both seats beside them are filled, they get utility 1. If they have a seat empty on both sides of them, but someone is sitting 1 away, they have utility 2. Basically, the utility is the occupancy of their seat plus the distance to the nearest person.

The threshold is usually assigned 3, since this is a small but interesting number - if it were 0, anyone would tolerate anyone sitting anywhere. A similar phenomenon happens with 1, and thus we chose 3 to produce interesting but not extreme results. Therefore, please assume that the threshold is always three in this document.

First, we made it so that players would move one at a time, randomly. Later, we changed this so that the players who were most disadvantaged in their utilities would be selected first to move. Then, we implemented coalitional moving, so that a group could be randomly chosen if one of their members were to be picked. This required us to calculate the payoffs of the entire group, such that members could prefer the payoff when able to move with other people into what could be a larger contiguous space to space themselves apart with better payoffs than if only one person could move.

## Payoffs within coalitional arrangements
- The payoffs which are calculated for a certain person in a certain coalitional arrangement per round are now calculated as the average payoff they can get based on whether their group is chosen vs whether their group is not chosen, so if a person was in a group by themselves and there were two other coalitions of 3, and 5, then they would have a 1/9 chance to get picked and get the best seat obtainable by just moving by themselves, 1/3 of a chance to get the payoff that the 3 person group would let them have, and 5/9 of a chance to get the payoff that the 5 person group would let them have: thus they would get a payoff of $A/9$ + $B/3$ + $C*5/9$ if A, B, and C were the payoffs in each situation.

## Optimality
In order to find if  the solution the players take is a close to good solution, we compare it to an optimal, assigned solution, where for each row of $s$ seats, we can assign $n$ students so they receive a maximum payoff, if the number of students $n$ is divisible by $r$.
- $s \geq 3n - 2r$
	- All players will receive 3 utility 
- $2n - r < s \leq 3n - 2r$
	- Some players will receive 3, some may receive 2 utility.
- $n < s \leq 2n - r$
	- Some players will receive 2, some may receive 1 utility.
-  $s \leq n$
	- Some players will receive 1, some may receive 0 utility.
	
For arranging without n being divisible by r, just split the rows into sub-situations where the remainder gets distributed among several of the rows, randomly.
## Explanation of making the dynamics graph

For every round, we must consider the partitions that can be made, i.e. the groups of coalitions that can be made up from the set of all students.
### Pseudocode
To create a list of adjacent partitions $A$
For every possible partition $P$ formed from the set of students $S$
- For every possible coalition $D$ formed from the set of students $S$
	- Create a partition $Q$ 
	- For every coalition $C \in P$
		- Create a coalition $U = \{ C - D\}$
		- If $U \neq \emptyset$
			- Add $U$ to $Q$ 
	- Add $D$ to $Q$ 
	- Add $Q$ to the set of adjacent partitions $A$
