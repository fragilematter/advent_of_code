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
var pageOrder = new Dictionary<uint, List<uint>>();

String? line;

while (!String.IsNullOrEmpty(line = sr.ReadLine())) {
    var parts = line.Split('|');
    
    uint keyPage = uint.Parse(parts[0]);
    uint valuePage = uint.Parse(parts[1]);

    if (pageOrder.ContainsKey(keyPage)) {
        pageOrder[keyPage].Add(valuePage);
    }
    else {
        pageOrder.Add(uint.Parse(parts[0]), new List<uint>() { uint.Parse(parts[1]) });
    }
}

foreach (var kvp in pageOrder) {
    Console.WriteLine($"Page {kvp.Key} should appear before {string.Join(", ", kvp.Value)}");
}

uint middlePageTotal = 0;

while(!String.IsNullOrEmpty(line = sr.ReadLine())) {
    Console.WriteLine($"Processing list {line}");

    var parts = line.Split(',');
    var pageList = Array.ConvertAll(parts, uint.Parse);
    bool lineIsValid = true;

    for (uint i = 1; i < pageList.Length; i++) {
        List<uint> pageRules;

        if(!pageOrder.TryGetValue(pageList[i], out pageRules)) continue;

        for(uint j = 0; j < i; j++) {
            if(pageRules.Contains(pageList[j])) {
                Console.WriteLine($"Page {pageList[i]} appears after {pageList[j]}");
                lineIsValid = false;
                break;
            }
        }

        if (!lineIsValid) break;
    }

    if (!lineIsValid) continue;

    var middlePageIndex = (pageList.Length - 1) / 2;
    middlePageTotal += pageList[middlePageIndex];
    Console.WriteLine($"Line is valid, middle page is {pageList[middlePageIndex]}");
}

Console.WriteLine($"Valid list middle page sum is {middlePageTotal}");

static void PrintHelp() {
    Console.WriteLine($"Usage: {System.AppDomain.CurrentDomain.FriendlyName} <INPUT_FILE>");
}
