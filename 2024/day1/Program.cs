using System.Text;

if (args.Length < 1) {
    PrintHelp();
    return;
}

if (!File.Exists(args[0])) {
    Console.WriteLine("Pls gib file");
    return;
}

var leftList = new List<uint>();
var rightList = new List<uint>();

using var fs = File.OpenRead(args[0]);
using var sr = new StreamReader(fs, Encoding.UTF8, true, 4096);

String line;
uint numLines = 0;

while ((line = sr.ReadLine()) != null) {
    var parts = line.Split(' ', StringSplitOptions.RemoveEmptyEntries);
    leftList.Add(uint.Parse(parts[0]));
    rightList.Add(uint.Parse(parts[1]));
    numLines++;
}

leftList.Sort();
rightList.Sort();

long totalDistance = 0;

for (var i = 0; i < numLines; i++) {
    uint result = (leftList[i] > rightList[i]) ? leftList[i] - rightList[i] : rightList[i] - leftList[i];
    totalDistance += result;
}

Console.WriteLine($"Total Distance: {totalDistance}");

uint similarityScore = 0;

var leftList_UniqueCount = leftList.GroupBy(item => item)
    .Select(g => new { Value = g.Key, Count = g.Count() })
    .ToList();
            
foreach (var group in leftList_UniqueCount) {
    similarityScore += group.Value * (uint)group.Count * (uint)rightList.Where(item => item == group.Value).Count();
}

Console.WriteLine($"Similarity Score: {similarityScore}");

void PrintHelp() {
    Console.WriteLine($"Usage: {System.AppDomain.CurrentDomain.FriendlyName} <INPUT_FILE>");
}
