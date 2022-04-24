// See https://aka.ms/new-console-template for more information
if (args.Length < 3)
{
    Console.WriteLine("Invalid number of parameters.");
    Console.WriteLine("Usage:");
    Console.WriteLine($"\t{System.IO.Path.GetFileName(System.Diagnostics.Process.GetCurrentProcess().MainModule?.FileName)} <main_cpp_path> <project_path> <output_cpp_path>");
    return 1;
}

string mainFileName = args[0];
string projectDir = args[1];
string outFileName = args[2];

Merge.Merger merger = new(mainFileName, projectDir, outFileName);

return merger.Merge();
