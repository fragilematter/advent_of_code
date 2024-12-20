﻿using System.Text;

if (args.Length < 1) {
    PrintHelp();
    return;
}

if (!File.Exists(args[0])) {
    Console.WriteLine("Pls gib file");
    return;
}

ReadOnlySpan<String> fileContent = File.ReadAllLines(args[0]);
ReadOnlySpan<char> lookingFor = "XMAS";
uint christmasCount = 0;
uint dashmasCount = 0;

/*
 * Dir  X  Y
 *  N   0 -1
 *  NE  1 -1
 *  E   1  0
 *  SE  1  1
 *  S   0  1
 *  SW -1  1
 *  W  -1  0
 *  NW -1 -1
 * */
// Intuition says that the directions could be derived by math and bitwise ops
// but... eh, just hardcode it.

int[] xDirection = [ 0,  1, 1, 1, 0, -1, -1, -1];
int[] yDirection = [-1, -1, 0, 1, 1,  1,  0, -1];

for (int yPos = 0; yPos < fileContent.Length; yPos++) {
    for (int xPos = 0; xPos < fileContent[yPos].Length; xPos++) {
        for (int direction = 0; direction < 8; direction++) {
            christmasCount += FoundChristmas(fileContent, lookingFor, xPos, yPos, direction, 0);
        }

        if (fileContent[yPos][xPos] == 'A') {
            dashmasCount += FoundDashmas(fileContent, xPos, yPos);
        }
    }
}

Console.WriteLine($"{lookingFor} instances: {christmasCount}");
Console.WriteLine($"X-MAS instances: {dashmasCount}");

uint FoundDashmas(
        ReadOnlySpan<String> fileContent,
        int xPos, int yPos
        ) 
{
    if (xPos == 0 || yPos == 0) return 0;
    if (yPos + 1 == fileContent.Length) return 0;
    if (xPos + 1 == fileContent[yPos].Length) return 0;

    char top;
    char bottom;

    // ran out of patience for this one
    top = fileContent[yPos - 1][xPos - 1];
    bottom = fileContent[yPos + 1][xPos + 1];

    if (!IsMas(top, bottom)) return 0;

    top = fileContent[yPos - 1][xPos + 1];
    bottom = fileContent[yPos + 1][xPos - 1];

    if (!IsMas(top, bottom)) return 0;

    return 1;
}

bool IsMas(char top, char bottom) {
    if (top == 'M' && bottom == 'S') return true;
    if (top == 'S' && bottom == 'M') return true;

    return false;
}

uint FoundChristmas(
        ReadOnlySpan<String> fileContent, 
        ReadOnlySpan<char> lookingFor,
        int xPos, int yPos, 
        int direction, int offset
        ) 
{

    if (fileContent[xPos][yPos] != lookingFor[offset]) return 0;
    if (++offset == lookingFor.Length) return 1;

    xPos += xDirection[direction];
    yPos += yDirection[direction];

    // check boundaries
    if (xPos < 0 || yPos < 0) return 0;
    if (yPos == fileContent.Length) return 0;
    if (xPos ==  fileContent[yPos].Length) return 0;

    return FoundChristmas(fileContent, lookingFor, xPos, yPos, direction, offset);
}

static void PrintHelp() {
    Console.WriteLine($"Usage: {System.AppDomain.CurrentDomain.FriendlyName} <INPUT_FILE>");
}

