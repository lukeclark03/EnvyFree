# First Concept
- Greedy students first take into consideration the rows available.
- They evaluate each row to find the best seat to sit in that row, by calculating the least current penalty.
## Examples
### Greedy
Empty Classroom:
- A student gets no penalties from sitting next to someone, so they do not get any penalty no matter where they sit, assuming all rows are equal.
Single Student:
- Pick another row than that student since the penalty is lower for other rows.
Multiple Rows with students:
- Find the largest space the student can get to themselves from sitting in each row, and pick the largest of those.
### Not Greedy
- A student will sit in their preferred row unless the density is higher than the expectancy of the occupancy, in which they will pick a different but close row to sit in otherwise.

# Second Concept
