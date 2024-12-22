/******************************************************************************
 * File Name: XO Game.c
 * Description: Source file for XO Game
 ******************************************************************************/

#include "../Drivers_Header/tm4c123gh6pm.h"
#include "../Drivers_Header/Nokia5110.h"
#include "../Drivers_Header/TExaS.h"
#include "../Drivers_Header/GameArt.h"
#include "../Drivers_Header/Game.h"
#include "../Drivers_Header/Timer.h"
#include "../Drivers_Header/ADC.h"
#include "../Drivers_Header/UART.h"
#include "../Drivers_Header/Sound.h"

/*******************************************************************************
 *                           Varibles                              *
 *******************************************************************************/

unsigned long TimerCount;

int playedCells, currentCell;
volatile long rateOrdaniry = 8000000, rateWinning = 8000; // 100 sec
char turnMark, GameMatrix[9], xWins = 0, oWins = 0;

extern unsigned char UART_Mailbox ;
extern unsigned short UART_Mail ;

extern unsigned char Sw1Flag, Sw2Flag;
extern volatile unsigned int ADCvalue;
// SW1 : moving (Potentiometer) , 9*420 , each cell width= 420
// SW2 : selecting (switch)
// row = (adcVal / 420*3) (from 0 to 9)
// col = (adcVal / 420)   (from 0 to 9) 

/*******************************************************************************
 *                              Functions                                      *
 *******************************************************************************/

/************************************************************************************
 * Service Name:GameIntro
 * Parameters (in): None
 * Parameters (out): None
 * Return value: None
 * Description: function to enter to the game
 ************************************************************************************/

void GameIntro(void)
{
	Nokia5110_PrintBMP(0, 47, introbg, 0); // darwing the frame of welcome window
	Nokia5110_DisplayBuffer();			   // display that window
	Timer2_delay_ms(3000);					   // 3 seconds
	Nokia5110_Clear();					   // clear the whole screen
	
	Nokia5110_SetCursor(0, 1);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString(" Embedded");	   // write this string to screen
	Nokia5110_SetCursor(0, 2);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString(" Systems");	   // write this string to screen
	Nokia5110_SetCursor(0, 3);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString(" Project");	   // write this string to screen
	Timer2_delay_ms(1500);
	Nokia5110_Clear(); // clear the whole screen
	
	Nokia5110_SetCursor(0, 0);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString("supervised  by:");	   // write this string to screen
	Nokia5110_SetCursor(0, 3);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString("Dr/Lamiaa");	   // write this string to screen
	Nokia5110_SetCursor(0, 4);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString("Eng/Mahmoud");	   // write this string to screen
	Timer2_delay_ms(2000);
	Nokia5110_Clear(); // clear the whole screen
	
	Nokia5110_SetCursor(0, 2);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString("  Exhausted");	   // write this string to screen
	Nokia5110_SetCursor(0, 3);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString("    Lions");	   // write this string to screen
	Nokia5110_SetCursor(0, 4);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString("    Team");	   // write this string to screen
	Timer2_delay_ms(1500);
	Nokia5110_Clear(); // clear the whole screen
	
	Nokia5110_SetCursor(0, 1);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString("  Abo Alaa");	   // write this string to screen
	Timer2_delay_ms(200);
	Nokia5110_SetCursor(0, 2);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString(" Abdo Osama");	   // write this string to screen
	Timer2_delay_ms(200);
	Nokia5110_SetCursor(0, 3);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString(" Abo Hamdey");	   // write this string to screen
	Timer2_delay_ms(1500);
	Nokia5110_Clear(); // clear the whole screen
	Nokia5110_SetCursor(1, 2);			   // set cursor at first X place on the screen and the 3rd y to be nearly in the middle
	Nokia5110_OutString("Welcome To");	   // write this string to screen
	Nokia5110_SetCursor(1, 4);
	Nokia5110_OutString(" XO Game");
	Timer2_delay_ms(1500);
	Nokia5110_Clear(); // clear the whole screen
}

/************************************************************************************/
unsigned long SW1,SW2;
int F1,F2;

void PlayXO(void){
	Nokia5110_SetCursor(0, 1);
	Nokia5110_OutString("How to Play?");
	Nokia5110_SetCursor(0, 3);
	Nokia5110_OutString("> POT: SW1");
	Nokia5110_SetCursor(0, 4);
	Nokia5110_OutString("> COM: SW2");
	
	while(1){
		SW1 = ( (GPIO_PORTF_DATA_R & (1<<4))>>4 );     // read PF4 into SW1
		SW2 = ( (GPIO_PORTF_DATA_R & (1<<0))>>0 );     // read PF0 into SW2
		if(SW1 == 0 ){
			F1=1;
			F2=0;
			break;
		}
		if(SW2 == 0 ){
			F2=1;
			F1=0;
			break;
		}
	}
	if(F1==1&&F2==0){
		Nokia5110_Clear(); // clear the whole screen
		Nokia5110_SetCursor(0, 3);
		Nokia5110_OutString("> POT: (OK)");
	}
	if(F2==1&&F1==0){
		Nokia5110_Clear(); // clear the whole screen
		Nokia5110_SetCursor(0, 4);
		Nokia5110_OutString("> COM: (OK)");
	}
	Timer2_delay_ms(1500);
	Nokia5110_Clear(); // clear the whole screen
	Nokia5110_SetCursor(2, 2);
	Nokia5110_OutString("X-Player");
	Nokia5110_SetCursor(1, 4);
	Nokia5110_OutString("Plays First");
	Timer2_delay_ms(1500);
	Nokia5110_Clear(); // clear the whole screen
	// Play using Potentiometer
	if(F1==1&&F2==0){
		GameInitialization(); // reset
		DrawClearGameMatrix(); // clear data
		while(1){RunGameADC();}
	}
	// Play using Computer
	if(F2==1&&F1==0){
		GameInitialization(); // reset
		DrawClearGameMatrix(); // clear data
		while(1){RunGameUART();}
	}
}

/************************************************************************************
 * Service Name: GameInitialization
 * Parameters (in): None
 * Return value: None
 * Description: Function for reset game "start again"
 ************************************************************************************/
void GameInitialization()
{
	int i;
	playedCells = 0;  // make all cells available to play at
	turnMark = 'X';	  // first player is always X
	Sound_Init(rateOrdaniry,turnMark);
	currentCell = -1; // no cells are hovered or selected
	for (i = 0; i < 9; i++)
		GameMatrix[i] = ' '; // erase all drawings in cells matrix
	Timer2_delay_ms(100);
}

/************************************************************************************
 * Service Name:DrawClearGameMatrix
 * Parameters (in): None
 * Return value: None
 * Description: Function for drawing X O boxes and player turn
 ************************************************************************************/

void DrawClearGameMatrix()
{
	Nokia5110_ClearBuffer();
	// square is defined in gameArt to make the shape of the 9 squares of the game
	Nokia5110_PrintBMP(0, (SQUAREHEIGHT - 1), square, 0); // Nokia5110_printBMP(x_cor , y_cor , pointer_to_image (square or VL) , threshold(0 or 1))
	Nokia5110_DisplayBuffer();
	Timer2_delay_ms(100);

	Nokia5110_PrintBMP((SQUAREHEIGHT - 1) + 3, (SQUAREHEIGHT - 1), square, 0);
	Nokia5110_DisplayBuffer();
	Timer2_delay_ms(100);

	Nokia5110_PrintBMP(2 * (SQUAREHEIGHT - 1) + 6, (SQUAREHEIGHT - 1), square, 0);
	Nokia5110_DisplayBuffer();
	Timer2_delay_ms(100);

	Nokia5110_PrintBMP(0, 2 * (SQUAREHEIGHT - 1) + 3, square, 0);
	Nokia5110_DisplayBuffer();
	Timer2_delay_ms(100);

	Nokia5110_PrintBMP((SQUAREHEIGHT - 1) + 3, 2 * (SQUAREHEIGHT - 1) + 3, square, 0);
	Nokia5110_DisplayBuffer();
	Timer2_delay_ms(100);

	Nokia5110_PrintBMP(2 * (SQUAREHEIGHT - 1) + 6, 2 * (SQUAREHEIGHT - 1) + 3, square, 0);
	Nokia5110_DisplayBuffer();
	Timer2_delay_ms(100);

	Nokia5110_PrintBMP(0, 3 * (SQUAREHEIGHT - 1) + 6, square, 0);
	Nokia5110_DisplayBuffer();
	Timer2_delay_ms(100);

	Nokia5110_PrintBMP((SQUAREHEIGHT - 1) + 3, 3 * (SQUAREHEIGHT - 1) + 6, square, 0);
	Nokia5110_DisplayBuffer();
	Timer2_delay_ms(100);

	Nokia5110_PrintBMP(2 * (SQUAREHEIGHT - 1) + 6, 3 * (SQUAREHEIGHT - 1) + 6, square, 0);
	Nokia5110_DisplayBuffer();
	Timer2_delay_ms(100);

	Nokia5110_PrintBMP(3 * (SQUAREHEIGHT - 1) + 9, (VH - 1), verticalLine, 0);
	Nokia5110_DisplayBuffer();

	displayStatus(); // to show whose player turn is it now
	currentCell=0;
}

/************************************************************************************
 * Service Name   :RunGame
 * Parameters (in) : None
 * Return value: None
 * Description: Function for handeling xo game play
 ************************************************************************************/
int selectFlag = 0 ;
void RunGameUART() {
		int row, col, lastRow, lastCol, lastCell = -1;
	
		char c;
		UART_OutString("--------------------------------\n");
		UART_OutString("\rPress Key :");
		c = UART_InChar();
		switch(c){
			// Go Right
			case '6':
					UART_OutChar(c);
					UART_OutString(" , Go Right\n");
					if(ADCvalue==8)	ADCvalue=8;
					else ADCvalue++;
			break;
			// Go Left
			case '4':
					UART_OutChar(c);
					UART_OutString(" , Go Left\n");
					if(ADCvalue==0)	ADCvalue=0;
					else ADCvalue--;
			break;
			// Go Up
			case '8':
					UART_OutChar(c);
					UART_OutString(" , Go Up\n");
					if(ADCvalue>=3)	ADCvalue-=3;;
			break;
			// Go Down
			case '2':
					UART_OutChar(c);
					UART_OutString(" , Go Down\n");
					if(ADCvalue<=5)	ADCvalue+=3;;
			break;
			// Select
			case '5':
					UART_OutChar(c);
					UART_OutString(" , Select Cell\n");
					selectFlag=1;
			break;
		default: break;
		}
		UART_OutString("\n");
		// Save the current cell before updating
		lastCell = currentCell;
		// Calculate the new cell based on ADC value
		//currentCell = (int)(ADCvalue / 420);
		currentCell = ADCvalue;
		if (currentCell == 9) currentCell = 8; // Still at last cell

		// Ensure lastCell is cleared properly
		if (lastCell != currentCell) {
				lastRow = lastCell / 3;
				lastCol = lastCell % 3;

				// Redraw the previous cell based on its state
				if (GameMatrix[lastCell] == 'X') {
						Nokia5110_PrintBMP(lastCol * (SQUAREHEIGHT - 1) + 3 * lastCol,
															 (lastRow + 1) * (SQUAREHEIGHT - 1) + 3 * lastRow, X, 0);
				} else if (GameMatrix[lastCell] == 'O') {
						Nokia5110_PrintBMP(lastCol * (SQUAREHEIGHT - 1) + 3 * lastCol,
															 (lastRow + 1) * (SQUAREHEIGHT - 1) + 3 * lastRow, O, 0);
				} else {
						Nokia5110_PrintBMP(lastCol * (SQUAREHEIGHT - 1) + 3 * lastCol,
															 (lastRow + 1) * (SQUAREHEIGHT - 1) + 3 * lastRow, square, 0);
				}
		}

		// Calculate the new cell's row and column
		row = currentCell / 3;
		col = currentCell % 3;

		// Draw the hover state for the current cell
		if (GameMatrix[currentCell] == 'X') {
				Nokia5110_PrintBMP(col * (SQUAREHEIGHT - 1) + 3 * col,
													 (row + 1) * (SQUAREHEIGHT - 1) + 3 * row, selectedX, 0);
		} else if (GameMatrix[currentCell] == 'O') {
				Nokia5110_PrintBMP(col * (SQUAREHEIGHT - 1) + 3 * col,
													 (row + 1) * (SQUAREHEIGHT - 1) + 3 * row, selectedO, 0);
		} else {
				Nokia5110_PrintBMP(col * (SQUAREHEIGHT - 1) + 3 * col,
													 (row + 1) * (SQUAREHEIGHT - 1) + 3 * row, selectedSquare, 0);
		}

		// Update the display and other statuses
		Nokia5110_DisplayBuffer();
		displayStatus();
		Timer2_delay_ms(100); // Small delay for stability
    if (selectFlag==1) {
				//case2:
				selectFlag=0;
        // Confirm that switch 2 is pressed and there is a selected cell
        if (GameMatrix[currentCell] == ' ') { // If the cell is empty
            row = currentCell / 3;
            col = currentCell % 3;

            if (turnMark == 'X') {
                Nokia5110_PrintBMP(col * (SQUAREHEIGHT - 1) + 3 * col,
                                   (row + 1) * (SQUAREHEIGHT - 1) + 3 * row, X, 0);
                GameMatrix[currentCell] = turnMark;
                if (checkWinner(turnMark)) {
									Sound_Tone(rateWinning, turnMark,0);
                    Display_Winner(turnMark);
                    if (CheckPlayAgain()) {
												Sound_Off();
                        GameInitialization();
                        DrawClearGameMatrix();
                        return;
                    } else {
												Sound_Off();
                        EndGame();
                        while (1);
                    }
                }
                turnMark = 'O'; // Switch to player O
            } else {
                Nokia5110_PrintBMP(col * (SQUAREHEIGHT - 1) + 3 * col,
                                   (row + 1) * (SQUAREHEIGHT - 1) + 3 * row, O, 0);
                GameMatrix[currentCell] = turnMark;

                if (checkWinner(turnMark)) {
										Sound_Tone(rateWinning, turnMark,0);
                    Display_Winner(turnMark);
                    if (CheckPlayAgain()) {
												Sound_Off();
                        GameInitialization();
                        DrawClearGameMatrix();
                        return;
                    } else {
                        Sound_Off();
												EndGame();
                        while (1);
                    }
                }
                turnMark = 'X'; // Switch to player X
            }
            Nokia5110_DisplayBuffer();
            displayStatus();
            playedCells++;

            if (playedCells == 9) { // Game ends in a draw
							Sound_Tone(rateWinning, turnMark,1);
                displayDraw();
                if (CheckPlayAgain()) {
											Sound_Off();
										GameInitialization();
                    DrawClearGameMatrix();
                    return;
                } else {
											Sound_Off();
                    EndGame();
                    while (1);
                }
            }
        } else { // Cell is already taken
            Nokia5110_SetCursor(8, 3);
            Nokia5110_OutString("Not");
            Nokia5110_SetCursor(8, 4);
            Nokia5110_OutString("Your");
            Nokia5110_SetCursor(8, 5);
            Nokia5110_OutString("Turn");
            displayStatus();
        }
				Sound_Tone(rateWinning, turnMark,0);
        Timer2_delay_ms(100); // Small delay
    }
}
/************************************************************************************/
void RunGameADC() {
		int row, col, lastRow, lastCol, lastCell = -1;
		// Save the current cell before updating
		lastCell = currentCell;
		// Calculate the new cell based on ADC value
		currentCell = (int)(ADCvalue / 420);
		if (currentCell == 9) currentCell = 8; // Still at last cell

		// Ensure lastCell is cleared properly
		if (lastCell != currentCell) {
				lastRow = lastCell / 3;
				lastCol = lastCell % 3;

				// Redraw the previous cell based on its state
				if (GameMatrix[lastCell] == 'X') {
						Nokia5110_PrintBMP(lastCol * (SQUAREHEIGHT - 1) + 3 * lastCol,
															 (lastRow + 1) * (SQUAREHEIGHT - 1) + 3 * lastRow, X, 0);
				} else if (GameMatrix[lastCell] == 'O') {
						Nokia5110_PrintBMP(lastCol * (SQUAREHEIGHT - 1) + 3 * lastCol,
															 (lastRow + 1) * (SQUAREHEIGHT - 1) + 3 * lastRow, O, 0);
				} else {
						Nokia5110_PrintBMP(lastCol * (SQUAREHEIGHT - 1) + 3 * lastCol,
															 (lastRow + 1) * (SQUAREHEIGHT - 1) + 3 * lastRow, square, 0);
				}
		}

		// Calculate the new cell's row and column
		row = currentCell / 3;
		col = currentCell % 3;

		// Draw the hover state for the current cell
		if (GameMatrix[currentCell] == 'X') {
				Nokia5110_PrintBMP(col * (SQUAREHEIGHT - 1) + 3 * col,
													 (row + 1) * (SQUAREHEIGHT - 1) + 3 * row, selectedX, 0);
		} else if (GameMatrix[currentCell] == 'O') {
				Nokia5110_PrintBMP(col * (SQUAREHEIGHT - 1) + 3 * col,
													 (row + 1) * (SQUAREHEIGHT - 1) + 3 * row, selectedO, 0);
		} else {
				Nokia5110_PrintBMP(col * (SQUAREHEIGHT - 1) + 3 * col,
													 (row + 1) * (SQUAREHEIGHT - 1) + 3 * row, selectedSquare, 0);
		}

		// Update the display and other statuses
		Nokia5110_DisplayBuffer();
		displayStatus();
		Timer2_delay_ms(100); // Small delay for stability
    if (Sw2Flag && currentCell != -1) {
        Sw2Flag = 0;
        // Confirm that switch 2 is pressed and there is a selected cell
        if (GameMatrix[currentCell] == ' ') { // If the cell is empty
            row = currentCell / 3;
            col = currentCell % 3;

            if (turnMark == 'X') {
                Nokia5110_PrintBMP(col * (SQUAREHEIGHT - 1) + 3 * col,
                                   (row + 1) * (SQUAREHEIGHT - 1) + 3 * row, X, 0);
                GameMatrix[currentCell] = turnMark;

                if (checkWinner(turnMark)) {
                    Display_Winner(turnMark);
										Sound_Tone(rateWinning, turnMark,0);
                    if (CheckPlayAgain()) {
												Sound_Off();
                        GameInitialization();
                        DrawClearGameMatrix();
                        return;
                    } else {
												Sound_Off();
                        EndGame();
                        while (1);
                    }
                }
                turnMark = 'O'; // Switch to player O
								
            } else {
                Nokia5110_PrintBMP(col * (SQUAREHEIGHT - 1) + 3 * col,
                                   (row + 1) * (SQUAREHEIGHT - 1) + 3 * row, O, 0);
                GameMatrix[currentCell] = turnMark;

                if (checkWinner(turnMark)) {
									Sound_Tone(rateWinning,turnMark,0);
                    Display_Winner(turnMark);
                    if (CheckPlayAgain()) {
										Sound_Off();
                        GameInitialization();
                        DrawClearGameMatrix();
                        return;
                    } else {
											Sound_Off();
                        EndGame();
                        while (1);
                    }
                }
                turnMark = 'X'; // Switch to player X

            }
            Nokia5110_DisplayBuffer();
            displayStatus();
            playedCells++;

            if (playedCells == 9) { // Game ends in a draw
                displayDraw();
							Sound_Tone(rateWinning,turnMark,1);
                if (CheckPlayAgain()) {
										Sound_Off();
                    GameInitialization();
                    DrawClearGameMatrix();
                    return;
                } else {
										Sound_Off();
                    EndGame();
                    while (1);
                }
            }
        } else { // Cell is already taken
            Nokia5110_SetCursor(8, 3);
            Nokia5110_OutString("Not");
            Nokia5110_SetCursor(8, 4);
            Nokia5110_OutString("Your");
            Nokia5110_SetCursor(8, 5);
            Nokia5110_OutString("Turn");
            displayStatus();
        }
				Sound_Tone(rateWinning,turnMark,0);
        Timer2_delay_ms(100); // Small delay
		}
}



/************************************************************************************
 * Service Name:checkWinner
 * Parameters (in): char player
 * Return value: 0 or 1
 * Description: Function to check if player x or o is winning or not
 ************************************************************************************/

int checkWinner(char player)
{
	int Winning_cell_1 = 0, Winning_cell_2 = 0, Winning_cell_3 = 0, i; // c: cell (they are 3 as 3 same connected cells are a must to win)
	if (GameMatrix[0] == player && GameMatrix[1] == player && GameMatrix[2] == player)
	{
		Winning_cell_1 = 0;
		Winning_cell_2 = 1;
		Winning_cell_3 = 2;
		i = 0;
	}
	else if (GameMatrix[3] == player && GameMatrix[4] == player && GameMatrix[5] == player)
	{
		Winning_cell_1 = 3;
		Winning_cell_2 = 4;
		Winning_cell_3 = 5;
		i = 1;
	}
	else if (GameMatrix[6] == player && GameMatrix[7] == player && GameMatrix[8] == player)
	{
		Winning_cell_1 = 6;
		Winning_cell_2 = 7;
		Winning_cell_3 = 8;
		i = 2;
	}
	else if (GameMatrix[0] == player && GameMatrix[3] == player && GameMatrix[6] == player)
	{
		Winning_cell_1 = 0;
		Winning_cell_2 = 3;
		Winning_cell_3 = 6;
		i = 3;
	}
	else if (GameMatrix[1] == player && GameMatrix[4] == player && GameMatrix[7] == player)
	{
		Winning_cell_1 = 1;
		Winning_cell_2 = 4;
		Winning_cell_3 = 7;
		i = 4;
	}
	else if (GameMatrix[2] == player && GameMatrix[5] == player && GameMatrix[8] == player)
	{
		Winning_cell_1 = 2;
		Winning_cell_2 = 5;
		Winning_cell_3 = 8;
		i = 5;
	}
	else if (GameMatrix[0] == player && GameMatrix[4] == player && GameMatrix[8] == player)
	{
		Winning_cell_1 = 0;
		Winning_cell_2 = 4;
		Winning_cell_3 = 8;
		i = 6;
	}
	else if (GameMatrix[2] == player && GameMatrix[4] == player && GameMatrix[6] == player)
	{
		Winning_cell_1 = 2;
		Winning_cell_2 = 4;
		Winning_cell_3 = 6;
		i = 7;
	}
	if (Winning_cell_1 || Winning_cell_2 || Winning_cell_2)
	{
		if (player == 'X')
		{ /*Hoovering the 3 selected winning cells*/
			Nokia5110_PrintBMP((Winning_cell_1 % 3) * (SQUAREHEIGHT - 1) + 3 * (Winning_cell_1 % 3), (Winning_cell_1 / 3 + 1) * (SQUAREHEIGHT - 1) + 3 * (Winning_cell_1 / 3), selectedX, 0);
			Nokia5110_PrintBMP((Winning_cell_2 % 3) * (SQUAREHEIGHT - 1) + 3 * (Winning_cell_2 % 3), (Winning_cell_2 / 3 + 1) * (SQUAREHEIGHT - 1) + 3 * (Winning_cell_2 / 3), selectedX, 0);
			Nokia5110_PrintBMP((Winning_cell_3 % 3) * (SQUAREHEIGHT - 1) + 3 * (Winning_cell_3 % 3), (Winning_cell_3 / 3 + 1) * (SQUAREHEIGHT - 1) + 3 * (Winning_cell_3 / 3), selectedX, 0);
			Nokia5110_DisplayBuffer();
		}
		else
		{
			Nokia5110_PrintBMP((Winning_cell_1 % 3) * (SQUAREHEIGHT - 1) + 3 * (Winning_cell_1 % 3), (Winning_cell_1 / 3 + 1) * (SQUAREHEIGHT - 1) + 3 * (Winning_cell_1 / 3), selectedO, 0);
			Nokia5110_PrintBMP((Winning_cell_2 % 3) * (SQUAREHEIGHT - 1) + 3 * (Winning_cell_2 % 3), (Winning_cell_2 / 3 + 1) * (SQUAREHEIGHT - 1) + 3 * (Winning_cell_2 / 3), selectedO, 0);
			Nokia5110_PrintBMP((Winning_cell_3 % 3) * (SQUAREHEIGHT - 1) + 3 * (Winning_cell_3 % 3), (Winning_cell_3 / 3 + 1) * (SQUAREHEIGHT - 1) + 3 * (Winning_cell_3 / 3), selectedO, 0);
			Nokia5110_DisplayBuffer();
		}
		/*draw the strike line with pixels (dots) on the 3 hoovered winning cells*/
		if (i == 0)
		{
			for (i = 0; i < 48; i++)
				Nokia5110_SetPixel(i, 6);
		}
		else if (i == 1)
		{
			for (i = 0; i < 48; i++)
				Nokia5110_SetPixel(i, 23);
		}
		else if (i == 2)
		{
			for (i = 0; i < 48; i++)
				Nokia5110_SetPixel(i, 40);
		}
		else if (i == 3)
		{
			for (i = 0; i < 48; i++)
				Nokia5110_SetPixel(6, i);
		}
		else if (i == 4)
		{
			for (i = 0; i < 48; i++)
				Nokia5110_SetPixel(23, i);
		}
		else if (i == 5)
		{
			for (i = 0; i < 48; i++)
				Nokia5110_SetPixel(40, i);
		}
		else if (i == 6)
		{
			for (i = 0; i < 48; i++)
				Nokia5110_SetPixel(i, i);
		}
		else if (i == 7)
		{
			for (i = 0; i < 48; i++)
				Nokia5110_SetPixel(i, 47 - i);
		}
		Nokia5110_DisplayBuffer();
		Timer2_delay_ms(2000);
		return 1; // there is a winner
	}
	return 0; // there is no winner
}

/************************************************************************************
 * Service Name:Display_Winner
 * Parameters (in): None
 * Return value: None
 * Description: Function to display the winner of xo game
 ************************************************************************************/
void Display_Winner(char player)
{
	(player == 'X') ? xWins++ : oWins++; // for every round count winning times
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(0, 47, border, 0); // draw the game frame
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor(2, 3);
	Nokia5110_OutChar(player);
	Nokia5110_OutString("-Player");
	Nokia5110_SetCursor(4, 4);
	Nokia5110_OutString("wins");
	Timer2_delay_ms(1500);
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor(2, 3);
	Nokia5110_OutString("X: ");
	Nokia5110_OutUDec(xWins);
	Nokia5110_SetCursor(2, 4);
	Nokia5110_OutString("O: ");
	Nokia5110_OutUDec(oWins);
	Timer2_delay_ms(2000);
	Nokia5110_Clear(); // clear the screen
}

/************************************************************************************
 * Service Name:displayDraw
 * Parameters (in): None
 * Return value: None
 * Description: Function to be executed if a draw game
 ************************************************************************************/
void displayDraw()
{
	Timer2_delay_ms(2000);
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(0, 47, border, 0); // draw the game frame
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor(2, 3);
	Nokia5110_OutString("Game is");
	Nokia5110_SetCursor(2, 4);
	Nokia5110_OutString("a draw");
	Timer2_delay_ms(1500);
	Nokia5110_Clear();
}

/************************************************************************************
 * Service Name:displayStatus
 * Parameters (in): None
 * Return value: None
 * Description: Function to show whose turn it is every play
 ************************************************************************************/
void displayStatus()
{
	Nokia5110_SetCursor(8, 1);
	Nokia5110_OutChar('<');
	Nokia5110_OutChar(turnMark);
	Nokia5110_OutChar('>');
	Nokia5110_SetCursor(8, 2);
	Nokia5110_OutString("Turn");
}
/************************************************************************************
 * Service Name: CheckPlayAgain
 * Parameters (in): None
 * Return value: None
 * Description: function to check if want to play again or not
 ************************************************************************************/
int CheckPlayAgain()
{
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(0, 47, border2, 0);
	Nokia5110_DisplayBuffer();
	Timer2_delay_ms(1000);
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(0, 47, border, 0); // draw the game frame
	Nokia5110_DisplayBuffer(); 
	Nokia5110_SetCursor(2, 3);
	Nokia5110_OutString("SW1 |SW2");
	Nokia5110_SetCursor(2, 4);
	Nokia5110_OutString("Yes |No");
	Sw1Flag = 0 ; 
	Sw2Flag = 0 ;
	while (1)
	{
		if (Sw1Flag == 1)
		{
			Sw1Flag = 0;
			return 1;
		}

		if (Sw2Flag == 1)
		{
			Sw2Flag = 0;
			return 0;
		}
	}
}

/************************************************************************************
 * Service Name: outr
 * Parameters (in): None
 * Return value: None
 * Description: function to out from game when fail
 ************************************************************************************/
void EndGame()
{ // if sw2 is pressed (no rematch)
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(0, 47, border, 0);
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor(2, 3);
	Nokia5110_OutString("Goodbye");
	Timer2_delay_ms(2000);
	Nokia5110_Clear();
}
