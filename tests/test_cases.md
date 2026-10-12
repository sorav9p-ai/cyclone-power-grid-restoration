
# Power Grid Restoration — Test Cases

These test cases will be used to verify the behaviour of our C++ prototype.

| Test | Input or Action | Expected Result |
|---|---|---|
| 1. Normal Grid | Start the program | The grid is displayed with all lines functional and all locations powered. |
| 2. Random Damage | Select the damage simulation and enter a valid number of lines | The selected number of unique lines is marked as damaged. |
| 3. Invalid Damage Input | Enter zero, a negative number or a number greater than the available lines | The program rejects the input without applying the requested damage. |
| 4. Power Outages | Generate damage and check power status | BFS identifies locations that are no longer connected to the power source. |
| 5. Repair Priorities | Generate damage and request the repair-priority list | Eligible repairs are ranked using estimated service importance and repair time, with effort used as a tie-breaker. |
| 6. Perform Repair | Select the next recommended repair | The line is restored, repair time and effort are recorded, and power status is recalculated. |
| 7. New Failure | Mark a functional line as damaged | The program records the failure and updates power status. |
| 8. Reset Grid | Select reset | All power lines return to their functional state and repair totals are reset. |
| 9. Search Line | Search using a valid and an invalid line ID | The program displays the line details or an invalid-ID message. |
| 10. Sort Damaged Lines | Generate damage and sort the affected lines | Damaged lines are listed by repair time, then repair effort. |
| 11. Distribution Tree | Select a valid city number | The program displays that city's branching distribution structure. |

## Validation Notes

The repair times, effort values and priority weights are simulated assumptions. Each test must be executed to confirm the actual result before it is reported as passed.

Future tests will cover limited repair crews, additional failures during restoration and more complex repair dependencies.
