# quiz-game

A quiz game developed in C.

The game loads the questions and answers from a file(quiz_file.txt) and asks the user to guess the answer.
It has 5 difficulty levels.
The questions and answers file and difficulty level are provided as commanded line arguments.

Difficulty level 1: No clue and only a '?' is shown.
Difficulty level 2: A set of blank dashes is displayed for each letter of the answer.
Difficulty level 3: The first and last letters are shown.
Difficulty level 4: Two random letters are shown in their correct position.
Difficulty level 5: All letters are shown in random order.

When the quiz is complete the user is asked if they want to view the correct answers to the questions that were wrong.
The score, total number of questions and difficulty are written to a file(quiz_history.txt).
