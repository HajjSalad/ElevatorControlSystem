# Elevator Control System
Overview

This project simulates a 4-floor elevator control system.

# Approach 1
Use a simple queue data structure to manage floor requests in a First-In-First-Out (FIFO) order.

Assumptions

- The elevator system starts at Floor 1.
- The elevator is in the IDLE state when at rest, with the doors open.
- Door operation (opening and closing) is not included in the design—doors automatically open and close when the elevator reaches the requested floor.

Implementation

- The system begins with the elevator at Floor 1 in the IDLE state.
- Users interact with the system via standard input, where they can request floors.
- Each request is placed into a queue to implement a FIFO scheduling system for elevator calls.

Drawbacks of Using FIFO linear queue

- Inefficiencies in queue processing. Ex. requests 1→4, 2→3,  the elevator goes 1→4 then comes back to process 2→3.
- Lack of directional awareness - need to optimize based on direction.

# Approach 2

- SCAN Algorithm - Elevator moves in a single direction (up/down) until it reaches the last requested floor in that direction, then reverses its direction.
