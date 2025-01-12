# Elevator Control System
This project simulates a 4-floor elevator control system.

## Overview
The system allows users to request floors through standard input. Floor requests are managed using a queue data structure, implementing a FIFO (First In, First Out) approach. 

## Implementation
Users input their floor requests, which are added to a queue for processing.
The elevator moves according to the SCAN algorithm, traveling in one direction (up or down) to service all requests in that direction before reversing to handle remaining requests in the opposite direction. (ex. 1 -> 4, 2 -> 3 : 1 -> 2 -> 3 -> 4)

## Visualization
The system provides a graphical representation of the elevator’s location and movement via standard output, making it easy to visualize the elevator’s progress as it services floor requests.
