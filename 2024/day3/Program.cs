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

uint result = 0;
char c;

while (!sr.EndOfStream) {
    c = (char)sr.Read();
    if (c != 'm') continue;

    result += MulTPass(sr);
}

Console.WriteLine($"Result: {result}");

static uint MulTPass(StreamReader sr) {
    uint firstNum = 0;
    uint secondNum = 0;
    States state = States.State_M;
    char c;

    while (!sr.EndOfStream) {
        c = (char)sr.Read();

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


