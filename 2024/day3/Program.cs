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

uint result = 0;
uint fullResult = 0;
uint enabledResult = 0;
char c;
int readByte;
bool enabled = true;

while ((readByte = fs.ReadByte()) > -1) {
    c = (char)readByte;
    if (c != 'm' && c != 'd') continue;

    if (c == 'd') {
        enabled = EnabledChecker(fs, enabled);
        continue;
    }

    result = MulTPass(fs);

    fullResult += result;
    if (enabled) enabledResult += result;
}

Console.WriteLine($"Result: {fullResult}");
Console.WriteLine($"Enabled result: {enabledResult}");

static bool EnabledChecker(FileStream fs, bool previouslyEnabled) {
    byte[] readBuffer = new byte[6];
    char[] checkBuffer = new char[6];

    if (fs.Read(readBuffer, 0, 6) == 0) 
        return previouslyEnabled;

    checkBuffer = Array.ConvertAll<byte, char>(readBuffer, c => (char)c);

    if (checkBuffer.SequenceEqual("on't()"))
        return false;

    var doSlice = new ArraySegment<char>(checkBuffer, 0, 3);
    if (doSlice.SequenceEqual("o()")) {
        fs.Seek(-3, SeekOrigin.Current);

        return true;
    }

    fs.Seek(-6, SeekOrigin.Current);

    return previouslyEnabled;
}

static uint MulTPass(FileStream fs) {
    uint firstNum = 0;
    uint secondNum = 0;
    States state = States.State_M;
    char c;
    int readByte;

    while ((readByte = fs.ReadByte()) > 0) {
        c = (char)readByte;

        switch (state) {
            case States.State_M:
                if (c != 'u') return 0;

                state = States.State_MU;
                break;
            case States.State_MU:
                if (c != 'l') return 0;

                state = States.State_MUL;
                break;
            case States.State_MUL:
                if (c != '(') return 0;

                state = States.State_FirstNum;
                break;
            case States.State_FirstNum:
                if (Char.IsDigit(c)) {
                    firstNum = firstNum * 10 + uint.Parse(c.ToString());
                    break;
                }
                else if (c == ',') {
                    state = States.State_SecondNum;
                    break;
                }
                return 0;
            case States.State_SecondNum:
                if (Char.IsDigit(c)) {
                    secondNum = secondNum * 10 + uint.Parse(c.ToString());
                    break;
                }
                else if (c == ')') {
                    return firstNum * secondNum;
                }
                return 0;
        }
    }

    return 0;
}

static void PrintHelp() {
    Console.WriteLine($"Usage: {System.AppDomain.CurrentDomain.FriendlyName} <INPUT_FILE>");
}

enum States {
    State_M,
    State_MU,
    State_MUL,
    State_FirstNum,
    State_SecondNum,
}


