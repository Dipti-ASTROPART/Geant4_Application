#!/bin/bash

ESC='\033'
RESET="${ESC}[0m"
BOLD="${ESC}[1m"
RED="${ESC}[31m"
GREEN="${ESC}[32m"
YELLOW="${ESC}[33m"
BLUE="${ESC}[34m"
MAGENTA="${ESC}[35m"
CYAN="${ESC}[36m"
SMILE="\U1F605"

# Define the box width
BOX_WIDTH=40

# Create the top and bottom borders of the box
BORDER=$(printf '%*s' "$BOX_WIDTH" | tr ' ' '-')

echo -e ""
echo -e "${MAGENTA}┌${BORDER}┐${RESET}"
echo -e "${MAGENTA}║ ${BOLD}${GREEN}  GEANT4 FRAMEWORK BUILD SUCCESS ${SMILE}${RESET} ${BOLD}${YELLOW}${AUTHOR_NAME}${RESET}${MAGENTA}   ║${RESET}"
echo -e "${MAGENTA}║${BORDER}║${RESET}"
echo -e "${MAGENTA}║ ${BOLD}${YELLOW}Author: Diptiranjan Pattanaik${RESET} ${BOLD}${YELLOW}${AUTHOR_NAME}${RESET}${MAGENTA}         ║${RESET}"
echo -e "${MAGENTA}║ ${BOLD}${YELLOW}Contact: dipti.astropart@gmail.com${RESET} ${BOLD}${YELLOW}${AUTHOR_ADDRESS}${RESET}${MAGENTA}    ║${RESET}"
echo -e "${MAGENTA}└${BORDER}┘${RESET}"
echo -e ""
