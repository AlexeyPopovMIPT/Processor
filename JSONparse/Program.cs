using System;
using System.IO;
using Newtonsoft.Json;
using System.Collections.Generic;

namespace JSONparse_for_my_processor
{
    class Command
    {
        public string name;
        public short code;
        public string [] asmfragment;
        public string [] procfragment;
        public string [] args;
        public bool scltoken;
        public Command()
        {
            this.name = "POOR HANS";
            this.asmfragment = null;
            this.procfragment = null;
            this.args = null;
            this.scltoken = false;
        }
    }
    class Data
    {
        public Command[] operations;
        public Data()
        {
            this.operations = null;
        }
    }
    class Program
    {
        static void WriteArray(string[] array, StreamWriter sw)
        {
            foreach(string elem in array) { sw.WriteLine(elem); }
        }

        static void WriteToScl(Command command, StreamWriter swScl)
        {
            if (! command.scltoken)
            {
                swScl.Write("case ");
                swScl.Write(command.code);
                swScl.Write(":\nDBG printf(\"[SecondCompile] ");
                swScl.Write(command.name);
                swScl.Write("\\n\");\n");

                if (command.args != null)
                foreach (string arg in command.args)
                switch (arg)
                {
                    case "rshort":
                        swScl.WriteLine("iter+=sizeof(short);"); break;
                    case "RAM":
                        break;

                    default:
                        swScl.WriteLine("iter+=sizeof(" + arg + ");"); break;
                }
                swScl.WriteLine("break;");

            }
        }

        static void WriteToAss(Command command, StreamWriter swAss)
        {
            if (command.asmfragment != null)
            {
                swAss.Write("case ");
                swAss.Write(Program.GetHashCode(command.name));
                swAss.Write(":\nDBG printf(\"");
                swAss.Write(command.name);
                swAss.Write("\\n\");\n");
                WriteArray(command.asmfragment, swAss);
                swAss.Write("goto writing;\n\n");
            }
        }

        static void WriteToDis(Command command, StreamWriter swDis)
        {
            bool RAM = false;
            swDis.Write("case ");
            swDis.Write(command.code);
            swDis.Write(":\nDBG printf(\"");
            swDis.Write(command.name);
            swDis.Write("\\n\");\n");
            swDis.WriteLine("fprintf(out, \"" + command.name + " \"); ");

            if (command.args != null)
            foreach(string arg in command.args)
            switch (arg)
            {
                case "rshort":
                    swDis.Write("fputc(\'r\', out); GETPARAM(short); ");
                    break;
                case "short":
                    swDis.Write("GETPARAM(short); ");
                    break;
                case "double":
                    swDis.Write("GETPARAM(double); ");
                    break;
                case "long":
                    swDis.Write("GETPARAM(long); ");
                    break;
                case "RAM":
                    swDis.Write("fputc(\'[\', out);");
                    RAM = true;
                    break;

                default:
                    Console.WriteLine(command.args);
                    break;
            }
            if (RAM)
            {
                swDis.Write("fputc(\']\', out);");
            }
            swDis.WriteLine("fputc(10, out); break;");
        }

        static void WriteToProc(Command command, StreamWriter swProc)
        {
            swProc.Write("case ");
            swProc.Write(command.code);
            swProc.Write(":\nDBG printf(\"");
            swProc.Write(command.name);
            swProc.Write("\\n\");\n");
            WriteArray(command.procfragment, swProc);
            swProc.Write('\n');
        }

        static int GetHashCode(string str)
        {
            const int HASHFACTOR = -1484991287;
            int result = 0;
            foreach (char i in str)
            {
                result = HASHFACTOR * result + i;
            }
            return result;
        }

        static void CheckCollisions(Command [] operations)
        {
            Console.WriteLine("Collisions checking:");
            HashSet<int> hashes = new HashSet<int>();
            int hash;

            foreach (Command probe in operations)
            if (probe.asmfragment != null)
            {
                hash = Program.GetHashCode(probe.name);

                if (hashes.Contains(hash))
                foreach (Command rprobe in operations)
                if (Program.GetHashCode(rprobe.name) == hash)
                {   
                    Console.Write("Collision between ");
                    Console.Write(probe.name);
                    Console.Write('['); Console.Write(probe.code); Console.Write("] and ");
                    Console.Write(rprobe.name);
                    Console.Write('['); Console.Write(rprobe.code); Console.WriteLine(']');
                }
                hashes.Add(hash);
            }

            Console.WriteLine("End of checking");
        }

        static void Main(string[] args)
        {
            Data data = new Data();

            using (StreamReader sr = new StreamReader("../../config.json"))
            using (JsonReader jr = new JsonTextReader(sr))
            {
                string buffer = sr.ReadToEnd();
                JsonConvert.PopulateObject(buffer, data);
            }

            Command[] operations = data.operations;

            CheckCollisions(operations);
            Console.ReadKey();

            StreamWriter swAss  = new StreamWriter("../../../../Assembler/Assembler/case_enumeration.cpp");
            StreamWriter swDis  = new StreamWriter("../../../../Disassembler/Disassembler/case_enumeration.cpp");
            StreamWriter swProc = new StreamWriter("../../../../Compiler/Compiler/case_enumeration.cpp");
            StreamWriter swScl  = new StreamWriter("../../../../Assembler/Assembler/second_compile.cpp");

            foreach(Command probe in operations)
            {
                WriteToAss (probe, swAss );
                WriteToDis (probe, swDis );
                WriteToProc(probe, swProc);
                WriteToScl (probe, swScl );
            }

            swAss.Close();
            swDis.Close();
            swProc.Close();
            swScl.Close();

            Console.WriteLine("end");
            Console.ReadKey();
        }
    }
}

//дорогой русский программист, я не понимаю ваши константы
//сохранил на случай важных переговоров
