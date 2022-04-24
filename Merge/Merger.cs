using System.IO;

namespace Merge
{
    internal class Merger
    {
        private string mainFileName;
        private string projectDir;
        private string outFileName;
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

            outFile.WriteLine($"// {fileName}");
            foreach (string line in fileLines)
            {
                outFile.WriteLine(line);
            }
        }
    }
}
