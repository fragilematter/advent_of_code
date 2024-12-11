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

uint middlePageTotal = 0;
uint fixedPageTotal = 0;

while(!String.IsNullOrEmpty(line = sr.ReadLine())) {
    var parts = line.Split(',');
    var pageList = Array.ConvertAll(parts, uint.Parse);

    if (!IsLineValid(pageList, pageOrder)) {
        fixedPageTotal += GetFixedMid(pageList, pageOrder);
        continue;
    }

    var middlePageIndex = (pageList.Length - 1) / 2;
    middlePageTotal += pageList[middlePageIndex];
}

Console.WriteLine($"Valid list middle page sum is {middlePageTotal}");
Console.WriteLine($"Fixed list middle page sum is {fixedPageTotal}");

static bool IsLineValid(uint[] pageList, Dictionary<uint, List<uint>> pageOrder) {
    for (uint i = 1; i < pageList.Length; i++) {
        List<uint>? pageRules;

        if(!pageOrder.TryGetValue(pageList[i], out pageRules)) continue;

        for(uint j = 0; j < i; j++) {
            if(pageRules.Contains(pageList[j])) {
                return false;
            }
        }
    }

    return true;
}

static uint GetFixedMid(uint[] pageArray, Dictionary<uint, List<uint>> pageOrder) {
    var pageList = new List<uint>(pageArray);

    pageList.Sort((x, y) => {
            List<uint>? pageRules;

            if (pageOrder.TryGetValue(x, out pageRules) && pageRules.Contains(y))
                return -1;

            if (pageOrder.TryGetValue(y, out pageRules) && pageRules.Contains(x))
                return 1;

            return 0;

            });

    return pageList[(pageList.Count - 1) / 2]; 
}

static void PrintHelp() {
    Console.WriteLine($"Usage: {System.AppDomain.CurrentDomain.FriendlyName} <INPUT_FILE>");
}
