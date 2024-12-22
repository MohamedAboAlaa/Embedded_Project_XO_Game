/******************************************************************************
 * Module: XO Game
 *
 * File Name: XO Game.h
 *
 * Description: Header file for XO Game functions and prototypes.
 *
 * Authors: El-Osood El-Mefrhda
 ******************************************************************************/

/*******************************************************************************
 *                       Headers                             * 
 *******************************************************************************/ 

#ifndef XO_GAME_H_
#define XO_GAME_H_

/*******************************************************************************
 *                       Functions                             *
 *******************************************************************************/

/**
 * @brief Function to display the introduction to the game.
 * @param None
 * @return None
 */
void GameIntro(void);

/**
 * @brief Starts the XO game.
 * @param None
 * @return None
 */
void PlayXO(void);

/**
 * @brief Resets the game and initializes it to start again.
 * @param None
 * @return None
 */
void GameInitialization(void);

/**
 * @brief Runs the XO game using UART for input.
 * @param None
 * @return None
 */
void RunGameUART(void);

/**
 * @brief Runs the XO game using ADC for input.
 * @param None
 * @return None
 */
void RunGameADC(void);

/**
 * @brief Checks if a player has won the game.
 * @param player: The current player to check.
 * @return int: 1 if the player has won, 0 otherwise.
 */
int checkWinner(char player);

/**
 * @brief Displays the winner of the game.
 * @param player: The player who won the game.
 * @return None
 */
void Display_Winner(char player);

/**
 * @brief Draws the XO shape on the game board.
 * @param None
 * @return None
 */
void displayDraw(void);

/**
 * @brief Displays the status of whose turn it is to play.
 * @param None
 * @return None
 */
void displayStatus(void);

/**
 * @brief Checks if the players want to play the game again.
 * @param None
 * @return int: 1 if replay is chosen, 0 otherwise.
 */
int CheckPlayAgain(void);

/**
 * @brief Ends the XO game and performs cleanup if needed.
 * @param None
 * @return None
 */
void EndGame(void);

/**
 * @brief Clears the game matrix to reset the board.
 * @param None
 * @return None
 */
void DrawClearGameMatrix(void);

/**
 * @brief Puts the processor in a low-power wait state.
 * @param None
 * @return None
 */
void WaitForInterrupt(void);

#endif /* XO_GAME_H_ */
