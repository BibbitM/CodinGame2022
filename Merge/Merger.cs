using System.IO;
using System.Text.RegularExpressions;

namespace Merge
{
    internal class Merger
    {
        private string mainFileName;
        private string projectDir;
        private string outFileName;

        private HashSet<string> includedHeaders = new();

        public Merger(string mainFileName, string projectDir, string outFileName)
        {
            this.mainFileName = mainFileName;
            this.projectDir = projectDir;
            this.outFileName = outFileName;
        }

        public int Merge()
        {
            using StreamWriter outFile = new(outFileName);

            string[] projectFiles = Directory.GetFiles(projectDir, "*.cpp");
            foreach (string projectFileName in projectFiles)
                WriteFile(outFile, projectFileName);

            WriteFile(outFile, mainFileName);

            return 0;
        }

        private void WriteFile(StreamWriter outFile, string fileName)
        {
            string[] fileLines = File.ReadAllLines(fileName);

            string incPattern = @"^\s*#\s*include\s*""([\w\.\\/]+)""";
            Regex incRgx = new Regex(incPattern);
            string oncePattern = @"^\s*#\s*pragma\s+once";
            Regex onceRgx = new Regex(oncePattern);

            outFile.WriteLine($"// {fileName}");
            foreach (string line in fileLines)
            {
                var incMatch = incRgx.Match(line);
                if (incMatch.Success)
                {
                    outFile.WriteLine($"// {line} begin");
                    WriteInclude(outFile, fileName, incMatch.Groups[1].Value);
                    outFile.WriteLine($"// {line} end");
                    continue;
                }

                if (onceRgx.IsMatch(line))
                {
                    outFile.Write($"// {line}");
                    continue;
                }

                outFile.WriteLine(line);
            }
        }

        private void WriteInclude(StreamWriter outFile, string fileName, string includeFileName)
        {
            string includePath = Path.Combine(Path.GetDirectoryName(fileName)!, includeFileName);
            string includeFullPath = Path.GetFullPath(includePath);

            if (includedHeaders.Contains(includeFullPath))
                return;
            includedHeaders.Add(includeFullPath);

            WriteFile(outFile, includePath);
        }
    }
}
