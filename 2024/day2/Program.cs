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

String line;
uint safeEntries = 0;
uint dampenedSafeEntries = 0;

while ((line = sr.ReadLine()) != null) {
    var parts = line.Split(' ', StringSplitOptions.RemoveEmptyEntries);
    uint[] report = Array.ConvertAll(parts, uint.Parse);

    if (IsSafe(report)) {
        safeEntries++;
        dampenedSafeEntries++;
    }
    else {
       var dampenedReport = new uint[report.Count() - 1];

       for(uint i = 0; i < report.Count(); i++) {
            for (uint j = 0; j < report.Count(); j++) {
                if (i == j) continue;

                dampenedReport[j > i ? j - 1 : j] = report[j];
            }

            if(IsSafe(dampenedReport)) {
                dampenedSafeEntries++;
                break;
            }
        }
    }
}

Console.WriteLine($"Safe entries: {safeEntries}");
Console.WriteLine($"Dampened safe entries: {dampenedSafeEntries}");

static bool IsSafe(uint[] report) {
    uint num1 = report[0];
    uint num2 = report[1];

    bool ascending = num1 < num2;

    for(var i = 1; i < report.Count(); i++) {
        num2 = report[i];

        if (num1 == num2)
            return false;

        if (!(ascending ^ num1 > num2))
            return false;
        
        if ((ascending ? num2 - num1 : num1 - num2) > 3)
            return false;

        num1 = num2;
    }

    return true;
}

static void PrintHelp() {
    Console.WriteLine($"Usage: {System.AppDomain.CurrentDomain.FriendlyName} <INPUT_FILE>");
}
