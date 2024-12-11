using System.Text;

if (args.Length < 1) {
    PrintHelp();
    return;
}

if (!File.Exists(args[0])) {
    Console.WriteLine("Pls gib file");
    return;
}

using var fs = File.OpenRead(args[0]);
using var sr = new StreamReader(fs, Encoding.UTF8, true, 4096);

String? line;
uint width = 0;
uint height = 0;

var map = new Map();
bool foundCharacter = false;
Direction characterDirection = Direction.North;
uint characterX = 0;
uint characterY = 0;

while (!String.IsNullOrEmpty(line = sr.ReadLine())) {
    if (width == 0) width = (uint)line.Length;

    for (int i = 0; i < width; i++) {
        if (line[i] == '.') continue;
        if (line[i] == '#') {
            map.AddObstacle((uint)i, height);
            continue;
        }

        characterDirection = (Direction)((int)line[i]);
        characterX = (uint)i;
        characterY = height;
        foundCharacter = true;
}
    
    height++;
}

map.Width = width;
map.Height = height;

if (!foundCharacter) {
    Console.WriteLine("Character not found on map!");
    return;
}

var character = new Character(characterDirection, map, characterX, characterY);

Console.WriteLine($"Character {characterDirection} {characterX} {characterY}");
Console.WriteLine($"Map {width} {height} {map.Obstacles.Count}");

uint movements = 1;
MoveResult moveResult;
while ((moveResult = character.Move()) != MoveResult.Exited) {
    if (moveResult == MoveResult.Step)
        movements++;

    Console.WriteLine($"{moveResult} {character.X} {character.Y}");
}

Console.WriteLine($"Character movements: {movements}");

static void PrintHelp() {
    Console.WriteLine($"Usage: {System.AppDomain.CurrentDomain.FriendlyName} <INPUT_FILE>");
}

readonly struct Coordinate {
    public uint X { get; init; }
    public uint Y { get; init; }

    public Coordinate(uint x, uint y) {
        X = x;
        Y = y;
    }
}

struct Map {
    public uint Width { get; set; }
    public uint Height { get; set; }

    public List<Coordinate> Obstacles { get; init; }

    public Map() {
        Width = 0;
        Height = 0;
        Obstacles = new();
    }

    public void AddObstacle(uint x, uint y) {
        Obstacles.Add(new Coordinate(x, y));
    }

    public bool ObstacleAt(uint x, uint y) {
        return Obstacles.Exists(o => o.X == x && o.Y == y);
    }
}

enum Direction : int {
    North = (int)'^',
    East = (int)'>',
    South = (int)'v',
    West = (int)'<',
}

enum MoveResult {
    Step,
    Rethread,
    AvoidedObstacle,
    Exited,
}

struct Character {
    public Direction Facing { get; private set; }
    public Map Map { get; init; }
    public uint X { get; private set; }
    public uint Y { get; private set; }
    public List<Coordinate> MovementHistory { get; init; } 

    public Character(Direction facing, Map map, uint x, uint y) {
        Facing = facing;
        Map = map;
        X = x;
        Y = y;
        MovementHistory = new();
        MovementHistory.Add(new Coordinate(x, y));
    }

    public MoveResult Move() {
        uint moveToX = X;
        uint moveToY = Y;

        switch (Facing) {
            case Direction.North:
                if (Y == 0) return MoveResult.Exited;
                moveToY--;

                break;
            case Direction.East:
                if (X == Map.Width - 1) return MoveResult.Exited;
                moveToX++;

                break;
            case Direction.South:
                if (Y == Map.Height - 1) return MoveResult.Exited;
                moveToY++;

                break;
            case Direction.West:
                if (X == 0) return MoveResult.Exited;
                moveToX--;

                break;
        }
        
        if (Map.ObstacleAt(moveToX, moveToY)) {
            Rotate();
            return MoveResult.AvoidedObstacle;
        }

        X = moveToX;
        Y = moveToY;

        bool rethread = MovementHistory.Exists(m => m.X == moveToX && m.Y == moveToY);
        if (!rethread) MovementHistory.Add(new Coordinate(X, Y));

        if (rethread)
            return MoveResult.Rethread;

        return MoveResult.Step;
    }

    private void Rotate() {
        Facing = Facing switch {
            Direction.North => Direction.East,
            Direction.East => Direction.South,
            Direction.South => Direction.West,
            Direction.West => Direction.North,
            _ => throw new Exception("Unknown direction")
        };
    }
}
