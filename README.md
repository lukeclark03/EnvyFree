# EnvyFree
Envy-Free Cake Cutter's Cooperative Game Theory Project
Here is the readme First Commit!!


# TEMP: Greedy vs Selfless Student Seating Thoughts

A greedy student takes into consideration all rows when considering which row to sit in. This means that they will have to find the best seat to sit in for each row and get the penalty associated with it. The best greedy seat for each row could be implemented as a variable so that it doesn't have to be calculated every time. For an empty classroom with no other students, the greedy student would receive the lowest penalty (0) from sitting in his preferred row with no other students in that row. If there was a single student in his preferred row and no other students anywhere else, the greedy student would once again examine all rows and pick the one with the lowest penalty. When examining a row with a student already in it, he should calculate the penalty that he will receive from sitting in the furthest seat away from all other students in the row, creating the largest gap that he can between him and all other students in the row. If a gap between him and another student is closer, then penalty will be larger, and he will be incentivized to just sit in a different row (more about this later). When examining a row without any students in it, the penalty level that he will assign to sitting in that row should be dependent on how close that row is to his preferred row. If it is one row away from his preferred row, then the penalty should not be significant, but the penalty will become significant for further away rows and the greedy student may end up calculating a lower penalty level in a row that has another student in it that is closer to his preferred row. With a penalty of 0 to 100 for example, sitting n rows away from the preferred row might incur a penalty of 10n. For sitting in a row with other students in it, perhaps the penalty for sitting n seats away from another student in the same row would be 100/(n^2), which would result in a penalty of 100 for 1 seat away, 25 for 2 seats away, 11 for 3, 6 for 4, etc.

A non greedy student is a lot more simple for my simplified implementation. I simply made the selfless students pick their preferred row until the density of it was higher than the expected density value of the classroom. When sitting in any row, they would simply sit as close to the lowest seat number as possible that would be a multiple of (1/(fullness_expectancy/100)). In other words 1/(50% fullness/100) would be 2, 1/(25%/100) would be 4, etc. If a greedy student had caused there not to be a seat in their preferred row that allowed for an even distribution, assuming the density of that row was still lower than their expectation of the classroom fullness, then they would use the greedy method for finding the best seat within that row. Ideally they would never revert to the greedy method, but the method is already very simplified because it doesn’t take into consideration the positions of the greedy students in that row, which would make it difficult to calculate.


# Explanation of making the dynamics graph

For every round, we must consider the partitions that can be made, i.e. the groups of coalitions that can be made up from the set of students.

Thus, for every possible partition $P$ formed from students $S$
- For every possible coalition $D$ formed from students $S$
	- Create a partition $Q$ 
	- For every coalition $C \in P$
		- Create a coalition $R = \{ C - D\}$
		- If $R \neq \emptyset$
			- Add $R$ to $Q$ 
	- Add $D$ to $Q$
